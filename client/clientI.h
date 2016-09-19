#ifndef CLIENTI_H
#define CLIENTI_H

#include <version.h>
#include <clientserver.h>
#include <fstream>
#include <map>
#include <filesystem>
#include <Ice/Ice.h>

typedef void (__stdcall *ClientConnectionInfoCallback)(int, int, const char*);
typedef void (__stdcall *ClientCheckInfoCallback)(int, const char*, const char*, const char*, const char*, const char*, const char*, const char*);

class ClientI : virtual public UVSS::Client, virtual public IceUtil::Thread, virtual public IceUtil::Monitor<IceUtil::Mutex> {
public:
	virtual void writeCheckInfo(
		const std::string&, const UVSS::ByteSeq&,
		const std::string&, const UVSS::ByteSeq&,
		const std::string&,
		const std::string&,
		const std::string&,
		const std::string&,
		const std::string&,
		const Ice::Current& = Ice::Current());
	virtual void heartBeat(const Ice::Current& = Ice::Current());

	static void setClientConnectionInfoCallback(ClientConnectionInfoCallback);
	static void setClientCheckInfoCallback(ClientCheckInfoCallback);

	ClientI();

	void createClientImageDirectory(const std::string&);
	//¿¼ÂÇÉ¾³ý´Ëº¯Êý
	void useClientConnectionInfoCallback(int, int, const std::string&);

	int index;

	std::map<UVSS::ServerPrx, std::string> serverProxyToEndpoint;
	std::map<std::string, int> endpointToIndex;

	virtual void run();
	void destroy();

private:
	static ClientConnectionInfoCallback clientConnectionInfoCallback;
	static ClientCheckInfoCallback clientCheckInfoCallback;

	bool isDestroyed;
};

typedef IceUtil::Handle<ClientI> ClientIPtr;

#endif
