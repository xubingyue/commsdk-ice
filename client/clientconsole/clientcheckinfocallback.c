#include "clientcheckinfocallback.h"

void __stdcall clientCheckInfoCallback(int handle, const char* clientUVSSImagePath, const char* clientPlateImagePath, const char* clientChannel, const char* clientPlateNumber, const char* clientDirection, const char* clientCheckDateTime, const char* clientExtension)
{
	printf("\n");
	printf("%d\n", handle);
	printf("%s\n", clientUVSSImagePath);
	printf("%s\n", clientPlateImagePath);
	printf("%s\n", clientChannel);
	printf("%s\n", clientPlateNumber);
	printf("%s\n", clientDirection);
	printf("%s\n", clientCheckDateTime);
	printf("%s\n", clientExtension);
}
