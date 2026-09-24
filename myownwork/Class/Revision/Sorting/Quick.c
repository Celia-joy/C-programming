#include <stdio.h>

// Function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function - returns the correct position of pivot
int partition(int arr[], int low, int high) {
    int pivot = arr[high];    // Choose last element as pivot
    int i = low - 1;          // Index of smaller element
    
    for(int j = low; j < high; j++) {
        if(arr[j] < pivot) {   // Change to > for descending order
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);  // Put pivot in correct position
    return i + 1;
}

// Main Quick Sort Function
void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);   // Partitioning index
        
        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);      // Left part
        quickSort(arr, pi + 1, high);     // Right part
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    
    printf("Before sorting: ");
    for(int i = 0; i < n; i++) 
        printf("%d ", arr[i]);
    
    quickSort(arr, 0, n-1);
    
    printf("\nAfter Quick Sort: ");
    for(int i = 0; i < n; i++) 
        printf("%d ", arr[i]);
    
    return 0;
}