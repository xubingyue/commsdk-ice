#include <uvssclient.h>
#include <map>
#include <boost/lexical_cast.hpp>
#include <clienti.h>
#include <clientserver.h>
#include <version.h>

UVSSClient::UVSSClient()
{
}

void UVSSClient::setConnectionInfoCallback(
        ClientConnectionInfoCallback connectionInfoCallback)
{
    ClientI::setConnectionInfoCallback(connectionInfoCallback);
}

void UVSSClient::setCheckInfoCallback(
        ClientCheckInfoCallback checkInfoCallback)
{
    ClientI::setCheckInfoCallback(checkInfoCallback);
}

int UVSSClient::init()
{
    try {
        this->client = new ClientI;

        Ice::PropertiesPtr props = Ice::createProperties();
        //props->setProperty("Ice.Warn.Connections", "1");
        //props->setProperty("Ice.MessageSizeMax", "51200");
        props->setProperty("Ice.MessageSizeMax", "2097152");
        props->setProperty("Ice.ACM.Client", "0");
        Ice::InitializationData initData;
        initData.properties = props;
        this->ic = Ice::initialize(initData);

        this->id.name = IceUtil::generateUUID();
        this->id.category = "";
        this->adapter = this->ic->createObjectAdapter("");
        this->adapter->add(this->client, this->id);
        this->adapter->activate();

        this->client->start();
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
    if (this->client != 0) {
        try {
            this->client->destroy();
        }
        catch (const Ice::Exception& ex) {
            std::cerr << ex << std::endl;
        }
    }

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
//     IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this->client);
    std::unique_lock<std::mutex> lock(this->client->_mutex);

    try {
        std::string endpoint = iPAddress + ":" + boost::lexical_cast<std::string>(port);

        //lock
        for (std::map<UVSS::ServerPrx, std::string>::const_iterator
                it = this->client->serverProxyToEndpoint.begin();
                it != this->client->serverProxyToEndpoint.end(); ++it) {
            if (it->second == endpoint) {
                return -2;
            }
        }

        Ice::ObjectPrx base = this->ic->stringToProxy(
                "Server:tcp -h " + iPAddress + " -p " +
                boost::lexical_cast<std::string>(port))->ice_twoway()->ice_timeout(-1)->ice_secure(
                false);

        UVSS::ServerPrx serverProxy = UVSS::ServerPrx::checkedCast(base);
        if (serverProxy == 0) {
            throw "Invalid proxy";
        }

        if (!serverProxy->checkVersion(UVSS_COMM_SDK_VER)) {
            return -3;
        }

        serverProxy->ice_getConnection()->setAdapter(this->adapter);
        serverProxy->addClient(this->id);

        //lock
        ++this->client->index;
        this->client->endpointToIndex[endpoint] = this->client->index;
        this->client->serverProxyToEndpoint[serverProxy] = endpoint;

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
//     IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*client);
    std::unique_lock<std::mutex> lock(this->client->_mutex);

    try {
        for (std::map<std::string, int>::const_iterator
                it1 = this->client->endpointToIndex.begin();
                it1 != this->client->endpointToIndex.end(); ++it1) {
            if (it1->second == index) {
                this->client->endpointToIndex.erase(it1);//此时删除？
                std::string endpoint = it1->first;
                for (std::map<UVSS::ServerPrx, std::string>::const_iterator
                        it2 = this->client->serverProxyToEndpoint.begin();
                        it2 != this->client->serverProxyToEndpoint.end(); ++it2) {
                    if (it2->second == endpoint) {
                        //server不能连到client
                        it2->first->ice_getConnection()->close(false);
                        //client不能连到server
                        this->client->serverProxyToEndpoint.erase(it2);//无须it2++

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
