#include <stdio.h>

int main() {
    double arr[2] = {1.0, 2.0};
    double * ptr = arr;
    printf("%f\n", *(ptr));
    double mid = *(ptr++);
    printf("%f\n", mid);
    printf("%f\n", *(ptr));
}