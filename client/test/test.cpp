#include <cstdlib>
#include <iostream>

#include <uvssclientwrapper.h>

void menu();
void onUvssMessageCallback(int, int, const char*);
void onUvssCheckInfoCallback(int, const char*, const char*, const char*, const char*, const char*, const char*, const char*);
void onUvssCheckInfoCallbackNormal(int, const char* const [], int,
                             const char* const [], int);
void onUvssCheckInfoCallbackNew(int, const char*, const char*);

int main(int argc, char* argv[])
{
    if (argc > 1) {
        std::cerr << argv[0] << ": too many arguments" << std::endl;

        return EXIT_FAILURE;
    }

    menu();
    SetUVSSMessageCallback(onUvssMessageCallback);
#if 0
    SetUVSSCheckInfoCallback(onUvssCheckInfoCallback);
#endif

#if 0
    SetUVSSCheckInfoCallbackNormal(onUvssCheckInfoCallbackNormal);
#endif

#if 1
    SetUVSSCheckInfoCallbackNew(onUvssCheckInfoCallbackNew);
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
                std::cout << "server IP:" << std::endl;
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
                std::cout << "server index:" << std::endl;
                int index;
                std::cin >> index;
                UVSSDisconnect(index);
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
    std::cout << "ClientVersion: " << GetUVSSVersion() << std::endl;
    std::cout << "usage:\n"
              "0: help\n"
              "1: init\n"
              "-1: uninit\n"
              "2: connect\n"
              "-2: disconnect\n"
              "9: exit\n";
}

void onUvssMessageCallback(int connectionId, int type, const char* message)
{
    std::cout << "\ncallback:\n" << connectionId << ", " << type << ", " << message << std::endl;
}

void onUvssCheckInfoCallback(int index,
        const char* uvssImagePath, const char* plateImagePath,
        const char* channel, const char* plateNumber, const char* direction,
        const char* time, const char* extension)
{
    printf("\ncallback:\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            index, uvssImagePath, plateImagePath,
            channel, plateNumber, direction, time, extension);
}

void onUvssCheckInfoCallbackNormal(int connectionId,
                             const char* const filePaths[], int filePathsSize,
                             const char* const strings[], int stringsSize)
{
    std::cout << "\ncallback:\n" << connectionId << std::endl;
    std::cout << "filePathsSize: " << filePathsSize << std::endl;
    std::cout << "stringsSize: " << stringsSize << std::endl;
    
    for (int i = 0; i != filePathsSize; ++i) {
        std::cout << filePaths[i] << std::endl;
    }
    for (int i = 0; i != stringsSize; ++i) {
        std::cout << strings[i] << std::endl;
    }
}

void onUvssCheckInfoCallbackNew(int connectionId, const char* filePathsDst, const char* stringsDst)
{
    printf("\ncallback:\n%d\n%s\n%s\n", connectionId, filePathsDst, stringsDst);
}
