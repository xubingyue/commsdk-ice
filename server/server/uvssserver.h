#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include "serverI.h"
#include <sstream>

class UVSSServer : virtual public IceUtil::Shared {
public:
	UVSSServer();
	~UVSSServer();

	void setPortNumber(int);

	int init();
	void uninit();

	void sendCheckInfo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

	void setConnectionInfoCallback(ServerConnectionInfoCallback);

private:
	Ice::CommunicatorPtr ic;
	ServerIPtr server;
	int portNumber;
};

typedef IceUtil::Handle<UVSSServer> UVSSServerPtr;

#endif
