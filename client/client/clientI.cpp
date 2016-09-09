#include "clientI.h"

ClientConnectionInfoCallback ClientI::clientConnectionInfoCallback = 0;
ClientCheckInfoCallback ClientI::clientCheckInfoCallback = 0;

void ClientI::writeCheckInfo(
	const std::string& clientUVSSImagePath,
	const UVSS::ByteSeq& serverUVSSImage,
	const std::string& clientPlateImagePath,
	const UVSS::ByteSeq& serverPlateImage,
	const std::string& serverChannel,
	const std::string& serverPlateNumber,
	const std::string& serverDirection,
	const std::string& serverCheckDateTime,
	const std::string& serverExtension,
	const Ice::Current&
	) {
	std::ofstream ofs1(clientUVSSImagePath, std::ios::binary);
	ofs1.write((char*)&serverUVSSImage[0], serverUVSSImage.size());
	
	std::ofstream ofs2(clientPlateImagePath, std::ios::binary);
	ofs2.write((char*)&serverPlateImage[0], serverPlateImage.size());

	this->clientUVSSImagePath = clientUVSSImagePath;
	this->clientPlateImagePath = clientPlateImagePath;
	this->clientChannel = serverChannel;
	this->clientPlateNumber = serverPlateNumber;
	this->clientDirection = serverDirection;
	this->clientCheckDateTime = serverCheckDateTime;
	this->clientExtension = serverExtension;
}

void ClientI::createClientImageDirectory(const std::string& clientImageDirectory, const Ice::Current&)
{
	std::tr2::sys::path p(clientImageDirectory);
	if (std::tr2::sys::exists(p)) {
		return;
	} else {
		std::tr2::sys::create_directory(p);
	}
}

void ClientI::useClientCheckInfoCallback(const Ice::Current& curr)
{
	if (this->clientCheckInfoCallback != 0) {
		if (curr.con != 0) {
			Ice::ConnectionInfoPtr info = curr.con->getInfo();
			Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
			if (tcpInfo != 0) {
				std::stringstream endpoint;
				endpoint << tcpInfo->remoteAddress << ":" << tcpInfo->remotePort;
				for (std::map<UVSS::ServerPrx, ID>::const_iterator it = serverProxyToID.begin(); it != serverProxyToID.end(); ++it) {
					if (it->second.endpoint == endpoint.str()) {
						int index = it->second.index;

						std::tr2::sys::path p = std::tr2::sys::current_path<std::tr2::sys::path>();
						std::string filePath = p.directory_string() + "\\";

						this->clientCheckInfoCallback(index, (filePath + this->clientUVSSImagePath).c_str(), (filePath + this->clientPlateImagePath).c_str(), this->clientChannel.c_str(), this->clientPlateNumber.c_str(), this->clientDirection.c_str(), this->clientCheckDateTime.c_str(), this->clientExtension.c_str());
						break;
					}
				}
			}
		}
	}
}

void ClientI::heartBeat(const Ice::Current&)
{
}

ClientI::ClientI(): isDestroyed(false), index(0)
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
		std::map<UVSS::ServerPrx, ID> serverProxyToID;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
			IceUtil::Monitor<IceUtil::Mutex>::timedWait(IceUtil::Time::seconds(2));

			if (this->isDestroyed) {
				return;
			}

			serverProxyToID = this->serverProxyToID;
		}

		if (!serverProxyToID.empty()) {
			for (std::map<UVSS::ServerPrx, ID>::const_iterator it = serverProxyToID.begin(); it != serverProxyToID.end(); ++it) {
				try {
					it->first->heartBeat();
					//std::cout << it->first->ice_getConnection()->getEndpoint()->toString() << std::endl;
				} catch (...) {

					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

					if (!this->isDestroyed) {
						std::stringstream idx;
						idx << it->second.index;
						useClientConnectionInfoCallback(it->second.index, -3, "服务器端 " + it->second.endpoint + ": " + "已断开 | 连接标识: " + idx.str());
						this->serverProxyToID.erase(it->first);
					} else {
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
