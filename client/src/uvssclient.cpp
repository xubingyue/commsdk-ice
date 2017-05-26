#include <uvssclient.h>
#include <map>
#include <boost/lexical_cast.hpp>
#include <clienti.h>
#include <clientserver.h>
#include <version.h>

#include <memory>

#include <algorithm>

UVSSClient::UVSSClient()
{
}

void UVSSClient::setConnectionInfoCallback(
        UVSSMessageCallback connectionInfoCallback)
{
    ClientI::setConnectionInfoCallback(connectionInfoCallback);
}

void UVSSClient::setCheckInfoCallback(
        UVSSCheckInfoCallback checkInfoCallback)
{
    this->checkInfoCallback = checkInfoCallback;
}

int UVSSClient::init()
{
    try {
        this->client = std::make_shared<ClientI>(std::make_shared<WorkQueue>()); //写在构造函数里
        this->client->_workQueue->setCheckInfoCallback(this->checkInfoCallback); //写在UVSSClient::setCheckInfoCallback里

        Ice::PropertiesPtr props = Ice::createProperties();
        props->setProperty("Ice.Warn.Connections", "1");//-
        //props->setProperty("Ice.MessageSizeMax", "51200");
        props->setProperty("Ice.MessageSizeMax", "2097152");
//        props->setProperty("Ice.ACM.Client", "0");
        Ice::InitializationData initData;
        initData.properties = props;
        this->ic = Ice::initialize(initData);

        //以上代码考虑写在构造函数中
        
        this->id.name = IceUtil::generateUUID();
        this->id.category = "";
        this->adapter = this->ic->createObjectAdapter("");
        this->adapter->add(this->client, this->id);
        
        this->adapter->activate();
        this->client->start(); //心跳线程
        this->client->_workQueue->start(); //AMD
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        this->client->useConnectionInfoCallback(-1, -1, "初始化失败");

        return -1;
    }

    return 1;
}

void UVSSClient::uninit()
{
    {
        std::unique_lock<std::mutex> lock(this->client->_mutex);

        this->client->_workQueue->destroy();
        this->client->_workQueue->join();
    }

    if (this->client != 0) {
        try {
            this->client->destroy();
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
}

int UVSSClient::connect(const std::string& iPAddress, int port)
{
    //锁的方式需要更细致！
    std::unique_lock<std::mutex> lock(this->client->_mutex);

    try {
        std::string endpoint = iPAddress + ":" + boost::lexical_cast<std::string>(port);

        //lock
        for (auto p : this->client->serverProxyToEndpoint) {
            if (p.second == endpoint) {
                return -2;
            }
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

        //lock
        ++this->client->index;
        this->client->endpointToIndex[endpoint] = this->client->index;
        this->client->serverProxyToEndpoint[server] = endpoint;

        this->client->useConnectionInfoCallback(this->client->index, 1,
                "服务器端 " + endpoint + ": " + "已连接 | 连接标识: " + boost::lexical_cast<std::string>(this->client->index));
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        this->client->useConnectionInfoCallback(-1, -2, "连接失败");

        return -1;
    }
    catch (const char* msg) {
        std::cerr << msg << std::endl;
        this->client->useConnectionInfoCallback(-1, -2, "连接失败");

        return -1;
    }

    return this->client->index;
}

int UVSSClient::disconnect(int index)
{
    //锁的方式需要更细致！
    std::unique_lock<std::mutex> lock(this->client->_mutex);

    try {
        for (auto x : this->client->endpointToIndex) {
            if (x.second == index) {
                std::string endpoint = x.first;
                this->client->endpointToIndex.erase(endpoint);
                for (auto y : this->client->serverProxyToEndpoint) {
                    if (y.second == endpoint) {
                        //使server到client的心跳失败
                        y.first->ice_getConnection()->close(Ice::ConnectionClose::Gracefully);
                        //client不再连接server y.first
                        this->client->serverProxyToEndpoint.erase(y.first);//无须it2++

                        //只能在此处通知！不能依靠心跳线程
                        this->client->useConnectionInfoCallback(index, -3,
                                                                "服务器端 " + endpoint + ": " +
                                                                "已断开 | 连接标识: " + boost::lexical_cast<std::string>(index));

                        return 1;
                    }
                }

                return -1;//可以去掉
            }
        }

        return -1;//没有此连接
    }
    catch (...) {
        return -1;//断开失败, 以前的程序没有考虑
    }
}
