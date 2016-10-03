#ifndef SERVERI_H
#define SERVERI_H

#include <clientserver.h>
#include <map>
#include <string>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

typedef void (__stdcall *ServerConnectionInfoCallback)(int, const char*);

class ServerI : virtual public UVSS::Server, virtual public IceUtil::Thread, virtual public IceUtil::Monitor<IceUtil::Mutex> {
public:
	ServerI();

	static void setConnectionInfoCallback(ServerConnectionInfoCallback);

	virtual bool checkVersion(const std::string&, const Ice::Current& = Ice::Current());
	virtual void addClient(const Ice::Identity&, const Ice::Current& = Ice::Current());
	virtual void run();

	void filePathToBinary(const std::string&, UVSS::ByteSeq&);
	const std::string createCurrentTime();
	void sendCheckInfo(
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&);
	void destroy();

private:
	static ServerConnectionInfoCallback connectionInfoCallback;

	std::map<UVSS::ClientPrx, std::string> clientProxyToEndpoint;
	bool isDestroyed;
};

typedef IceUtil::Handle<ServerI> ServerIPtr;

#endif
