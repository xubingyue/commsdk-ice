#include <uvssclient.h>

#include <map>
#include <memory>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include <callbackreceiveri.h>
#include <global.h>

UvssClient::UvssClient() :
    peerProxies_(std::make_shared<PeerProxies>()),
    workQueue_(std::make_shared<WorkQueue>()),
    client_(std::make_shared<CallbackReceiverI>(peerProxies_, workQueue_))
{
//     try...catch?
    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");
    props->setProperty("Ice.MessageSizeMax", "0");
//     props->setProperty("Ice.ACM.Client", "0");
    Ice::InitializationData initData;
    initData.properties = props;

    ic_ = Ice::initialize(initData);
    adapter_ = ic_->createObjectAdapter("");
    ident_.name = IceUtil::generateUUID();
    ident_.category = "";
}

UvssClient::~UvssClient()
{
    ic_->destroy();
    peerProxies_->join();
    workQueue_->join();
}

int UvssClient::start()
{
    try {
        adapter_->add(client_, ident_);
        adapter_->activate();
        peerProxies_->start(); // 启动心跳线程
        workQueue_->start(); // AMD
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_initializationCallback(-1, -1, "初始化失败");
        return -1;
    }

    return 1;
}

int UvssClient::connect(const std::string& ipAddress, int port)
{
    try {
        std::string endpoint(ipAddress + ":" + boost::lexical_cast<std::string>(port));
        if (peerProxies_->isRepeated(endpoint)) {
            return -2;
        }

        auto base = ic_->stringToProxy("UvssServer:tcp -h " + ipAddress + " -p " +
                        boost::lexical_cast<std::string>(port));
        auto server = Ice::checkedCast<Uvss::CallbackSenderPrx>(base);
        if (!server) {
            throw std::runtime_error("Invalid proxy");
        }

//         std::cout << server->ice_getConnection()->getEndpoint()->toString() << std::endl;
//         std::cout << endpoint << std::endl;
//         tcp -h 192.168.1.9 -p 20145 -t 60000
//         192.168.1.9:20145

//         保留检查对端版本的功能
        if (!server->checkVersion(Uvss::version)) {
            return -3;
        }

        server->ice_getConnection()->setAdapter(adapter_);
        server->addClient(ident_);

        int connectionId = peerProxies_->add(server, endpoint);

        std::string message("服务器端 " + endpoint + ": " + "已连接 | 连接标识: " + boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, 1, message.c_str());

        return connectionId;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_connectionCallback(-1, -2, "连接失败");

        return -1;
    }
}

int UvssClient::disconnect(int index)
{
    std::string endpoint;
    std::shared_ptr<Uvss::CallbackSenderPrx> server;
    bool ok = peerProxies_->findAndRemove(index, endpoint, server);
    if (ok) {
//         移除要断开的server代理，无论发生在心跳线程中的servers副本拷贝前或后，心跳都不会检测到连接错误，不会有server断开的通知
//         所以只能在此处通知，不能依靠心跳线程，这里断开通知仅和移除有关
        std::string message("服务器端 " + endpoint + ": " +
                            "已断开 | 连接标识: " + boost::lexical_cast<std::string>(index));
        g_connectionCallback(index, -3, message.c_str());

//         使server端到client的心跳失败，发生回调通知
        server->ice_getConnection()->close(Ice::ConnectionClose::Gracefully);

        return 1;
    }
    else {
        return -1;
    }
}

void UvssClient::shutdown()
{
    peerProxies_->destroy();
    workQueue_->destroy();
    ic_->shutdown();
}
