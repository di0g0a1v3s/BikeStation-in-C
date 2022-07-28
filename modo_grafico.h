#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "modo_textual.h"
#include "viagem.h"
#include "estacao.h"

#ifndef MODO_GRAFICO_H
#define MODO_GRAFICO_H


#define GROSSURA_PONTO 6        //grossura de um ponto que representa uma estação
#define MAP_SIZE_X 696        //número de pixeis horizontais do mapa
#define MAP_SIZE_Y 470          //número de pixeis verticais do mapa
#define RIGHT_BAR_SIZE 150       //tamanho da barra da direita
#define BOTTOM_BAR_SIZE 100     // tamanho da barra de baixo
#define WINDOW_POSX 50      // posição x inicial da janela
#define WINDOW_POSY 50       // posição y inicial da janela
#define MAX_LAT 42.385000       //latitude máxima de um ponto no mapa
#define MIN_LON -71.153349      //longitude mínima de um ponto no mapa
#define MIN_LAT 42.324381       //latitude mínima de um ponto no mapa
#define MAX_LON -71.034500      //longitude máxima de um ponto no mapa
#define NOME1 "Diogo Alves"
#define NUMERO1 "IST186980"
#define NOME2 "Manuel Reis"
#define NUMERO2 "IST187062"
#define CORCHEGADA_R 33     //cor das rotas de chegada
#define CORCHEGADA_G 48
#define CORCHEGADA_B 154
#define CORCHEGADA_A 0
#define CORPARTIDA_R 190       //cor das rotas de partida
#define CORPARTIDA_G 200
#define CORPARTIDA_B 6
#define CORPARTIDA_A 0


//definição de funções relacionadas com o modo gráfico
void modo_grafico(estacao_node *head_estacao, viagem_node **head_viagem);
void pedir_dias_semana(int* dias_semana);
void remover_nodes_viagens_modo_grafico(viagem_node **head_viagens, int* dias_semana);
int manter_viagem_modo_grafico(viagem _viagem, int* dias_semana);
void calcular_coordenadas(estacao_node* head_estacao, int **_posicao_x,int **_posicao_y);
estacao_node* tecla_premida(estacao_node* head_estacao, estacao_node* estacao_selecionada, SDL_Event event);
estacao_node* clique_rato(estacao_node* head_estacao, int *posicao_x, int *posicao_y);
int RenderLogo(SDL_Surface *logo, SDL_Renderer* _renderer, int pos_y);
void RenderMap( SDL_Surface *_boston_map, SDL_Renderer* _renderer );
void display_texto(estacao_node* estacao_selecionada, TTF_Font* font, SDL_Renderer *renderer);
void desenharEstacoes(estacao_node* head_estacao,estacao_node* estacao_escolhida, SDL_Renderer *renderer, int *_posicao_x,int *_posicao_y);
void desenharRotas(estacao_node *head_estacao,viagem_node *head_viagem,estacao_node *estacao_selecionada,
                   SDL_Renderer *renderer, int *posicao_x, int *posicao_y);
void desenhar_rotas_estacao_selec(estacao_node *head_estacao,viagem_node *head_viagem,estacao_node *estacao_selecionada,
                   SDL_Renderer *renderer, int *posicao_x, int *posicao_y);
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface **_boston_map, SDL_Surface **_logoIST, SDL_Surface **_logoBS, SDL_Window** _window, SDL_Renderer** _renderer);
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int width, int height);
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer);



#endif // MODO_GRAFICO_H
