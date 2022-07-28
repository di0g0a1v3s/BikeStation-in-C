#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "viagem.h"
#include "estacao.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif

#ifndef MODO_TEXTUAL_H
#define MODO_TEXTUAL_H

//definição de funções relacionadas com o modo textual
void verificarArgumentos(int argc, char* argv[]);
void modo_textual(char* ficheiro_viagens, estacao_node *head_estacao, viagem_node **head_viagem);
void selecao_dados(char* ficheiro_viagens, viagem_node **head_viagens, estacao_node *head_estacoes, int *todas_viagens, int *periodo_tempo_enable,
                   int *dia_semana_enable, int *duracao_enable, hora_data *tempo_inicial, hora_data *tempo_final, int *dia_semana, int *duracao);
void display_menus_selecao_dados(int periodo_tempo_enable, hora_data tempo_inicial, hora_data tempo_final, int dia_semana_enable,
                                int dia_semana, int duracao_enable, int duracao, int* criterio);
void periodo_tempo_sel_dados(hora_data* tempo_inicial, hora_data* tempo_final, int *periodo_tempo_enable);
void dia_semana_sel_dados(int *dia_semana, int *dia_semana_enable);
void duracao_sel_dados(int *duracao, int *duracao_enable);
void remover_nodes_viagens(viagem_node **head_viagens, hora_data* tempo_inicial, hora_data* tempo_final, int* duracao,
                           int *dia_semana, int *duracao_enable,int* dia_semana_enable, int *periodo_tempo_enable);
int manter_viagem(viagem _viagem, hora_data tempo_inicial, hora_data tempo_final, int duracao, int dia_da_semana, int duracao_enable,
              int dia_semana_enable, int periodo_tempo_enable);
int verificar_periodo_tempo(viagem _viagem, hora_data tempo_inicial, hora_data tempo_final);
int dia_semana(int dia, int mes, int ano);
void listagem_viagens(viagem_node *head_viagens);
void imprimir_viagem(viagem _viagem);
void listagem_estacoes(viagem_node *head_viagens, estacao_node *head_estacoes, int periodo_tempo_enable, hora_data tempo_inicial,
                       hora_data tempo_final);
estacao_node* pedir_estacao(estacao_node *head_estacoes);
void contagem_horas(hora_data tempo_inicial, hora_data tempo_final, float* num_horas_efetivo, int* num_horas);
void bicicletas_chegar_hora(int num_horas, estacao_node* current_estacao, viagem_node* head_viagens, hora_data tempo_inicial, float num_horas_efetivo);
void bicicletas_partir_hora(int num_horas, estacao_node* current_estacao, viagem_node* head_viagens, hora_data tempo_inicial, float num_horas_efetivo);
void listagem_rotas(viagem_node *head_viagens, estacao_node *head_estacoes);
void contagem_viagens_rotas(int *contagem_estacoes, estacao_node* head_estacoes, estacao_node* estacao_escolhida,
                            viagem_node *head_viagens, int **num_viagens_chegada, int **num_viagens_partida );
int imprimir_rota(int *num_impressoes, int *num_viagens_chegada,int *num_viagens_partida, int contagem_estacoes,
                  estacao_node* head_estacoes, estacao_node* estacao_escolhida);
void listagem_estatisticas(viagem_node *head_viagem, int periodo_tempo_enable, int dia_semana_enable, int duracao_enable, hora_data tempo_inicial,
                         hora_data tempo_final, int dia_semana, int duracao);
void estatisticas_genero(viagem_node *head_viagens);
void estatisticas_idade(viagem_node *head_viagens);
void estatisticas_duracao(viagem_node *head_viagens);
void estatisticas_velocidade(viagem_node *head_viagens);
float graus_para_radianos(float angulo_em_graus);
float distancia(float lat1, float lon1, float lat2, float lon2);
void velocidade_masculino(viagem_node *head_viagens, float* num_km_masc, float* duracao_total_masc, int idade);
void velocidade_feminino(viagem_node *head_viagens, float* num_km_fem, float* duracao_total_fem, int idade);
void imprimir_velocidade(int idade, float duracao_total_masc, float duracao_total_fem, float num_km_masc, float num_km_fem);
void limparJanela();
void esperar_resposta();
void imprimir_em_tabela(char* texto, int tamanho);
void imprimir_restricoes(int periodo_tempo_enable, int dia_semana_enable, int duracao_enable, hora_data tempo_inicial,
                         hora_data tempo_final, int dia_semana, int duracao);
int avancar_pagina();
int pedir_menu(int *menu, int lim_inferior, int lim_superior);


#endif // MODO_TEXTUAL_H
