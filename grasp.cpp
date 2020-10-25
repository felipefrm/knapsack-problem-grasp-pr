#include "grasp.h"

/******************************************************************************************/
/*										MAIN											  */
/******************************************************************************************/
int main()
{
	int n;              // numero de objetos
	int b;           // capacidade da mochila
	int *s;             // vetor solucao corrente
	int *s_star;        // vetor melhor solucao
	double *w;          // vetor de peso de cada objeto
	double *p;          // vetor de utilidade de cada objeto
	double fo;          // funcao objetivo corrente
	double fo_star;     // melhor funcao objetivo
	
	srand((unsigned) time(NULL)); // pega a hora do relogio como semente
	
	// Cria os vetores
	s = (int *) malloc(n * sizeof(int));
	s_star = (int *) malloc(n * sizeof(int));	
	
	// operações de leitura do arquivo de instancias
	FILE* f = fopen("large_scale/knapPI_2_500_1000_1", "r");
	le_cabecalho_arquivo(f, &n, &b);
	printf("%d, %ld\n", n, b);
	w = (double*)malloc(n * sizeof(double));
    p = (double*)malloc(n * sizeof(double));
	le_corpo_arquivo(f, n, w, p);
	fclose (f);   

	fo_star = - DBL_MAX; // inicializa FO da melhor solucao

    grasp(n,s,p,w,b,50,0.7);
    printf("Solucao do GRASP:\n");
    imprime_solucao(s,n,p,w,b);
	
	// Libera memoria
	free(s);
	free(s_star);
	free(w);
	free(p);
	return 0;
	
}

/******************************************************************************************/
/*				UTIL							  */
/******************************************************************************************/

// Leitura primeira linha do arquivo
void le_cabecalho_arquivo(FILE *f, int *n, int *b) {
	fscanf (f, "%d %d", n, b);
}

// Leitrua restante do arquivo
void le_corpo_arquivo(FILE *f, int n, double *w, double *p)
{
    int i = 0;
    while (i < n)
    {  
        fscanf (f, "%lf %lf", &p[i], &w[i]);
        i++;
    }
}

// Calcula fo
double calcula_fo(int *s, int num_objetos, double *p, double *w, int b)
{
	double fo;
	double utilidade = 0, peso = 0, penalidade = 0;
	
	for (int i = 0; i < num_objetos; i++) {
		if (s[i]) {
			utilidade += p[i];
			peso += w[i];
		}
		penalidade += w[i];
	}
	
	// FO e a soma das utilidades dos objetos na mochila
	// menos o excesso de peso na mochila x penalidade
	fo = utilidade - penalidade * MAX(0, peso - b);
	
	return fo;
}


// Imprime solucao
void imprime_solucao(int *s, int num_objetos, double *p, double *w, int b)
{
	double fo;
	double utilidade = 0, peso = 0, penalidade = 0;
	
	for (int i = 0; i < num_objetos; i++) {
		if (s[i]) {
			utilidade += p[i];
			peso += w[i];
		}
		penalidade += w[i];
	}
	
	// FO e a soma das utilidades dos objetos na mochila
	// menos o excesso de peso na mochila x penalidade
	fo = utilidade - penalidade * MAX(0, peso - b);
	
	
	printf("FO = %.2lf\n", fo);
    printf("Peso = %.2lf\n", peso);
    printf("Utilidade = %.2lf\n", utilidade);
    for (int j=0; j<num_objetos; j++) printf("s[%2d]=%d \n",j,s[j]);
}

// Verifica se objetos sao iguais
const Boolean obj_equals(const Object object_1, const Object object_2){
	
	Objeto *o1 = (Objeto*)object_1;
	Objeto *o2 = (Objeto*)object_2;
	
	if (o1->id == o2->id) return TRUE;
	else return FALSE;
}

// Cria lista dos objetos 
Arraylist cria_lista_objetos(int n, int *s, double *p, double *w)
{
	Objeto* o;
	Arraylist objetos;
	objetos = arraylist_create(obj_equals);
	   
	// Cria lista de objetos
	for (int i = 0; i < n; i++) {
		o = (Objeto*)malloc(sizeof(Objeto));
		o->id = i;
		o->peso = w[i];
		o->utilidade = p[i];
		o->profit = p[i]/w[i];
		arraylist_add(objetos, o);
	}
	return objetos;
}

