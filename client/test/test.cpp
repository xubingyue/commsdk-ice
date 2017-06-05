#include <cstdio>
#include <iostream>
#include <string>
#include <uvssclientwrapper.h>

void menu();

void clientConnectionInfoCallback(int, int, const char*);
void clientCheckInfoCallback(int, const char* const [], int,
                             const char* const [], int);

int main(int argc, char* /*argv*/[])
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

void clientCheckInfoCallback(int index,
                             const char* const a1[], int sz1,
                             const char* const a[], int sz)
{
    printf("\ncallback:\n%d\n", index);
    for (int i = 0; i != sz1; ++i) {
        std::cout << a1[i] << std::endl;
    }
    for (int i = 0; i != sz; ++i) {
        std::cout << a[i] << std::endl;
    }
}

void clientConnectionInfoCallback(int index, int type,
                                  const char* connectionInfo)
{
    printf("\ncallback:\n%d, %d, %s\n", index, type, connectionInfo);
}
