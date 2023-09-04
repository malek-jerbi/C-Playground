#include <stdio.h>

int main () {
    int arr[10];

    *(arr +2) = 42;

    printf("arr +2 = %p, *(arr+11) = %d\n", arr +2, *(arr +2));
}