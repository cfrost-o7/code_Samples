#include <stdio.h>
#include <string.h>
#include "fileops.cwfrost.h"
#include "file-tests.h"

int main() {
    FILE *fp;

    int fileExists = 0;

    //char filename[14] = "../myFile.dat";
    char filename[16] = "../testFile.dat";

    fp = fopen(filename, "r+"); // open for read and write, if the file already exists
    if (fp != NULL) {
        fileExists = 1;
        if (fileTestTwo(fp) == 0) {
            printf("testTwo() passed");
        }
    }

    if (!fileExists) {
        fp = fopen(filename, "w+"); // open for read and write and create the file
        if(fileTestOne(fp) == 0) {
            printf("testOne() passed");
        }
        if (fp == NULL) {
            printf("cannot open file '%s'\n", filename);
            return 8;
        }
    }
    return 0;
}
