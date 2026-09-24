#include <stdio.h>

void selectionSort(int arr[], int n) {
    int i, j, min_idx, temp;
    
    for(i = 0; i < n-1; i++) {
        min_idx = i;                    // Assume current is minimum
        
        for(j = i+1; j < n; j++) {      // Find the real minimum
            if(arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Swap the found minimum with the first element
        if(min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    
    printf("Before sorting: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    
    selectionSort(arr, n);
    
    printf("\nAfter Selection Sort: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    return 0;
}