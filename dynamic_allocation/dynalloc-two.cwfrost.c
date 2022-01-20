//
// Created by Cole on 11/4/2021.
//

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "dynalloc-one.cwfrost.h"

void initializeAllocation(int capacity, AllocInfo *allocInfo) {
    allocInfo->capacity = capacity;
    allocInfo->allocations = (BlockListNode *) malloc(sizeof(BlockListNode));
    allocInfo->allocations = NULL;
    allocInfo->holes = (BlockListNode *) malloc(sizeof(BlockListNode));
    allocInfo->holes->start = 0;
    allocInfo->holes->size = capacity;
    allocInfo->holes->next = NULL;
}

int addAllocation(int start, int size, AllocInfo *allocInfo) {
    // build node from passed params
    BlockListNode *newNode = (BlockListNode *) malloc(sizeof(BlockListNode));
    newNode->start = start;
    newNode->size = size;
    newNode->next = NULL;

    // List is empty - add newNode and return
    if (allocInfo->allocations == NULL) {
        allocInfo->allocations = newNode;
        return 0;
    }

    // create temp ptr to the beginning of the list
    BlockListNode *currNode = allocInfo->allocations;
    BlockListNode *nextNode = currNode->next;
    while(currNode) {
        // Can't insert if another allocation at start location
        if (currNode->start == newNode->start) {
            return 1;
        // Puts newnode at the start of the list
        } else if (newNode->start < currNode->start && allocInfo->allocations == currNode) {
            if (currNode->start <= (newNode->start + newNode->size - 1)) {
                // newnode won't fit in desired allocation
                return 1;
            } else {
                allocInfo->allocations = newNode;
                newNode->next = currNode;
                return 0;
            }
        // Insert newnode into the list
        } else if (currNode->next != NULL && newNode->start < nextNode->start) {
            if (currNode->next->start <= (newNode->start + newNode->size - 1)) {
                // newnode won't fit in desired allocation
                return 1;
            } else {
                currNode->next = newNode;
                newNode->next = nextNode;
                return 0;
            }
        // insert newnode at the end of the list
        } else if (newNode->start > currNode->start && currNode->next == NULL) {
            if ((newNode->start + newNode->size - 1) > allocInfo->capacity) {
                // newnode won't fit in desired allocation
                return 1;
            } else {
                currNode->next = newNode;
                return 0;
            }
        }
        currNode = nextNode;
        if (currNode != NULL) {
            nextNode = currNode->next;
        }
    }

    // Couldn't insert the allocation
    return 1;
}

int deleteAllocation(int start, AllocInfo *allocInfo) {
    BlockListNode *prevNode = NULL;
    BlockListNode *currNode = allocInfo->allocations;
    BlockListNode *nextNode = NULL;
    if (!currNode->next) {
        nextNode = currNode->next;
    }

    while (currNode) {
        if (currNode->start == start && allocInfo->allocations == currNode) {
            allocInfo->allocations = currNode->next;
            currNode->next = NULL;
            free(currNode);
            return 0;
        } else if (currNode->start == start) {
            if (prevNode) {
                prevNode->next = nextNode;
                currNode->next = NULL;
                free(currNode);
                return 0;
            }
        } else {
            prevNode = currNode;
            currNode = currNode->next;
            if (currNode) {
                nextNode = currNode->next;
            }
        }
    }
    // If currNode is NULL at this point - there was no allocation at start index to be deleted
    assert(currNode != NULL);
    return 1;
}

