#include <uvssclient.h>

#include <map>
#include <memory>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include <callbackreceiveri.h>
#include <version.h>

UVSSInitializeCallback UVSSClient::initializeCallback = 0;
ConnectCallback UVSSClient::ccb_ = 0;

UVSSClient::UVSSClient() :
    peerProxies_(std::make_shared<PeerProxies>()),
    _workQueue(std::make_shared<WorkQueue>()),
    client(std::make_shared<CallbackReceiverI>(peerProxies_, _workQueue))
{
    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");//-
    props->setProperty("Ice.MessageSizeMax", "2097152"); // 51200
//     props->setProperty("Ice.ACM.Client", "0");
    Ice::InitializationData initData;
    initData.properties = props;
    this->ic = Ice::initialize(initData);
    this->adapter = this->ic->createObjectAdapter("");
    this->id.name = IceUtil::generateUUID();
    this->id.category = "";
}

int UVSSClient::init()
{
    try {
        this->adapter->add(this->client, this->id);
        this->adapter->activate();
        peerProxies_->start(); //心跳线程
        _workQueue->start(); //AMD
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;

        initializeCallback(-1, -1, "初始化失败");

        return -1;
    }

    return 1;
}

void UVSSClient::uninit()
{
    try {
        peerProxies_->destroy();
        _workQueue->destroy();
//         加上adapter->deactivate();？！！！
        this->ic->destroy(); //shutdown?
        peerProxies_->join();
        _workQueue->join();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int UVSSClient::connect(const std::string& iPAddress, int port)
{
    try {
        std::string endpoint = iPAddress + ":" + boost::lexical_cast<std::string>(port);
        int index;
        if (peerProxies_->isRepeated(endpoint)) {
            return -2;
        }

        auto base = this->ic->stringToProxy(
                        "UvssServer:tcp -h " + iPAddress + " -p " +
                        boost::lexical_cast<std::string>(port));
        auto server = Ice::checkedCast<Uvss::CallbackSenderPrx>(base);
        if (!server) {
            std::cerr << "Invalid proxy" << std::endl;
            ccb_(-1, -2, "连接失败");
            return -1;
        }

//         std::cout << server->ice_getConnection()->getEndpoint()->toString() << std::endl;
//         std::cout << endpoint << std::endl;
//         tcp -h 192.168.1.9 -p 20145 -t 60000
//         192.168.1.9:20145

//         考虑去掉个功能
        if (!server->checkVersion(UVSS_COMM_SDK_VER)) {
            return -3;
        }

        server->ice_getConnection()->setAdapter(this->adapter);
        server->addClient(this->id);

        index = peerProxies_->add(server, endpoint);

        std::string message("服务器端 " + endpoint + ": " + "已连接 | 连接标识: " + boost::lexical_cast<std::string>(index));
        ccb_(index, 1, message.c_str());

        return index;
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        ccb_(-1, -2, "连接失败");

        return -1;
    }
}

int UVSSClient::disconnect(int index)
{
    std::string endpoint;
    bool ret = peerProxies_->findAndRemove(index, endpoint);
    if (ret) {
//         移除要断开的server代理，无论发生在心跳线程中的servers副本拷贝前或后，心跳都不会发生连接错误，不会有server断开的通知
//         所以只能在此处通知！不能依靠心跳线程
        std::string message("服务器端 " + endpoint + ": " +
                            "已断开 | 连接标识: " + boost::lexical_cast<std::string>(index));
        ccb_(index, -3, message.c_str());
        return 1;
    } else {
        return -1;
    }
}

void UVSSClient::setInitializeCallback(UVSSInitializeCallback cb)
{
    initializeCallback = cb;
}

void UVSSClient::setCCB(ConnectCallback ccb)
{
    ccb_ = ccb;
}

void UVSSClient::setConnectionCallback(
    UVSSMessageCallback connectionCallback)
{
    PeerProxies::setConnectionCallback(connectionCallback);
}

void UVSSClient::setCheckInfoCallback(
    UVSSCheckInfoCallback checkInfoCallback)
{
    WorkQueue::setCheckInfoCallback(checkInfoCallback);
}
