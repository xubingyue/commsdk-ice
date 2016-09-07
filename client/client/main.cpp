#include "uvssclientsdk.h"
extern "C" {
#include "clientcheckinfocallback.h"
#include "clientconnectioninfocallback.h"
}

extern "C" void __stdcall clientConnectionInfoCallback(int, int, const char*);
extern "C" void __stdcall clientCheckInfoCallback(int, const char*, const char*, const char*, const char*, const char*, const char*, const char*);
void menu();

int main(int argc, char* argv[])
{
	if (argc > 1) {
		std::cerr << "too many arguments" << std::endl;
		return 1;
	}

	SetUVSSMessageCallback(clientConnectionInfoCallback);
	SetUVSSCheckInfoCallback(clientCheckInfoCallback);

	menu();

	std::string ip = "127.0.0.1";
	int port = 20145;

	int key;
	do {
		try {
			std::cout << "==>";
			std::cin >> key;

			switch (key) {
			case 0:
				menu();
				break;
			case 1:
				UVSSInitialize();
				break;
			case -1:
				UVSSUninitialize();
				break;
			case 2:
				std::cout << "server IP:" << std::endl;
				std::cin >> ip;
				break;
			case 3:
				std::cout << "server port:" << std::endl;
				std::cin >> port;
				break;
			case 4:
				UVSSConnect(ip.c_str(), port);
				//UVSSConnect("127.0.0.1", 20145);
				break;
			case -4:
				UVSSDisconnect(1);
				break;
			case 9:
				break;
			}
		} catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	} while ((std::cin.good()) && (key != 9));

	return 0;
}

void menu()
{
	std::cout << "usage:\n"
		"0: help\n"
		"1: init\n"
		"-1: uninit\n"
		"2: set server IP\n"
		"3: set server port\n"
		"4: connect\n"
		"-4: disconnect\n"
		"9: exit\n";
}
