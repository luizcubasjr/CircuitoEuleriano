#include <stdio.h>
#include <stdlib.h>

#define BRANCO 1

/* ----- Structs ----- */

/* Estrutura para um vértice em uma lista encadeada: */
typedef struct vertice {
  int cor;
  int pai;
  int identificador; /* Identificador do vértice armazenado no nó. */
  struct vertice *proximo; /* Próximo nó na lista encadeada. */
} Vertice;

/* Estrutura do Grafo: */
typedef struct grafo {
  int E; /* Quantidade de arestas. */
  int V; /* Quantidade de vértices. */
  Vertice **listaAdj; /* Lista de adjacências. */
} Grafo;

/* ----- Structs ----- */

/* ----- Funções ----- */

Grafo* criar_grafo (int tamanho);

void inserir_aresta (Grafo *G, int u, int v);

void liberar_grafo (Grafo *G);

int grauVertice (Grafo *G, int v);

int verificaCircuito (Grafo *G);

void imprimir_grafo (Grafo *G);

int deletaAresta (Grafo *G, int filho, int pai);

void DFS_Visit_Trabalho2 (Grafo *G, Vertice *V, int u, int raiz);

void Busca_Profundidade_Trabalho2 (Grafo *G, int raiz);
