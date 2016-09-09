#ifndef SERVERI_H
#define SERVERI_H

#include <clientserver.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <chrono>
#include <ctime>
#include <Ice/Ice.h>

typedef void (__stdcall *ServerConnectionInfoCallback)(int, const char*);

class ServerI : virtual public UVSS::Server, virtual public IceUtil::Thread, virtual public IceUtil::Monitor<IceUtil::Mutex> {
public:
	virtual void addClient(const ::Ice::Identity&, const Ice::Current& = Ice::Current());
	virtual void useServerConnectionInfoCallback(Ice::Int, const std::string&, const Ice::Current& = Ice::Current());
	virtual void heartBeat(const Ice::Current& = Ice::Current());
	virtual bool checkVersion(const std::string&, const Ice::Current& = Ice::Current());

	static void setServerConnectionInfoCallback(ServerConnectionInfoCallback);

	ServerI();

	void filePathToBinary(const std::string&, UVSS::ByteSeq&);
	const std::string createFileName(const std::string& = "", const std::string& = "", const std::string& = "jpg");
	void sendCheckInfo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);


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
