  #include "headerT2.h"

/* ---------- Início das Funções de Criação. ---------- */

/* Inicializa o grafo. */
Grafo* criar_grafo (int tamanho) {
  int v;
  Grafo *G = (Grafo *)malloc(sizeof(Grafo));
  G->E = 0;
  G->V = tamanho;
  G->listaAdj = (Vertice **)malloc(tamanho * sizeof(Vertice *));
  for (v = 0; v < G->V; v++) {
    G->listaAdj[v] = NULL;
  }
  return G;
}

/* Insere novo elemento (v) na lista de adjacências
de determinado elemento (u). */
void inserir_aresta (Grafo *G, int u, int v) {
  Vertice *temp, *ultimo = NULL;
  /* Verificando se o vértice v já existe na lista de adjacências de u: */
  for (temp = G->listaAdj[u]; temp != NULL; temp = temp->proximo) {
    if (temp->identificador == v) {
      return; //Encerra a função.
    }
    ultimo = temp;
  }
  /* Inserindo a aresta (u,v): */
  Vertice *novo = (Vertice *)malloc(sizeof(Vertice));
  novo->identificador = v;
  novo->proximo = NULL;
  if (ultimo != NULL) {
    /* Inserção na última posição da lista: */
    ultimo->proximo = novo;
  }
  else {
    /* Nova cabeça da lista: */
    G->listaAdj[u] = novo;
  }
  /* Incrementando o número de arestas: */
  G->E++;
}

/* ---------- Fim das Funções de Criação. ---------- */

/* Está função é responsável pela eliminação de arestas já visitadas.
Ela retorna o inteiro que identifica o filho entre dois vértices
para dar continuidade a função DFS_Visit_Trabalho2, pois algum ponteiro
foi deletado e certamente este valor foi perdido naquela função. */
int deletaAresta (Grafo *G, int filho, int pai) {
  Vertice *tmp1 = G->listaAdj[pai];
  Vertice *tmp2 = G->listaAdj[filho];
  Vertice *aux1, *aux2 = NULL;

  /* Busca os elementos na lista de adjacências. */
  while (tmp1->identificador != filho) {
    aux1 = tmp1; //Salva o anterior.
    tmp1 = tmp1->proximo;
  }
  while (tmp2->identificador != pai) {
    aux2 = tmp2; //Salva o anterior ao procurado.
    tmp2 = tmp2->proximo;
  }

  /* Casos do pai. */
  if (tmp1 == G->listaAdj[pai]) { //Encontrou o filho na cabeça.
    aux1 = tmp1;
    free(tmp1);
    G->listaAdj[pai] = aux1->proximo;
  } else if (tmp1->proximo == NULL) {  //Encontrou o filho no fim.
    aux1->proximo = tmp1->proximo; //Aponta pra NULL.
    free(tmp1);
  } else { //Encontrou no "meio".
    aux1->proximo = tmp1->proximo;
    free(tmp1);
  }

  /* Casos do filho. */
  if (tmp2 == G->listaAdj[filho]) { //Encontrou o pai na cabeça.
    aux2 = tmp2;
    free(tmp2);
    G->listaAdj[filho] = aux2->proximo;
  } else if (tmp2->proximo == NULL) {  //Encontrou o pai no fim.
    aux2->proximo = NULL;
    free(tmp2);
  } else { //Encontrou no "meio".
    aux2->proximo = tmp2->proximo;
    free(tmp2);
  }
  return filho;
}

/* Libera a memória alocada pela estrutura do grafo. */
void liberar_grafo (Grafo *G) {
  int v;
  for (v = 0; v < G->V; v++) {
    if (G->listaAdj[v] != NULL) {
      free(G->listaAdj[v]);
    }
  }
  free(G->listaAdj);
  free(G);
}

/* Retorna o número de adjacências de determinado vértice. */
int grauVertice (Grafo *G, int v) {
  int numAdj = 0;
  Vertice *tmp = G->listaAdj[v];
  while (tmp != NULL) {
    numAdj++;
    tmp = tmp->proximo;
  }
  return numAdj;
}

