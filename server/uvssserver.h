#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include "serveri.h"
#include <string>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

class UVSSServer : virtual public IceUtil::Shared {
public:
	UVSSServer();

	void setConnectionInfoCallback(ServerConnectionInfoCallback);
	void setPort(int);
	int init();
	void uninit();
	void sendCheckInfo(
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&,
			const std::string&);

private:
	Ice::CommunicatorPtr ic;
	ServerIPtr server;
	int port;
};

typedef IceUtil::Handle<UVSSServer> UVSSServerPtr;

#endif
