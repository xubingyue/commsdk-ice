#include <uvssclient.h>
#include <map>
#include <boost/lexical_cast.hpp>
#include <clienti.h>
#include <clientserver.h>
#include <version.h>

#include <memory>

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
        this->client = std::make_shared<ClientI>(std::make_shared<WorkQueue>());
        this->client->_workQueue->setCheckInfoCallback(this->checkInfoCallback);

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
        this->client->_workQueue->start();
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        this->client->useConnectionInfoCallback(-1, -1, "�1�7�1�7�0�3�1�7�1�7�0�2�1�7�1�7");

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
    //�1�7�1�7�1�7�0�9�1�7�0�4�1�7�1�7�0�8�1�7�1�7�0�6�1�7�0�5�1�7
    std::unique_lock<std::mutex> lock(this->client->_mutex);

    try {
        std::string endpoint = iPAddress + ":" + boost::lexical_cast<std::string>(port);

        //lock
        for (auto p : this->client->serverProxyToEndpoint) {
            if (p.second == endpoint) {
                return -2;
            }
        }

        auto base = this->ic->stringToProxy(
                "Server:tcp -h " + iPAddress + " -p " +
                boost::lexical_cast<std::string>(port))->ice_twoway()->ice_timeout(-1)->ice_secure(
                false);

        auto serverProxy = Ice::checkedCast<UVSS::ServerPrx>(base);
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
                "�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7 " + endpoint + ": " + "�1�7�1�7�1�7�1�7�1�7�1�7 | �1�7�1�7�1�7�0�9�1�7�0�7: " + boost::lexical_cast<std::string>(this->client->index));
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        this->client->useConnectionInfoCallback(-1, -2, "�1�7�1�7�1�7�1�7�0�2�1�7�1�7");

        return -1;
    }
    catch (const char* msg) {
        std::cerr << msg << std::endl;
        this->client->useConnectionInfoCallback(-1, -2, "�1�7�1�7�1�7�1�7�0�2�1�7�1�7");

        return -1;
    }

    return this->client->index;
}

int UVSSClient::disconnect(int index)
{
    //�1�7�1�7�1�7�0�9�1�7�0�4�1�7�1�7�0�8�1�7�1�7�0�6�1�7�0�5�1�7
    std::unique_lock<std::mutex> lock(this->client->_mutex);

    try {
        for (auto x : this->client->endpointToIndex) {
            if (x.second == index) {
                std::string endpoint = x.first;
                this->client->endpointToIndex.erase(endpoint);
                for (auto y : this->client->serverProxyToEndpoint) {
                    if (y.second == endpoint) {
                        //server�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7client
                        y.first->ice_getConnection()->close(Ice::ConnectionClose::Forcefully);
                        //client�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7server
                        this->client->serverProxyToEndpoint.erase(y.first);//�1�7�1�7�1�7�1�7it2++

                        //�0�1�1�7�1�7�1�7�1�0�0�4�1�7�0�0�0�4�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�9�1�7
                        this->client->useConnectionInfoCallback(index, -3,
                                                                "�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7 " + endpoint + ": " +
                                                                "�1�7�0�6�0�3�1�7 | �1�7�1�7�1�7�0�9�1�7�0�7: " + boost::lexical_cast<std::string>(index));

                        return 1;
                    }
                }

                return -1;//�1�7�1�7�1�7�1�7�0�4�1�7�1�7
            }
        }

        return -1;//�0�4�1�7�Մ1�7�1�7�1�7�1�7�1�7
    }
    catch (...) {
        return -1;//�1�7�0�3�1�7�0�2�1�7�1�7, �1�7�1�7�0�2�1�7�0�5�1�7�1�7�1�7�0�4�1�7��1�7�1�7�1�7
    }
}
