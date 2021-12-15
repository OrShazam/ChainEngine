
#ifndef console_h
#define console_h


#include "common.h"

#define CONSOLE_RED FOREGROUND_RED | FOREGROUND_INTENSITY

#define CONSOLE_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY

#define CONSOLE_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY


void printColor(const WORD wColor, const LPSTR fmt,... );

void PrintError(const LPSTR fmt, ...);
void PrintSuccess(const LPSTR fmt, ...);
#endif