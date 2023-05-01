#include "../inc/libmx.h"

void swap(char **a, char **b){
    char *temp = *a;
    *a = *b;
    *b = temp;
}

int mx_quicksort(char **arr, int left, int right){
    if (arr == NULL) return -1;
    char *pivot = arr[(left + right) / 2];
    int count = 0;
    int i = left;
    int j = right;
    while (i <= j) {
        while (mx_strlen(arr[i]) < mx_strlen(pivot)) {
            i++;
        }
        while (mx_strlen(arr[j]) > mx_strlen(pivot)) {
            j--;
        }
        if (i > j) break;
        if (mx_strlen(arr[i]) != mx_strlen(arr[j])){
            swap(&arr[i], &arr[j]);
            count++;
        }
        i++;
        j--;
    }
    if (left < j){
        count += mx_quicksort(arr, left, j);
    }
    if (right > i){
        count += mx_quicksort(arr, i, right);
    }
    return count;
}
