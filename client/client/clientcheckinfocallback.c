#include "clientcheckinfocallback.h"

void __stdcall clientCheckInfoCallback(int handle, const char* CClientUVSSImagePath, const char* CClientPlateImagePath, const char* CClientChannel, const char* CClientPlateNumber, const char* CClientDirection, const char* CClientCheckDateTime, const char* CClientExtension)
{
	printf("\n");
	printf("%d\n", handle);
	printf("%s\n", CClientUVSSImagePath);
	printf("%s\n", CClientPlateImagePath);
	printf("%s\n", CClientChannel);
	printf("%s\n", CClientPlateNumber);
	printf("%s\n", CClientDirection);
	printf("%s\n", CClientCheckDateTime);
	printf("%s\n", CClientExtension);
}
