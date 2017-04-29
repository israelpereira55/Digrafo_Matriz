#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "digrafo_matriz.h"
#include "linkedlist.h"

#define tam 5

int** matriz_inicializa(int linhas, int colunas, int valor){
	int** m = (int**) malloc (linhas * sizeof(int*));

	int i = 0;
	while(i < linhas){
		m[i] = (int*) malloc (colunas * sizeof(int*));
		i++;
	}
	
	int j = 0;
	for(i = 0; i < linhas; i++){
		for(j = 0; j < colunas; j++)
			m[i][j] = valor;
	}

	return m;
}

int** matriz_libera(int** matriz, int linhas){
	int i = 0;
	while(i < linhas){
		free(matriz[i]);
		i++;
	}
	free(matriz);
	return NULL;
}

Digrafo* digrafo_inicializa(int V){
	Digrafo* digrafo = (Digrafo*) malloc (sizeof(Digrafo));
	
	digrafo->V = V;
	digrafo->A = 0;
	digrafo->adj = matriz_inicializa(V, V, 0);
	return digrafo;
}

Digrafo* digrafo_libera(Digrafo* digrafo){
	digrafo->adj = matriz_libera(digrafo->adj, digrafo->V);
	free(digrafo);
	return NULL;
}

void digrafo_insere_arco(Digrafo* digrafo, int v, int w){
	if(digrafo->adj[v][w] == 0){
		digrafo->adj[v][w] = 1;
		digrafo->A++;
	}

	return;
}

void digrafo_remove_arco(Digrafo* digrafo, int v, int w){
	if(digrafo->adj[v][w] != 0){
		digrafo->adj[v][w] = 0;
		digrafo->A--;
	}

	return;
}

void digrafo_imprime(Digrafo* digrafo){
	int n = digrafo->V;
	printf("\n  Número de vértices x Número de arestas:\n  %d x %d\n", n, digrafo->A);
	
	if(n == 0){
		printf("\n");
		return;
	}
	
	int** adj = digrafo->adj;
	int* adj_linha = NULL;

	int v,w;
	for(v = 0; v < n; v++){
		adj_linha = adj[v];
		for(w = 0; w < n; w++) {
			if(adj_linha[w])
				printf("   %d %d\n", v, w);
		}
	}
	return;
}

void digrafo_imprime_adjacentes_saida(Digrafo* digrafo, int* conjunto, int n){
	int num_linhas = digrafo->V,
	    selecionados[num_linhas],
	    adjacentes[num_linhas]; //Indica os vizinhos elegiveis.
	
	int i = 0;
	for(i = 0; i < num_linhas; i++)
		adjacentes[i] = 1;
		
	for(i = 0; i < n; i++)
		adjacentes[ conjunto[i] ] = 0;
	
	int** adj = digrafo->adj;
	int* adj_linha = NULL;
	int num_adjacentes = 0,
	    index = 0,
	    j = 0;
	    
	for(i = 0; i < n; i++){
		adj_linha = adj[ conjunto[i] ]; //Seleciona a linha de um elemento do conjunto X.
		for(j = 0; j < num_linhas; j++){
			if(adj_linha[j] > 0 && adjacentes[j]){
				num_adjacentes++;
				adjacentes[j] = 0;
				selecionados[index] = j;
				index++;
			}
		}
	}
	
	printf("\n  Grau de saída de de X: %d.\n", num_adjacentes);
	printf("  Vizinhos:");			
	
	for(i = 0; i < index; i++)
		printf(" %d", selecionados[i]);
	
	printf(".\n");	
	return;
}

void digrafo_imprime_adjacentes_entrada(Digrafo* digrafo, int* conjunto, int n){
	int num_colunas = digrafo->V,
	    selecionados[num_colunas],
	    adjacentes[num_colunas]; //Indica os vizinhos elegiveis.
	
	int i = 0;
	for(i = 0; i < num_colunas; i++)
		adjacentes[i] = 1;
		
	for(i = 0; i < n; i++)
		adjacentes[ conjunto[i] ] = 0;
	
	int** adj = digrafo->adj;
	int num_adjacentes = 0,
	    index = 0,
	    elemento = 0,
	    j = 0;
	    
	for(i = 0; i < n; i++){
		elemento = conjunto[i];
		for(j = 0; j < num_colunas; j++){
			if(adj[j][elemento] > 0 && adjacentes[j]){
				num_adjacentes++;
				adjacentes[j] = 0;
				selecionados[index] = j;
				index++;
			}
		}
	}
	
	printf("\n  Grau de entrada de de X: %d.\n", num_adjacentes);
	printf("  Vizinhos:");			
	
	for(i = 0; i < index; i++)
		printf(" %d", selecionados[i]);
	
	printf(".\n");	
	return;
}

