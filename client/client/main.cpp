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
	//try{
	if (argc > 1) {
		std::cerr << "too many arguments" << std::endl;
		return 1;
	}

	menu();

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
			case 2:
				SetUVSSMessageCallback(clientConnectionInfoCallback);
				break;
			case 3:
				SetUVSSCheckInfoCallback(clientCheckInfoCallback);
				break;
			case 4:
				UVSSConnect("127.0.0.1", 20145);
				break;
			case 5:
				UVSSDisconnect(1);
				break;
			//case 6:
			//	UVSSUninitialize();
			//	break;
			case -1:
				break;
			}
		} catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
		}
	} while ((std::cin.good()) && (key != -1));
	//} catch (const Ice::ConnectionLostException& ex)
	//} catch (...)
	//} catch (const Ice::Exception& ex)
	//{
	//	std::cerr << "XXXXX" << std::endl;
	//}

	return 0;
}

void menu()
{
	std::cout << "usage:\n"
		"0: help\n"
		"1: init\n"
		"2: set ConnectionInfoCallback Function\n"
		"3: set CheckInfoCallback Function\n"
		"4: connect\n"
		"5: disconnect\n"
		//"6: uninit\n"
		"-1: exit\n";
}
