#ifndef UVSSCLIENTWRAPPER_H
#define UVSSCLIENTWRAPPER_H

#ifdef _WIN32
#   ifdef UVSS_EXPORTS
#      define UVSS_API _declspec(dllexport)
#   else
#      define UVSS_API _declspec(dllimport)
#   endif
#else
#   define UVSS_API
#endif

// UVSSMessageCallback: ������Ϣ�ص���������
// connectionId: ���ӱ�ʶ (��UVSSConnect����, ������ʶ��UVSS����������������)
// ע: ���������ֵΪ������; ��û�ж�Ӧ����ʱ (SDK��ʼ��ʧ�ܻ�����UVSS������ʧ��), ֵΪ-1
// code: ��Ϣ���
// 1: ���ӳɹ� (UVSSConnect)
// -1: ��ʼ������ (UVSSInitialize)
// -2: ����ʧ�� (UVSSConnect)
// -3: ���ӶϿ�
// message: ��Ϣ����
typedef void(*UVSSMessageCallback)(int connectionId, int code,
                                   const char* message);

// UVSSCheckInfoCallback: ���������Ϣ�ص���������
// connectionId: ���ӱ�ʶ (��UVSSConnect����, ������ʶ��UVSS����������������)
// uvssImagePath: ����ͼ��·��
// plateImagePath: ����ͼ��·��
// channel: ����ͨ����Ϣ
// plateNumber: ���ƺ���
// direction: ��������
// dateTime: �������ʱ�� (��ʽ: yyyy/m/d h:mm:ss, ��2016/1/1 13:01:02)
// extension: ��չ��Ϣ, һ��Ϊ�� ("")
typedef void(*UVSSCheckInfoCallback)(int connectionId,
                                     const char* uvssImagePath,
                                     const char* plateImagePath,
                                     const char* channel,
                                     const char* plateNumber,
                                     const char* direction,
                                     const char* dateTime,
                                     const char* extension);

// UVSSCheckInfoExCallback: ���������ϢEx�ص���������
// connectionId: ���ӱ�ʶ (��UVSSConnect����, ������ʶ��UVSS����������������)
// concatedString: �����ַ���
// concatedFilePath: �����������ļ�·�����ӳɵ��ַ���, ��·������ "|" �ָ�
typedef void(*UVSSCheckInfoExCallback)(int connectionId,
                                       const char* concatedString,
                                       const char* concatedFilePath);

#ifdef __cplusplus
extern "C" {
#endif

// GetUVSSVersion: ��ȡ�ͻ��˰汾��
// ����ֵ���ͻ��˰汾��
UVSS_API const char* GetUVSSVersion();

// SetUVSSMessageCallback: ���õ�����Ϣ�ص�����
UVSS_API void SetUVSSMessageCallback(UVSSMessageCallback uvssMessageCallback);

// SetUVSSCheckInfoCallback: ���ó��������Ϣ�ص�����
UVSS_API void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback
                                       uvssCheckInfoCallback);

// SetUVSSCheckInfoExCallback: ���ó��������ϢEx�ص�����
UVSS_API void SetUVSSCheckInfoExCallback(UVSSCheckInfoExCallback
                                         uvssCheckInfoExCallback);

// UVSSInitialize: ��ʼ��SDK (�������һ��)
// ����ֵ: 1: ��ʼ���ɹ�, -1: ����
UVSS_API int UVSSInitialize();

// UVSSConnect: ����UVSS������
// ipAddress: UVSS������IP��ַ, Ĭ��ֵ: 127.0.0.1
// port: UVSS�������˿�, Ĭ��ֵ: 20145
// ����ֵ:
// ����0�����ӱ�ʶ (connectionId): ���ӳɹ�
// -1: ����
// -2: ������(����������, ���ѽ���������Ӱ��)
// -3: ��UVSS�������汾�Ų�һ��(����������)
UVSS_API int UVSSConnect(const char* ipAddress, int port);

// UVSSDisconnect: ��UVSS�������Ͽ�����
// connectionId: ���ӱ�ʶ (��UVSSConnect����, ������ʶ��UVSS����������������)
// ����ֵ: 1: �ɹ�, -1: ��connectionId��Ӧ������
UVSS_API int UVSSDisconnect(int connectionId);

// SendUVSSCheckInfo: ���ͳ��������Ϣ
// uvssImagePath: ����ͼ��·��
// plateImagePath: ����ͼ��·��
// channel: ����ͨ����Ϣ
// plateNumber: ���ƺ���
// direction: ��������
// dateTime: �������ʱ�� (��ʽ: yyyy/m/d h:mm:ss, ��2016/1/1 13:01:02)
// extension: ��չ��Ϣ, һ��Ϊ�� ("")
UVSS_API void SendUVSSCheckInfo(const char* uvssImagePath,
                                const char* plateImagePath,
                                const char* channel,
                                const char* plateNumber,
                                const char* direction,
                                const char* dateTime,
                                const char* extension);

// SendUVSSCheckInfoByEndpoint: ��UVSS�������˵㷢�ͳ��������Ϣ
// endpoint: UVSS�������˵� (��ʽ: ipAddress:port, ��127.0.0.1:20145)
// uvssImagePath: ����ͼ��·��
// plateImagePath: ����ͼ��·��
// channel: ����ͨ����Ϣ
// plateNumber: ���ƺ���
// direction: ��������
// dateTime: �������ʱ�� (��ʽ: yyyy/m/d h:mm:ss, ��2016/1/1 13:01:02)
// extension: ��չ��Ϣ, һ��Ϊ�� ("")
UVSS_API void SendUVSSCheckInfoByEndpoint(const char* endpoint,
                                          const char* uvssImagePath,
                                          const char* plateImagePath,
                                          const char* channel,
                                          const char* plateNumber,
                                          const char* direction,
                                          const char* dateTime,
                                          const char* extension);

// SendUVSSCheckInfoEx: ���ͳ��������ϢEx
// concatedString: �����ַ���
// concatedFilePath: �����������ļ�·�����ӳɵ��ַ���, ��·������ "|" �ָ�
UVSS_API void SendUVSSCheckInfoEx(const char* concatedString,
                                  const char* concatedFilePath);

// SendUVSSCheckInfoExByEndpoint: ��UVSS�������˵㷢�ͳ��������ϢEx
// endpoint: UVSS�������˵� (��ʽ: ipAddress:port, ��127.0.0.1:20145)
// concatedString: �����ַ���
// concatedFilePath: �����������ļ�·�����ӳɵ��ַ���, ��·������ "|" �ָ�
UVSS_API void SendUVSSCheckInfoExByEndpoint(const char* endpoint,
                                            const char* concatedString,
                                            const char* concatedFilePath);

// UVSSUninitialize: ����SDK, �ͷ�ռ�õ���Դ (���ڳ����˳�ʱ����һ��)
UVSS_API void UVSSUninitialize();

#ifdef __cplusplus
}
#endif

#endif
