#include "serverI.h"

ServerConnectionInfoCallback ServerI::serverConnectionInfoCallback = 0;
const std::string ServerI::version = "20160905";

void ServerI::addClient(const Ice::Identity& ident, const Ice::Current& curr)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	UVSS::ClientPrx clientProxy = UVSS::ClientPrx::uncheckedCast(curr.con->createProxy(ident));

	Ice::ConnectionInfoPtr info = curr.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
	std::stringstream endpoint;
	endpoint << tcpInfo->remoteAddress << ":" << tcpInfo->remotePort;

	this->clientProxyToEndpoint[clientProxy] = endpoint.str().replace(0, 7, "");//去掉开头的::ffff:

	//std::cout << endpoint.str() << std::endl;
	//std::cout << tcpInfo->remoteAddress << std::endl;
	//std::cout << tcpInfo->remotePort << std::endl;
	//std::cout << curr.con->getEndpoint()->toString() << std::endl;
}

void ServerI::useServerConnectionInfoCallback(Ice::Int type, const std::string& serverConnectionInfo, const Ice::Current&)
{
	if (this->serverConnectionInfoCallback != 0) {
		this->serverConnectionInfoCallback(type, serverConnectionInfo.c_str());
	}
}

void ServerI::heartBeat(const Ice::Current&)
{
}

bool ServerI::checkVersion(const std::string& version, const Ice::Current&)
{
	return this->version == version;
}

ServerI::ServerI(): isDestroyed(false)
{
}

void ServerI::filePathToBinary(const std::string& filePath, UVSS::ByteSeq& file)
{
	std::ifstream ifs(filePath, std::ios::binary);
	ifs.seekg(0, std::ios::end);
	std::streampos fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	file.resize(fileSize);
	ifs.read((char*)&file[0], fileSize);
}

void ServerI::sendServerUVSSImagePath(const std::string& serverUVSSImagePath, const std::string& clientUVSSImagePath)
{
	UVSS::ByteSeq serverUVSSImage;
	filePathToBinary(serverUVSSImagePath, serverUVSSImage);
	this->clientProxy->writeClientUVSSImagePath(clientUVSSImagePath, serverUVSSImage);
}

void ServerI::sendServerPlateImagePath(const std::string& serverPlateImagePath, const std::string& clientPlateImagePath)
{
	UVSS::ByteSeq serverPlateImage;
	filePathToBinary(serverPlateImagePath, serverPlateImage);
	this->clientProxy->writeClientPlateImagePath(clientPlateImagePath, serverPlateImage);
}

void ServerI::sendServerUVSSImageRelativePath(const std::string& serverUVSSImageRelativePath)
{
	this->clientProxy->writeClientUVSSImageRelativePath(serverUVSSImageRelativePath);
}

void ServerI::sendServerPlateImageRelativePath(const std::string& serverPlateImageRelativePath)
{
	this->clientProxy->writeClientPlateImageRelativePath(serverPlateImageRelativePath);
}

void ServerI::sendServerChannel(const std::string& serverChannel)
{
	this->clientProxy->writeClientChannel(serverChannel);
}

void ServerI::sendServerPlateNumber(const std::string& serverPlateNumber)
{
	this->clientProxy->writeClientPlateNumber(serverPlateNumber);
}

void ServerI::sendServerDirection(const std::string& serverDirection)
{
	this->clientProxy->writeClientDirection(serverDirection);
}

void ServerI::sendServerCheckDateTime(const std::string& serverCheckDateTime)
{
	this->clientProxy->writeClientCheckDateTime(serverCheckDateTime);
}

void ServerI::sendServerExtension(const std::string& serverExtension)
{
	this->clientProxy->writeClientExtension(serverExtension);
}

void ServerI::setServerConnectionInfoCallback(ServerConnectionInfoCallback serverConnectionInfoCallback)
{
	ServerI::serverConnectionInfoCallback = serverConnectionInfoCallback;
}

void ServerI::sendCheckInfo(const std::string& UVSSImagePath, const std::string& plateImagePath, const std::string& channel, const std::string& plateNumber, const std::string& direction, const std::string& checkDateTime, const std::string& extension)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	std::string timeName = createFileName();
	std::string UVSSImageRelativePath("UVSS\\UVSS_" + timeName);
	std::string plateImageRelativePath("UVSS\\ANPR_" + timeName);

	for (std::map<UVSS::ClientPrx, std::string>::const_iterator it = clientProxyToEndpoint.begin(); it != clientProxyToEndpoint.end();) {
		try {
			this->clientProxy = it->first;
			this->clientProxy->createClientImageDirectory("UVSS");

			sendServerUVSSImagePath(UVSSImagePath, UVSSImageRelativePath);
			sendServerUVSSImageRelativePath(UVSSImageRelativePath);
			sendServerPlateImagePath(plateImagePath, plateImageRelativePath);
			sendServerPlateImageRelativePath(plateImageRelativePath);

			sendServerChannel(channel);
			sendServerPlateNumber(plateNumber);
			sendServerDirection(direction);
			sendServerCheckDateTime(checkDateTime);
			sendServerExtension(extension);

			this->clientProxy->useClientCheckInfoCallback();

		} catch (const Ice::Exception& ex) {
			//it = clientProxyToEndpoint.erase(it);
			std::cerr << ex << std::endl;
			++it;
			continue;
		}
		
		++it;
	}
}

const std::string ServerI::createFileName(const std::string& prefix, const std::string& suffix, const std::string& extension)
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

void ServerI:: run()
{
	while (true) {
		std::map<UVSS::ClientPrx, std::string> clientProxyToEndpoint;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
			IceUtil::Monitor<IceUtil::Mutex>::timedWait(IceUtil::Time::seconds(2));

			if (this->isDestroyed) {
				return;
			}

			clientProxyToEndpoint = this->clientProxyToEndpoint;
		}

		if (!clientProxyToEndpoint.empty()) {
			for (std::map<UVSS::ClientPrx, std::string>::const_iterator it = clientProxyToEndpoint.begin(); it != clientProxyToEndpoint.end(); ++it) {
				try {
					it->first->heartBeat();
					//std::cout << it->first->ice_getConnection()->getEndpoint()->toString() << std::endl;
					//std::cout << it->first->ice_getCommunicator()->identityToString(it->first->ice_getIdentity()) << std::endl;
				} catch (...) {
					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
					//Ice::Identity ident = it->first->ice_getIdentity();

					if (!this->isDestroyed) {
						std::string endpoint = this->clientProxyToEndpoint[it->first];
						useServerConnectionInfoCallback(-1, std::string("客户端 " + endpoint + ": 已断开").c_str());
						this->clientProxyToEndpoint.erase(it->first);
					} else {
						return;
					}
				}
			}
		}
	}
}

void ServerI::destroy()
{
	{
		IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
		this->isDestroyed = true;

		notify();
	}

	getThreadControl().join();
}
