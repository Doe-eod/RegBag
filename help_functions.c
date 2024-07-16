// help_funktions.c

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "regbag.h"



void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void printWithDelay(const char *str, int delay, const char *color) {
    printf("%s", color);

    while (*str) {
        printf("%c", *str);
        fflush(stdout);
        Sleep(delay);
        str++;
    }
    printf("%s", ANSI_RESET);
}