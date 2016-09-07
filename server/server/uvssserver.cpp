#include "uvssserver.h"

UVSSServer::UVSSServer(): ic(0), server(0), portNumber(20145)
{
}

UVSSServer::~UVSSServer()
{
}

void UVSSServer::setPortNumber(int portNumber)
{
	this->portNumber = portNumber;
}

int UVSSServer::init()
{
	try {
		server = new ServerI;

		Ice::PropertiesPtr props = Ice::createProperties();
		props->setProperty("Ice.MessageSizeMax", "51200");
		//props->setProperty("Ice.MessageSizeMax", "2097152");
		//props->setProperty("Ice.Warn.Connections", "1");

		Ice::InitializationData id;
		id.properties = props;

		this->ic = Ice::initialize(id);

		std::stringstream port;
		port << this->portNumber;
		//Ice::ObjectAdapterPtr adapter = this->ic->createObjectAdapterWithEndpoints("UVSS.Server", "tcp -h " + this->IPAddress + " -p " + port.str());
		Ice::ObjectAdapterPtr adapter = this->ic->createObjectAdapterWithEndpoints("UVSS.Server", "tcp -p " + port.str());
		adapter->add(server, this->ic->stringToIdentity("Server"));
		adapter->activate();

		server->start();
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return -1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		return -1;
	}
	return 0;
}

void UVSSServer::uninit()
{
	if (this->server != 0) {
		try {
			this->server->destroy();
		} catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	}

	if (this->ic != 0) {
		try {
			this->ic->destroy();
		} catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	}
}

void UVSSServer::sendCheckInfo(const std::string& UVSSImagePath, const std::string& plateImagePath, const std::string& channel, const std::string& plateNumber, const std::string& direction, const std::string& checkDateTime, const std::string& extension)
{
	server->sendCheckInfo(UVSSImagePath, plateImagePath, channel, plateNumber, direction, checkDateTime, extension);
}

void UVSSServer::setConnectionInfoCallback(ServerConnectionInfoCallback serverConnectionInfoCallback)
{
	ServerI::setServerConnectionInfoCallback(serverConnectionInfoCallback);
}
