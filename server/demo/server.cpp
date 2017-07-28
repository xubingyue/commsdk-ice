#include <cstdlib>
#include <iostream>
#include <string>

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
                SendUVSSCheckInfo("1.jpg", "2.jpg", "Channel 1", "ABC1234",
                                  "In", "2016/1/1 13:01:02", "old");
                break;
            case 4:
                {
                    std::string endpoint;
                    std::cin >> endpoint;
                    SendUVSSCheckInfoByEndpoint(
                        endpoint.c_str(),
                        "1.jpg", "2.jpg",
                        "Channel 1", "ABC1234", "In", "2016/1/1 13:01:02", "old");
                }
                break;
            case 5:
                {
                    const char* concatedString =
                        "Channel 1|ABC1234|In|2016/1/1 13:01:02|ex";
                    const char* concatedFilePath = "1.jpg|2.jpg";
                    SendUVSSCheckInfoEx(concatedString, concatedFilePath);
                }
                break;
            case 6:
                {
                    std::string endpoint;
                    std::cin >> endpoint;
                    const char* concatedString =
                        "Channel 1|ABC1234|In|2016/1/1 13:01:02|ex";
                    const char* concatedFilePath = "1.jpg|2.jpg";
                    SendUVSSCheckInfoByEndpointEx(endpoint.c_str(), concatedString, concatedFilePath);
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
    std::cout << "server version: " << GetUVSSServerVersion() << "\n"
                 "usage:\n"
                 "0:help\n"
                 "1: set port\n"
                 "2: init\n"
                 "-2: uninit\n"
                 "3: send checkInfo\n"
                 "4: send checkInfo by endpoint\n"
                 "5: send checkInfo ex\n"
                 "6: send checkInfo by endpoint ex\n"
                 "9: exit\n";
}

void onUvssServerCallback(int code, const char* message)
{
    std::cout << "\ncallback:\n"
                 "code: " << code << "\n"
                 "message: " << message << "\n";
}
