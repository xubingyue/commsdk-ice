#include "../uvssserversdk.h"
extern "C" {
#include "serverconnectioninfocallback.h"
}

extern "C" void __stdcall serverConnectionInfoCallback(int, const char*);
void menu();

int main(int argc, char* argv[])
{
	if (argc > 1) {
		std::cerr << "too many arguments" << std::endl;
		return 1;
	}

	SetUVSSServerCallback(serverConnectionInfoCallback);

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
				{
					std::cout << "server port:" << std::endl;
					int port;
					std::cin >> port;
					SetUVSSServerPort(port);
				}
				break;
			case 2:
				InitUVSSServer();
				break;
			case -2:
				UninitUVSSServer();
				break;
			case 3:
				SendUVSSCheckInfo("1.jpg", "2.jpg", "通道1", "鲁A12345", "进车", "2016", "扩展");
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
		"1: set port\n"
		"2: init\n"
		"-2: uninit\n"
		"3: send CheckInfo\n"
		"9: exit\n";
}
