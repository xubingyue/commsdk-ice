�������߰�װ��
1.VS2013
2.Ice3.6.3
    ��װĿ¼��C:\ZeroC
    ����������C:\ZeroC\Ice-3.6.3\bin;C:\ZeroC\Ice-3.6.3\bin\x64
3.IceBuilder4.3.6
    ��ѡ��
    SDK������CMake�����ò��������
    Ϊ�˲��Է��㣬�ڲ��Գ���clientconsole��serverconsole���ô˲����
    
    VS2013->TOOLS->Options->Projects and Solutions->Ice Builder->Ice home directory: C:\ZeroC\Ice-3.6.3\��
    ʹ��ʱ�ڹ����Ҽ�ѡ��Add Ice Builder to Project��
    �ڹ����Ҽ�->Properties->Common Properties->Ice Builder���Խ���һЩ���ã�����Slice�ļ����ɵ�C++�ļ�λ�ã�Ĭ��Ϊgenerated\����






Ŀ¼�ṹ��

��source
   ��  .gitignore
   ��  clientserver.ice
   ��  README.txt
   ��  UVSS���ο����ĵ�.doc
   ��
   ����client
   ��  ��  clienti.cpp
   ��  ��  clienti.h
   ��  ��  CMakeLists.txt
   ��  ��  uvssclient.cpp
   ��  ��  uvssclient.h
   ��  ��  uvssclientsdk.cpp
   ��  ��  uvssclientsdk.h
   ��  ��  version.h.in
   ��  ��
   ��  ����build(�½�)
   ��  ����clientconsole
   ��  ��      clientcheckinfocallback.c
   ��  ��      clientcheckinfocallback.h
   ��  ��      clientconnectioninfocallback.c
   ��  ��      clientconnectioninfocallback.h
   ��  ��      clientconsole.sln
   ��  ��      clientconsole.vcxproj
   ��  ��      clientconsole.vcxproj.filters
   ��  ��      main.cpp
   ��  ��      version.h
   ��  ��
   ��  ����include
   ��  ��      UVSSSDK.h
   ��  ��
   ��  ����x64(�½�)
   ��  ����x86(�½�)
   ����server
       ��  CMakeLists.txt
       ��  serveri.cpp
       ��  serveri.h
       ��  uvssserver.cpp
       ��  uvssserver.h
       ��  uvssserversdk.cpp
       ��  uvssserversdk.h
       ��  version.h.in
       ��
       ����build(�½�)
       ����include
       ��      UVSSServer.h
       ��
       ����serverconsole
       ��      1.jpg
       ��      2.jpg
       ��      main.cpp
       ��      serverconnectioninfocallback.c
       ��      serverconnectioninfocallback.h
       ��      serverconsole.sln
       ��      serverconsole.vcxproj
       ��      serverconsole.vcxproj.filters
       ��      version.h
       ��
       ����x64(�½�)
       ����x86(�½�)






��֯�ṹ��˵����
1.clientserver.ice��Slice�ļ�����source��Ŀ¼�¡�
2.client��server�������̻�������clientserver.ice���ɵ�C++���루clientserver.h/cpp����
3.clientserver.ice���ӿ��ļ���->clientserver.h/cpp��client��server�ĹǼ��ࣩ----->
    ��client�ˣ�----->clienti.h/cpp��client��ʵ���ࣩ->uvssclient.h/cpp������clienti��->uvssclientsdk.h/cpp����װ��C�ӿڣ���
    ��server�ˣ�----->serveri.h/cpp��server��ʵ���ࣩ->uvssserver.h/cpp������serveri��->uvssserversdk.h/cpp����װ��C�ӿڣ���
4.clientconsole��serverconsole�ֱ���client��server�Ŀ���̨���Գ���
    clientconsole��client��SDKԴ��Ļ����ϼ���2���ص�������C������̨���������һ��main������C++����
    serverconsole��server��SDKԴ��Ļ����ϼ���1���ص�������C������̨���������һ��main������C++����






����SDK��
��client��serverĿ¼���½�x86��x64��buildĿ¼��
����buildĿ¼����������������ĳ�����������Ӧ�汾��.sln���ٱ������Ӧ�Ŀ��ļ���

cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013" ..
cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013" ..
cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013 Win64" ..
cmake -DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013 Win64" ..

version.h.in�а汾��(UVSS_COMM_SDK_VER)������CMakeLists.txt��ָ��(����"test"������ʱ���ļ�ʱ��������ʱ�䣬��"20160911")��ʵ�ʱ����õ�version.h��CMake��buildĿ¼�����ɡ�
clientserver.ice��buildĿ¼������clientserver.h/cpp��






��������̨���Գ���
Ϊ�˵��Ժ��޸ķ��㣬ʹ��IceBuilder�����Գ�����ֱ������SDKԴ�룬������ʹ�����ɵ�SDK���ļ�����ʹ����һ�����ڲ��Ե�version.h���汾�ź�Ϊ"test"��
clientserver.ice��generatedĿ¼������clientserver.h/cpp��






ʹ�ò��Գ���
�ֱ�����serverconsole.exe��clientconsole.exe�����Էֱ����������

server:
    0.����
    1.���ö˿ڣ�Ĭ��20145
    2.��ʼ��
    -2.����ʼ��
    3.������Ϣ����֤���Ŀ¼����1.jpg��2.jpg��
    9.�˳�
client:
    0.����
    1.��ʼ��
    -1.����ʼ��
    2.��������
    -2.�Ͽ�����
    9.�˳�

��server��client��һ̨�����ϲ���ʱ��clientconsole�����п��Լ���Ԥ�����LOCALHOST��ÿ������ʱʡȥ����server IP��ֻ��Ҫ����˿ںż��ɡ�
��Ҫ�ھ������ڲ��ԣ��������е�Ԥ�����LOCALHOSTȥ����

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
