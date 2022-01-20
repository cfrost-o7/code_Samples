//
// Created by Cole on 9/18/2021.
//

#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "linkedlist.cwfrost.h"

/*
 * Passes all tests - stable and can be submitted in present state
 * TO DO: Could clean up if time before submission date - cleanup comments - refactor to improve readability
 */

/*
 * If list is not NULL (careful—this is different from saying that *list is not NULL), and if name is not NULL,
and the id value is greater than zero, and if there is not already a record in the list with id, then create a
new Record and insert it in the correct place in the list, such that the list remains sorted (in an ascending
fashion) by id, and return zero. Otherwise, if any of the previous conditions holds, then return one and
don’t change the list.
 */
int insertStudent(Record **list, char *name, int id) {
    if (*&list && name && id > 0) {
        // Create record
        Record *rec = (Record *) malloc(sizeof(Record));
        strcpy(rec->name, name);
        rec->id = id;
        rec->next = NULL;

        Record *curr = *list;
        if (curr == NULL) { // Means list has no records in it yet
            *list = rec;
            return 0;
        }
        // Getting to here means there is at least one record already in list
        Record *next = curr->next; // Always will be the one after current

        while (*&curr) { // Loops while curr isn't null - meaning we aren't at the end of the list yet
            if (rec->id == curr->id) { // Can't insert a record with same id - return 1 immediately
                return 1;
            } else if (rec->id < curr->id && *list == curr) { // This is only entered if the new record is less than the front of the list
                *list = rec;
                rec->next = curr;
                return 0;
            } else if (curr->next != NULL && rec->id < next->id) { // If curr-next is null short circuits the conditional and means at end of list
                curr->next = rec;
                rec->next = next;
                return 0;
            } else if (rec->id > curr->id && curr->next == NULL)  { // this means we are at the end of the list and the new rec needs to be put at the back of the list
                curr->next = rec;
                return 0;
            } else { // This block increments the pointers through the list if nothing above holds true
                curr = next;
                next = curr->next;
            }
        }

        // If we've gotten to here - insert wasn't possible and should return 1
        return 1;
    } else {
        return 1;
    }
}

/*
 * To delete an existing record from the list:
int deleteStudent(Record **list, int id);
If list is not NULL and if there is a record with matching id value, then delete the record from the list
and return zero. Otherwise, return one.
 */
int deleteStudent(Record **list, int id) {
    if (*&list && *list) { // if list exist

        Record *prev = NULL;
        Record *curr = *list;
        Record *next = NULL;
        if (!curr->next) {
            next = curr->next;
        }

        while(curr) {
            if (curr->id == id && *list == curr) { //
                *list = curr->next; // first item in list is now out of linked ptrs
                // now must deallocate the memory
                curr->next = NULL;
                free(curr);
                return 0;
            } else if (curr->id == id) {
                if (prev) {
                    prev->next = next;
                    curr->next = NULL;
                    free(curr);
                    return 0;
                }
            } else { // means on this loop we've not matched the id and need to move forward in the list
                prev = curr;
                curr = curr->next;
                if (curr) {
                    next = curr->next;
                }
            }
        }
        return 1;
    } else {
        return 1;
    }
};

/*
 *To fetch the name value from an existing record from the list:
int getStudent(Record *list, int id, char *buffer);
If there is a record with matching id value in the list and if buffer is not NULL, then copy the record’s name
value into buffer and return zero. Otherwise, return one.
 */
int getStudent(Record *list, int id, char *buffer) {
    if(*&list) {
        if (!list) {
            return 1;
        }
        Record *curr = list;

        while (curr) {
            if (curr->id == id) {
                if (buffer) {
                    strcpy(buffer, curr->name);
                    return 0;
                }
            }
            curr = curr->next;
        }
        return 1;
    } else {
        return 1;
    }
};

/*
 * To count how many records are in the list:
int countStudents(Record *list);
Return the number of records in the list. Return zero if the list is empty.
 */
int countStudents(Record *list) {
    int count = 0;
    if (*&list) {
        if (!list) {
            return 0;
        } else {
            Record *curr = list;

            while (curr) {
                ++count;
                curr = curr->next;
            }
            return count;
        }
    } else {
        return 0;
    }
};

/*
 * To print the records in the list:
int printStudents(Record *list);
Print each record in the list on a single line, in this format:
|Thomas| id = 6
|Harriet| id = 56
|Henry| id = 60
|Elizabeth| id = 67
|Roxanne| id = 69
If the list is empty, then print this:
(list is empty)
 */
int printStudents(Record *list) {
    if (list) {
        Record *curr = list;
        printf("\n");
        while (curr) {
            printf("|%s| id = %d\n", curr->name, curr->id);
            curr = curr->next;
        }
        printf("\n");
        return 0;
    } else {
        printf("(list is empty)\n");
        return 1;
        }
};