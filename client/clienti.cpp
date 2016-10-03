#include "clienti.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <clientserver.h>

ClientConnectionInfoCallback ClientI::connectionInfoCallback = 0;
ClientCheckInfoCallback ClientI::checkInfoCallback = 0;

ClientI::ClientI() : isDestroyed(false), index(0)
{
}

void ClientI::setConnectionInfoCallback(ClientConnectionInfoCallback connectionInfoCallback)
{
	ClientI::connectionInfoCallback = connectionInfoCallback;
}

void ClientI::setCheckInfoCallback(ClientCheckInfoCallback checkInfoCallback)
{
	ClientI::checkInfoCallback = checkInfoCallback;
}

void ClientI::writeCheckInfo(
		const std::string& uVSSImageName, const UVSS::ByteSeq& uVSSImage,
		const std::string& plateImageName, const UVSS::ByteSeq& plateImage,
		const std::string& channel,
		const std::string& plateNumber,
		const std::string& direction,
		const std::string& time,
		const std::string& extension,
		const Ice::Current& curr)
{
	createImageDirectory("UVSS");

	std::tr2::sys::path currentPath = std::tr2::sys::current_path<std::tr2::sys::path>();
	std::string imagePath = currentPath.directory_string() + "\\UVSS\\";

	std::string uVSSImagePath;
	if (!uVSSImageName.empty()) {
		uVSSImagePath = imagePath + uVSSImageName;
		std::ofstream ofs(uVSSImagePath, std::ios::binary);
		ofs.write((char*)&uVSSImage[0], uVSSImage.size());
	}

	std::string plateImagePath;
	if (!plateImageName.empty()) {
		plateImagePath = imagePath + plateImageName;
		std::ofstream ofs(plateImagePath, std::ios::binary);
		ofs.write((char*)&plateImage[0], plateImage.size());
	}

	if (this->checkInfoCallback != 0) {
		if (curr.con != 0) {
			//std::cout << curr.con->getEndpoint()->toString();
			Ice::ConnectionInfoPtr info = curr.con->getInfo();
			Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);
			if (tcpInfo != 0) {
				std::stringstream endpoint;
				endpoint << tcpInfo->remoteAddress << ":" << tcpInfo->remotePort;
				int index = this->endpointToIndex[endpoint.str()];

				this->checkInfoCallback(
						index,
						uVSSImagePath.c_str(), plateImagePath.c_str(),
						channel.c_str(), plateNumber.c_str(), direction.c_str(), time.c_str(), extension.c_str());
			}
		}
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
					it->first->ice_ping();
				}
				catch (...) {
					IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

					if (!this->isDestroyed) {
						UVSS::ServerPrx serverProxy = it->first;
						std::string endpoint = it->second;
						int index = this->endpointToIndex[endpoint];
						std::stringstream idx;
						idx << index;

						useConnectionInfoCallback(index, -3, "服务器端 " + endpoint + ": " + "已断开 | 连接标识: " + idx.str());
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

void ClientI::useConnectionInfoCallback(int index, int type, const std::string& connectionInfo)
{
	if (this->connectionInfoCallback != 0) {
		this->connectionInfoCallback(index, type, connectionInfo.c_str());
	}
}

void ClientI::createImageDirectory(const std::string& imageDirectory)
{
	std::tr2::sys::path dir(imageDirectory);

	if (std::tr2::sys::exists(dir)) {
		return;
	}
	else {
		std::tr2::sys::create_directory(dir);
	}
}

void ClientI::destroy()
{
	{
		IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
		this->isDestroyed = true;

		IceUtil::Monitor<IceUtil::Mutex>::notify();
	}

	IceUtil::Thread::getThreadControl().join();
}
