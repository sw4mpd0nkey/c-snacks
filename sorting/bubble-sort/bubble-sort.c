#include <stdlib.h>
#include <stdio.h>

void bubble_sort(int *array, int size) {

    for(int i = 0; i < size - 1; i++) {
        for (int j = 0; j < (size - i - 1); j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
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
    bubble_sort(array, array_size);
    for (int i = 0; i < array_size; i++)
        printf("%d ", array[i]);
    printf("\n");
    printf("---------------------------------------\n");
}