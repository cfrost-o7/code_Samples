//
// Created by Cole on 11/29/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fileops.cwfrost.h"

int initializeFile(FILE *fp) {
    assert(fp != NULL);
    int initialNum = 0;
    int rc = fwrite(&initialNum, sizeof(int), 1, fp);
    if (rc == 1) {
        return 0;
    } else {
        return 1;
    }
}

int readRecord(FILE *fp, int position, PartRecord *partData) {
    assert(position > 0);
    assert(fp != NULL);

    // check if the numRecords in the file is less than the one asked for - if so return 1
    int numRecords = 0;
    int seek = fseek(fp, 0, SEEK_SET);
    int rc_read = fread(&numRecords, sizeof(int), 1, fp);
    if (numRecords < position || rc_read != 1) {
        return 1;
    }

    // if we've made it here then its time to seek into the file for the appropriate index to read the partData from
    int index = 4 + (position-1) * sizeof(PartRecord);
    seek = fseek(fp, index, SEEK_SET);
    if (seek == 0) {
        // successful seek
        int rc = fread(partData, sizeof(PartRecord), 1, fp);
        if (rc == 1) {
            return 0;
        } else {
            return 0;
        }
    } else {
        // unsuccessful seek
        return 1;
    }
}

int writeRecord(FILE *fp, PartRecord *partData) {
    // first check for the numRecords
    int numRecords = 0;
    int seek = fseek(fp, 0, SEEK_SET);
    int rc_read = fread(&numRecords, sizeof(int), 1, fp);

    if (rc_read != 1) {
        return -1;
    }

    int index = 4 + (numRecords) * sizeof(PartRecord);;

    // now seek to where we will write the partrecord
    seek = fseek(fp, index, SEEK_SET);
    if (seek == 0) {
        // successful seek
        int rc_write = fwrite(partData, sizeof(int)+PART_NAME_LENGTH+sizeof(int), 1, fp);
        if(rc_write != 1) {
            return -1;
        }
    } else {
        // unsuccessful seek
        return -1;
    }

    // seek back to the beginning
    seek = fseek(fp, 0, SEEK_SET);
    if (seek == 0) {
        // successful seek to start of file
        ++numRecords;
        int rc_write = fwrite(&numRecords, sizeof(int), 1, fp);
        if (rc_write == 1) {
            return numRecords;
        } else {
            return -1;
        }
    } else {
        // unsuccessful seek to start of file
        return -1;
    }
}