int addHole(int start, int size, AllocInfo *allocInfo) {
    // build node from passed params
    BlockListNode *newNode = (BlockListNode *) malloc(sizeof(BlockListNode));
    newNode->start = start;
    newNode->size = size;
    newNode->next = NULL;

    // List is empty - add newNode and return
    if (allocInfo->holes == NULL) {
        allocInfo->holes = newNode;
        return 0;
    }

    // create temp ptr to the beginning of the list
    BlockListNode *currNode = allocInfo->holes;
    BlockListNode *nextNode = currNode->next;
    while(currNode) {
        // Can't insert if another allocation at start location
        if (currNode->start == newNode->start) {
            return 1;
            // Puts newnode at the start of the list
        } else if (newNode->start < currNode->start && allocInfo->holes == currNode) {
            if (currNode->start <= (newNode->start + newNode->size - 1)) {
                // newnode won't fit in desired hole
                return 1;
            } else {
                allocInfo->holes = newNode;
                newNode->next = currNode;
                return 0;
            }
            // Insert newnode into the list
        } else if (currNode->next != NULL && newNode->start < nextNode->start) {
            if (currNode->next->start <= (newNode->start + newNode->size - 1)) {
                // newnode won't fit in desired hole
                return 1;
            } else {
                currNode->next = newNode;
                newNode->next = nextNode;
                return 0;
            }
            // insert newnode at the end of the list
        } else if (newNode->start > currNode->start && currNode->next == NULL) {
            if ((newNode->start + newNode->size - 1) > allocInfo->capacity) {
                // newnode won't fit in desired hole
                return 1;
            } else {
                currNode->next = newNode;
                return 0;
            }
            // increment ptrs through the list for next loop
        }
        currNode = nextNode;
        nextNode = currNode->next;
    }

    // Couldn't insert the hole
    return 1;
}

int deleteHole(int start, AllocInfo *allocInfo) {
    BlockListNode *prevNode = NULL;
    BlockListNode *currNode = allocInfo->holes;
    BlockListNode *nextNode = NULL;
    if (!currNode->next) {
        nextNode = currNode->next;
    }

    while (currNode) {
        if (currNode->start == start && allocInfo->holes == currNode) {
            allocInfo->holes = currNode->next;
            currNode->next = NULL;
            free(currNode);
            return 0;
        } else if (currNode->start == start) {
            if (prevNode) {
                prevNode->next = nextNode;
                currNode->next = NULL;
                free(currNode);
                return 0;
            }
        } else {
            prevNode = currNode;
            currNode = currNode->next;
            if (currNode) {
                nextNode = currNode->next;
            }
        }
    }
    // If currNode is NULL at this point - there was no hole at start index to be deleted
    assert(currNode != NULL);
    return 1;
}

int shrinkHole(int requestSize, BlockListNode *holeNode, AllocInfo *allocInfo) {
    assert(requestSize >= 0);
    assert(requestSize <= holeNode->size);
    assert(holeNode != NULL);

    holeNode->start += requestSize;
    holeNode->size -= requestSize;
    return 0;
}

int expandHole(int newSize, BlockListNode *holeNode, AllocInfo *allocInfo) {
    assert(newSize >= holeNode->size);
    assert(holeNode != NULL);
    assert((holeNode->start + newSize) <= (allocInfo->capacity));
    holeNode->size = newSize;
    return 0;
}

