
#include "scrap.h"
#include "console.h"
#include "utils.h"
void PrintUsage(){
	printf("Usage: ntdllscraper.exe <bytes (hex)>\n");
}
int main(int argc, char* argv[]){
	if (argc < 2){
		PrintUsage();
		return 0;
	}
	int len;
	BYTE* bytes = HexTranslateSafe(argv[1], &len);
	for (int i = 0; i < len; i++){
	}
	if (bytes == NULL){
		PrintError("Couldn't translate bytes\n");
		PrintUsage();
		return 1;
	}
	ScraperData data;
	FillScraperData(&data, "ntdll.dll");
	LPVOID address;
	if (FindBytes(&data,bytes,len,&address)){
		printf("Found byte sequence at address at %p\n", address);
	}
	else 
		PrintError("Couldn't find byte sequence\n");
	
	HeapFree(GetProcessHeap(),
		0,
		bytes);
}