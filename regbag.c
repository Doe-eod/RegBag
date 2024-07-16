// regbag.c

#include <windows.h>
#include <stdio.h>
#include "regbag.h"



bool verbose = false;
bool approximate1 = false;
bool approximate2 = false;
char tmp[256];
char tmp2[256];
int totalSubKeys = 0;



// Print the registry data in hex and interpreted format
void PrintHexData(BYTE *data, DWORD dataSize, FILE *outputFile) {

    if (approximate1 && !approximate2) {
        fprintf(outputFile, "Hex Data: ");
        for (DWORD i = 0; i < dataSize; i++) {
            fprintf(outputFile, "%02x ", data[i]);
        }
        fprintf(outputFile, "\n");

        fprintf(outputFile, "Interpreted Data: ");
        int consecutiveDots = 0;

        for (DWORD i = 0; i < dataSize; i++) {
            if (isalpha(data[i])) {
                fprintf(outputFile, "%c", data[i]);
                consecutiveDots = 0;
            } else {
                if (consecutiveDots < 2) {
                    consecutiveDots++;
                } else if (consecutiveDots == 2) {
                    fprintf(outputFile, ". ");
                    consecutiveDots = 0;
                }
            }
        }
        fprintf(outputFile, "\n\n");


    } else if (approximate2) {
        fprintf(outputFile, "Hex Data: ");
        for (DWORD i = 0; i < dataSize; i++) {
            fprintf(outputFile, "%02x ", data[i]);
        }
        fprintf(outputFile, "\n");

        fprintf(outputFile, "Interpreted Data: ");
        int consecutiveDots = 0;
        int consecutiveSpaces = 0;
        for (DWORD i = 0; i < dataSize; i++) {
            if (isalpha(data[i])) {
                fprintf(outputFile, "%c", data[i]);
                consecutiveDots = 0;
    
            } else {
                if (consecutiveDots < 2) {
                    consecutiveDots++;
                } else if (consecutiveDots == 2) {
                    if (consecutiveSpaces < 2) {
                        consecutiveSpaces++;
                    } else {
                        fprintf(outputFile, " ");
                        consecutiveSpaces = 0;
                    }
                }
            }
        }
        fprintf(outputFile, "\n\n");


    } else {
        fprintf(outputFile, "Hex Data: ");
        for (DWORD i = 0; i < dataSize; i++) {
            fprintf(outputFile, "%02x ", data[i]);
        }
        fprintf(outputFile, "\n");
    }
}






// Read the registry values
void ReadRegistryValues(HKEY hKey, FILE *outputFile, LPCSTR subKey) {
    DWORD index = 0;
    CHAR valueName[256];
    BYTE data[1024];
    DWORD valueNameSize, dataSize, valueType;
    LONG result;

    while (TRUE) {
        valueNameSize = sizeof(valueName);
        dataSize = sizeof(data);
        result = RegEnumValueA(hKey, index, valueName, &valueNameSize, NULL, &valueType, data, &dataSize);
        if (result == ERROR_NO_MORE_ITEMS) {
            break;
        } else if (result == ERROR_SUCCESS) {
            fprintf(outputFile, "\nSubkey: %s\n", subKey);
            fprintf(outputFile, "Value Index: %d\n", index);
            fprintf(outputFile, "Value Type: %ld\n", valueType);
            fprintf(outputFile, "Value Name: %s\n", valueName);
            PrintHexData(data, dataSize, outputFile);
        } else {
            fprintf(outputFile, "Error reading value at index %d: %ld\n", index, result);
        }
        index++;
    }
}







// Read the registry key recursively
void ReadRegistryKeyRecursively(HKEY hKeyRoot, LPCSTR subKey, FILE *outputFile) {
    HKEY hKey;
    LONG result = RegOpenKeyExA(hKeyRoot, subKey, 0, KEY_READ, &hKey);

    if (result == ERROR_SUCCESS) {
        fprintf(outputFile, "\n\n\n---------------------------------------------------------------------\n\n");
        fprintf(outputFile, "Reading Key: %s\n", subKey);
        totalSubKeys++;

        if (verbose) {
            snprintf(tmp, sizeof(tmp), "[***] SubKey: %s\n", subKey);
            printWithDelay(tmp, 0.5, ANSI_TEAL);
        }

        ReadRegistryValues(hKey, outputFile, subKey);

        DWORD index = 0;
        CHAR subKeyName[256];
        DWORD subKeyNameSize;

        while (TRUE) {
            subKeyNameSize = sizeof(subKeyName);
            result = RegEnumKeyExA(hKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL);
            if (result == ERROR_NO_MORE_ITEMS) {
                break;
            } else if (result == ERROR_SUCCESS) {
                CHAR fullSubKey[512];
                snprintf(fullSubKey, sizeof(fullSubKey), "%s\\%s", subKey, subKeyName);
                ReadRegistryKeyRecursively(hKeyRoot, fullSubKey, outputFile);
            } else {
                fprintf(outputFile, "Error reading subkey at index %d: %ld\n", index, result);
            }
            index++;
        }

        RegCloseKey(hKey);
    } else {
        if (verbose) {
        const char *text = "[**] Failed to open registry key: %s, error code: %ld\n", subKey, result;
        printWithDelay(text, 1, ANSI_RED);
        }
    }
}










