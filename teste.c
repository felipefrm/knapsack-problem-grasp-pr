#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {

    int n;           // numero de objetos
	int b;           // capacidade da mochila
	int *w;          // vetor de peso de cada objeto
	int *p;          // vetor de utilidade de cada objeto

    FILE* f = fopen ("large_scale/knapPI_3_10000_1000_1", "r");
    int valor;
    int i = 0;

    fscanf (f, "%d %d", &n, &b);    

    w = (int*)malloc(n * sizeof(int));
    p = (int*)malloc(n * sizeof(int));

    printf("n: %d\n", n);
    printf("b: %d\n", b);

    // fscanf (f, "%d", &valor);
    while (i < n)
    {  
        fscanf (f, "%d %d", &p[i], &w[i]);
        i++;
    }
    fclose (f);   

    for (int i = 0; i < n; i++) {
        printf("%d %d\n", p[i], w[i]);
    }
}