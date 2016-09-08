#ifndef CLIENTI_H
#define CLIENTI_H

#include <clientserver.h>
#include <fstream>
#include <map>
#include <filesystem>
#include <Ice/Ice.h>

typedef void (__stdcall *ClientConnectionInfoCallback)(int, int, const char*);
typedef void (__stdcall *ClientCheckInfoCallback)(int, const char*, const char*, const char*, const char*, const char*, const char*, const char*);

class ClientI : virtual public UVSS::Client, virtual public IceUtil::Thread, virtual public IceUtil::Monitor<IceUtil::Mutex> {
public:
	virtual void writeClientUVSSImagePath(const std::string&, const UVSS::ByteSeq&, const Ice::Current& = Ice::Current());
	virtual void writeClientPlateImagePath(const std::string&, const UVSS::ByteSeq&, const Ice::Current& = Ice::Current());

	virtual void writeClientUVSSImageRelativePath(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientPlateImageRelativePath(const std::string&, const Ice::Current& = Ice::Current());

	virtual void writeClientChannel(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientPlateNumber(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientDirection(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientCheckDateTime(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientExtension(const std::string&, const Ice::Current& = Ice::Current());

	virtual void createClientImageDirectory(const std::string&, const Ice::Current& = Ice::Current());
	virtual void useClientCheckInfoCallback(const Ice::Current& = Ice::Current());
	virtual void heartBeat(const Ice::Current& = Ice::Current());

	ClientI();

	static void setClientConnectionInfoCallback(ClientConnectionInfoCallback);
	static void setClientCheckInfoCallback(ClientCheckInfoCallback);
	void useClientConnectionInfoCallback(int, int, const std::string&);

	virtual void run();
	void destroy();

	int index;

	typedef struct {
		int index;
		std::string endpoint;
	} ID;

	std::map<UVSS::ServerPrx, ID> serverProxyToID;

private:
	std::string clientUVSSImagePath;
	std::string clientPlateImagePath;

	std::string clientChannel;
	std::string clientPlateNumber;
	std::string clientDirection;
	std::string clientCheckDateTime;
	std::string clientExtension;

	static ClientConnectionInfoCallback clientConnectionInfoCallback;
	static ClientCheckInfoCallback clientCheckInfoCallback;

	bool isDestroyed;
};

typedef IceUtil::Handle<ClientI> ClientIPtr;

#endif
