#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <limits.h>
#include <float.h>
#include "arraylist.h"

#define MAX(x,y) ((x)<(y) ? (y) : (x))

typedef struct{
	int id;
	double peso;        // peso do objeto
	double utilidade;   // utilidade do objeto
	double profit;      // profit do objeto (utilidade/peso)
} Objeto;


// Util
void le_arquivo(const char *nomearq, double *vetor);
double calcula_fo(int *s, int num_objetos, double *p, double *w, double b);
void imprime_solucao(int *s, int num_objetos, double *p, double *w, double b);
const Boolean obj_equals(const Object object_1, const Object object_2);
void troca_bit(int *s, int j);

// Lista encadeada
Arraylist cria_lista_objetos(int n, int *s, double *p, double *w);
Arraylist cria_lista_objetos_ordenada(int n, int *s, double *p, double *w);
void imprime_lista(Arraylist lista);

// Contrutivos
void constroi_solucao_grasp(int n, int *s, double *p, double *w, double b, double alfa);

// Metaheuristicas
void VND(int n, int *s, double *p, double *w, double b);
void grasp(int n, int *s, double *p, double *w, double b, int iter_max, double alfa);

// Path Relinking
int* path_relinking(int n, int *s_corrente, int *s_guia, double *p, double *w, double b);
void adiciona_solucao_conjunto_elite(int n, int *s, int **conjunto_elite, int *tamanho_atual_elite);