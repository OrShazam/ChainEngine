
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
	PBYTE buffers[50]; int i =0, j, idx=0,prevIdx;
	BYTE ROPCHAIN[1024];
	memset(&ROPCHAIN[0],0,1024);
	int lengths[50];
	while (*text != '\n' && i < 50){
		len = strlen(text);
		text[len] = (char)RET;
		buffers[i++] = HexTranslateSafe(text,&lengths[i]);
		text += len + 2;
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
			if (FindBytes(&data,buffers[i],len,&address)){
				printf("Found byte sequence '%x...RET' at address %p\n", *buffers[i], address);
			}
			else {				
				PrintError("Couldn't find byte sequence '%x...'\n", *buffers[i]);
				goto cleanup;
			}
			prevIdx = idx;
			for (; idx < prevIdx + sizeof(LPVOID) / sizeof(BYTE); idx++){
				
				ROPCHAIN[idx] = (BYTE)(((ULONGLONG)address >> (8 * idx)) & 0xff);
			}
		}
	}
	len = strlen(&ROPCHAIN[0]);
	printf("char ROPCHAIN[] =\n");
	for (int i = 0; i < len; i++){
		if (i % 16 == 0){
			printf("\n");
		}
		printf("\\x%x",ROPCHAIN[i]);
	}
	cleanup:
	for (int i = 0; i < j; i++){
		if (buffers[i] != NULL){
			HeapFree(GetProcessHeap(),
				0,
				buffers[i]);
		}
	}
}