void matriz_adj_componentes(Digrafo* digrafo){
	int num_vertices = digrafo->V;
	int rotulo[num_vertices];
	
	int** adj = digrafo->adj;
	int* adj_linha = NULL; 
	
	int alpha = 0,
	    beta = 0;

	int i = 0;
	while(i < num_vertices){
		rotulo[i] = i;
		i++;
	}
	
	int num_arcos_atuais = 0,
	    num_arcos = digrafo->A,
	    index = 0,
	    j = 0;

	for(i = 0; i < num_vertices; i++){
		alpha = rotulo[i];
		adj_linha = adj[i];
		for(j = 0; j < num_vertices; j++){
			if(adj_linha[j]){
				beta = rotulo[j];
				
				if(alpha != beta){
					for(index = 0; index < num_vertices; index++){
						if(rotulo[index] == beta)
							rotulo[index] = alpha;
					}
				}
			}
		}
		
		num_arcos_atuais++;
		if(num_arcos_atuais == num_arcos)
			goto IMPRIME_ROTULOS;
	}
	
IMPRIME_ROTULOS:
	printf("Vértices x Componente:\n");
	for(i = 0; i < num_vertices; i++)
		printf("   %d %d\n", i, rotulo[i]);
	
	return;
}

Digrafo* processa_ladders(FILE* leitura, char** palavras, int num_palavras, int tam_palavra){
	Digrafo* digrafo = digrafo_inicializa(num_palavras);
	
	int i = 0;
	int dif = 0;
	int index_p1 = 0;
	int index_p2 = 1;
	char* palavra1 = NULL;
	char* palavra2 = NULL;
	
	while(index_p1 < num_palavras){
		palavra1 = palavras[index_p1];
		
		index_p2 = index_p1 ;
		if(index_p2 == index_p1)
			index_p2++;
			
		while(index_p2 < num_palavras){
			palavra2 = palavras[index_p2];
			
			dif = 0;
			for(i = 0; i < tam && dif < 2; i++){
				if(palavra1[i] != palavra2[i])
					dif++;
			}
		
			if(dif == 1){
				digrafo_insere_arco(digrafo, index_p1, index_p2);
				digrafo_insere_arco(digrafo, index_p2, index_p1);
			}
			
			index_p2++;
			if(index_p2 == index_p1)
				index_p2++;
		}
		index_p1++;
	}
	
	return digrafo;
}

void ladders(Digrafo* digrafo, char ** palavras, int num_palavras, char* palavra1, char* palavra2, int tam_palavra){
	int visitado[num_palavras];
	int indices[num_palavras];
	int pai[num_palavras];
	int caminho[num_palavras];
	int i = 0;
	int index = 0;
	while(i < num_palavras){
		visitado[i] = 0;
		indices[i] = i;
		pai[i] = -1;
		i++;
	}
	
	for(i = 0; i < num_palavras; i++){
		if(!strcmp(palavras[i], palavra2))
			break;
	}
	int index_p2 = i;
	
	for(i = 0; i < num_palavras; i++){
		if(!strcmp(palavras[i], palavra1))
			break;
	}
	int index_p1 = i;
	
	
	int *u;
	int** adj = digrafo->adj;
	int v;
	
	List* fila = list_create();
	list_insert_last(fila, &indices[i]);
	visitado[i] = 1;
	
	while(!list_is_empty(fila)){
		u = list_remove_first(fila);
	
		for(i = 0; i < num_palavras; i++){
			if(adj[*u][i] && !visitado[i]){
				list_insert_last(fila, &indices[i]);
				visitado[i] = 1;
				pai[i] = *u;
				
				//Encontrou a palavra2.
				if(i == index_p2)
					goto FINALIZA_BUSCA;
			}
		}	
	}

FINALIZA_BUSCA:
	v = index_p2;
	
	
	if(pai[v] == -1){
		printf("    Não existe caminho.\n");
	} else {
	
		caminho[0] = index_p2;
		index = 1;
		while(v != index_p1){
			v = pai[v];
			caminho[index] = v;
			index++;
		}
		
		for(i = index -1; i >= 0; i--)
			printf("     %2d - %s\n", index -i, palavras[ caminho[i] ]);
	}
	
	list_clear(fila);
	return;
}
	
/*
void ladders(Digrafo* digrafo, int num_palavras, char* palavra1, char* palavra2){
	int i = 0;
	int j = 0;
	
	int fechados[num_palavras];
	int indices[num_palavras];
	while(i < num_palavras){
		fechados[i] = 0;
		indices[i] = i;
	}
	
	for(i = 0; i < num_palavras; i++){
		if(!strcmp(palavras[i], palavra1)
			break;
	}
	
	for(i = 0; i < num_palavras; i++){
		if(!strcmp(palavras[i], palavra2)
			break;
	}
	int index_p2 = i;
	
	List* pilha = list_create();
	List* solucao = list_create();
	
	list_insert_last(pilha, &indices[i]);
	int* p, *u;
	int** adj = digrafo->adj;
	int possui_adj = 0;
	
	while(!list_is_empty(pilha)){
		u = list_get_last(pilha);
	
		for(i = 0; i < num_palavras; i++){
			if(adj[*u][j] && !fechado[j] && !possui_adj){
				list_insert_last(pilha, &indices[j]);
				possui_adj = 1;
			}
			
			if(j == index_p2){
				goto FINALIZA_BUSCA;
			}
			
			if(!possui_adj){
				list_remove_last(pilha);
			}
		}
	}

FINALIZA_BUSCA:
	int n = list_size(pilha);
	if(n > 0){
		for(i = 0; i < n; i++){
			p = list_get_first(pilha);
			printf("%s\n", palavras[*p]);
			list_remove_first(pilha);
		}
	} else  printf("Não existe caminho.\n");
	
	list_clear(pilha);
	return;
}*/
