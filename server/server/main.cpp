#include "uvssserversdk.h"
extern "C" {
#include "serverconnectioninfocallback.h"
}

extern "C" void __stdcall serverConnectionInfoCallback(int, const char*);
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
				InitUVSSServer();
				break;
			case 2:
				{
					int port;
					std::cin >> port;
					SetUVSSServerPort(port);
				}
				break;
			case 3:
				SetUVSSServerCallback(serverConnectionInfoCallback);
				break;
			case 4:
				//{
				//	std::string s1;
				//	std::cin >> s1;
				//	std::string s2;
				//	std::cin >> s2;
				//	std::string s3;
				//	std::cin >> s3;
				//	std::string s4;
				//	std::cin >> s4;
				//	std::string s5;
				//	std::cin >> s5;
				//	std::string s6;
				//	std::cin >> s6;
				//	std::string s7;
				//	std::cin >> s7;

				//	SendUVSSCheckInfo(s1.c_str(), s2.c_str(), s3.c_str(), s4.c_str(), s5.c_str(), s6.c_str(), s7.c_str());
				//}
				SendUVSSCheckInfo("1.jpg", "2.jpg", "hello", "world", "C", "C+", "C++");
				break;
			case 5:
				UninitUVSSServer();
				break;
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
		"2: set port\n"
		"3: set ConnectionInfoCallback Function\n"
		"4: send CheckInfo\n"
		//"5: uninit\n"
		"-1: exit\n";
}
