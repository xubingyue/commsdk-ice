#ifndef CLIENTI_H
#define CLIENTI_H

#include "clientserver.h"
#include <fstream>
#include <Windows.h>

typedef void (__stdcall *ClientConnectionInfoCallback)(int, int, const char*);
typedef void (__stdcall *ClientCheckInfoCallback)(int, const char*, const char*, const char*, const char*, const char*, const char*, const char*);

class ClientI : virtual public UVSS::Client {
public:
	virtual std::string readClientUVSSImagePath(const Ice::Current& = Ice::Current());
	virtual std::string readClientPlateImagePath(const Ice::Current& = Ice::Current());

	virtual void writeClientUVSSImagePath(const std::string&, const UVSS::ByteSeq&, const Ice::Current& = Ice::Current());
	virtual void writeClientPlateImagePath(const std::string&, const UVSS::ByteSeq&, const Ice::Current& = Ice::Current());

	virtual void writeClientUVSSImageRelativePath(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientPlateImageRelativePath(const std::string&, const Ice::Current& = Ice::Current());

	virtual void writeClientChannel(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientPlateNumber(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientDirection(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientCheckDateTime(const std::string&, const Ice::Current& = Ice::Current());
	virtual void writeClientExtension(const std::string&, const Ice::Current& = Ice::Current());

	virtual void createClientImageDirectory(const Ice::Current& = Ice::Current());

	virtual void useClientCheckInfoCallback(const Ice::Current& = Ice::Current());

	ClientI();

	void setClientUVSSImagePath(const std::string&);
	void setClientPlateImagePath(const std::string&);

	void setClientConnectionInfoCallback(ClientConnectionInfoCallback);
	void setClientCheckInfoCallback(ClientCheckInfoCallback);

	void useClientConnectionInfoCallback(int, int, const std::string&);

private:
	std::string clientUVSSImagePath;
	std::string clientPlateImagePath;

	std::string clientChannel;
	std::string clientPlateNumber;
	std::string clientDirection;
	std::string clientCheckDateTime;
	std::string clientExtension;

	ClientConnectionInfoCallback clientConnectionInfoCallback;
	ClientCheckInfoCallback clientCheckInfoCallback;
};

typedef IceInternal::Handle<ClientI> ClientIPtr;

#endif

	//const std::string& getClientUVSSImagePath() const;
	//const std::string& getClientPlateImagePath() const;
	//const std::string& getClientChannel() const;
	//const std::string& getClientPlateNumber() const;
	//const std::string& getClientDirection() const;
	//const std::string& getClientCheckDateTime() const;
	//const std::string& getClientExtension() const;
	//void getClientCheckInfo(std::string&, std::string&, std::string&, std::string&, std::string&, std::string&, std::string&);
