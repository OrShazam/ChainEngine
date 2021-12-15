
#include "console.h"
#include "scrap.h"
#include "psapi.h"

void FillScraperData(PScraperData data, const char* dllName){
	static NTSTATUS(WINAPI *_GetModuleInformation)(
		IN  HANDLE hProcess,
		IN  HMODULE hModule,
		OUT LPMODULEINFO lpmodinfo,
		IN DWORD cb
	) = NULL;
	data->hModule = LoadLibraryA(dllName);
	if (data->hModule == NULL){
		PrintError("%s Couldn't be loaded.\n", dllName);
		return;
	}
	if (!_GetModuleInformation){
		HMODULE hPsapi = LoadLibraryA("psapi.dll");
		if (!hPsapi){
			PrintError("psapi.dll couldn't be loaded.\n");
			return;
		}
		FARPROC procPtr = GetProcAddress(hPsapi,"GetModuleInformation");
		if (!procPtr){
			PrintError("GetModuleInformation couldn't be found\n");
			return;
		}
		
		_GetModuleInformation = (NTSTATUS(WINAPI*)(
			HANDLE,
			HMODULE,
			LPMODULEINFO,
			DWORD)		
		)procPtr;
	}
	MODULEINFO info;
	DWORD cb = sizeof(MODULEINFO);
	if (!_GetModuleInformation(GetCurrentProcess(),
		data->hModule,
		&info, cb)){
		PrintError("Can't retrieve module information.\n");
		return;
	}
	data->Base = (LPVOID)info.lpBaseOfDll;
	data->ImageSize = info.SizeOfImage;
}

BOOL FindBytes(PScraperData data, BYTE* bytes, size_t sizeOfBytes, LPVOID* address){
	BOOL success = FALSE;
	LPVOID start = data->Base;
	printf("Base: %p\n",start);
	printf("Searching from %p to %p\n",data->Base, data->Base + data->ImageSize);
	for (;start < data->Base + data->ImageSize; (BYTE*)start++){
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
