#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include "clientI.h"
#include <sstream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

class UVSSClient {
public:
	UVSSClient();
	~UVSSClient();

	//void setIPAddress(const std::string& = "127.0.0.1");
	//void setPortNumber(int = 10001);

	void setServerIPAddress(const std::string& = "127.0.0.1");
	void setServerPortNumber(int = 20145);

	int init();
	void uninit();

	int connect(const std::string&, int);
	int disconnect(int);

	void setConnectionInfoCallback(ClientConnectionInfoCallback);
	void setCheckInfoCallback(ClientCheckInfoCallback);

private:
	Ice::CommunicatorPtr ic;
	ClientIPtr client;
	UVSS::ServerPrx serverProxy;
	UVSS::ClientPrx clientProxy;

	std::string IPAddress;
	int portNumber;

	std::string serverIPAddress;
	int serverPortNumber;

	bool isConnected;

	Ice::ObjectAdapterPtr adapter;
	Ice::Identity ident;
};

#endif

//void stopServer();