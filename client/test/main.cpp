#include <cstdio>
#include <iostream>
#include <string>
#include "uvssclientsdk.h"

void menu();

void STDCALL clientConnectionInfoCallback(int, int, const char*);
void STDCALL clientCheckInfoCallback(int, const char*, const char*,
        const char*, const char*, const char*, const char*, const char*);

int main(int argc, char* argv[])
{
    if (argc > 1) {
        std::cerr << "too many arguments" << std::endl;

        return 1;
    }

    SetUVSSMessageCallback(clientConnectionInfoCallback);
    SetUVSSCheckInfoCallback(clientCheckInfoCallback);

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
                    std::cout << "server IP:" << std::endl;
                    std::string ip;
//#ifdef LOCALHOST
//                    ip = "127.0.0.1";
//                    std::cout << ip << std::endl;
//#else
                    std::cin >> ip;
//#endif
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

    return 0;
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

void STDCALL clientCheckInfoCallback(int index,
        const char* uVSSImagePath, const char* plateImagePath,
        const char* channel, const char* plateNumber, const char* direction,
        const char* time, const char* extension)
{
    printf("\ncallback:\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            index, uVSSImagePath, plateImagePath,
            channel, plateNumber, direction, time, extension);
}

void STDCALL clientConnectionInfoCallback(int index, int type,
        const char* connectionInfo)
{
    printf("\ncallback:\n%d, %d, %s\n", index, type, connectionInfo);
}
