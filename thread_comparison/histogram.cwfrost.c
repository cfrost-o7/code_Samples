//
// Created by Cole on 10/3/2021.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "histogram.cwfrost.h"

int histogram[MAXVAL];
int values[N];

int serialHistComplete[MAXVAL];
int tallyBadHistComplete[MAXVAL];
int tallyGoodHistComplete[MAXVAL];

int main(int argc, char *argv[]) {
    /***************** Initial Block Setting Up for all Implementaitons **********************/
    struct timeval start, end;
    float elapsedTime;
    int i, j, k;

    // put the random values in the values array
    for (i = 0; i < N; ++i) {
        double r = drand48();
        int val = (int) MAXVAL * r;
        values[i] = val;
    }

    FILE *f = fopen("./threadTimes.csv", "w");
    fprintf(f,"Method, Runtime1, Runtime2, Runtime3, Runtime4, Runtime5, AvgRuntime\n");

    /********************** Start of Serial Implementation ********************/

    fprintf(f,"Serial,");

    float serialSum = 0;
    for (i = 0; i < 5 ; ++i) {
        // Set all histogram values to zero
        for (j = 0; j < MAXVAL; ++j) {
            histogram[j] = 0;
        }

        // Start the timer
        gettimeofday(&start, NULL);

        // Run the Serial function
        tallySerial(histogram);

        // End the timer
        gettimeofday(&end, NULL);

        // Calculate and print the elapsed time
        elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;    // sec to usec
        elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // usec to ms
        printf("elapsed time: %f ms  for the serialized version\n", elapsedTime);
        fprintf(f, "%f,", elapsedTime);
        serialSum += elapsedTime;
    }

    printf("elapsed time avg: %f ms  for the serialized version\n", serialSum/5);
    fprintf(f, "%f\n", serialSum/5);

    // Create snapshot of histogram at the end of implementation
    for (i = 0; i < MAXVAL; ++i) {
        serialHistComplete[i] = histogram[i];
    }

    /***********************  End of Serial Implementation ***********************/
    /***********************  Start of TallyBad Implementation *******************/

    fprintf(f,"TallyBad,");

    // Create mutex declaration
    pthread_mutex_t mutex;

    // Create arrays for threadInfo and the threadIDs
    ThreadInfo threadInfo[NUM_THREADS];
    pthread_t threadIDs[NUM_THREADS];

    for(j = 0; j < NUM_THREADS; ++j) {
        threadInfo[j].mutex = &mutex;
    }

    // Init mutex
    pthread_mutex_init(&mutex, NULL);

    // Set index ranges
    threadInfo[0].startIndex = 0;
    threadInfo[0].endIndex = N / NUM_THREADS - 1;
    threadInfo[1].startIndex = threadInfo[0].endIndex + 1;
    threadInfo[1].endIndex = threadInfo[1].startIndex + threadInfo[0].endIndex;
    threadInfo[2].startIndex = threadInfo[1].endIndex + 1;
    threadInfo[2].endIndex = threadInfo[2].startIndex + threadInfo[0].endIndex;
    threadInfo[3].startIndex = threadInfo[2].endIndex + 1;
    threadInfo[3].endIndex = N - 1;

    float badSum = 0;
    for (i = 0; i < 5; ++i) {
        // Reset all histogram values to zero for next run
        for (j = 0; j < MAXVAL; ++j) {
            histogram[j] = 0;
        }

        // Start new timer for these threads
        gettimeofday(&start, NULL);

        // Create and start the threads
        for (k = 0; k < NUM_THREADS; ++k) {
            pthread_create(&threadIDs[k], NULL, tallyBad, &threadInfo[k]);
        }

        for (k = 0; k < NUM_THREADS; ++k) {
            pthread_join(threadIDs[k], NULL);
        }

        // End timer for the threads
        gettimeofday(&end, NULL);

        // Calculate and print the elapsed time
        elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;    // sec to usec
        elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // usec to ms
        printf("elapsed time: %f ms  for the tallyBad version\n", elapsedTime);
        fprintf(f, "%f,", elapsedTime);
        badSum += elapsedTime;
    }

    printf("elapsed time avg: %f ms  for the tallyBad version\n", badSum/5);
    fprintf(f, "%f\n", badSum/5);

    // Create snapshot of histogram at the end of implementation
    for (i = 0; i < MAXVAL; ++i) {
        tallyBadHistComplete[i] = histogram[i];
    }

    /***********************  End of TallyBad Implementation ***********************/
    /***********************  Start of TallyGood Implementation *******************/

    fprintf(f,"TallyGood,");

    float goodSum = 0;
    for (i = 0; i < 5; ++i) {
        // Reset all histogram values to zero for next run
        for (j = 0; j < MAXVAL; ++j) {
            histogram[j] = 0;
        }

        // Start new timer for these threads
        gettimeofday(&start, NULL);

        // Create and start the threads
        for (k = 0; k < NUM_THREADS; ++k) {
            pthread_create(&threadIDs[k], NULL, tallyGood, &threadInfo[k]);
        }

        for (k = 0; k < NUM_THREADS; ++k) {
            pthread_join(threadIDs[k], NULL);
        }

        // End timer for the threads
        gettimeofday(&end, NULL);

        // Calculate and print the elapsed time
        elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;    // sec to usec
        elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // usec to ms
        printf("elapsed time: %f ms  for the tallGood version\n", elapsedTime);
        fprintf(f, "%f,", elapsedTime);
        goodSum += elapsedTime;
    }

    printf("elapsed time avg: %f ms  for the tallyGood version\n", goodSum/5);
    fprintf(f, "%f\n", goodSum/5);

    // Create snapshot of histogram at the end of implementation
    for (i = 0; i < MAXVAL; ++i) {
        tallyGoodHistComplete[i] = histogram[i];
    }

    /******************* End of Tally Good Implementation ************************/

    // Close runtimes file
    fclose(f);

    /******************* Testing Section ******************************************/

    // Checks each histogram value against the ones in the others
    int passed = 0;
    for (i = 0; i < MAXVAL; ++i) {
        if (serialHistComplete[i] != tallyBadHistComplete[i]
            || tallyBadHistComplete[i] != tallyGoodHistComplete[i]) {
            passed = 1;
        } else {
            passed = 0;
        }
    }

    if (passed == 0) {
        printf("SUCCESS - all histograms are identical\n");
    } else {
        printf("FAILURE - histograms are not identical for all implementations\n");
    }

    return 0;
}

