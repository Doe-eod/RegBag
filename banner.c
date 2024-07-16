// banner.c

#include <stdio.h>
#include "regbag.h"



const char *bannerLines[] = {
    "",
    "         ____            ____ \n",  
    "        |  _ \\ ___  __ _| __ )  __ _  __ _\n",
    "        | |_) / _ \\/ _` |  _ \\ / _` |/ _` |\n",
    "        |  _ <  __/ (_| | |_) | (_| | (_| |\n",
    "        |_| \\_\\___|\\__, |____/ \\__,_|\\__, |\n",
    "                   |___/             |___/\n",
    "                      developed by Sven Herz\n",
    "",
    "\n\n\n"
};


int banner() {
    
    int numberOfLines = sizeof(bannerLines) / sizeof(bannerLines[0]);
    for (int i = 0; i < numberOfLines; i++) {
        printWithDelay(bannerLines[i], 0, ANSI_DARK_ORANGE);
    }

    printf("\n\n\n");
    return 0;
}