int main(int argc, char *argv[]) {
    printf("\n");
    clearScreen();
    banner();
    
    // Check for settings
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-a1") == 0) {
            approximate1 = true;
        } else if (strcmp(argv[i], "-a2") == 0) {
            approximate2 = true;
        } else {
            const char *text = "\nSyntax: .\\regbag.exe <options>\n\nOptions\n---------------------\n-v  (verbose mode)\n-a1 (approximate mode 1)\n-a2 (approximate mode 2)\n";
            printWithDelay(text, 10, ANSI_TEAL);
            printf("\n");
            return 1;
        }
    }

    // Output settings
    if (verbose) {
        const char *text = "[X] Verbose mode is enabled";
        verbose = true;
        printWithDelay(text, 10, ANSI_GREEN);
        printf("\n");
    } else {
        const char *text = "[-] Verbose mode disabled";
        verbose = false;
        printWithDelay(text, 10, ANSI_RED);
        printf("\n");
    }
    if (approximate1) {
        const char *text = "[X] Approximate mode 1 enabled";
        approximate1 = true;
        printWithDelay(text, 10, ANSI_GREEN);
        printf("\n");
    } else {
        const char *text = "[-] Approximate mode 1 disabled";
        approximate1 = false;
        printWithDelay(text, 10, ANSI_RED);
        printf("\n");
    } 
    if (approximate2) {
        const char *text = "[X] Approximate mode 2 enabled";
        approximate2 = true;
        printWithDelay(text, 10, ANSI_GREEN);
        printf("\n");
    } else {
        const char *text = "[-] Approximate mode 2 disabled";
        approximate2 = false;
        printWithDelay(text, 10, ANSI_RED);
        printf("\n");
    }
    printf("\n");






    const char *text1 = "\n[*] Start preparation...\n";
    printWithDelay(text1, 1, ANSI_DARK_ORANGE);

    if (verbose) {
        const char *text = "[**] Create shellbags.txt...\n";
        printWithDelay(text, 1, ANSI_GREEN);
    }

    FILE *outputFile = fopen("shellbags.txt", "w");
    if (outputFile == NULL) {
        const char *text = "[-] Failed to create output file\n";
        printWithDelay(text, 1, ANSI_RED);
        return 1;
    }

    if (verbose) {
        const char *text = "[**] Successfully create shellbags.txt\n";
        printWithDelay(text, 1, ANSI_GREEN);
    }

    const char *text2 = "[*] Preparation finished!\n\n";
    printWithDelay(text2, 1, ANSI_DARK_ORANGE);

    const char *text3 = "\n[*] Start read shellbags from BagMRU...\n";
    printWithDelay(text3, 1, ANSI_DARK_ORANGE);

    // Action 1: Read Shellbags from BagMRU
    fprintf(outputFile, "\n\n\n\n\n##########################################################################################################\n\n");
    fprintf(outputFile, "Shellbags from BagMRU:\n\n");

    if (verbose) {
        const char *text = "[**] Entry function ReadRegistryKeyRecursively() for BagMRU\n";
        printWithDelay(text, 1, ANSI_GREEN);
    }

    ReadRegistryKeyRecursively(HKEY_CURRENT_USER, "Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU", outputFile);
    
    const char *text5 = "[*] Read BagMRU finished!\n\n";
    printWithDelay(text5, 1, ANSI_DARK_ORANGE);

    const char *text6 = "\n[*] Start read shellbags from Bags...\n";
    printWithDelay(text6, 1, ANSI_DARK_ORANGE);



    // Action 2: Read Shellbags from Bags
    fprintf(outputFile, "\n\n\n\n\n##########################################################################################################\n\n");
    fprintf(outputFile, "\nShellbags from Bags:\n\n");

    if (verbose) {
        const char *text7 = "[**] Entry function ReadRegistryKeyRecursively() for Bags\n";
        printWithDelay(text7, 1, ANSI_GREEN);
    }

    ReadRegistryKeyRecursively(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\Shell\\Bags", outputFile);

    fclose(outputFile);

    const char *text8 = "[*] Read BagMRU finished!\n\n";
    printWithDelay(text8, 1, ANSI_DARK_ORANGE);

    const char *text9 = "[--->] Shellbags saved to shellbags.txt!\n\n";
    printWithDelay(text9, 1, ANSI_GREEN);



    // Output the summary
    const char *text10 = "\n\n[*] Summary\n";
    printWithDelay(text10, 1, ANSI_DARK_ORANGE);

    if (verbose) {
        const char *text11 = "[--->] Mode: Verbose\n";
        printWithDelay(text11, 1, ANSI_GREEN);
    }
    if (approximate1) {
        const char *text11 = "[--->] Mode: Approximate 1\n";
        printWithDelay(text11, 1, ANSI_GREEN);
    }
    if (approximate2) {
        const char *text12 = "[--->] Mode: Approximate 2\n";
        printWithDelay(text12, 1, ANSI_GREEN);
    }
    snprintf(tmp2, sizeof(tmp2), "[--->] Total Subkeys: %d\n", totalSubKeys);
    printWithDelay(tmp2, 1, ANSI_GREEN);

    const char *text13 = "[*] Servus\n\n";
    printWithDelay(text13, 1, ANSI_DARK_ORANGE);

    return 0;
}
