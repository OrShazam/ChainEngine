
#include "utils.h"

static BOOL IsCharHex(const char ch){
	return (ch >= 'A' && ch <= 'Z') 
		|| (ch >= 'a' && ch <= 'z')
		|| (ch >= '0' && ch <= '9');
}

static BOOL IsHex(const char* text, size_t* len){
	size_t textLen = strlen(text);
	if (textLen % 2 != 0)
		return FALSE;
	for (size_t i = 0; i < textLen; i+=2){
		if (!IsCharHex(text[i]) 
			|| !IsCharHex(text[i+1])){
			
			return FALSE;
		}
	}
	*len = textLen / 2;
	return TRUE;
}

static void HexTranslate(const char* text, BYTE* bytes, int len){
	BYTE currByte;

	for (int i = 0; i < len;i++){
		currByte = (*text <= '9') ? *text-'0' : (*text <= 'Z') ? 
			*text - 'A' + 10 : *text - 'a' + 10;
		currByte *= 16;
		text++;
		currByte += (*text <= '9') ? *text-'0' : (*text <= 'Z') ? 
			*text - 'A' + 10 : *text - 'a' + 10;
		text++;
		bytes[i] = currByte;
	}
	
}
BYTE* HexTranslateSafe(const char* text, int* length){
	size_t len;
	if (!IsHex(text, &len))
		return NULL;
	BYTE* bytes = HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		len+1);
	HexTranslate(text, bytes,len);
	*length = len;
	return bytes;
}

void TokenizeSemicolons(char* text){
	while (*text != 0){
		if (*text == ';'){
			*text = 0;
		}
		text++;
	}
}

