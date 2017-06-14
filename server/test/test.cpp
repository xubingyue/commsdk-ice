#include <cstdlib>
#include <iostream>

#include <uvssserverwrapper.h>

void menu();
void onUvssServerCallback(int, const char*);

int main(int argc, char* argv[])
{
    if (argc > 1) {
        std::cerr << argv[0] << ": too many arguments" << std::endl;
        return EXIT_FAILURE;
    }

    menu();
    SetUVSSServerCallback(onUvssServerCallback);

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
                    std::cout << "port:" << std::endl;
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
                {
#if 0
                    SendUVSSCheckInfo(
                        "1.jpg", "2.jpg",
                        "Channel 1", "ABC1234", "In", "2016/1/1 13:01:02", "extend information");
#endif

#if 1
                    const char* const strings[] = { "Channel 1", "ABC1234", "In", "2016/1/1 13:01:02", "extend information", "test1", "test2", NULL };
                    const char* const filePaths[] = { "1.jpg", "2.jpg", NULL };
                    SendUVSSCheckInfoNormal(strings, filePaths);
#endif

#if 0
                const char* stringsSrc = "Channel 1|ABC1234|In|2016/1/1 13:01:02|extend information|test1|test2";
                const char* filePathsSrc = "1.jpg|2.jpg";
                SendUVSSCheckInfoEx(stringsSrc, filePathsSrc);
                SendUVSSCheckInfoEx(stringsSrc, filePathsSrc);
                SendUVSSCheckInfoEx(stringsSrc, filePathsSrc);
#endif
                }
                break;
            case 9:
                break;
            }
        }
        catch (...) {
            throw;
        }
    } while ((std::cin.good()) && (key != 9));

    return EXIT_SUCCESS;
}

void menu()
{
    std::cout << "Server Version: " << GetUVSSServerVersion() << std::endl;
    std::cout << "Usage:\n"
                "0: help\n"
                "1: set port\n"
                "2: init\n"
                "-2: uninit\n"
                "3: send CheckInfo\n"
                "9: exit\n";
}

void onUvssServerCallback(int type, const char* message)
{
    std::cout << "callback\n" << type << ", " << message << std::endl;
}

