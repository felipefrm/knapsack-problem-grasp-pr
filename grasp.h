#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <limits.h>
#include <float.h>
#include <string.h>
#include "arraylist.h"

#define MAX(x,y) ((x)<(y) ? (y) : (x))

typedef struct{
	int id;
	double peso;        // peso do objeto
	double utilidade;   // utilidade do objeto
	double profit;      // profit do objeto (utilidade/peso)
} Objeto;


// Util
int le_cabecalho_arquivo(const char *nomeArq, int *n, int *b);
int le_corpo_arquivo(const char *nomeArq, int n, double *w, double *p);
double calcula_fo(int *s, int num_objetos, double *p, double *w, int b);
void imprime_solucao(int *s, int num_objetos, double *p, double *w, int b);
const Boolean obj_equals(const Object object_1, const Object object_2);
void troca_bit(int *s, int j);
void contaTempoProcessador(double *utime, double *stime);
void imprimeTempo(double user_time, double system_time);

// Lista encadeada
Arraylist cria_lista_objetos(int n, int *s, double *p, double *w);
Arraylist cria_lista_objetos_ordenada(int n, int *s, double *p, double *w);

// Contrutivos
void constroi_solucao_grasp(int n, int *s, double *p, double *w, int b, double alfa);

// Metaheuristicas
void VND(int n, int *s, double *p, double *w, int b);
void grasp(int n, int *s, double *p, double *w, int b, int iter_max, double alfa);
void grasp_pr(int n, int *s, double *p, double *w, int b, int iter_max, double alfa);

// Path Relinking
int* path_relinking(int n, int *s_corrente, int *s_guia, double *p, double *w, int b);
void adiciona_solucao_conjunto_elite(int n, int *s, int **conjunto_elite, int *tamanho_atual_elite);