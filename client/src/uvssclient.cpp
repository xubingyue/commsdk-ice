#include <uvssclient.h>
#include <map>
#include <boost/lexical_cast.hpp>
#include <clienti.h>
#include <clientserver.h>

#include <memory>
#include <algorithm>

#include <version.h>

UVSSInitializeCallback UVSSClient::initializeCallback = 0;

UVSSClient::UVSSClient() :
    _rpcExecutor(std::make_shared<RpcExecutor>()),
    _workQueue(std::make_shared<WorkQueue>()),
    client(std::make_shared<ClientI>(_rpcExecutor, _workQueue))
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
        _rpcExecutor->start(); //心跳线程
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
        _rpcExecutor->destroy();
        _workQueue->destroy();
//         加上adapter->deactivate();？！！！
        this->ic->destroy(); //shutdown?
        _rpcExecutor->join();
        _workQueue->join();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int UVSSClient::connect(const std::string& iPAddress, int port)
{
    // 考虑是否改变
    std::string endpoint = iPAddress + ":" + boost::lexical_cast<std::string>(port);

    if (_rpcExecutor->isRepeated(endpoint)) {
        return -2;
    }

//     auto base = this->ic->stringToProxy(
//             "Server:tcp -h " + iPAddress + " -p " +
//             boost::lexical_cast<std::string>(port))->ice_twoway()->ice_timeout(-1)->ice_secure(
//             false);

    auto base = this->ic->stringToProxy(
        "Server:tcp -h " + iPAddress + " -p " +
        boost::lexical_cast<std::string>(port));
    auto server = Ice::checkedCast<UVSS::ServerPrx>(base);
    if (!server) {
        throw "Invalid proxy";
        //有问题！！！！
        //return -1 //callback
    }

// std::cout << server->ice_getConnection()->getEndpoint()->toString() << std::endl;
// std::cout << endpoint << std::endl;
// tcp -h 192.168.1.9 -p 20145 -t 60000
// 192.168.1.9:20145

    if (!server->checkVersion(UVSS_COMM_SDK_VER)) {
        return -3;
    }

    server->ice_getConnection()->setAdapter(this->adapter);
    server->addClient(this->id);

    return _rpcExecutor->connect(server, endpoint);
}

int UVSSClient::disconnect(int index)
{
    return _rpcExecutor->disconnect(index);
}

void UVSSClient::setInitializeCallback(UVSSInitializeCallback cb)
{
    initializeCallback = cb;
}

void UVSSClient::setConnectionCallback(
        UVSSMessageCallback connectionCallback)
{
    RpcExecutor::setConnectionCallback(connectionCallback);
}

void UVSSClient::setCheckInfoCallback(
        UVSSCheckInfoCallback checkInfoCallback)
{
    WorkQueue::setCheckInfoCallback(checkInfoCallback);
}
