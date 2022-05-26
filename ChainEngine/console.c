
#include "console.h"

void printColor(const WORD wColor, const LPSTR fmt,... ){
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(
		hOutput,
		&info);
	SetConsoleTextAttribute(
		hOutput,
		wColor);
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	SetConsoleTextAttribute(
		hOutput,
		info.wAttributes);
}

void PrintError(const LPSTR fmt, ...){
	printColor(CONSOLE_WHITE, "[");
	printColor(CONSOLE_RED, "-");
	printColor(CONSOLE_WHITE,"] ");
	va_list args;
	va_start(args, fmt);
	printColor(CONSOLE_WHITE,fmt,args);
	DWORD errorCode = GetLastError();
		char* const message = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                      | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, errorCode,
                  MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                  (LPTSTR)&message, 0, NULL);
	printColor(CONSOLE_WHITE,"LastError: %s\n",message);
	va_end(args);
}
void PrintSuccess(const LPSTR fmt, ...){
	printColor(CONSOLE_WHITE, "[");
	printColor(CONSOLE_GREEN, "+");
	printColor(CONSOLE_WHITE,"] ");
	va_list args;
	va_start(args, fmt);
	printColor(CONSOLE_WHITE,fmt,args);
	va_end(args);
}
