#include "clientcheckinfocallback.h"

void __stdcall clientCheckInfoCallback(
		int index,
		const char* uVSSImagePath, const char* plateImagePath,
		const char* channel, const char* plateNumber, const char* direction, const char* time, const char* extension)
{
	printf(
			"\ncallback:\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
			index,
			uVSSImagePath, plateImagePath,
			channel, plateNumber, direction, time, extension);
}
