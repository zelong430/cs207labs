
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

int CAPACITY_INIT = 1;
int GROWTH_FACTOR = 2;

int DArray_init(DArray* arr){
    arr->array = (int *) malloc(CAPACITY_INIT*sizeof(int));
    if (arr->array == NULL) {
        return -1;
    }
    arr->upto = 0;
    arr->size = CAPACITY_INIT;
    return 1;
}

int append(DArray* arr, int i){
    int *iptr;
    if (arr->upto >= arr->size) {
        /* tmp pointer needed below as if allocation failed, original array would be lost */
        iptr = (int *) realloc(arr->array, arr->size*GROWTH_FACTOR*sizeof(int));
        if (iptr == NULL) {
            return -1;
        }
        arr->array = iptr;
        arr->size *= GROWTH_FACTOR;
    }
    arr->array[arr->upto] = i;
    return arr->upto++;
    
        
}

int get(DArray* arr, int index) {
    if (index >= arr->upto || index < 0) {
        return -1;
    }
    return arr->array[index];
}

int get_index(DArray* arr, int value) {
    int i;
    for (i=0; i< arr->upto; i++){
        if (arr->array[i]==value) {
            return i;
        }
    }
    return -1;
}

void DArray_free(DArray *arr) {
  free(arr->array);
}

int set(DArray* arr, int index, int value){
    if(index < 0 || index >= arr->upto ){
        return -1;
    }
    return arr->array[index];
}

int delete(DArray* arr, int value){
    int remove = -1;
    for(int i=0;i<arr->upto;i++){
        if(arr->array[i] == value){
            remove = i;
        }
    }
    if(remove == -1) return -1;
    int* temp = malloc((arr->size - 1) * sizeof(int));
    if (remove != 0)
        memcpy(temp, arr->array, (remove - 1) * sizeof(int));
    if (remove != (arr->size - 1))
        memcpy(temp+remove, arr->array+remove+1, (arr->size - remove - 1) * sizeof(int));
    arr->array = temp;
    return remove;
}