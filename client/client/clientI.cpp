#include "clientI.h"

ClientConnectionInfoCallback ClientI::clientConnectionInfoCallback = 0;
ClientCheckInfoCallback ClientI::clientCheckInfoCallback = 0;

void ClientI::writeCheckInfo(
	const std::string& clientUVSSImagePath, const UVSS::ByteSeq& serverUVSSImage,
	const std::string& clientPlateImagePath, const UVSS::ByteSeq& serverPlateImage,
	const std::string& serverChannel,
	const std::string& serverPlateNumber,
	const std::string& serverDirection,
	const std::string& serverCheckDateTime,
	const std::string& serverExtension,
	const Ice::Current& curr) {

	createClientImageDirectory("UVSS");

	std::ofstream ofs1(clientUVSSImagePath, std::ios::binary);
	ofs1.write((char*)&serverUVSSImage[0], serverUVSSImage.size());

	std::ofstream ofs2(clientPlateImagePath, std::ios::binary);
	ofs2.write((char*)&serverPlateImage[0], serverPlateImage.size());

	if (this->clientCheckInfoCallback != 0) {
		if (curr.con != 0) {
			//std::cout << curr.con->getEndpoint()->toString();
			Ice::ConnectionInfoPtr info = curr.con->getInfo();
			Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
			if (tcpInfo != 0) {
				std::stringstream endpoint;
				endpoint << tcpInfo->remoteAddress << ":" << tcpInfo->remotePort;
				int index = endpointToIndex[endpoint.str()];

				std::tr2::sys::path p = std::tr2::sys::current_path<std::tr2::sys::path>();
				std::string exePath = p.directory_string() + "\\";

				this->clientCheckInfoCallback(index, (exePath + clientUVSSImagePath).c_str(), (exePath + clientPlateImagePath).c_str(), serverChannel.c_str(), serverPlateNumber.c_str(), serverDirection.c_str(), serverCheckDateTime.c_str(), serverExtension.c_str());
			}
		}
	}
}

void ClientI::createClientImageDirectory(const std::string& clientImageDirectory)
{
	std::tr2::sys::path p(clientImageDirectory);
	if (std::tr2::sys::exists(p)) {
		return;
	}
	else {
		std::tr2::sys::create_directory(p);
	}
}

void ClientI::heartBeat(const Ice::Current&)
{
}

ClientI::ClientI() : isDestroyed(false), index(0)
{
}

void ClientI::setClientConnectionInfoCallback(ClientConnectionInfoCallback clientConnectionInfoCallback)
{
	ClientI::clientConnectionInfoCallback = clientConnectionInfoCallback;
}

void ClientI::setClientCheckInfoCallback(ClientCheckInfoCallback clientCheckInfoCallback)
{
	ClientI::clientCheckInfoCallback = clientCheckInfoCallback;
}

//本地函数
void ClientI::useClientConnectionInfoCallback(int handle, int type, const std::string& clientConnectionInfo)
{
	if (this->clientConnectionInfoCallback != 0) {
		this->clientConnectionInfoCallback(handle, type, clientConnectionInfo.c_str());
	}
}

void ClientI::run()
{
	while (true) {
		std::map<UVSS::ServerPrx, std::string> serverProxyToEndpoint;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
			IceUtil::Monitor<IceUtil::Mutex>::timedWait(IceUtil::Time::seconds(2));

			if (this->isDestroyed) {
				return;
			}

			serverProxyToEndpoint = this->serverProxyToEndpoint;
		}

		if (!serverProxyToEndpoint.empty()) {
			for (std::map<UVSS::ServerPrx, std::string>::const_iterator it = serverProxyToEndpoint.begin(); it != serverProxyToEndpoint.end(); ++it) {
				try {
					it->first->heartBeat();
					//std::cout << it->first->ice_getConnection()->getEndpoint()->toString() << std::endl;
				}
				catch (...) {
					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

					if (!this->isDestroyed) {
						UVSS::ServerPrx serverProxy = it->first;
						std::string endpoint = it->second;
						int index = endpointToIndex[endpoint];
						std::stringstream idx;
						idx << index;
						useClientConnectionInfoCallback(index, -3, "服务器端 " + endpoint + ": " + "已断开 | 连接标识: " + idx.str());
						this->serverProxyToEndpoint.erase(serverProxy);
						this->endpointToIndex.erase(endpoint);
					}
					else {
						return;
					}
				}
			}
		}
	}
}

void ClientI::destroy()
{
	{
		IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
		this->isDestroyed = true;

		notify();
	}

	getThreadControl().join();
}
