#include "serverI.h"

ServerConnectionInfoCallback ServerI::serverConnectionInfoCallback = 0;

void ServerI::addClient(const Ice::Identity& ident, const Ice::Current& curr)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	UVSS::ClientPrx clientProxy = UVSS::ClientPrx::uncheckedCast(curr.con->createProxy(ident));
	this->clientProxies.insert(clientProxy);

	Ice::ConnectionInfoPtr info = curr.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

	std::stringstream endpoint;
	endpoint << tcpInfo->remoteAddress << ":" << tcpInfo->remotePort;
	this->identityToEndpoint[ident] = endpoint.str().replace(0, 7, "");//去掉开头的::ffff:
	
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

void ServerI::sendUVSSImagePath(const std::string& serverUVSSImagePath)
{
	this->clientProxy->createClientImageDirectory("UVSS");
	std::string UVSSImageRelativePath("UVSS/" + createFileName("UVSS_"));
	sendServerUVSSImagePath(serverUVSSImagePath, UVSSImageRelativePath);
	sendServerUVSSImageRelativePath(UVSSImageRelativePath);
}

void ServerI::sendPlateImagePath(const std::string& serverPlateImagePath)
{
	this->clientProxy->createClientImageDirectory("UVSS");
	std::string plateImageRelativePath("UVSS/" + createFileName("ANPR_"));
	sendServerPlateImagePath(serverPlateImagePath, plateImageRelativePath);
	sendServerPlateImageRelativePath(plateImageRelativePath);
}

void ServerI::sendCheckInfo(const std::string& UVSSImagePath, const std::string& plateImagePath, const std::string& channel, const std::string& plateNumber, const std::string& direction, const std::string& checkDateTime, const std::string& extension)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	for (std::set<UVSS::ClientPrx>::const_iterator it = clientProxies.begin(); it != clientProxies.end();) {
		try {
			this->clientProxy = *it;

			sendUVSSImagePath(UVSSImagePath);
			sendPlateImagePath(plateImagePath);
			sendServerChannel(channel);
			sendServerPlateNumber(plateNumber);
			sendServerDirection(direction);
			sendServerCheckDateTime(checkDateTime);
			sendServerExtension(extension);

			this->clientProxy->useClientCheckInfoCallback();

			++it;
		} catch (const Ice::Exception& ex) {
			it = clientProxies.erase(it);
			std::cerr << ex << std::endl;
		}
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
		std::set<UVSS::ClientPrx> clientProxies;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
			IceUtil::Monitor<IceUtil::Mutex>::timedWait(IceUtil::Time::seconds(2));

			if (this->isDestroyed) {
				break;
			}

			clientProxies = this->clientProxies;
		}

		if (!clientProxies.empty()) {
			for (std::set<UVSS::ClientPrx>::const_iterator it = clientProxies.begin(); it != clientProxies.end(); ++it) {
				try {
					(*it)->heartBeat();
					//std::cout << (*it)->ice_getConnection()->getEndpoint()->toString() << std::endl;
					//std::cout << (*it)->ice_getCommunicator()->identityToString((*it)->ice_getIdentity()) << std::endl;
					//++it;
				} catch (...) {
					Ice::Identity ident = (*it)->ice_getIdentity();
					std::string endpoint = this->identityToEndpoint[ident];
					this->useServerConnectionInfoCallback(-1, std::string("客户端 " + endpoint + ": 已断开").c_str());
					//it = clientProxies.erase(it);

					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
					this->clientProxies.erase(*it);
					this->identityToEndpoint.erase(ident);
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
