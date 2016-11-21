#ifndef UVSSServer_H
#define UVSSServer_H

typedef void (__stdcall *UVSSServerCallback)(int messageType,
        const char *message);

#ifdef __cplusplus
extern "C" {
#endif

void SetUVSSServerCallback(UVSSServerCallback callback);
void SetUVSSServerPort(int port);
int InitUVSSServer();
void UninitUVSSServer();
void SendUVSSCheckInfo(const char *UVSSImagePath, const char *PlateImagePath,
        const char *Channel, const char *PlateNumber, const char *Direction,
        const char *CheckDateTime, const char *Extension);
const char *GetUVSSServerVersion();

#ifdef __cplusplus
}
#endif

#endif
