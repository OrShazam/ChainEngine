
#include "common.h"
#include "string.h"
typedef struct {
	HMODULE hModule;
	LPVOID SearchBase;
	ULONG SearchSize;
} ScraperData, *PScraperData;

void FillScraperData(PScraperData, const char*);
VOID FindCodeSection(LPVOID imageBase, LPVOID* start, ULONG* size)
BOOL FindBytes(PScraperData, BYTE*, size_t,LPVOID*);
