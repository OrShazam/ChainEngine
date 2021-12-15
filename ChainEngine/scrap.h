
#include "common.h"
typedef struct {
	HMODULE hModule;
	LPVOID Base;
	ULONGLONG ImageSize;
} ScraperData, *PScraperData;

void FillScraperData(PScraperData, const char*);
BOOL FindBytes(PScraperData, BYTE*, size_t,LPVOID*);
