#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include "clientI.h"
#include <sstream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

class UVSSClient : public IceUtil::Shared {
public:
	UVSSClient();
	~UVSSClient();

	int init();
	void uninit();

	int connect(const std::string&, int);
	int disconnect(int);

	void setConnectionInfoCallback(ClientConnectionInfoCallback);
	void setCheckInfoCallback(ClientCheckInfoCallback);

private:
	Ice::CommunicatorPtr ic;
	ClientIPtr client;

	std::string IPAddress;
	int portNumber;

	//bool isConnected;

	Ice::ObjectAdapterPtr adapter;
	Ice::Identity ident;
};

typedef IceUtil::Handle<UVSSClient> UVSSClientPtr;

#endif
