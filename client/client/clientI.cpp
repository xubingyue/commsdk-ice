#include "clientI.h"

ClientConnectionInfoCallback ClientI::clientConnectionInfoCallback = 0;
ClientCheckInfoCallback ClientI::clientCheckInfoCallback = 0;

void ClientI::writeClientUVSSImagePath(const std::string& clientUVSSImagePath, const UVSS::ByteSeq& serverUVSSImage, const Ice::Current&)
{
	std::ofstream ofs(clientUVSSImagePath, std::ios::binary);
	ofs.write((char*)&serverUVSSImage[0], serverUVSSImage.size());
}

void ClientI::writeClientPlateImagePath(const std::string& clientPlateImagePath, const UVSS::ByteSeq& serverPlateImage, const Ice::Current&)
{
	std::ofstream ofs(clientPlateImagePath, std::ios::binary);
	ofs.write((char*)&serverPlateImage[0], serverPlateImage.size());
}

void ClientI::writeClientUVSSImageRelativePath(const std::string& serverUVSSImageRelativePath, const Ice::Current&)
{
	this->clientUVSSImagePath = serverUVSSImageRelativePath;
}

void ClientI::writeClientPlateImageRelativePath(const std::string& serverPlateImageRelativePath, const Ice::Current&)
{
	this->clientPlateImagePath = serverPlateImageRelativePath;
}

void ClientI::writeClientChannel(const std::string& serverChannel, const Ice::Current&)
{
	this->clientChannel = serverChannel;
}

void ClientI::writeClientPlateNumber(const std::string& serverPlateNumber, const Ice::Current&)
{
	this->clientPlateNumber = serverPlateNumber;
}

void ClientI::writeClientDirection(const std::string& serverDirection, const Ice::Current&)
{
	this->clientDirection = serverDirection;
}

void ClientI::writeClientCheckDateTime(const std::string& serverCheckDateTime, const Ice::Current&)
{
	this->clientCheckDateTime = serverCheckDateTime;
}

void ClientI::writeClientExtension(const std::string& serverExtension, const Ice::Current&)
{
	this->clientExtension = serverExtension;
}

void ClientI::createClientImageDirectory(const std::string& clientImageDirectory, const Ice::Current&)
{
	WIN32_FIND_DATA findData;
	if (FindFirstFile(clientImageDirectory.c_str(), &findData) == INVALID_HANDLE_VALUE) {
		CreateDirectory(clientImageDirectory.c_str(), NULL);
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

						char temp[MAX_PATH];
						GetModuleFileName(NULL, temp, MAX_PATH);
						std::string configFilePath = temp;
						configFilePath = configFilePath.substr(0, configFilePath.find_last_of('\\'));
						configFilePath += "\\";

						this->clientCheckInfoCallback(index, (configFilePath + this->clientUVSSImagePath).c_str(), (configFilePath + this->clientPlateImagePath).c_str(), this->clientChannel.c_str(), this->clientPlateNumber.c_str(), this->clientDirection.c_str(), this->clientCheckDateTime.c_str(), this->clientExtension.c_str());
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

//���غ���
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
						useClientConnectionInfoCallback(it->second.index, -3, "�������� " + it->second.endpoint + ": " + "�ѶϿ� | ���ӱ�ʶ: " + idx.str());
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
