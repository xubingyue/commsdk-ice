#include "uvssserver.h"

UVSSServer::UVSSServer(): ic(0), server(new ServerI), IPAddress("127.0.0.1"), portNumber(20145)
{
	try {
		Ice::PropertiesPtr props = Ice::createProperties();
		props->setProperty("Ice.MessageSizeMax", "51200");
		//props->setProperty("Ice.MessageSizeMax", "2097152");
		props->setProperty("Ice.Warn.Connections", "1");

		Ice::InitializationData id;
		id.properties = props;

		this->ic = Ice::initialize(id);
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
	}
}

UVSSServer::~UVSSServer()
{
	if (ic) {
		try {
			ic->destroy();
		} catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	}
}

void UVSSServer::setIPAddress(const std::string& IPAddress)
{
	this->IPAddress = IPAddress;
}

void UVSSServer::setPortNumber(int portNumber)
{
	this->portNumber = portNumber;
}

int UVSSServer::init()
{
	try {
		std::stringstream port;
		port << this->portNumber;
		Ice::ObjectAdapterPtr adapter = this->ic->createObjectAdapterWithEndpoints("UVSS.Server", "tcp -h " + this->IPAddress + " -p " + port.str());
		adapter->add(server, this->ic->stringToIdentity("Server"));
		adapter->activate();
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
	UVSSServer::~UVSSServer();
}

void UVSSServer::sendUVSSImagePath(const std::string& serverUVSSImagePath)
{
	server->getClientProxy()->createClientImageDirectory();
	std::string UVSSImageRelativePath("UVSS/" + createFileName("UVSS_"));
	server->sendServerUVSSImagePath(serverUVSSImagePath, UVSSImageRelativePath);
	server->sendServerUVSSImageRelativePath(UVSSImageRelativePath);
}

void UVSSServer::sendPlateImagePath(const std::string& serverPlateImagePath)
{
	server->getClientProxy()->createClientImageDirectory();
	std::string plateImageRelativePath("UVSS/" + createFileName("ANPR_"));
	server->sendServerPlateImagePath(serverPlateImagePath, plateImageRelativePath);
	server->sendServerPlateImageRelativePath(plateImageRelativePath);
}

void UVSSServer::sendCheckInfo(const std::string& UVSSImagePath, const std::string& plateImagePath, const std::string& channel, const std::string& plateNumber, const std::string& direction, const std::string& checkDateTime, const std::string& extension)
{
	try {
		sendUVSSImagePath(UVSSImagePath);
		sendPlateImagePath(plateImagePath);
		server->sendServerChannel(channel);
		server->sendServerPlateNumber(plateNumber);
		server->sendServerDirection(direction);
		server->sendServerCheckDateTime(checkDateTime);
		server->sendServerExtension(extension);

		server->getClientProxy()->useClientCheckInfoCallback();
	} catch (...) {
		return;
	}
}

void UVSSServer::setConnectionInfoCallback(ServerConnectionInfoCallback serverConnectionInfoCallback)
{
	server->setServerConnectionInfoCallback(serverConnectionInfoCallback);
}

const std::string UVSSServer::createFileName(const std::string& prefix, const std::string& suffix, const std::string& extension)
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	std::stringstream fileName;

	fileName << prefix << systemTime.wYear
		<< std::setw(2) << std::setfill('0') << systemTime.wMonth
		<< std::setw(2) << std::setfill('0') << systemTime.wDay
		<< std::setw(2) << std::setfill('0') << systemTime.wHour
		<< std::setw(2) << std::setfill('0') << systemTime.wMinute
		<< std::setw(2) << std::setfill('0') << systemTime.wSecond
		<< std::setw(3) << std::setfill('0') << systemTime.wMilliseconds
		<< suffix << "." << extension;

	return fileName.str();
}
