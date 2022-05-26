
#include "console.h"
#include "scrap.h"

void FillScraperData(PScraperData data, const char* dllName){
	data->hModule = LoadLibraryA(dllName);
	FindCodeSection(data->hModule, &data->SearchBase, &data->SearchSize);
}
VOID FindCodeSection(LPVOID imageBase, LPVOID* start, ULONG* size){
	*start = NULL; *size = 0;
	PIMAGE_NT_HEADERS libNtHeader = (PIMAGE_NT_HEADERS)(imageBase +
		((PIMAGE_DOS_HEADER)imageBase)->e_lfanew);
	int numSections = libNtHeader->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER firstSection = (PIMAGE_SECTION_HEADER)
		((char*)imageBase + sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS));
	for (int i = 0; i < numSections; i++){
		PIMAGE_SECTION_HEADER currSection = firstSection + i;
		if (strcmp(".text",currSection.Name) == 0){
			*start = (char*)imageBase + start.VirtualAddress;
			*size = start.SizeOfRawData;
		}
	}
	if (!*start){
		// assume it's first section
		*start = (char*)imageBase + firstSection.VirtualAddress;
		*size = firstSection.SizeOfRawData;
	}
	
}

BOOL FindBytes(PScraperData data, BYTE* bytes, size_t sizeOfBytes, LPVOID* address){
	BOOL success = FALSE;
	LPVOID start = data->SearchBase;
	printf("Base: %p\n",start);
	printf("Searching from %p to %p\n",data->SearchBase, data->SearchBase + data->SearchSize);
	for (;start < data->SearchBase + data->SearchSize; (BYTE*)start++){
		if (success) break;
		for (size_t i = 0; i < sizeOfBytes; i++){
			if (bytes[i] != ((BYTE *)start)[i])
				break;
			if (i == sizeOfBytes-1)
				success = TRUE;
		}
	}
	if (success)
		*address = start;
	else 
		*address = NULL;
	return success;
}
