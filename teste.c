#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {

    int a[5] = {1, 2, 3, 4, 5};
    int b[5] = {5, 4, 3, 2, 1};

    int *aux;

    printf("A: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }
    printf("\n\n");

    printf("B: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", b[i]);
    }
    printf("\n\n");

    printf("AUX: ");
    aux = b;
    
    for (int i = 0; i < 5; i++) {
        printf("%d ", aux[i]);
    }
    printf("\n\n");

    aux[0] = 10;

    printf("NOVO B: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", b[i]);
    }
    printf("\n\n");

    printf("NOVO AUX: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", aux[i]);
    }
    printf("\n\n");

}