#include <cstdio>
#include <iostream>

#include <uvssserverwrapper.h>

void menu();
void serverConnectionInfoCallback(int, const char*);

int main(int argc, char* /*argv*/[])
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
            {
                const char* const a1[2] = { "1.jpg", "2.jpg" };
                const char* const a[7] = { "Channel 1", "ABC1234", "In", "2016/1/1 13:01:02", "extend information", "test1", "test2" };
                SendUVSSCheckInfo(a1, 2, a, 7);
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
