#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include "serverI.h"
#include <iomanip>
#include <sstream>
#include <Windows.h>

class UVSSServer {
public:
	UVSSServer();
	~UVSSServer();

	void setIPAddress(const std::string& = "127.0.0.1");
	void setPortNumber(int = 20145);

	int init();
	void uninit();

	void sendUVSSImagePath(const std::string&);
	void sendPlateImagePath(const std::string&);
	void sendCheckInfo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

	void setConnectionInfoCallback(ServerConnectionInfoCallback);

	const std::string createFileName(const std::string& = "", const std::string& = "", const std::string& = "jpg");

private:
	Ice::CommunicatorPtr ic;
	ServerIPtr server;
	std::string IPAddress;
	int portNumber;
};

#endif