// Criar lista dos objetos ordenada
Arraylist cria_lista_objetos_ordenada(int n, int *s, double *p, double *w)
{
	int pos;
	double maior;
	Objeto* o;
	
	Arraylist objetos = cria_lista_objetos(n,s,p,w);
	Arraylist objetosOrd = arraylist_create(obj_equals);
	   
	// Cria lista de objetos ordenada
	while (arraylist_size(objetos) > 0) {
		
		// Busca objeto com maior profit
        maior = -1;
		for (int i = 0; i < arraylist_size(objetos); i++) {
			if ( ((Objeto*)arraylist_get(objetos, i))->profit > maior) {
                maior = ((Objeto*)arraylist_get(objetos, i))->profit;
                pos = i;
			}
		}
        
		// Insere objeto na lista ordenada
		o = (Objeto*) arraylist_remove_index(objetos, pos);
		arraylist_add(objetosOrd, o);
	}
	return objetosOrd;
}

// Imprime lista
void imprime_lista(Arraylist lista){
	printf("\nIMPRIME LISTA\n");
	
	int size;
	Objeto *o;
	
	if(lista != NULL){
		printf("[ID]\tUtilidade\tPeso\tProfit\n");
		size = arraylist_size(lista);
		for (int i = 0; i < size; i++)
		{
			o = (Objeto*)arraylist_get(lista, i);
			printf("[%d]\t%lf\t%lf\t%lf\n", o->id, o->utilidade, o->peso, o->profit);
		}
		printf("\n");
	}
}

// Insere ou retira o objeto j da mochila
void troca_bit(int *s, int j)
{
	// printf("\nTROCA BIT\n");

    if (s[j])
	s[j] = 0;
    else
	s[j] = 1;
}

/* aplica busca local pela estrategia do melhor aprimorante */
void melhor_vizinho_N1(int n, int *s, double *p, double *w, int b)
{

    double fo_original;
    double fo_max;
    double fo_vizinho;
    int melhor_bit;
    
    fo_original = calcula_fo(s, n, p, w, b);
    
    fo_max = -DBL_MAX;
        
    for (int j = 0; j < n; j++) {
            
        // cria vizinho
        troca_bit(s, j);
            
        fo_vizinho = calcula_fo(s,n,p,w,b);
            
        // armazena melhor vizinho
        if (fo_vizinho > fo_max) {
            melhor_bit = j;
            fo_max = fo_vizinho;
        }
            
        // volta a solucao inicial
        troca_bit(s,j);
    }
        
    // se encontrou algum vizinho melhor
    if (fo_max > fo_original) {
        troca_bit(s,melhor_bit);
        printf("Vizinho melhor em N1! FO = %lf\n", fo_max);
    }

}

/* aplica busca local pela estrategia do melhor aprimorante */
void melhor_vizinho_N2(int n, int *s, double *p, double *w, int b)
{

    double fo_max, fo_vizinho;
    int melhor_bit_1, melhor_bit_2;
    
    double fo_original = calcula_fo(s, n, p, w, b);
    
    fo_max = -DBL_MAX;
        
        // para cada par de posicoes do vetor
        for (int i = 0; i < n; i++) {
            
            // troca primeiro bit
            troca_bit(s, i);
            
            for (int j = i+1; j < n; j++) {
                
                
                // troca segundo bit
                troca_bit(s, j);
                
                // fo vizinho
                fo_vizinho = calcula_fo(s,n,p,w,b);
                
                // armazena melhor vizinho
                if (fo_vizinho > fo_max) {
                    melhor_bit_1 = i;
                    melhor_bit_2 = j;
                    fo_max = fo_vizinho;
                }
                
                // volta segundo bit
                troca_bit(s,j);
                
            }
            // volta a solucao inicial
            troca_bit(s,i);	
        }
        
        // se encontrou algum vizinho melhor
        if (fo_max > fo_original) {
            troca_bit(s,melhor_bit_1);
            troca_bit(s,melhor_bit_2);			
            // printf("Vizinho melhor em N2! FO = %lf\n", fo_max);
        }
}


/******************************************************************************************/
/*				CONSTRUTIVOS						  */
/******************************************************************************************/

