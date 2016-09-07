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

void ClientI::useClientCheckInfoCallback(const Ice::Current&)
{
	if (this->clientCheckInfoCallback != 0) {
		//要改！！！！
		this->clientCheckInfoCallback(1, this->clientUVSSImagePath.c_str(), this->clientPlateImagePath.c_str(), this->clientChannel.c_str(), this->clientPlateNumber.c_str(), this->clientDirection.c_str(), this->clientCheckDateTime.c_str(), this->clientExtension.c_str());
	}
}

void ClientI::heartBeat(const Ice::Current&)
{
}

ClientI::ClientI(): serverIPAddress("127.0.0.1"), serverPortNumber(20145), isDestroyed(false), isConnected(false)
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
		UVSS::ServerPrx serverProxy;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
			IceUtil::Monitor<IceUtil::Mutex>::timedWait(IceUtil::Time::seconds(2));

			if (this->isDestroyed) {
				break;
			}

			serverProxy = this->serverProxy;
		}

		if (serverProxy != NULL) {
			try {
				this->serverProxy->heartBeat();
			} catch (...) {
				std::stringstream serverPort;
				serverPort << this->serverPortNumber;
				useClientConnectionInfoCallback(1, -3, "服务器端 " + this->serverIPAddress + ":" + serverPort.str() + ": " + "已断开 | 连接标识: 1");

				IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
				this->serverProxy = NULL;
				this->isConnected = false;
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

void ClientI::setServerIPAddress(const std::string& serverIPAddress)
{
	this->serverIPAddress = serverIPAddress;
}

void ClientI::setServerPortNumber(int serverPortNumber)
{
	this->serverPortNumber = serverPortNumber;
}

const std::string& ClientI::getServerIPAddress() const
{
	return this->serverIPAddress;
}

const int ClientI::getServerPortNumber() const
{
	return this->serverPortNumber;
}

const UVSS::ServerPrx ClientI::getServerProxy() const
{
	return this->serverProxy;
}

void ClientI::setServerProxy(const UVSS::ServerPrx& serverProxy)
{
	this->serverProxy = serverProxy;
}
