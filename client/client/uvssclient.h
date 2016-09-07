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

	int connect(const std::string& = "127.0.0.1", int = 20145);
	int disconnect(int);

	void setConnectionInfoCallback(ClientConnectionInfoCallback);
	void setCheckInfoCallback(ClientCheckInfoCallback);

private:
	Ice::CommunicatorPtr ic;
	ClientIPtr client;

	Ice::ObjectAdapterPtr adapter;
	Ice::Identity ident;

	static const std::string version;
};

typedef IceUtil::Handle<UVSSClient> UVSSClientPtr;

#endif