void constroi_solucao_grasp(int n, int *s, double *p, double *w, int b, double alfa)
// (n, s, p, w, b) = (numero de objetos, solucao corrente, vetor utilidade, vetor peso, capacidade mochila)
{
	double peso = 0;
	double capacidade = 0;
	int j, tamRestrito;
	double value;
	Objeto *o, *o1, *o2;
	
	// Limpa solucao
	for (int j=0; j<n; j++) s[j] = 0;
	
	// Cria lista de objetos ordenados
	Arraylist objetosOrd = cria_lista_objetos_ordenada(n,s,p,w);
	//imprime_lista(objetosOrd);
	
	// Constroi solucao elemento a elemento, verificando se cada objeto cabe na capacidade residual da mochila
	while (arraylist_size(objetosOrd) > 0 && peso < b) {
		
        	// Zera tam restrito
        	tamRestrito = 0;
		
		// Define o tamanho da lista restrita, ou seja, os alfa % mais interessantes
        
        	// Recupera objeto
		o1 = (Objeto*)arraylist_get(objetosOrd, 0);
        	o2 = (Objeto*)arraylist_get(objetosOrd, arraylist_size(objetosOrd)-1);
        	//printf("cmax: %.2lf   cmin: %.2lf\n", );
        	value = o1->profit - alfa * (o1->profit - o2->profit);
        	//printf("Valor referencia: %.2lf\n", value);
        
        	for (int i = 0; i < arraylist_size(objetosOrd); i++) {
            		o = (Objeto*)arraylist_get(objetosOrd, i);
            		if (o->profit >= value) tamRestrito++;
            		else break;
        	}
        	//printf("Tam restrito: %d\n", tamRestrito);
		
		// Sorteia posicao aleatoria da lista residual
		j = (int)((float)rand()/RAND_MAX * tamRestrito);
        
		// Recupera objeto
		o = (Objeto*)arraylist_get(objetosOrd, j);
		
		// Se objeto ainda nao esta na mochila e cabe nela, adiciona objeto a mochila
		if (s[o->id] != 1 && peso + o->peso <= b) {
			s[o->id] = 1;
			peso += o->peso;
		}
		
		// Remove objeto da lista, pois ja foi testado
		arraylist_remove_index(objetosOrd, j);
	}
}

/******************************************************************************************/
/*				METAHEURISTICAS						  */
/******************************************************************************************/

/* aplica metaheuristica VND */
void VND(int n, int *s, double *p, double *w, int b)
{
	int k;
	double fo_s;
	
    	k = 1;
	
	while(k <= 2) //while 1
    	{
		// calcula o valor da fo
		fo_s = calcula_fo(s,n,p,w,b);
		
		switch(k){
			case 1: melhor_vizinho_N1(n, s, p, w, b);
				break;
			case 2: melhor_vizinho_N2(n, s, p, w, b);
				break;
			default:
				break;
		}
		if (calcula_fo(s,n,p,w,b) > fo_s)
		{
			k = 1;
		}
		else k++;
    }
}

void adiciona_solucao_conjunto_elite(int n, double *p, double *w, int b, int *s, int tamanho_elite, int **conjunto_elite, int *tamanho_atual_elite) {

		int *pior_solucao;
		int indice_pior;

		if ((*tamanho_atual_elite) < tamanho_elite) {					// se o conjunto elite n foi totalmente preenchido
			for (int j = 0; j < n; j++) { 							// apenas adiciona a solucao a ele
				conjunto_elite[*tamanho_atual_elite][j] = s[j];
			}
			(*tamanho_atual_elite)++;
		}
		else {														// senao, busca a pior solucao, compara com a solucao
			pior_solucao = conjunto_elite[0];						// pretendente, se ela for melhor q a pior, entra no conjunto
			indice_pior = 0;
			for (int j = 1; j < tamanho_elite; j++) {
				if (calcula_fo(conjunto_elite[j], n, p, w, b) < calcula_fo(pior_solucao, n, p, w, b)) {
					pior_solucao = conjunto_elite[j];
					indice_pior = j;
				}
			}

			if (calcula_fo(pior_solucao, n, p, w, b) < calcula_fo(s, n, p, w, b)) {
				for (int j = 0; j < n; j++) {
					conjunto_elite[indice_pior][j] = s[j];
				}
			}
		}
}

