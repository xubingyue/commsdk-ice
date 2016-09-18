#include "serverI.h"

ServerConnectionInfoCallback ServerI::serverConnectionInfoCallback = 0;

void ServerI::addClient(const Ice::Identity& ident, const Ice::Current& curr)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	UVSS::ClientPrx clientProxy = UVSS::ClientPrx::uncheckedCast(curr.con->createProxy(ident));

	Ice::ConnectionInfoPtr info = curr.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
	std::stringstream endpoint;
	endpoint << tcpInfo->remoteAddress << ":" << tcpInfo->remotePort;

	this->clientProxyToEndpoint[clientProxy] = endpoint.str().replace(0, 7, "");//ȥ����ͷ��::ffff:

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
	return COMMUNICATION_SDK_VERSION == version;
}

ServerI::ServerI() : isDestroyed(false)
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

			std::tr2::sys::path p1(UVSSImagePath);
			UVSS::ByteSeq serverUVSSImage;
			if (std::tr2::sys::exists(p1)) {
				filePathToBinary(UVSSImagePath, serverUVSSImage);
			}
			else {
				UVSSImageRelativePath = "";
			}

			std::tr2::sys::path p2(plateImagePath);
			UVSS::ByteSeq serverPlateImage;
			if (std::tr2::sys::exists(p2)) {
				filePathToBinary(plateImagePath, serverPlateImage);
			}
			else {
				plateImageRelativePath = "";
			}

			this->clientProxy->writeCheckInfo(
				UVSSImageRelativePath,
				serverUVSSImage,
				plateImageRelativePath,
				serverPlateImage,
				channel,
				plateNumber,
				direction,
				checkDateTime,
				extension
				);
		}
		catch (const Ice::Exception& ex) {
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
	auto now = std::chrono::system_clock::now();

	std::time_t timer = std::chrono::system_clock::to_time_t(now);
	std::tm timeInfo = *std::localtime(&timer);

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	auto s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
	auto msPart = ms - s;

	std::stringstream fileName;
	fileName << prefix << std::put_time(&timeInfo, "%Y%m%d%H%M%S")
		<< std::setw(3) << std::setfill('0') << msPart.count()
		<< suffix << '.' << extension;

	return fileName.str();
}

void ServerI::run()
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
				}
				catch (...) {
					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
					//Ice::Identity ident = it->first->ice_getIdentity();

					if (!this->isDestroyed) {
						std::string endpoint = this->clientProxyToEndpoint[it->first];
						useServerConnectionInfoCallback(-1, std::string("�ͻ��� " + endpoint + ": �ѶϿ�").c_str());
						this->clientProxyToEndpoint.erase(it->first);
					}
					else {
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