/* Verifica se determinado grafo é um circuito euleriano. */
int verificaCircuito (Grafo *G) {
  int v;
  for (v = 0; v < G->V; v++) {
    /* Um grafo é um cícuito euleriano quando todos os seus vértices são
    pares. */
    if (grauVertice(G, v) % 2 != 0)
      return 0;
  }
  return 1;
}

/* Imprime cada vértice do grafo, bem como a lista de adjacências de cada. */
void imprimir_grafo (Grafo *G) {
  printf("Listas de adjacencias de G:\n");
  int v;
  for (v = 0; v < G->V; v++) {
    if (G->listaAdj[v] != NULL) {
      Vertice *temp;
      printf("Nó %d : ", v);
      for (temp = G->listaAdj[v]; temp != NULL; temp = temp->proximo) {
        printf(" %d ", temp->identificador);
      }
      printf("\n");
    }
  }
}

/* Visita cada vértice do grafo por recursão (backtracking) a partir de um
vértice raiz e elimina as arestas já utilizadas. */
void DFS_Visit_Trabalho2 (Grafo *G, Vertice *V, int u, int raiz) {
  printf(" %d ", u); //Imprime nó.
  Vertice *v;
  for (v = G->listaAdj[u]; v != NULL; v = v->proximo) {
    /* Nas condições abaixo, a função grauVertice auxilia no impedimento
    da aceitação condicional dos retornos das recursões, evitando assim
    acessos inválidos na memória. */
    if (V[v->identificador].cor == BRANCO && v->identificador != raiz
      && grauVertice(G, v->identificador) > 0) {
      V[v->identificador].pai = u; //Define o pai do vértice atual.
      int vIdentificadorSalvo = deletaAresta (G, v->identificador,
        V[v->identificador].pai);
      DFS_Visit_Trabalho2 (G, V, vIdentificadorSalvo, raiz);
    } else {
      /* Deixa a raiz pra depois. Tem adjacência diferente da raiz. */
      if (v->proximo != NULL && grauVertice(G, v->identificador) > 0) {
        V[v->proximo->identificador].pai = u;
        int vIdentificadorSalvo = deletaAresta (G, v->proximo->identificador,
          V[v->proximo->identificador].pai);
        DFS_Visit_Trabalho2 (G, V, vIdentificadorSalvo, raiz);
      } else
        /* Tratamento da raiz. */
        if (grauVertice(G, v->identificador) > 0) {
          V[v->identificador].pai = u;
          int vIdentificadorSalvo = deletaAresta (G, v->identificador,
            V[v->identificador].pai);
          DFS_Visit_Trabalho2 (G, V, vIdentificadorSalvo, raiz);
        }
      }
    }
}

/* Realiza uma busca em profundidade apenas em grafos conexos do tipo circuito
euleriano. A pesquisa é iniciada por uma raiz previamente informada. */
void Busca_Profundidade_Trabalho2 (Grafo *G, int raiz) {
  int u;
  if (verificaCircuito(G)) {
    /* V é um vetor que possui a informação da cor e do pai de cada vértice. */
    Vertice *V = (Vertice *)malloc(G->V * sizeof(Vertice));
    /* Inicialização de todos os vértices. */
    for (u = 0; u < G->V; u++) {
      V[u].cor = BRANCO;
      V[u].pai = -1;
    }
    printf("\nCaminho euleriano: ");
    DFS_Visit_Trabalho2 (G, V, raiz, raiz); //Função de backtracking.
    printf("\n");
  }else
    printf("\nO grafo informado nao possui um circuito euleriano.\n");
}

