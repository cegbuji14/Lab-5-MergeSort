#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 10

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

/* Runs mergesort on the array segment described in the
   argument. Spawns two threads to mergesort each half
   of the array segment, and then merges the results.
   void* mergeSort(void* args) {
     return NULL;
}using merge_sort123();
*/


struct divider { 
    int partNum;
    int partLow;
    int partHigh;
};//using this struct instead of the StartEndIndexes

void merge(int low, int middle, int high){//function used to merge 2 parts

    int i,j;
    int leftP = (middle - low + 1);
    int rightP = (high - middle);

    int *left = malloc(leftP * sizeof(int));
    int *right = malloc(rightP * sizeof(int));

    for (i = 0; i < leftP; i++){
      left[i] = array[i + low];
    }
  
    for (i = 0; i < rightP; i++){
      right[i] = array[i + middle + 1];
    }
  
    int k = low;

    i = 0;
    j = 0;


    while (i < leftP && j < rightP){//merging the left and right
        if (left[i] <= right[j]){
            array[k++] = left[i++];
        }
        else{
            array[k++] = right[j++];
        }
    }

    while (i < leftP){//insert the values
      array[k++] = left[i++];
    }

    while (j < rightP){
      array[k++] = right[j++];
    }

    free(left);
    free(right);
}

void merge_sort(int low, int high){//added merge_sort function
    int middle = low+(high - low)/2;

    if (low < high){
        merge_sort(low, middle);
        merge_sort(middle + 1, high);
        merge(low, middle, high);
    }
}

void *merge_sort123(void *arg){//function used for multit-threading (thread function)
    struct divider *divider = arg; // pointer to divider struct 
    int low;
    int high;
    int num;


    high = divider->partHigh; // setting high and low
    low = divider->partLow;

    int mid = low+(high-low)/2; // evaluating mid point

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid+1, high);
        merge(low, mid, high);
    }

    return 0;
}

int main()
{
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    struct divider *divider;


    pthread_t threads[2];
    struct divider list[2];

    int len = SIZE/2;
    int low = 0;

    for (int i = 0; i<2; i++, low += len) {
        divider = &list[i];
        divider->partNum = i;

        divider->partLow = i * (SIZE/2);
        divider->partHigh = (i+1) * (SIZE/2)-1;
    }

    // 3. Create a 2 threads for merge sort.
    for (int i=0; i<2; i++) {
        divider = &list[i];
        pthread_create(&threads[i], NULL, merge_sort123, divider); // passes the threads being created, passes the struct, and calls the function for multi threading
    }

    for (int i = 0; i < 2; i++){//joins threads
        pthread_join(threads[i], NULL);
        merge(0, (SIZE / 2 - 1) / 2, SIZE / 2 - 1);
        merge(SIZE / 2, SIZE / 2 + (SIZE - 1 - SIZE / 2) / 2, SIZE - 1);
        merge(0, (SIZE - 1) / 2, SIZE - 1);
    }
    // 4. Wait for mergesort to finish.

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);

    return 0;

}
//worked with spencer and used https://stackoverflow.com/questions/52767944/merge-sort-with-pthreads-in-c for reference