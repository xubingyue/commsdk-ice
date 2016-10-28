#ifndef UVSSSDK_H
#define UVSSSDK_H

// UVSSMessageCallback: ������Ϣ�ص���������
// handle: ���ӱ�ʶ (��Connect����, �������ڷ��������������ӽ��б�ʶ)
// ע: ��û�ж�Ӧ����ʱ (��SDK��ʼ��ʧ�ܻ�����UVSS������ʧ��ʱ), ֵΪ-1
// code: ��Ϣ���
// 1: ���ӳɹ� (UVSSConnect)
// -1: ��ʼ������ (UVSSInitialize)
// -2: ����ʧ�� (UVSSConnect)
// -3: ���ӶϿ�
// message: ��Ϣ����
typedef void (__stdcall *UVSSMessageCallback)(int handle, int code,
        const char *message);

// UVSSCheckInfoCallback: ���������Ϣ�ص���������
// handle: ���ӱ�ʶ (��Connect����, �������ڷ��������������ӽ��б�ʶ)
// UVSSImagePath: ����ͼ��·��
// PlateImagePath: ����ͼ��·��
// Channel: ����ͨ����Ϣ
// PlateNumber: ���ƺ���
// Direction: ��������
// CheckDateTime: �������ʱ�� (��ʽ: 2016/1/1 13:01:02)
// Extension: ��չ��Ϣ, һ��Ϊ�� ("")
typedef void (__stdcall *UVSSCheckInfoCallback)(int handle,
        const char *UVSSImagePath, const char *PlateImagePath,
        const char *Channel, const char *PlateNumber, const char *Direction,
        const char *CheckDateTime, const char *Extension);

// SetUVSSMessageCallback: ���õ�����Ϣ�ص�����
void SetUVSSMessageCallback(UVSSMessageCallback callback);

// SetUVSSCheckInfoCallback: ���ó��������Ϣ�ص�����
void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback callback);

// UVSSInitialize: ��ʼ��SDK (�������һ��)
// ����ֵ: 1: ��ʼ���ɹ� -1: ����
int UVSSInitialize();

// UVSSUninitialize: ����SDK, �ͷ�ռ�õ���Դ (���ڳ����˳�ʱ����һ��)
void UVSSUninitialize();

// UVSSConnect: ����UVSS������
// ServerIPAddress: UVSS������IP��ַ, Ĭ��ֵ: 127.0.0.1
// ServerPort: UVSS�������˿�, Ĭ��ֵ: 20145
// ����ֵ:
// ���ӳɹ�: ����0�����ӱ�ʶ (handle)
// ����: -1
// ������: -2 (����������, ���ѽ���������Ӱ��)
// ��������汾�Ų�һ��: -3 (����������)
int UVSSConnect(const char *ServerIPAddress, int ServerPort);

// UVSSDisconnect: ��������Ͽ�����
// handle: ���ӱ�ʶ (��Connect����, �������ڷ��������������ӽ��б�ʶ)
// ����ֵ: 1: �ɹ�, -1: ��ָ��handle��Ӧ������
int UVSSDisconnect(int handle);

#endif
