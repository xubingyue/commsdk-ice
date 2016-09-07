#include "clientI.h"

std::string ClientI::readClientUVSSImagePath(const Ice::Current&)
{
	return this->clientUVSSImagePath;
}

std::string ClientI::readClientPlateImagePath(const Ice::Current&)
{
	return this->clientPlateImagePath;
}

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

void ClientI::createClientImageDirectory(const Ice::Current&)
{
	WIN32_FIND_DATA findData;
	if (FindFirstFile("UVSS", &findData) == INVALID_HANDLE_VALUE) {
		CreateDirectory("UVSS", NULL);
	}
}

void ClientI::useClientCheckInfoCallback(const Ice::Current&)
{
	if (this->clientCheckInfoCallback != 0) {
		//Òª¸Ä£¡£¡£¡£¡
		this->clientCheckInfoCallback(1, this->clientUVSSImagePath.c_str(), this->clientPlateImagePath.c_str(), this->clientChannel.c_str(), this->clientPlateNumber.c_str(), this->clientDirection.c_str(), this->clientCheckDateTime.c_str(), this->clientExtension.c_str());
	}
}

ClientI::ClientI(): clientConnectionInfoCallback(0), clientCheckInfoCallback(0)
{
}

void ClientI::setClientUVSSImagePath(const std::string& clientUVSSImagePath)
{
	this->clientUVSSImagePath = clientUVSSImagePath;
}

void ClientI::setClientPlateImagePath(const std::string& clientPlateImagePath)
{
	this->clientPlateImagePath = clientPlateImagePath;
}

void ClientI::setClientConnectionInfoCallback(ClientConnectionInfoCallback clientConnectionInfoCallback)
{
	this->clientConnectionInfoCallback = clientConnectionInfoCallback;
}

void ClientI::setClientCheckInfoCallback(ClientCheckInfoCallback clientCheckInfoCallback)
{
	this->clientCheckInfoCallback = clientCheckInfoCallback;
}

void ClientI::useClientConnectionInfoCallback(int handle, int type, const std::string& clientConnectionInfo)
{
	if (this->clientConnectionInfoCallback != 0) {
		this->clientConnectionInfoCallback(handle, type, clientConnectionInfo.c_str());
	}
}

//const std::string& ClientI::getClientUVSSImagePath() const
//{
//	return this->clientUVSSImagePath;
//}
//
//const std::string& ClientI::getClientPlateImagePath() const
//{
//	return this->clientPlateImagePath;
//}
//
//const std::string& ClientI::getClientChannel() const
//{
//	return this->clientChannel;
//}
//
//const std::string& ClientI::getClientPlateNumber() const
//{
//	return this->clientPlateNumber;
//}
//
//const std::string& ClientI::getClientDirection() const
//{
//	return this->clientDirection;
//}
//
//const std::string& ClientI::getClientCheckDateTime() const
//{
//	return this->clientCheckDateTime;
//}
//
//const std::string& ClientI::getClientExtension() const
//{
//	return this->clientExtension;
//}
//
//void ClientI::getClientCheckInfo(std::string& clientUVSSImagePath, std::string& clientPlateImagePath, std::string& clientChannel, std::string& clientPlateNumber, std::string& clientDirection, std::string& clientCheckDateTime, std::string& clientExtension)
//{
//	clientUVSSImagePath = getClientUVSSImagePath();
//	clientPlateImagePath = getClientPlateImagePath();
//	clientChannel = getClientChannel();
//	clientPlateNumber = getClientPlateNumber();
//	clientDirection = getClientDirection();
//	clientCheckDateTime = getClientCheckDateTime();
//	clientExtension = getClientExtension();
//}
