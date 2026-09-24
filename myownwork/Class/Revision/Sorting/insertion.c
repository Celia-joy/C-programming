#include <stdio.h>

void insertionSort(int arr[], int n) {
    int i, j, key;
    
    for(i = 1; i < n; i++) {
        key = arr[i];           // Element to be inserted
        j = i - 1;
        
        // Move elements greater than key to one position ahead
        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;         // Insert the key at correct position
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    
    printf("Before sorting: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    
    insertionSort(arr, n);
    
    printf("\nAfter Insertion Sort: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    return 0;
}