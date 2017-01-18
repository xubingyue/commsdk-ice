#include "uvssserversdk.h"

#include <cstdio>
#include <iostream>

void menu();
void serverConnectionInfoCallback(int, const char*);

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
                SendUVSSCheckInfo(
                        "1.jpg", "2.jpg",
                        "通道1", "鲁A12345", "进车", "2016", "扩展信息");
                break;
            case 9:
                break;
            }
        }
        catch (...) {
        }
    } while ((std::cin.good()) && (key != 9));

    return 0;
}

void menu()
{
    std::cout << "ServerVersion: " << GetUVSSServerVersion() << std::endl;
    std::cout << "usage:\n"
        "0: help\n"
        "1: set port\n"
        "2: init\n"
        "-2: uninit\n"
        "3: send CheckInfo\n"
        "9: exit\n";
}

void serverConnectionInfoCallback(int type,
        const char* connectionInfo)
{
    printf("\ncallback:\n%d, %s\n", type, connectionInfo);
}

