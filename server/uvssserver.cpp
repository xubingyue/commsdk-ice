#include "uvssserver.h"
#include <sstream>
#include "serveri.h"

UVSSServer::UVSSServer() : port(20145)
{
}

void UVSSServer::setConnectionInfoCallback(
        ServerConnectionInfoCallback connectionInfoCallback)
{
    ServerI::setConnectionInfoCallback(connectionInfoCallback);
}

void UVSSServer::setPort(int port)
{
    this->port = port;
}

int UVSSServer::init()
{
    try {
        this->server = new ServerI;

        Ice::PropertiesPtr props = Ice::createProperties();
        //props->setProperty("Ice.Warn.Connections", "1");
        //props->setProperty("Ice.MessageSizeMax", "51200");
        props->setProperty("Ice.MessageSizeMax", "2097152");
        Ice::InitializationData id;
        id.properties = props;
        this->ic = Ice::initialize(id);

        std::stringstream port;
        port << this->port;
        Ice::ObjectAdapterPtr adapter =
                this->ic->createObjectAdapterWithEndpoints(
               "UVSS.Server", "tcp -p " + port.str());
        adapter->add(this->server, this->ic->stringToIdentity("Server"));
        adapter->activate();

        this->server->start();
    }
    catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;

        return -1;
    }

    return 0;
}

void UVSSServer::uninit()
{
    if (this->server != 0) {
        try {
            this->server->destroy();
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

void UVSSServer::sendCheckInfo(
        const std::string& uVSSImagePath, const std::string& plateImagePath,
        const std::string& channel, const std::string& plateNumber,
        const std::string& direction, const std::string& time,
        const std::string& extension)
{
    this->server->sendCheckInfo(uVSSImagePath, plateImagePath,
            channel, plateNumber, direction, time, extension);
}
