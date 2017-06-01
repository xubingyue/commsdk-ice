#include <uvssclient.h>
#include <map>
#include <boost/lexical_cast.hpp>
#include <clienti.h>
#include <clientserver.h>


#include <memory>
#include <algorithm>

#include <version.h>


UVSSClient::UVSSClient()
{
    _rpcExecutor = std::make_shared<RpcExecutor>();
    this->client = std::make_shared<ClientI>(std::make_shared<WorkQueue>(), _rpcExecutor); //写在构造函数里
    
    
    

    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.Warn.Connections", "1");//-
    //props->setProperty("Ice.MessageSizeMax", "51200");
    props->setProperty("Ice.MessageSizeMax", "2097152");
//        props->setProperty("Ice.ACM.Client", "0");
    Ice::InitializationData initData;
    initData.properties = props;
    this->ic = Ice::initialize(initData);
}

void UVSSClient::setConnectionInfoCallback(
        UVSSMessageCallback connectionInfoCallback)
{
    RpcExecutor::setConnectionInfoCallback(connectionInfoCallback);
}

void UVSSClient::setCheckInfoCallback(
        UVSSCheckInfoCallback checkInfoCallback)
{
    WorkQueue::setCheckInfoCallback(checkInfoCallback);
}

int UVSSClient::init()
{
    try {
        //以上代码考虑写在构造函数中
        
        this->id.name = IceUtil::generateUUID();
        this->id.category = "";
        this->adapter = this->ic->createObjectAdapter("");
        this->adapter->add(this->client, this->id);
        
        this->adapter->activate();
        _rpcExecutor->start(); //心跳线程
        this->client->_workQueue->start(); //AMD
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        _rpcExecutor->useConnectionInfoCallback(-1, -1, "初始化失败");

        return -1;
    }

    return 1;
}

void UVSSClient::uninit()
{
//     {
//         std::unique_lock<std::mutex> lock(this->client->_mutex);

        this->client->_workQueue->destroy();//要改成独立的调用它！！！
        
//     }

    if (this->client != 0) {
        try {
            _rpcExecutor->destroy();
        }
        catch (const Ice::Exception& ex) {
            std::cerr << ex << std::endl;
        }
    }

    //加上adapter->deactivate();？！！！

    if (this->ic != 0) {
        try {
            this->ic->destroy();
        }
        catch (const Ice::Exception& ex) {
            std::cerr << ex << std::endl;
        }
    }
    
    this->client->_workQueue->join();
    _rpcExecutor->join();
}

int UVSSClient::connect(const std::string& iPAddress, int port)
{
    std::string endpoint = iPAddress + ":" + boost::lexical_cast<std::string>(port);

        //lock
        if (_rpcExecutor->isRepeated(endpoint)) {
            return -2;
        }

//         auto base = this->ic->stringToProxy(
//                 "Server:tcp -h " + iPAddress + " -p " +
//                 boost::lexical_cast<std::string>(port))->ice_twoway()->ice_timeout(-1)->ice_secure(
//                 false);

        auto base = this->ic->stringToProxy(
            "Server:tcp -h " + iPAddress + " -p " +
            boost::lexical_cast<std::string>(port));
        
        auto server = Ice::checkedCast<UVSS::ServerPrx>(base);
        if (!server) {
            throw "Invalid proxy";
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
