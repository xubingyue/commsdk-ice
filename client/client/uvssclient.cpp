#include "uvssclient.h"

UVSSClient::UVSSClient(): ic(0), client(new ClientI), serverProxy(0), clientProxy(0),
	//IPAddress("127.0.0.1"),// portNumber(10001),
	serverIPAddress("127.0.0.1"), serverPortNumber(20145),
	isConnected(false)
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

UVSSClient::~UVSSClient()
{
	if (ic) {
		try {
			ic->destroy();
		} catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	}
}

//void UVSSClient::setIPAddress(const std::string& IPAddress)
//{
//	this->IPAddress = IPAddress;
//}
//
//void UVSSClient::setPortNumber(int portNumber)
//{
//	this->portNumber = portNumber;
//}

void UVSSClient::setServerIPAddress(const std::string& serverIPAddress)
{
	this->serverIPAddress = serverIPAddress;
}

void UVSSClient::setServerPortNumber(int serverPortNumber)
{
	this->serverPortNumber = serverPortNumber;
}

int UVSSClient::init()
{
	try {
		//std::stringstream port;
		//port << this->portNumber;
		//Ice::ObjectAdapterPtr adapter = this->ic->createObjectAdapterWithEndpoints("UVSS.Client", "tcp -h " + this->IPAddress + " -p " + port.str());
		//adapter->add(client, ic->stringToIdentity("Client"));

		this->adapter = this->ic->createObjectAdapter("");

		this->ident.name = IceUtil::generateUUID();
		this->ident.category = "";
		this->adapter->add(client, ident);
		this->adapter->activate();

		this->clientProxy = UVSS::ClientPrx::uncheckedCast(adapter->createProxy(ic->stringToIdentity("Client")));
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		client->useClientConnectionInfoCallback(-1, -1, "初始化失败");
		return -1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		client->useClientConnectionInfoCallback(-1, -1, "初始化失败");
		return -1;
	}
	return 1;
}

void UVSSClient::uninit()
{
	UVSSClient::~UVSSClient();
}

int UVSSClient::connect(const std::string& serverIPAddress, int serverPortNumber)
{
	if (isConnected) {
		return -2;
	}

	setServerIPAddress(serverIPAddress);
	setServerPortNumber(serverPortNumber);

	try {
		std::stringstream serverPort;
		serverPort << this->serverPortNumber;
		Ice::ObjectPrx base = this->ic->stringToProxy("Server:tcp -h " + this->serverIPAddress + " -p " + serverPort.str())->ice_twoway()->ice_timeout(-1)->ice_secure(false);
		this->serverProxy = UVSS::ServerPrx::checkedCast(base);
		if (serverProxy == 0) {
			std::cerr << "invalid proxy" << std::endl;
			throw "Invalid proxy";
			//return -1;
		}
		serverProxy->ice_getConnection()->setAdapter(this->adapter);
		serverProxy->addClient(ident);

		Ice::ConnectionInfoPtr info = serverProxy->ice_getConnection()->getInfo();
		Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
		this->IPAddress = tcpInfo->localAddress;
		this->portNumber = tcpInfo->localPort;
		//if (tcpInfo) {
		//	std::cout << "local address = " << tcpInfo->localAddress << ":"
		//		<< tcpInfo->localPort << std::endl;
		//	std::cout << "remote address = " << tcpInfo->remoteAddress << ":"
		//		<< tcpInfo->remotePort << std::endl;
		//}

		std::stringstream port;
		port << this->portNumber;

		serverProxy->useServerConnectionInfoCallback(0, "客户端 " + IPAddress + ":" + port.str() + ": 已连接");
		client->useClientConnectionInfoCallback(1, 1, "服务器端 " + this->serverIPAddress + ":" + serverPort.str() + ": " + "已连接 | 连接标识: 1");
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		client->useClientConnectionInfoCallback(-1, -2, "连接失败");
		//此处服务器应该检测到客户断开, 要加上!
		return -1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		client->useClientConnectionInfoCallback(-1, -2, "连接失败");
		//此处服务器应该检测到客户断开, 要加上!
		return -1;
	}
	isConnected = true;
	return 1;
}

int UVSSClient::disconnect(int handle)
{
	if (!isConnected) {
		return -1;
	}

	try {
		//有问题, 应该是服务器检测到
		//Ice::ConnectionInfoPtr info = serverProxy->ice_getConnection()->getInfo();
		//Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

		std::stringstream port;
		port << this->portNumber;

		serverProxy->useServerConnectionInfoCallback(-1, "客户端 " + IPAddress + ":" + port.str() + ": 已断开");
		//serverProxy->ice_getConnection()->close(true);//不确定
		//this->ic->shutdown();
		Ice::Identity ident;
		//this->ident.name = "";
		//this->ident.category = "";
		serverProxy->addClient(ident);

		std::stringstream serverPort;
		serverPort << this->serverPortNumber;
		client->useClientConnectionInfoCallback(1, -3, "服务器端 " + this->serverIPAddress + ":" + serverPort.str() + ": " + "已断开 | 连接标识: 1");
	} catch(...) {
		return -1;//断开失败, 以前的程序没有考虑
	}
	isConnected = false;
	return 1;
}

void UVSSClient::setConnectionInfoCallback(ClientConnectionInfoCallback clientConnectionInfoCallback)
{
	client->setClientConnectionInfoCallback(clientConnectionInfoCallback);
}

void UVSSClient::setCheckInfoCallback(ClientCheckInfoCallback clientCheckInfoCallback)
{
	client->setClientCheckInfoCallback(clientCheckInfoCallback);
}

//void UVSSClient::stopServer()
//{
//	serverProxy->shutdown();
//}
