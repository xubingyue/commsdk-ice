Ice 3.7.0 ֧�ֵ�ƽ̨
    https://doc.zeroc.com/display/Rel/Supported+Platforms+for+Ice+3.7.0
slice2cpp������ӳ�䷽ʽ
    ��������Windows7�±���ʱ��������VS2010�µ�C++98ӳ�䣬��VS2015�µ�C++11ӳ��
    Linuxѡ��ķ��а���Debian9������Ĭ��gcc�汾6.3.0��ʹ��C++98/C++11����ӳ�䷽ʽ


�������߰�װ��

Windows7
1.������
    VS2010��VS2015
2.Ice3.7.0
    https://doc.zeroc.com/display/Ice37/Using+the+Windows+Binary+Distributions
    �ο�NuGet Package Installation����
    ����Ŀ�ļ����µ�packages/zeroc.ice.v100.3.7.0��packages/zeroc.ice.v140.3.7.0��ȡ��������ΪIce SDK
3.boost
    boost_1_62_0-msvc-10.0-32.exe
    boost_1_62_0-msvc-10.0-64.exe
    boost_1_62_0-msvc-14.0-32.exe
    boost_1_62_0-msvc-14.0-64.exe
4.cmake
    cmake-3.7.2-win64-x64.msi

Debian9
1.������
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


Ŀ¼�ṹ��
uvsscomm/
������ callback.ice
������ client
���0�2�0�2 ������ CMakeLists.txt
���0�2�0�2 ������ demo
���0�2�0�2 ���0�2�0�2 ������ client.cpp
���0�2�0�2 ���0�2�0�2 ������ CMakeLists.txt
���0�2�0�2 ������ include
���0�2�0�2 ���0�2�0�2 ������ callbackreceiveri.h
���0�2�0�2 ���0�2�0�2 ������ global.h
���0�2�0�2 ���0�2�0�2 ������ rpcproxies.h
���0�2�0�2 ���0�2�0�2 ������ uvssclient.h
���0�2�0�2 ���0�2�0�2 ������ uvssclientwrapper.h
���0�2�0�2 ���0�2�0�2 ������ workqueue.h
���0�2�0�2 ������ src
���0�2�0�2     ������ callbackreceiveri.cpp
���0�2�0�2     ������ CMakeLists.txt
���0�2�0�2     ������ global.cpp
���0�2�0�2     ������ rpcproxies.cpp
���0�2�0�2     ������ uvssclient.cpp
���0�2�0�2     ������ uvssclientwrapper.cpp
���0�2�0�2     ������ workqueue.cpp
������ README.txt
������ server
���0�2�0�2 ������ CMakeLists.txt
���0�2�0�2 ������ demo
���0�2�0�2 ���0�2�0�2 ������ 1.jpg
���0�2�0�2 ���0�2�0�2 ������ 2.jpg
���0�2�0�2 ���0�2�0�2 ������ CMakeLists.txt
���0�2�0�2 ���0�2�0�2 ������ server.cpp
���0�2�0�2 ������ include
���0�2�0�2 ���0�2�0�2 ������ callbacksenderi.h
���0�2�0�2 ���0�2�0�2 ������ global.h
���0�2�0�2 ���0�2�0�2 ������ rpcproxies.h
���0�2�0�2 ���0�2�0�2 ������ uvssserver.h
���0�2�0�2 ���0�2�0�2 ������ uvssserverwrapper.h
���0�2�0�2 ������ src
���0�2�0�2     ������ callbacksenderi.cpp
���0�2�0�2     ������ CMakeLists.txt
���0�2�0�2     ������ global.cpp
���0�2�0�2     ������ rpcproxies.cpp
���0�2�0�2     ������ uvssserver.cpp
���0�2�0�2     ������ uvssserverwrapper.cpp
������ UVSS���ο����ĵ�.doc


����SDK��
��client��serverĿ¼���½�buildĿ¼
����buildĿ¼����������������ĳ������

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

SDK�汾����callback.ice�е�const string versionȷ��
lib/��Ϊ���ɵĶ�̬���ļ�
bin/��Ϊ���ɵ�demo��ִ�г���
include/uvssserverwrapper.h��include/uvssclientwrapper.hΪ������Ҫ��ͷ�ļ�


demo�����ʹ�÷���
ʹ�ò��Գ���
�ֱ�����serverconsole.exe��clientconsole.exe�����Էֱ����������

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


���ԣ�
server��1//���ö˿ڣ�Ĭ��20145
        20146
        2//��ʼ��

client: 1//��ʼ��
        2//����
        20146

server: 3//������Ϣ

client: -2//�Ͽ�
        1//��һ������ʱserver���Ϊ1#��ÿ�����ӱ�ŵ���
        2//��һ������
        20146

server: -2
        9//-2+9�������ر�server

client��-1
        9//-1+9�������ر�client