int tallySerial(int *histogram) {
    int i;
    // Increase counts of each value in the histogram
    for (i = 0; i < N; ++i) {
        histogram[values[i]] += 1;
    }
    return 0;
}

void *tallyBad(void *param) {
    ThreadInfo *data;
    data = (ThreadInfo*) param;

    pthread_mutex_t *mutex;
    mutex = data->mutex;

    int i;

    // this is the code inside each thread
    // grabbing mutex from the struct in each thread
    for (i = data->startIndex; i <= data->endIndex; ++i) {
        pthread_mutex_lock(mutex);
        histogram[values[i]] += 1;
        pthread_mutex_unlock(mutex);
    }

    pthread_exit(0);
}

void *tallyGood(void *param) {
    ThreadInfo *data;
    data = (ThreadInfo*) param;

    pthread_mutex_t *mutex;
    mutex = data->mutex;

    int i, j;

    // this is the code inside each thread
    int localHistogram[MAXVAL];
    for (j = 0; j < MAXVAL; ++j) {
        localHistogram[j] = 0;
    }

    for (i = data->startIndex; i <= data->endIndex; ++i) {
        localHistogram[values[i]] += 1;
    }

    // now merge
    pthread_mutex_lock(mutex);
    for (j = 0; j < MAXVAL; ++j) {
        histogram[j] += localHistogram[j];
    }
    pthread_mutex_unlock(mutex);

    pthread_exit(0);
}