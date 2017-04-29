#ifndef __DIGRAFO_H
#define __DIGRAFO_H

//typedef int Vertice;
typedef struct arco Arco;

struct arco{
	int v, w;
};

typedef struct digrafo Digrafo;

struct digrafo{
	int V, A;
	int** adj;
};

typedef struct palavra Palavra;

struct palavra{
	char* info;
	int index;
};


int** matriz_inicializa(int linhas, int colunas, int valor);

int** matriz_libera(int** matriz, int linhas);

Digrafo* digrafo_inicializa(int V);

Digrafo* digrafo_libera(Digrafo* digrafo);

void digrafo_insere_arco(Digrafo* digrafo, int v, int w);

void digrafo_remove_arco(Digrafo* digrafo, int v, int w);

void digrafo_imprime(Digrafo* digrafo);

Digrafo* digrafo_libera(Digrafo* digrafo);


void matriz_adj_componentes(Digrafo* digrafo);

Digrafo* processa_ladders(FILE* leitura, char** palavras, int num_palavras, int tam_palavra);

void ladders(Digrafo* digrafo, char ** palavras, int num_palavras, char* palavra1, char* palavra2, int tam_palavra);

//void ladders(Digrafo* digrafo, int num_palavras, char* palavra1, char* palavra2);


#endif
