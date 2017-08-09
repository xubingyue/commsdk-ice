#include <cstdlib>
#include <iostream>
#include <string>

#include <uvssclientwrapper.h>

void menu();
void onUvssMessageCallback(int, int, const char*);
void onUvssCheckInfoCallback(int, const char*, const char*, const char*,
                             const char*, const char*, const char*,
                             const char*);
void onUvssCheckInfoExCallback(int, const char*, const char*);

int main(int argc, char* argv[])
{
    if (argc > 1) {
        std::cerr << argv[0] << ": too many arguments" << std::endl;

        return EXIT_FAILURE;
    }

    SetUVSSMessageCallback(onUvssMessageCallback);
    SetUVSSCheckInfoCallback(onUvssCheckInfoCallback);
    SetUVSSCheckInfoExCallback(onUvssCheckInfoExCallback);

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
            case -1:
                UVSSUninitialize();
                break;
            case 2:
                {
                    std::cout << "server ip:" << std::endl;
                    std::string ip;
                    std::cin >> ip;

                    std::cout << "server port:" << std::endl;
                    int port;
                    std::cin >> port;

                    UVSSConnect(ip.c_str(), port);
                }
                break;
            case -2:
                {
                    std::cout << "server connectionId:" << std::endl;
                    int connectionId;
                    std::cin >> connectionId;
                    UVSSDisconnect(connectionId);
                }
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
                    SendUVSSCheckInfoExByEndpoint(endpoint.c_str(), concatedString, concatedFilePath);
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
    std::cout << "client version: " << GetUVSSVersion() << "\n"
                 "usage:\n"
                 "0: help\n"
                 "1: init\n"
                 "-1: uninit\n"
                 "2: connect\n"
                 "-2: disconnect\n"
                 "3: send checkInfo\n"
                 "4: send checkInfo by endpoint\n"
                 "5: send checkInfoEx\n"
                 "6: send checkInfoEx by endpoint\n"
                 "9: exit\n";
}

void onUvssMessageCallback(int connectionId, int code, const char* message)
{
    std::cout << "\ncallback:\n"
                 "connectionId: " << connectionId << "\n"
                 "code: " << code << "\n"
                 "message: " << message << "\n";
}

void onUvssCheckInfoCallback(int connectionId, const char* uvssImagePath,
                             const char* plateImagePath, const char* channel,
                             const char* plateNumber, const char* direction,
                             const char* dateTime, const char* extension)
{
    std::cout << "\ncallback:\n"
                 "connectionId: " << connectionId << "\n"
                 "uvssImagePath: " << uvssImagePath << "\n"
                 "plateImagePath: " << plateImagePath << "\n"
                 "channel: " << channel << "\n"
                 "plateNumber: " << plateNumber << "\n"
                 "direction: " << direction << "\n"
                 "dateTime: " << dateTime << "\n"
                 "extension: " << extension << "\n";
}

void onUvssCheckInfoExCallback(int connectionId, const char* concatedString,
                               const char* concatedFilePath)
{
    std::cout << "\ncallback:\n"
                 "connectionId: " << connectionId << "\n"
                 "concatedString: " << concatedString << "\n"
                 "concatedFilePath: " << concatedFilePath << "\n";
}
