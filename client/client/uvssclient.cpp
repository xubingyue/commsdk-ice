#include "uvssclient.h"

UVSSClient::UVSSClient(): ic(0), client(0)
{
}

UVSSClient::~UVSSClient()
{
}

int UVSSClient::init()
{
	try {
		client = new ClientI;

		Ice::PropertiesPtr props = Ice::createProperties();
		props->setProperty("Ice.MessageSizeMax", "51200");
		//props->setProperty("Ice.MessageSizeMax", "2097152");
		//props->setProperty("Ice.Warn.Connections", "1");
		props->setProperty("Ice.ACM.Client", "0");

		Ice::InitializationData id;
		id.properties = props;

		this->ic = Ice::initialize(id);

		this->ident.name = IceUtil::generateUUID();
		this->ident.category = "";

		this->adapter = this->ic->createObjectAdapter("");
		this->adapter->add(this->client, this->ident);
		this->adapter->activate();

		client->start();
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		client->useClientConnectionInfoCallback(-1, -1, "��ʼ��ʧ��");
		return -1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		client->useClientConnectionInfoCallback(-1, -1, "��ʼ��ʧ��");
		return -1;
	}
	return 1;
}

void UVSSClient::uninit()
{
	if (this->client != 0) {
		try {
			this->client->destroy();
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

	if (this->client->isConnected) {
		this->client->isConnected = false;
	}
}

int UVSSClient::connect(const std::string& serverIPAddress, int serverPortNumber)
{
	if (client->isConnected) {
		return -2;
	}

	client->setServerIPAddress(serverIPAddress);
	client->setServerPortNumber(serverPortNumber);

	try {
		std::stringstream serverPort;
		serverPort << serverPortNumber;
		Ice::ObjectPrx base = this->ic->stringToProxy("Server:tcp -h " + serverIPAddress + " -p " + serverPort.str())->ice_twoway()->ice_timeout(-1)->ice_secure(false);

		//IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

		client->serverProxy = UVSS::ServerPrx::checkedCast(base);
		if (client->serverProxy == 0) {
			std::cerr << "invalid proxy" << std::endl;
			throw "Invalid proxy";
			//return -1;
		}
		client->serverProxy->ice_getConnection()->setAdapter(this->adapter);
		client->serverProxy->addClient(ident);

		Ice::ConnectionInfoPtr info = client->serverProxy->ice_getConnection()->getInfo();
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

		client->serverProxy->useServerConnectionInfoCallback(0, "�ͻ��� " + this->IPAddress + ":" + port.str() + ": ������");
		client->useClientConnectionInfoCallback(1, 1, "�������� " + client->getServerIPAddress() + ":" + serverPort.str() + ": " + "������ | ���ӱ�ʶ: 1");
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		client->useClientConnectionInfoCallback(-1, -2, "����ʧ��");
		return -1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		client->useClientConnectionInfoCallback(-1, -2, "����ʧ��");
		return -1;
	}
	client->isConnected = true;
	return 1;
}

int UVSSClient::disconnect(int handle)
{
	if (!client->isConnected) {
		return -1;
	}

	try {
		client->serverProxy->ice_getConnection()->close(true);

		std::stringstream serverPort;
		serverPort << client->getServerPortNumber();
		this->client->useClientConnectionInfoCallback(1, -3, "�������� " + client->getServerIPAddress() + ":" + serverPort.str() + ": " + "�ѶϿ� | ���ӱ�ʶ: 1");
	} catch(...) {
		return -1;//�Ͽ�ʧ��, ��ǰ�ĳ���û�п���
	}
	client->isConnected = false;
	return 1;
}

void UVSSClient::setConnectionInfoCallback(ClientConnectionInfoCallback clientConnectionInfoCallback)
{
	ClientI::setClientConnectionInfoCallback(clientConnectionInfoCallback);
}

void UVSSClient::setCheckInfoCallback(ClientCheckInfoCallback clientCheckInfoCallback)
{
	ClientI::setClientCheckInfoCallback(clientCheckInfoCallback);
}