int allocateBlock(int size, AllocType allocType, AllocInfo *allocInfo) {
    BlockListNode *currNode = allocInfo->holes;
    BlockListNode *bestNode = NULL;
    BlockListNode *worstNode = NULL;
    int bestDist = allocInfo->capacity;
    int worstDist = 0;
    int valToReturn = -1;

    switch(allocType) {
        case ALLOC_FIRST_FIT:
            while (currNode != NULL) {
                if (currNode->size == size) {
                    valToReturn = currNode->start;
                    addAllocation(currNode->start, size, allocInfo);
                    deleteHole(currNode->start, allocInfo);
                    return valToReturn;
                } else if (currNode->size > size) {
                    valToReturn = currNode->start;
                    addAllocation(currNode->start, size, allocInfo);
                    shrinkHole(size, currNode, allocInfo);
                    return valToReturn;
                } else {
                    currNode = currNode->next;
                }
            }
            break;
        case ALLOC_BEST_FIT:
            while (currNode != NULL) {
                if (currNode->size <= bestDist && currNode->size >= size) {
                    bestDist = currNode->size;
                    bestNode = currNode;
                }
                currNode = currNode->next;
            }
            if (bestNode != NULL) {
                if (bestDist == size) {
                    valToReturn = bestNode->start;
                    addAllocation(bestNode->start, size, allocInfo);
                    deleteHole(bestNode->start, allocInfo);
                    return valToReturn;
                } else {
                    valToReturn = bestNode->start;
                    addAllocation(bestNode->start, size, allocInfo);
                    shrinkHole(size, bestNode, allocInfo);
                    return valToReturn;
                }
            }
            break;
        case ALLOC_WORST_FIT:
            while (currNode != NULL) {
                if (currNode->size > worstDist && currNode->size >= size) {
                    worstDist = currNode->size;
                    worstNode = currNode;
                }
                currNode = currNode->next;
            }
            if (worstNode != NULL) {
                if (worstDist == size) {
                    valToReturn = worstNode->start;
                    addAllocation(worstNode->start, size, allocInfo);
                    deleteHole(worstNode->start, allocInfo);
                    return valToReturn;
                } else {
                    valToReturn = worstNode->start;
                    addAllocation(worstNode->start, size, allocInfo);
                    shrinkHole(size, worstNode, allocInfo);
                    return valToReturn;
                }

            }
            break;
        default:
            return valToReturn;
    }
    return valToReturn;
}

int deallocateBlock(int location, AllocInfo *allocInfo) {
    // look at allocations and delete block if there is one at the supplied location
    BlockListNode *currAlloc = allocInfo->allocations;
    int size;
    int didTheDelete = 0;
    while (currAlloc != NULL) {
        if (currAlloc->start == location) {
            // found the node - delete it and break out of the loop but save size before its removed to use for hole work
            size = currAlloc->size;
            deleteAllocation(location, allocInfo);
            didTheDelete = 1;
            break;
        } else {
            currAlloc = currAlloc->next;
        }
    }

    // Assert if no node was deleted
    assert(didTheDelete = 1);

    BlockListNode *currHole = allocInfo->holes;
    BlockListNode *prevHole = NULL;
    BlockListNode *nextHole = NULL;

    // loop over holes and determine if there is a left and/or right hole to the deleted alloc location
    while (currHole != NULL) {
        if (currHole->start + currHole->size == location) {
            prevHole = currHole;
            nextHole = currHole->next;
            break;
        } else if (currHole->start >= location + size) {
            nextHole = currHole;
            break;
        }
        prevHole = currHole;
        currHole = currHole->next;
        if (currHole != NULL)
            nextHole = currHole->next;
    } // not handling the situation where the location is before any hole

    if (prevHole == NULL && nextHole == NULL) {
        addHole(location, size, allocInfo);
    } else if (prevHole != NULL && nextHole == NULL) {
        if (prevHole->start + prevHole->size == location) { // prevHole abuts so need to expand
            size += prevHole->size;
            expandHole(size, prevHole, allocInfo);
        } else {
            addHole(location, size, allocInfo);
        }
    } else if (prevHole == NULL && nextHole != NULL) {
        if (location + size == nextHole->start) { // nextHole abuts so need to delete then add new larger hole
            size += nextHole->size;
            deleteHole(nextHole->start, allocInfo);
        }
        addHole(location, size, allocInfo);
    } else { // only can get here if both ptrs are not null
        if (prevHole->start + prevHole-> size != location && location + size != nextHole->start) { // neither node abuts
            addHole(location, size, allocInfo);
        } else { // if both don't abut - check if either one does on its own - use two ifs bc both could be true
            if (location + size == nextHole->start) { // nextHole abuts so need to delete then add new larger hole
                size += nextHole->size;
                deleteHole(nextHole->start, allocInfo);
                addHole(location, size, allocInfo);
            }
            if (prevHole->start + prevHole->size == location) { // prevHole abuts so need to expand
                size += prevHole->size;
                deleteHole(nextHole->start, allocInfo);
                expandHole(size, prevHole, allocInfo);
            }
        }
    }
    return 0;
}
