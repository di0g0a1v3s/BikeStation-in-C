#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ESTACAO_H
#define ESTACAO_H

#define ID_SIZE 7   //tamanho de uma string de nome curto da estação
#define MUNICIPIO_SIZE 30   //tamanho máximo de uma string de município
#define MAX_STR 300 //tamanho máximo de uma string
#define VIRGULA ','
#define EXISTING "Existing"
#define REMOVED "Removed"

//estrutura que guarda a informação relativa às estações
typedef struct
{
    int id_estacao;
    char nome_curto[ID_SIZE];
    char descricao[MAX_STR];
    double longitude;
    double latitude;
    char municipio[MUNICIPIO_SIZE];
    char estado; //Estado da estação - 'E' - Existente, 'R' - Removida

}estacao;

//nó de estações
typedef struct estacao_node{

    estacao estacao_info;
    struct estacao_node *next; //ponteiro para o próximo elemento da lista

} estacao_node;

//definição de funções relacionadas com as estações
void load_estacoes(char* ficheiro_estacoes, estacao_node **head);
void inserir_estacao_ordenadamente(estacao_node **head, estacao_node **last_node, estacao new_estacao);
estacao ler_linha_estacao(char* buffer, int* ignorar_estacao);
estacao_node* estacao_anterior(estacao_node *head, estacao_node *current);
void libertar_lista_estacoes(estacao_node **head_estacoes);
void proxima_virgula(char** p_aux, char** p_aux2);
void verificar_alocacao_memoria(void* ponteiro);

#endif // ESTACAO_H
