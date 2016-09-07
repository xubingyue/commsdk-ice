#ifndef SERVERI_H
#define SERVERI_H

#include <clientserver.h>
#include <fstream>
#include <iomanip>
#include <set>
#include <map>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <Windows.h>

typedef void (__stdcall *ServerConnectionInfoCallback)(int, const char*);

class ServerI : virtual public UVSS::Server, virtual public IceUtil::Thread, virtual public IceUtil::Monitor<IceUtil::Mutex> {
public:
	virtual void addClient(const ::Ice::Identity&, const Ice::Current& = Ice::Current());
	virtual void useServerConnectionInfoCallback(Ice::Int, const std::string&, const Ice::Current& = Ice::Current());
	virtual void heartBeat(const Ice::Current& = Ice::Current());
	virtual bool checkVersion(const std::string&, const Ice::Current& = Ice::Current());

	ServerI();

	void filePathToBinary(const std::string&, UVSS::ByteSeq&);

	void sendServerUVSSImagePath(const std::string&, const std::string&);
	void sendServerPlateImagePath(const std::string&, const std::string&);

	void sendServerUVSSImageRelativePath(const std::string&);
	void sendServerPlateImageRelativePath(const std::string&);
	void sendServerChannel(const std::string&);
	void sendServerPlateNumber(const std::string&);
	void sendServerDirection(const std::string&);
	void sendServerCheckDateTime(const std::string&);
	void sendServerExtension(const std::string&);

	static void setServerConnectionInfoCallback(ServerConnectionInfoCallback);

	void sendCheckInfo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

	const std::string createFileName(const std::string& = "", const std::string& = "", const std::string& = "jpg");

	virtual void run();
	void destroy();

private:
	UVSS::ClientPrx clientProxy;
	std::map<UVSS::ClientPrx, std::string> clientProxyToEndpoint;

	static ServerConnectionInfoCallback serverConnectionInfoCallback;

	bool isDestroyed;

	static const std::string version;
};

typedef IceUtil::Handle<ServerI> ServerIPtr;

#endif
