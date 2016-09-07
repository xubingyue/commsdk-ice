#include "serverI.h"

void ServerI::addClient(const ::Ice::Identity& ident, const Ice::Current& curr)
{
	UVSS::ClientPrx clientProxy = UVSS::ClientPrx::uncheckedCast(curr.con->createProxy(ident));
	this->clientProxy = clientProxy;
}

void ServerI::shutdown(const Ice::Current& c)
{
	//std::cout << "shutting down..." << std::endl;
	try {
		c.adapter->getCommunicator()->shutdown();
	} catch (const Ice::Exception& ex) {
		std::cout << ex << std::endl;
	}
}

void ServerI::useServerConnectionInfoCallback(Ice::Int type, const std::string& serverConnectionInfo, const Ice::Current&)
{
	if (this->serverConnectionInfoCallback != 0) {
		this->serverConnectionInfoCallback(type, serverConnectionInfo.c_str());
	}
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

std::string ServerI::receiveClientUVSSImagePath()
{
	return this->clientProxy->readClientUVSSImagePath();
}

std::string ServerI::receiveClientPlateImagePath()
{
	return this->clientProxy->readClientPlateImagePath();
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
	this->serverConnectionInfoCallback = serverConnectionInfoCallback;
}

const UVSS::ClientPrx& ServerI::getClientProxy() const
{
	return this->clientProxy;
}

//void ServerI::setServerUVSSImagePath(const std::string& serverUVSSImagePath)
//{
//	this->serverUVSSImagePath = serverUVSSImagePath;
//}
//
//void ServerI::setServerPlateImagePath(const std::string& serverPlateImagePath)
//{
//	this->serverPlateImagePath = serverPlateImagePath;
//}
//
//void ServerI::setServerChannel(const std::string& serverChannel)
//{
//	this->serverChannel = serverChannel;
//}
//
//void ServerI::setServerPlateNumber(const std::string& serverPlateNumber)
//{
//	this->serverPlateNumber = serverPlateNumber;
//}
//
//void ServerI::setServerDirection(const std::string& serverDirection)
//{
//	this->serverDirection = serverDirection;
//}
//
//void ServerI::setServerCheckDateTime(const std::string& serverCheckDateTime)
//{
//	this->serverCheckDateTime = serverCheckDateTime;
//}
//
//void ServerI::setServerExtension(const std::string& serverExtension)
//{
//	this->serverExtension = serverExtension;
//}

//void ServerI::setServerCheckInfo(const std::string& serverUVSSImagePath, const std::string& serverPlateImagePath, const std::string& serverChannel, const std::string& serverPlateNumber, const std::string& serverDirection, const std::string& serverCheckDateTime, const std::string& serverExtension)
//{
//	setServerUVSSImagePath(serverUVSSImagePath);
//	setServerPlateImagePath(serverPlateImagePath);
//	setServerChannel(serverChannel);
//	setServerPlateNumber(serverPlateNumber);
//	setServerDirection(serverDirection);
//	setServerCheckDateTime(serverCheckDateTime);
//	setServerExtension(serverExtension);
//}

//void ServerI::sendServerCheckInfo(const std::string& serverUVSSImagePath, const std::string& serverPlateImagePath, const std::string& serverChannel, const std::string& serverPlateNumber, const std::string& serverDirection, const std::string& serverCheckDateTime, const std::string& serverExtension)
//{
//	sendServerUVSSImagePath(serverUVSSImagePath, this->clientUVSSImagePath);
//	sendServerPlateImagePath(serverPlateImagePath, this->clientPlateImagePath);
//	sendServerChannel(serverChannel);
//	sendServerPlateNumber(serverPlateNumber);
//	sendServerDirection(serverDirection);
//	sendServerCheckDateTime(serverCheckDateTime);
//	sendServerExtension(serverExtension);
//	this->clientProxy->useClientCheckInfoCallback();
//}

//void ServerI::sendServerCheckInfo()
//{
//	sendServerCheckInfo(this->serverUVSSImagePath, this->serverPlateImagePath, this->serverChannel, this->serverPlateNumber, this->serverDirection, this->serverCheckDateTime, this->serverExtension);
//}
