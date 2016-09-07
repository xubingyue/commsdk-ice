#ifndef SERVERI_H
#define SERVERI_H

#include "clientserver.h"
#include <fstream>
#include <Ice/Ice.h>

typedef void (__stdcall *ServerConnectionInfoCallback)(int, const char*);

class ServerI : virtual public UVSS::Server {
public:
	virtual void addClient(const ::Ice::Identity&, const Ice::Current& = Ice::Current());
	virtual void shutdown(const Ice::Current& = Ice::Current());
	virtual void useServerConnectionInfoCallback(Ice::Int, const std::string&, const Ice::Current& = Ice::Current());

	void filePathToBinary(const std::string&, UVSS::ByteSeq&);

	std::string receiveClientUVSSImagePath();
	std::string receiveClientPlateImagePath();

	void sendServerUVSSImagePath(const std::string&, const std::string&);
	void sendServerPlateImagePath(const std::string&, const std::string&);

	void sendServerUVSSImageRelativePath(const std::string&);
	void sendServerPlateImageRelativePath(const std::string&);
	void sendServerChannel(const std::string&);
	void sendServerPlateNumber(const std::string&);
	void sendServerDirection(const std::string&);
	void sendServerCheckDateTime(const std::string&);
	void sendServerExtension(const std::string&);

	void setServerConnectionInfoCallback(ServerConnectionInfoCallback);

	const UVSS::ClientPrx& getClientProxy() const;

private:
	UVSS::ClientPrx clientProxy;
	ServerConnectionInfoCallback serverConnectionInfoCallback;
};

typedef IceInternal::Handle<ServerI> ServerIPtr;

#endif

	//void setServerUVSSImagePath(const std::string&);
	//void setServerPlateImagePath(const std::string&);
	//void setServerChannel(const std::string&);
	//void setServerPlateNumber(const std::string&);
	//void setServerDirection(const std::string&);
	//void setServerCheckDateTime(const std::string&);
	//void setServerExtension(const std::string&);
	//void setServerCheckInfo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
	//void setClientUVSSImagePath(const std::string&);
	//void setClientPlateImagePath(const std::string&);
	//void sendServerCheckInfo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
	//void sendServerCheckInfo();

	//std::string serverUVSSImagePath;
	//std::string serverPlateImagePath;
	//std::string serverChannel;
	//std::string serverPlateNumber;
	//std::string serverDirection;
	//std::string serverCheckDateTime;
	//std::string serverExtension;