int main () {

  int tamanho = 6; /* Número de vértices! */

  Grafo *G = criar_grafo (tamanho);

  /* Inserção das arestas: */

  /* Ex. 1.
  inserir_aresta(G, 1, 3);
  inserir_aresta(G, 1, 4);
  inserir_aresta(G, 3, 1);
  inserir_aresta(G, 3, 4);
  inserir_aresta(G, 3, 2);
  inserir_aresta(G, 3, 7);
  inserir_aresta(G, 3, 6);
  inserir_aresta(G, 3, 9);
  inserir_aresta(G, 4, 1);
  inserir_aresta(G, 4, 3);
  inserir_aresta(G, 4, 5);
  inserir_aresta(G, 4, 7);
  inserir_aresta(G, 4, 11);
  inserir_aresta(G, 4, 10);
  inserir_aresta(G, 2, 3);
  inserir_aresta(G, 2, 8);
  inserir_aresta(G, 5, 4);
  inserir_aresta(G, 5, 10);
  inserir_aresta(G, 7, 4);
  inserir_aresta(G, 7, 10);
  inserir_aresta(G, 7, 3);
  inserir_aresta(G, 7, 9);
  inserir_aresta(G, 6, 3);
  inserir_aresta(G, 6, 9);
  inserir_aresta(G, 9, 3);
  inserir_aresta(G, 9, 7);
  inserir_aresta(G, 9, 6);
  inserir_aresta(G, 9, 8);
  inserir_aresta(G, 9, 10);
  inserir_aresta(G, 9, 12);
  inserir_aresta(G, 8, 2);
  inserir_aresta(G, 8, 9);
  inserir_aresta(G, 12, 9);
  inserir_aresta(G, 12, 10);
  inserir_aresta(G, 10, 12);
  inserir_aresta(G, 10, 9);
  inserir_aresta(G, 10, 7);
  inserir_aresta(G, 10, 4);
  inserir_aresta(G, 10, 5);
  inserir_aresta(G, 10, 11);
  inserir_aresta(G, 11, 10);
  inserir_aresta(G, 11, 4);
  //inserir_aresta(G, 11, 5); //Sem caminho euleriano! */

  /* Ex. 2.
  inserir_aresta(G, 1, 2);
  inserir_aresta(G, 1, 3);
  inserir_aresta(G, 2, 3);
  inserir_aresta(G, 2, 1);
  inserir_aresta(G, 2, 4);
  inserir_aresta(G, 3, 1);
  inserir_aresta(G, 3, 2);
  inserir_aresta(G, 3, 5);
  inserir_aresta(G, 2, 5);
  inserir_aresta(G, 3, 4);
  inserir_aresta(G, 4, 2);
  inserir_aresta(G, 4, 3);
  inserir_aresta(G, 4, 5);
  inserir_aresta(G, 5, 3);
  inserir_aresta(G, 5, 2);
  inserir_aresta(G, 5, 4);*/

  /* Ex 3.
  inserir_aresta(G, 0, 1);
  inserir_aresta(G, 0, 2);
  inserir_aresta(G, 1, 0);
  inserir_aresta(G, 1, 2);
  inserir_aresta(G, 1, 3);
  inserir_aresta(G, 1, 4);
  inserir_aresta(G, 2, 0);
  inserir_aresta(G, 2, 1);
  inserir_aresta(G, 2, 3);
  inserir_aresta(G, 2, 5);
  inserir_aresta(G, 3, 1);
  inserir_aresta(G, 3, 2);
  inserir_aresta(G, 3, 4);
  inserir_aresta(G, 3, 5);
  inserir_aresta(G, 4, 1);
  inserir_aresta(G, 4, 3);
  inserir_aresta(G, 4, 5);
  inserir_aresta(G, 4, 6);
  inserir_aresta(G, 5, 2);
  inserir_aresta(G, 5, 3);
  inserir_aresta(G, 5, 4);
  inserir_aresta(G, 5, 6);
  inserir_aresta(G, 6, 4);
  inserir_aresta(G, 6, 5); */

  /* Ex. 4. */
  inserir_aresta(G, 1, 2);
  inserir_aresta(G, 1, 3);
  inserir_aresta(G, 1, 4);
  inserir_aresta(G, 2, 1);
  inserir_aresta(G, 2, 3);
  inserir_aresta(G, 2, 5);
  inserir_aresta(G, 3, 1);
  inserir_aresta(G, 3, 2);
  inserir_aresta(G, 3, 4);
  inserir_aresta(G, 3, 5);
  inserir_aresta(G, 4, 1);
  inserir_aresta(G, 4, 3);
  inserir_aresta(G, 4, 5);
  inserir_aresta(G, 5, 2);
  inserir_aresta(G, 5, 3);
  inserir_aresta(G, 5, 4);

  imprimir_grafo (G);

  Busca_Profundidade_Trabalho2 (G, 3);

  liberar_grafo (G);

  return 0;
}
