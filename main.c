//Ishank Nijhawan : 1710110150

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

//declaring global variables of array and size
int *primaryArray;
int size;

//structure for passing arguments to the thread function
typedef struct array_struct {
    int *sorted_array_1;
    int *sorted_array_2;
} arrayStruct;

//thread to sort first half of array
void *sort_1(void *args)
{
    arrayStruct *pArrayStruct = args;

    int size1 = size/2;
    int temp;

    //applying bubble sort to sort the array
    for (int i = 0; i < size1 - 1; ++i) {
        for (int j = 0; j < size1 -i -1; ++j) {
            if (pArrayStruct->sorted_array_1[j] > pArrayStruct->sorted_array_1[j + 1]){
                temp = pArrayStruct->sorted_array_1[j];
                pArrayStruct->sorted_array_1[j] = pArrayStruct->sorted_array_1[j + 1];
                pArrayStruct->sorted_array_1[j + 1] = temp;
            }
        }
    }
    return NULL;
}

//thread to sort second half of array
void *sort_2(void *args)
{
    arrayStruct *primaryArrayStruct = args;
    int size2 = size - (size/2);
    int temp;

    //applying bubble sort to sort the array
    for (int i = 0; i < size2 - 1; ++i) {
        for (int j = 0; j < size2 -i -1; ++j) {
            if (primaryArrayStruct->sorted_array_2[j] > primaryArrayStruct->sorted_array_2[j + 1]){
                temp = primaryArrayStruct->sorted_array_2[j];
                primaryArrayStruct->sorted_array_2[j] = primaryArrayStruct->sorted_array_2[j + 1];
                primaryArrayStruct->sorted_array_2[j + 1] = temp;
            }
        }
    }
    return NULL;
}

//merge function to merge and sort both the sorted arrays
void *merge(void *args)
{
    arrayStruct *pArrayStruct = args;
    int size1 = size/2;
    int size2 = size - size1;
    int i = 0, j = 0, k = 0;

    while (i<size1 && j <size2)
    {
        //if instance of sorted_array_1 is smaller than sorted_array_2, put array1 element in primaryArray
        //else put element of sorted_array_2 in primaryArray
        if (pArrayStruct->sorted_array_1[i] < pArrayStruct->sorted_array_2[j])
            primaryArray[k++] = pArrayStruct->sorted_array_1[i++];
        else
            primaryArray[k++] = pArrayStruct->sorted_array_2[j++];
    }

    //Store remaining elements of sorted_array_1
    while (i < size1)
        primaryArray[k++] = pArrayStruct->sorted_array_1[i++];

    //Store remaining elements of sorted_array_2
    while (j < size2)
        primaryArray[k++] = pArrayStruct->sorted_array_2[j++];

    return NULL;
}

int main()
{
    pthread_t sorting_thread_1;
    pthread_t sorting_thread_2;
    pthread_t  merging_thread;
    arrayStruct args;

    printf("Enter the size of an array\n");
    scanf("%d",&size);

    primaryArray = (int *)malloc(size * sizeof(int));
    args.sorted_array_1 = (int *)malloc((size/2) * sizeof(int));
    args.sorted_array_2 = (int *)malloc((size - (size/2)) * sizeof(int));

    printf("Enter the array: \n");
    for (int i = 0; i < size; ++i) {
        scanf("%d",&primaryArray[i]);
    }

    //passing first half of primaryArray to sorted_array_1
    for (int j = 0; j < size/2; ++j) {
        args.sorted_array_1[j] = primaryArray[j];
    }

    //passing second half of primaryArray to sorted_array_2
    for (int k = 0; k < size - (size/2); ++k) {
        args.sorted_array_2[k] = primaryArray[k + (size / 2)];
    }

    //create threads and pass the array
    pthread_create(&sorting_thread_1, NULL, &sort_1, (void *)&args);
    pthread_create(&sorting_thread_2, NULL, &sort_2, (void *)&args);

    //wait until both the threads are finished sorting
    pthread_join(sorting_thread_1, NULL);
    pthread_join(sorting_thread_2, NULL);

    //print the individual sorted array
    printf("\nThread 1 sorted array: ");
    for (int k = 0; k < size/2; ++k) {
        printf("%d ",args.sorted_array_1[k]);
    }
    printf("\n");

    printf("Thread 2 sorted array: ");
    for (int x = 0; x < size - (size/2); ++x) {
        printf("%d ",args.sorted_array_2[x]);
    }
    printf("\n");

    //create the merging thread and pass both the sorted arrays
    pthread_create(&merging_thread, NULL, &merge, (void *)&args);

    //wait until merging thread is finished merging the numbers
    pthread_join(merging_thread,NULL);

    //print the primaryArray
    printf("\nSorted array is: ");
    for (int l = 0; l < size; ++l) {
        printf("%d ",primaryArray[l]);
    }
    printf("\n\n");
}