int* path_relinking(int n, int *s_corrente, int *s_guia, double *p, double *w, int b) {

	int cardinalidade = 0;
	int *s_melhor = (int*)malloc(sizeof(int) * n);
	int *s_candidata = (int*)malloc(sizeof(int) * n);
	int *s_melhor_movimento = (int*)malloc(sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		if (s_corrente[i] != s_guia[i]) {
			cardinalidade++;
		}
	}

	if (calcula_fo(s_corrente,n,p,w,b) > calcula_fo(s_guia,n,p,w,b)) {	// para inciar solucao melhor com algum valor
		for (int i = 0; i < n; i++)										// verifica qual tem maior fo: s_corrente ou s_guia
			s_melhor[i] = s_corrente[i];
	}
	else {
		for (int i = 0; i < n; i++) 
			s_melhor[i] = s_guia[i];
	}

	while (cardinalidade > 0) {

		for (int i = 0; i < n; i++) s_melhor_movimento[i] = 0;	// melhor movimento inicializa vazio

		for (int i = 0; i < n; i++) {
			if (s_corrente[i] != s_guia[i]) {
				for (int j = 0; j < n; j++) {
					s_candidata[j] = s_corrente[j];
				}
				troca_bit(s_candidata, i);
				
				if (calcula_fo(s_candidata,n,p,w,b) > calcula_fo(s_melhor_movimento,n,p,w,b)) {
					for (int j = 0; j < n; j++) 
						s_melhor_movimento[j] = s_candidata[j];
				}

				if (calcula_fo(s_candidata,n,p,w,b) > calcula_fo(s_melhor,n,p,w,b)) {
					for (int j = 0; j < n; j++) 
						s_melhor[j] = s_candidata[j];
				}
			}
		}

		for (int i = 0; i < n; i++) {
			s_corrente[i] = s_melhor_movimento[i];
		}	
		cardinalidade--;
	}
	return s_melhor;
}

/* aplica metaheuristica GRASP */
void grasp(int n, int *s, double *p, double *w, int b, int iter_max, double alfa)
{
	int *sl, *s_corrente, *s_guia;
	int random_index;
	int tamanho_atual_elite = 0;
	int tamanho_elite = 10;

    srand(time(NULL));


	double fo_star = -DBL_MAX;
	
	// cria solucao auxiliar
	sl = (int *) malloc(n * sizeof(int));
	
	// cria conjunto elite vazio
	int **conjunto_elite = (int**)malloc(tamanho_elite * sizeof(int*)); //Aloca um Vetor de Ponteiros
	
	for (int i = 0; i < n; i++){ //Percorre as linhas do Vetor de Ponteiros
		conjunto_elite[i] = (int*) malloc(n * sizeof(int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
	}

	// Enquanto melhoria
	for (int i = 0; i < iter_max; i++) {
		
		// Limpa solucao
		for (int j=0; j<n; j++) sl[j] = 0;
		
		// Constroi solucao parcialmente gulosa
		constroi_solucao_grasp(n,sl,p,w,b,alfa);
		printf("solucao construida: %lf\t", calcula_fo(sl,n,p,w,b));
		
		// Aplica busca local na solucao construida
		VND(n,sl,p,w,b);
		printf("solucao refinada: %lf\t", calcula_fo(sl,n,p,w,b));


		if (tamanho_atual_elite > 1) {	
			s_corrente = sl;
			s_guia = conjunto_elite[rand() % tamanho_atual_elite]; // escolhe uma solucao aleatoria do conjunto elite
			sl = path_relinking(n, s_corrente, s_guia, p, w, b);
			printf("solucao apos path relinking: %lf\n", calcula_fo(sl,n,p,w,b));
		}

		adiciona_solucao_conjunto_elite(n, p, w, b, sl, tamanho_elite, conjunto_elite, &tamanho_atual_elite);
		
		// Atualiza melhor solucao
		if (calcula_fo(sl,n,p,w,b) > fo_star) {
			
			// copia em s a melhor solucao
			for (int i = 0; i < n; i++) s[i] = sl[i];
			
			// atualiza fo
			fo_star = calcula_fo(sl,n,p,w,b);
		}
	}
}



