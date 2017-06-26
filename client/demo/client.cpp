#include <cstdlib>
#include <iostream>
#include <string>

#include <uvssclientwrapper.h>

void menu();
void onUvssMessageCallback(int, int, const char*);
void onUvssCheckInfoCallback(int, const char*, const char*, const char*,
                             const char*, const char*, const char*,
                             const char*);
void onUvssCheckInfoCallbackEx(int, const char*, const char*);

int main(int argc, char* argv[])
{
    if (argc > 1) {
        std::cerr << argv[0] << ": too many arguments" << std::endl;

        return EXIT_FAILURE;
    }

    menu();
    SetUVSSMessageCallback(onUvssMessageCallback);
#if 1
    SetUVSSCheckInfoCallback(onUvssCheckInfoCallback);
#endif
#if 1
    SetUVSSCheckInfoCallbackEx(onUvssCheckInfoCallbackEx);
#endif

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

void onUvssCheckInfoCallbackEx(int connectionId, const char* concatedString, const char* concatedFilePath)
{
    std::cout << "\ncallback:\n"
                 "connectionId: " << connectionId << "\n"
                 "concatedString: " << concatedString << "\n"
                 "concatedFilePath: " << concatedFilePath << "\n";
}
