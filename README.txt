Ice 3.7.0 支持的平台
    https://doc.zeroc.com/display/Rel/Supported+Platforms+for+Ice+3.7.0
slice2cpp有两种映射方式
    本程序在Windows7下编译时，仅采用VS2010下的C++98映射，和VS2015下的C++11映射
    Linux选择的发行版是Debian9，采用默认gcc版本6.3.0，使用C++98/C++11两种映射方式


开发工具安装：

Windows7
1.编译器
    VS2010、VS2015
2.Ice3.7.0
    https://doc.zeroc.com/display/Ice37/Using+the+Windows+Binary+Distributions
    参看NuGet Package Installation部分
    将项目文件夹下的packages/zeroc.ice.v100.3.7.0和packages/zeroc.ice.v140.3.7.0提取出来，此为Ice SDK
3.boost
    boost_1_62_0-msvc-10.0-32.exe
    boost_1_62_0-msvc-10.0-64.exe
    boost_1_62_0-msvc-14.0-32.exe
    boost_1_62_0-msvc-14.0-64.exe
4.cmake
    cmake-3.7.2-win64-x64.msi

Debian9
1.编译器
    sudo apt-get install build-essential
2.Ice3.7.0
    sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
    sudo apt-add-repository "deb http://dev.zeroc.com/apt/ice/debian9/3.7 stable main"
    sudo apt-get update
    sudo apt-get install zeroc-ice-all-runtime zeroc-ice-all-dev
    sudo apt-add-repository --remove "deb http://dev.zeroc.com/apt/ice/debian9/3.7 stable main"
    sudo rm -rf /etc/apt/sources.list.bak
3.boost
    sudo apt-get install libboost-all-dev
4.cmake
    sudo apt-get install cmake


目录结构：
uvsscomm/
├── callback.ice
├── client
│   ├── CMakeLists.txt
│   ├── demo
│   │   ├── client.cpp
│   │   └── CMakeLists.txt
│   ├── include
│   │   ├── callbackreceiveri.h
│   │   ├── global.h
│   │   ├── rpcproxies.h
│   │   ├── uvssclient.h
│   │   ├── uvssclientwrapper.h
│   │   └── workqueue.h
│   └── src
│       ├── callbackreceiveri.cpp
│       ├── CMakeLists.txt
│       ├── global.cpp
│       ├── rpcproxies.cpp
│       ├── uvssclient.cpp
│       ├── uvssclientwrapper.cpp
│       └── workqueue.cpp
├── README.txt
├── server
│   ├── CMakeLists.txt
│   ├── demo
│   │   ├── 1.jpg
│   │   ├── 2.jpg
│   │   ├── CMakeLists.txt
│   │   └── server.cpp
│   ├── include
│   │   ├── callbacksenderi.h
│   │   ├── global.h
│   │   ├── rpcproxies.h
│   │   ├── uvssserver.h
│   │   └── uvssserverwrapper.h
│   └── src
│       ├── callbacksenderi.cpp
│       ├── CMakeLists.txt
│       ├── global.cpp
│       ├── rpcproxies.cpp
│       ├── uvssserver.cpp
│       └── uvssserverwrapper.cpp
└── UVSS二次开发文档.doc


构建SDK：
在client或server目录下新建build目录
进入build目录，命令行运行以下某条命令

Windows7

    cmake .. -G "Visual Studio 10 2010 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug
    cmake .. -G "Visual Studio 10 2010 Win64" -DCMAKE_CONFIGURATION_TYPES=Release
    cmake .. -G "Visual Studio 10 2010" -DCMAKE_CONFIGURATION_TYPES=Debug
    cmake .. -G "Visual Studio 10 2010" -DCMAKE_CONFIGURATION_TYPES=Release

    cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=1
    cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Release -DCXX11=1
    cmake .. -G "Visual Studio 14 2015" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=1
    cmake .. -G "Visual Studio 14 2015" -DCMAKE_CONFIGURATION_TYPES=Release -DCXX11=1

Debian9
    cmake ..
    cmake .. -DCXX11=1

SDK版本号由callback.ice中的const string version确定
lib/内为生成的动态库文件
bin/内为生成的demo可执行程序
include/uvssserverwrapper.h和include/uvssclientwrapper.h为引用需要的头文件


demo程序的使用方法
使用测试程序：
分别启动serverconsole.exe和clientconsole.exe，可以分别启动多个。

server:
0:help
1: set port
2: init
-2: uninit
3: send checkInfo
9: exit

client:
0: help
1: init
-1: uninit
2: connect
-2: disconnect
9: exit


测试：
server：1//设置端口，默认20145
        20146
        2//初始化

client: 1//初始化
        2//连接
        20146

server: 3//发送信息

client: -2//断开
        1//第一次连接时server标号为1#，每次连接标号递增
        2//再一次连接
        20146

server: -2
        9//-2+9是正常关闭server

client：-1
        9//-1+9是正常关闭client
