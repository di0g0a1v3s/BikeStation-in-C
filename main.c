/*  24/05/2017
 *  Diogo Martins Alves nº 86980
 *  Manuel Reis nº 87062
 *  Projeto Final de Programação - IST - Ano Letivo 2016/17 - 2º Semestre
 *  BikeStation - Este programa dispõe de dois modo - gráfico e textual e requer que o utilizador introduza dois ficheiros .csv -
 *  um com informação sobre as estações e outro sobre as viagens. No modo textual é possível fazer uma seleção de dados,
 *  restringindo as viagens a um período de tempo, duração ou dia da semana e acerca das viagens selecionadas é possível
 *  visualizar tabelas sobre as viagens, estações, rotas entre estações e ainda diferente estatísticas em relação às viagens:
 *  de género, idade, duração e velocidade. Já no modo gráfico, o utilizador pode selecionar um ou mais dias da semana e
 *  visualizar num mapa de boston as diferentes rotas entre estações, selecionando a estação pretendida. Neste modo gráfico,
 *  a espessura da reta que identifica cada rota é tanto mais espessa quanto maior for o número de viagens nessa rota.
 */
#include "modo_textual.h"
#include "modo_grafico.h"
#include "viagem.h"
#include "estacao.h"

int main(int argc, char* argv[])
{
    //verifica se os parâmetros de inicio do programa são válidos
    verificarArgumentos(argc, argv);

    char* ficheiro_viagens = argv[2];
    char* ficheiro_estacoes = argv[3];

    viagem_node *head_viagens = NULL; //cabeça da lista de viagens
    estacao_node *head_estacao = NULL;  //cabeça da lista de estações

    //carrega as viagens do ficheiro para a lista(esta lista não se altera durante a execução do programa
    load_estacoes(ficheiro_estacoes, &head_estacao);

    //inicia o modo textual
    if(strcmp(argv[1], "-t") == 0)
    {
        modo_textual(ficheiro_viagens, head_estacao, &head_viagens);
    }
    //inicia  o modo gráfico
    else if(strcmp(argv[1], "-g") == 0)
    {
        load_viagens(ficheiro_viagens, &head_viagens, head_estacao);
        modo_grafico(head_estacao, &head_viagens);
    }

    //liberta a memória alocada pelas listas
    libertar_lista_viagens(&head_viagens);
    libertar_lista_estacoes(&head_estacao);

    return EXIT_SUCCESS;
}

