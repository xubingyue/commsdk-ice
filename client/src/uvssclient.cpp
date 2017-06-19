#include <uvssclient.h>

#include <boost/lexical_cast.hpp>

#include <global.h>

UvssClient::UvssClient() :
    proxies_(std::make_shared<RpcProxies>()),
    workQueue_(std::make_shared<WorkQueue>()),
    servant_(std::make_shared<CallbackReceiverI>(proxies_, workQueue_))
{
//     try...catch?
    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");
    props->setProperty("Ice.MessageSizeMax", "0");
//     props->setProperty("Ice.ACM.Client", "0");
    Ice::InitializationData initData;
    initData.properties = props;

    ich_ = Ice::initialize(initData);
    adapter_ = ich_->createObjectAdapter("");
    ident_.name = IceUtil::generateUUID();
    ident_.category = "";
}

int UvssClient::start()
{
    try {
        adapter_->add(servant_, ident_);
        adapter_->activate();
        proxies_->startHeartbeat();
        workQueue_->start(); // AMD
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_initializationCallback(-1, -1, "Initialization Failed");

        return -1;
    }

    return 1;
}

int UvssClient::connect(const std::string& ipAddress, int port)
{
    try {
        std::string endpoint(ipAddress + ":" +
            boost::lexical_cast<std::string>(port));
        if (proxies_->has(endpoint)) {
            return -2;
        }

        auto base = ich_->stringToProxy("UvssServer:tcp -h " +
            ipAddress + " -p " + boost::lexical_cast<std::string>(port));
        auto proxy = Ice::checkedCast<Uvss::CallbackSenderPrx>(base);
        if (!proxy) {
            throw std::runtime_error("Invalid proxy");
        }

//         std::cout << proxy->ice_getConnection()->getEndpoint()->toString() << std::endl;
//         std::cout << endpoint << std::endl;
//         tcp -h 192.168.1.9 -p 20145 -t 60000
//         192.168.1.9:20145

//         保留检查对端版本的功能
        if (!proxy->checkVersion(Uvss::version)) {
            return -3;
        }

        proxy->ice_getConnection()->setAdapter(adapter_);
        proxy->addProxy(ident_);

        int connectionId = proxies_->add(proxy, endpoint);

        std::string message("Server " + endpoint + ": " +
            "Connected | Connection Id: " +
            boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, 1, message.c_str());

        return connectionId;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        g_connectionCallback(-1, -2, "Connection Failed");

        return -1;
    }
}

int UvssClient::disconnect(int connectionId)
{
    std::string endpoint;
    std::shared_ptr<Uvss::CallbackSenderPrx> proxy;
    bool ok = proxies_->remove(connectionId, endpoint, proxy);
    if (ok) {
//         移除要断开的proxy，无论发生在心跳线程中的proxy副本拷贝前或后，心跳都不会检测到连接错误，不会有proxy断开的通知
//         所以只能在此处通知，不能依靠心跳线程，这里断开通知仅和移除有关
        std::string message("Server " + endpoint + ": " +
            "Disconnected | Connection Id: " +
            boost::lexical_cast<std::string>(connectionId));
        g_connectionCallback(connectionId, -3, message.c_str());

//         使server端到client的心跳失败，在server端回调通知
        proxy->ice_getConnection()->close(Ice::ConnectionClose::Gracefully);

        return 1;
    }
    else {
        return -1;
    }
}

void UvssClient::shutdown()
{
    proxies_->destroyHeartbeat();
    workQueue_->destroy();
    ich_->shutdown();
}

UvssClient::~UvssClient()
{
    proxies_->joinHeartbeat();
    workQueue_->join();
}
