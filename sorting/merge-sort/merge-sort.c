#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void merge(int array[], int left, int middle, int right) {
    int temp1_idx = 0,
        temp2_idx = 0,
        array_idx = left;

    int temp_array_size1 = middle - left + 1,
        temp_array_size2 = right - middle;

    int temp1[temp_array_size1],
        temp2[temp_array_size2];

    for (temp1_idx = 0; temp1_idx < temp_array_size1; temp1_idx++) {
        temp1[temp1_idx] = array[left + temp1_idx];
    }
    for (temp2_idx = 0; temp2_idx < temp_array_size2; temp2_idx++) {
        temp2[temp2_idx] = array[middle + 1 + temp2_idx];
    }

    temp1_idx = 0;
    temp2_idx = 0;    

    while (temp1_idx < temp_array_size1 && temp2_idx < temp_array_size2) {
        if (temp1[temp1_idx] <= temp2[temp2_idx]) {
            array[array_idx] = temp1[temp1_idx];
            temp1_idx++;
        } else {
            array[array_idx] = temp2[temp2_idx];
            temp2_idx++;
        }
        array_idx++;
    }

    while(temp1_idx < temp_array_size1) {
        array[array_idx] = temp1[temp1_idx];
        temp1_idx++;
        array_idx++;
    }
    while(temp2_idx < temp_array_size2) {
        array[array_idx] = temp2[temp2_idx];
        temp2_idx++;
        array_idx++;
    }
}

void merge_sort(int array[], int left, int right) {
    
    if (left < right) {
        int middle = left + (right - left) / 2;

        merge_sort(array, left, middle);
        merge_sort(array, middle + 1, right);

        merge(array, left, middle, right);
    }
}

void main() {
    int array[] = {5,9,2,1, 15, 8, 21, 13};
    int array_size = sizeof(array) / sizeof(array[0]);
    printf("Array size: %d\n", array_size);
    printf("Array: ");
    for (int i = 0; i < array_size; i++)
        printf("%d ", array[i]);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Sorted: "); 
    merge_sort(array, 0, array_size - 1);
    for (int i = 0; i < array_size; i++)
        printf("%d ", array[i]);
    printf("\n");
    printf("---------------------------------------\n");
}