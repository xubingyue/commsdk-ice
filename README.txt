Ice 3.7.0 支持的平台
    https://doc.zeroc.com/display/Rel/Supported+Platforms+for+Ice+3.7.0
slice2cpp有两种映射方式
    本程序在Windows7下编译时，仅采用VS2010下的C++98映射，和VS2015下的C++11映射
    Linux下选择的发行版是Debian9，采用默认gcc版本6.3.0，使用C++98/C++11两种映射方式


开发工具安装：

Windows7
1.编译器
    VS2010(SP1)、VS2015(Update3)
2.Ice3.7.0
    https://doc.zeroc.com/display/Ice37/Using+the+Windows+Binary+Distributions
    参看NuGet Package Installation部分
    将工程目录下的packages/zeroc.ice.v100.3.7.0和packages/zeroc.ice.v140.3.7.0提取出来，此为Ice SDK
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


Windows7下第三方库安装位置：
C:\local\zeroc.ice.v100.3.7.0
C:\local\zeroc.ice.v140.3.7.0
C:\local\boost_1_62_0-msvc-10.0-32
C:\local\boost_1_62_0-msvc-10.0-64
C:\local\boost_1_62_0-msvc-14.0-32
C:\local\boost_1_62_0-msvc-14.0-64


目录结构：
hf@debian:~/dev$ tree tinycomm/
tinycomm/
├── callback.ice
├── client
│   ├── CMakeLists.txt
│   ├── demo
│   │   ├── 1.jpg
│   │   ├── 2.jpg
│   │   ├── client-demo.cpp
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
│   │   └── server-demo.cpp
│   ├── include
│   │   ├── callbacksenderi.h
│   │   ├── global.h
│   │   ├── rpcproxies.h
│   │   ├── uvssserver.h
│   │   ├── uvssserverwrapper.h
│   │   └── workqueue.h
│   └── src
│       ├── callbacksenderi.cpp
│       ├── CMakeLists.txt
│       ├── global.cpp
│       ├── rpcproxies.cpp
│       ├── uvssserver.cpp
│       ├── uvssserverwrapper.cpp
│       └── workqueue.cpp
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

    构建时，需要更改部分源码编码格式为"Chinese Simplified(GB2312)-Codepage 936"
    (Visual Studio --> File --> Advanced Save Options)

Debian9
    cmake ..
    cmake .. -DCXX11=1

    make

SDK版本号由callback.ice中的const string version确定
lib/内为生成的动态库文件
bin/内为生成的demo可执行程序
include/uvssserverwrapper.h和include/uvssclientwrapper.h为引用需要的头文件


使用demo程序：
分别启动server-demo和client-demo，可以启动多个

./server-demo
server version: 1.0
usage:
0: help
1: set port
2: init
-2: uninit
3: send checkInfo
4: send checkInfo by endpoint
5: send checkInfoEx
6: send checkInfoEx by endpoint
9: exit

./client-demo
client version: 1.0
usage:
0: help
1: init
-1: uninit
2: connect
-2: disconnect
3: send checkInfo
4: send checkInfo by endpoint
5: send checkInfoEx
6: send checkInfoEx by endpoint
9: exit

测试：
server：1 // 设置端口，默认20145
        20146
        2 // 初始化

client: 1 // 初始化
        2 // 连接
        127.0.0.1
        20146

server: 3 // 发送检查信息
        4 // 按端点发送检查信息
        127.0.0.1:38618
        5 // 发送检查信息Ex
        6 // 按端点发送检查信息Ex
        127.0.0.1:38618

client: 3 // 发送检查信息
        4 // 按端点发送检查信息
        127.0.0.1:20145
        5 // 发送检查信息Ex
        6 // 按端点发送检查信息Ex
        127.0.0.1:20145

client: -2 // 断开
        1 // connectionId
        2 // 再一次连接
        127.0.0.1
        20146

server: -2
        9 // -2+9是正常关闭server

client：-1
        9 // -1+9是正常关闭client
