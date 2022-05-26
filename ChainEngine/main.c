
#include "scrap.h"
#include "console.h"
#include "utils.h"
#include "string.h"

#define RET 0xC3
void PrintUsage(){
	printf("Usage: ChainEngine.exe <inst;inst...>\n");
}
int main(int argc, char* argv[]){
	if (argc < 2){
		PrintUsage();
		return 0;
	}
	int len;
	char* text = argv[1];
	TokenizeSemicolons(text);
	PBYTE buffers[50]; int i =0, j, idx=0;
	BYTE ROPCHAIN[1024];
	memset(&ROPCHAIN[0],0,1024);
	int lengths[50];
	while (*text != '\n' && i < 50){
		buffers[i] = HexTranslateSafe(text,&lengths[i]);
		*((BYTE*)buffers[i] + lengths[i]) = RET;
		lengths[i]++; i++;
		text += strlen(text);
	}
	j = --i;
	for (; i >= 0; i--){
		if (buffers[i] == NULL && lengths[i] != 0){
			PrintError("Couldn't translate bytes\n");
			PrintUsage();
			return 1;
		}
	}
	ScraperData data;
	FillScraperData(&data, "ntdll.dll");
	LPVOID address;
	for (int i = 0; i < j; i++){
		if (lengths[i] > 0){
			if (FindBytes(&data,buffers[i],lengths[i],&address)){
				printf("Found byte sequence #%d at address %p\n", i, address);
			}
			else {				
				PrintError("Couldn't find byte sequence #%d\n", i);
				goto cleanup;
			}
			for (int k = 0; k < sizeof(LPVOID) / sizeof(BYTE); k++){
				
				ROPCHAIN[idx+k] = (BYTE)(((ULONGLONG)address >> (8 * k)) & 0xff);
			}
			idx += sizeof(LPVOID) / sizeof(BYTE);
		}
	}
	len = strlen(&ROPCHAIN[0]);
	printf("char ROPCHAIN[] =\n\"");
	for (int i = 0; i < len; i++){
		if (i % 16 == 0){
			printf("\n");
		}
		printf("\\x%x",ROPCHAIN[i]);
	}
	printf("\";");
	cleanup:
	for (int i = 0; i < j; i++){
		if (buffers[i] != NULL){
			HeapFree(GetProcessHeap(),
				0,
				buffers[i]);
		}
	}
}
