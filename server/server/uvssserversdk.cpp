#include "uvssserversdk.h"

UVSSServer serverSDK;

void SetUVSSServerPort(int port)
{
	serverSDK.setPortNumber(port);
}

int InitUVSSServer()
{
	return serverSDK.init();
}

void UninitUVSSServer()
{
	serverSDK.uninit();
}

void SetUVSSServerCallback(UVSSServerCallback callback)
{
	serverSDK.setConnectionInfoCallback(callback);
}

void SendUVSSCheckInfo(const char *UVSSImagePath, const char *PlateImagePath, const char *Channel, const char *PlateNumber, const char *Direction, const char *CheckDateTime, const char *Extension)
{
	serverSDK.sendCheckInfo(UVSSImagePath, PlateImagePath, Channel, PlateNumber, Direction, CheckDateTime, Extension);
}
