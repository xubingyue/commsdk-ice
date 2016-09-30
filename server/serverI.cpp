#include "serverI.h"

ServerConnectionInfoCallback ServerI::serverConnectionInfoCallback = 0;

void ServerI::addClient(const Ice::Identity& ident, const Ice::Current& curr)
{
	IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

	UVSS::ClientPrx clientProxy = UVSS::ClientPrx::uncheckedCast(curr.con->createProxy(ident));

	Ice::ConnectionInfoPtr info = curr.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
	std::stringstream endpoint;
	endpoint << tcpInfo->remoteAddress.replace(0, 7, "") << ":" << tcpInfo->remotePort;//去掉开头的::ffff:

	this->clientProxyToEndpoint[clientProxy] = endpoint.str();

	if (this->serverConnectionInfoCallback != 0) {
		this->serverConnectionInfoCallback(0, std::string("客户端 " + endpoint.str() + ": 已连接").c_str());
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

	std::string timeName = createCurrentTime();

	std::tr2::sys::path p1(UVSSImagePath);
	std::string UVSSImageName;
	UVSS::ByteSeq serverUVSSImage;
	if (std::tr2::sys::exists(p1)) {
		UVSSImageName = "UVSS_" + timeName + ".jpg";
		filePathToBinary(UVSSImagePath, serverUVSSImage);
	}

	std::tr2::sys::path p2(plateImagePath);
	std::string plateImageName;
	UVSS::ByteSeq serverPlateImage;
	if (std::tr2::sys::exists(p2)) {
		plateImageName = "ANPR_" + timeName + ".jpg";
		filePathToBinary(plateImagePath, serverPlateImage);
	}

	for (std::map<UVSS::ClientPrx, std::string>::const_iterator it = clientProxyToEndpoint.begin(); it != clientProxyToEndpoint.end();) {
		try {
			it->first->writeCheckInfo(
				UVSSImageName,
				serverUVSSImage,
				plateImageName,
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

const std::string ServerI::createCurrentTime()
{
	auto now = std::chrono::system_clock::now();

	std::time_t timer = std::chrono::system_clock::to_time_t(now);
	std::tm timeInfo = *std::localtime(&timer);

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	auto s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
	auto msPart = ms - s;

	std::stringstream currentTime;
	currentTime << std::put_time(&timeInfo, "%Y%m%d%H%M%S")
		<< std::setw(3) << std::setfill('0') << msPart.count();

	return currentTime.str();
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
					it->first->ice_ping();
					//std::cout << it->first->ice_getConnection()->getEndpoint()->toString() << std::endl;
					//std::cout << it->first->ice_getCommunicator()->identityToString(it->first->ice_getIdentity()) << std::endl;
				}
				catch (...) {
					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
					//Ice::Identity ident = it->first->ice_getIdentity();

					if (!this->isDestroyed) {
						std::string endpoint = this->clientProxyToEndpoint[it->first];
						if (serverConnectionInfoCallback != 0) {
							serverConnectionInfoCallback(-1, std::string("客户端 " + endpoint + ": 已断开").c_str());
						}
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
