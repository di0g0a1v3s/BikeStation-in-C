#include <stdio.h>
#include <stdlib.h>
#include "estacao.h"

#ifndef VIAGEM_H
#define VIAGEM_H

#define CASUAL "Casual"
#define REGISTERED "Registered"
#define MALE "Male"
#define FEMALE "Female"

//estrutura que guarda a informação relativa à hora e data de uma viagem
typedef struct
{
    int ano;
    int mes;
    int dia;
    int hora;
    int minuto;
    int segundo;

} hora_data;

//estrutura que guarda a informação relativa a cada viagem
typedef struct
{
    int id_viagem;
    int duracao;
    hora_data data_inicio;
    hora_data data_fim;
    int id_inicial;
    estacao_node* estacao_final; //ponteiro para a estação de fim da viagem
    int id_final;
    estacao_node* estacao_inicial; //ponteiro para a estação de inicio da viagem
    char bicicleta[ID_SIZE];
    char tipo_usuario; //tipo de usuário - 'R': Registado ,'C' - Casual
    int ano_nascimento;
    char genero; //género do usuário - 'M': Masculino, 'F': Feminino

} viagem;

//nó de viagens
typedef struct viagem_node{

    viagem viagem_info;
    struct viagem_node *next; //ponteiro para o próximo elemento da lista

} viagem_node;

//definição de funções relacionadas com as viagens
void load_viagens(char* ficheiro_viagens, viagem_node **head, estacao_node *head_estacao);
void inserir_viagem_ordenadamente(viagem_node **head, viagem_node **last_node, viagem new_viagem);
void ligar_viagens_estacoes(viagem* new_viagem, estacao_node *head_estacao);
viagem ler_linha_viagem(char* buffer, int* ignorar_viagem);
void libertar_lista_viagens(viagem_node **head_viagens);
int cmp_datas(hora_data data1, hora_data data2);

#endif // VIAGEM_H
