#include "uvssclient.h"

const std::string UVSSClient::version = "20160905";

UVSSClient::UVSSClient()
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
		//props->setProperty("Ice.Warn.Connections", "1");
		//props->setProperty("Ice.MessageSizeMax", "51200");
		props->setProperty("Ice.MessageSizeMax", "2097152");

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
}

int UVSSClient::connect(const std::string& serverIPAddress, int serverPortNumber)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*client);

	try {
		std::stringstream serverPort;
		serverPort << serverPortNumber;
		std::string endpoint = serverIPAddress + ":" + serverPort.str();

		for (std::map<UVSS::ServerPrx, ClientI::ID>::const_iterator it = client->serverProxyToID.begin(); it != client->serverProxyToID.end(); ++it) {
			if (it->second.endpoint == endpoint) {
				return -2;
			}
		}

		Ice::ObjectPrx base = this->ic->stringToProxy("Server:tcp -h " + serverIPAddress + " -p " + serverPort.str())->ice_twoway()->ice_timeout(-1)->ice_secure(false);

		UVSS::ServerPrx serverProxy = UVSS::ServerPrx::checkedCast(base);
		if (serverProxy == 0) {
			throw "Invalid proxy";
		}

		if (!serverProxy->checkVersion(version)) {
			return -3;
		}

		serverProxy->ice_getConnection()->setAdapter(this->adapter);
		serverProxy->addClient(ident);

		Ice::ConnectionInfoPtr info = serverProxy->ice_getConnection()->getInfo();
		Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

		//if (tcpInfo) {
		//}

		std::string IPAddress = tcpInfo->localAddress;
		int portNumber = tcpInfo->localPort;
		
		std::stringstream port;
		port << portNumber;
		serverProxy->useServerConnectionInfoCallback(0, "�ͻ��� " + IPAddress + ":" + port.str() + ": ������");

		++client->index;

		std::stringstream idx;
		idx << client->index;
		client->useClientConnectionInfoCallback(client->index, 1, "�������� " + endpoint + ": " + "������ | ���ӱ�ʶ: " + idx.str());

		client->serverProxyToID[serverProxy].index = client->index;
		client->serverProxyToID[serverProxy].endpoint = endpoint;
	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		client->useClientConnectionInfoCallback(-1, -2, "����ʧ��");
		return -1;
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		client->useClientConnectionInfoCallback(-1, -2, "����ʧ��");
		return -1;
	}
	return client->index;
}

int UVSSClient::disconnect(int handle)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*client);

	try {
		for (std::map<UVSS::ServerPrx, ClientI::ID>::const_iterator it = client->serverProxyToID.begin(); it != client->serverProxyToID.end();) {
			if (it->second.index == handle) {
				it->first->ice_getConnection()->close(false);
				std::stringstream idx;
				idx << handle;
				this->client->useClientConnectionInfoCallback(handle, -3, "�������� " + it->second.endpoint + ": " + "�ѶϿ� | ���ӱ�ʶ: " + idx.str());
				client->serverProxyToID.erase(it++);
				return 1;
			} else {
				++it;
			}
		}
		return -1;
	} catch(...) {
		return -1;//�Ͽ�ʧ��, ��ǰ�ĳ���û�п���
	}
}

void UVSSClient::setConnectionInfoCallback(ClientConnectionInfoCallback clientConnectionInfoCallback)
{
	ClientI::setClientConnectionInfoCallback(clientConnectionInfoCallback);
}

void UVSSClient::setCheckInfoCallback(ClientCheckInfoCallback clientCheckInfoCallback)
{
	ClientI::setClientCheckInfoCallback(clientCheckInfoCallback);
}
