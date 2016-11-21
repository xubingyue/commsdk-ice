开发工具安装：
1.VS2013
2.Ice3.6.3
    安装目录：C:\ZeroC
    环境变量：C:\ZeroC\Ice-3.6.3\bin;C:\ZeroC\Ice-3.6.3\bin\x64
3.IceBuilder4.3.6
    可选。
    SDK程序用CMake管理，用不到插件。
    为了测试方便，在测试程序clientconsole、serverconsole中用此插件。
    
    VS2013->TOOLS->Options->Projects and Solutions->Ice Builder->Ice home directory: C:\ZeroC\Ice-3.6.3\。
    使用时在工程右键选择Add Ice Builder to Project。
    在工程右键->Properties->Common Properties->Ice Builder可以进行一些设置，如由Slice文件生成的C++文件位置（默认为generated\）。






目录结构：

─source
   │  .gitignore
   │  clientserver.ice
   │  README.txt
   │  UVSS二次开发文档.doc
   │
   ├─client
   │  │  clienti.cpp
   │  │  clienti.h
   │  │  CMakeLists.txt
   │  │  uvssclient.cpp
   │  │  uvssclient.h
   │  │  uvssclientsdk.cpp
   │  │  uvssclientsdk.h
   │  │  version.h.in
   │  │
   │  ├─build(新建)
   │  ├─clientconsole
   │  │      clientcheckinfocallback.c
   │  │      clientcheckinfocallback.h
   │  │      clientconnectioninfocallback.c
   │  │      clientconnectioninfocallback.h
   │  │      clientconsole.sln
   │  │      clientconsole.vcxproj
   │  │      clientconsole.vcxproj.filters
   │  │      main.cpp
   │  │      version.h
   │  │
   │  ├─include
   │  │      UVSSSDK.h
   │  │
   │  ├─x64(新建)
   │  └─x86(新建)
   └─server
       │  CMakeLists.txt
       │  serveri.cpp
       │  serveri.h
       │  uvssserver.cpp
       │  uvssserver.h
       │  uvssserversdk.cpp
       │  uvssserversdk.h
       │  version.h.in
       │
       ├─build(新建)
       ├─include
       │      UVSSServer.h
       │
       ├─serverconsole
       │      1.jpg
       │      2.jpg
       │      main.cpp
       │      serverconnectioninfocallback.c
       │      serverconnectioninfocallback.h
       │      serverconsole.sln
       │      serverconsole.vcxproj
       │      serverconsole.vcxproj.filters
       │      version.h
       │
       ├─x64(新建)
       └─x86(新建)






组织结构简单说明：
1.clientserver.ice是Slice文件，在source根目录下。
2.client和server两个工程会引用由clientserver.ice生成的C++代码（clientserver.h/cpp）。
3.clientserver.ice（接口文件）->clientserver.h/cpp（client和server的骨架类）----->
    （client端）----->clienti.h/cpp（client的实现类）->uvssclient.h/cpp（引用clienti）->uvssclientsdk.h/cpp（封装成C接口）。
    （server端）----->serveri.h/cpp（server的实现类）->uvssserver.h/cpp（引用serveri）->uvssserversdk.h/cpp（封装成C接口）。
4.clientconsole和serverconsole分别是client和server的控制台测试程序。
    clientconsole在client端SDK源码的基础上加入2个回调函数（C，控制台输出），和一个main函数（C++）。
    serverconsole在server端SDK源码的基础上加入1个回调函数（C，控制台输出），和一个main函数（C++）。






构建SDK：
在client或server目录下新建x86、x64、build目录。
进入build目录，命令行运行以下某条命令，生成相应版本的.sln，再编译得相应的库文件：

cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013" ..
cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013" ..
cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013 Win64" ..
cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013 Win64" ..

version.h.in中版本号(UVSS_COMM_SDK_VER)可以由CMakeLists.txt中指定(暂用"test"，发布时库文件时可用日期时间，如"20160911")，实际被引用的version.h由CMake在build目录下生成。
clientserver.ice在build目录下生成clientserver.h/cpp。






构建控制台测试程序：
为了调试和修改方便，使用IceBuilder，测试程序中直接引用SDK源码，而不是使用生成的SDK库文件，并使用了一个用于测试的version.h，版本号恒为"test"。
clientserver.ice在generated目录下生成clientserver.h/cpp。






使用测试程序：
分别启动serverconsole.exe和clientconsole.exe，可以分别启动多个。

server:
    0.帮助
    1.设置端口，默认20145
    2.初始化
    -2.反初始化
    3.发送信息（保证相关目录下有1.jpg和2.jpg）
    9.退出
client:
    0.帮助
    1.初始化
    -1.反初始化
    2.建立连接
    -2.断开连接
    9.退出

当server和client在一台机器上测试时，clientconsole工程中可以加入预编译宏LOCALHOST，每次连接时省去输入server IP，只需要输入端口号即可。
若要在局域网内测试，将工程中的预编译宏LOCALHOST去掉。

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
