#include "modo_textual.h"

//função que verifica se os parâmetros iniciais do programa são válidos
void verificarArgumentos(int argc, char* argv[])
{
    //o programa requer 4 argumentos
    if(argc < 4)
    {
        printf("Argumentos insuficientes\n");
        exit(EXIT_FAILURE);

    }
    else if(argc > 4)
    {
        printf("Demasiados argumentos\n");
        exit(EXIT_FAILURE);
    }
    // o argumento 1 é o modo do programa: tem que ser "-g" ou "-t"
    if(strcmp(argv[1], "-g") != 0 && strcmp(argv[1], "-t") != 0)
    {
        printf("Modo inválido\n");
        exit(EXIT_FAILURE);
    }
    //os argumentos 2 e 3 correspondem aos ficheiros, que têm que estar no formato .csv
    if(strstr(argv[2], ".csv\0") == NULL || strstr(argv[3], ".csv\0") == NULL)
    {
        printf("Os ficheiros devem estar no formato .cvs\n");
        exit(EXIT_FAILURE);
    }
}

//mostra no ecrã o ecrã do modo textual e redireciona o programa para outras funções conforme a escolha do utilizador
void modo_textual(char* ficheiro_viagens, estacao_node *head_estacao, viagem_node **head_viagem)
{
    int menu = -1;
    //retorno da função sscanf
    int ret = 0;
    int todas_viagens = 0; //corresponde à escolha do utilizador à primeira pergunta da sel dados: 0-não escolheu, 1-carregar todas as viagens, 2-restringir as viagens
    int periodo_tempo_enable = 0;
    int dia_semana_enable = 0;
    int duracao_enable = 0;
    hora_data tempo_inicial;
    hora_data tempo_final;
    int dia_semana = 0;
    int duracao = 0;

    while(menu != 0)
    {
        limparJanela();
        printf("(1) Seleção de dados\n(2) Listagem de viagens\n(3) Listagem de estações\n(4) Listagem de rotas\n(5) Listagem de estatísticas\n(0) Sair do programa\n");
        do
        {   //antes de selecionar uma das listagens, o utilizador necessita de selecionar os dados
            ret = pedir_menu(&menu, 0 , 5);
            if(todas_viagens == 0 && (menu == 2 || menu == 3 || menu == 4 ||  menu == 5))
            {
                printf("É necessário selecionar primeiro os dados\n");
                ret = 0;
            }

        }while(ret != 1);

        limparJanela();
        //hipóteses de escolha do utilizador:
        if(menu == 1)
        {   //seleção de dados
            selecao_dados(ficheiro_viagens, head_viagem, head_estacao, &todas_viagens, &periodo_tempo_enable, &dia_semana_enable,
                          &duracao_enable, &tempo_inicial, &tempo_final, &dia_semana, &duracao);

        }
        else if(menu == 2)
        {   //listagem de viagens
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            listagem_viagens(*head_viagem);
        }
        else if(menu == 3)
        {      //listagem de estações
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            listagem_estacoes(*head_viagem, head_estacao, periodo_tempo_enable, tempo_inicial, tempo_final);
        }
        else if(menu == 4)
        {   //listagem de rotas
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            listagem_rotas(*head_viagem, head_estacao);
        }
        else if(menu == 5)
        {   //listagem de estatísticas
            listagem_estatisticas(*head_viagem, periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana,
                                duracao);
        }
    }
}

//função que lida com o menu seleção de dados
void selecao_dados(char* ficheiro_viagens, viagem_node **head_viagens, estacao_node *head_estacoes, int *todas_viagens, int *periodo_tempo_enable,
                   int *dia_semana_enable, int *duracao_enable, hora_data *tempo_inicial, hora_data *tempo_final, int *dia_semana, int *duracao)
{
    char resposta[MAX_STR] = {0};
    int ret = 0;
    int menu = -1;
    //inicialmente o programa começa por libertar a memória da lista de viagens para o caso de o utilizador ter feito uma
    //seleção anteriormente
    libertar_lista_viagens(head_viagens);
    //de seguida carrega toda a informação do ficheiro viagens para uma lista
    load_viagens(ficheiro_viagens, head_viagens, head_estacoes);

    //pergunta ao utilizador se quer utilizar todas as viagens ou filtrá-las
    printf("Deseja que as listagens/estatísticas a imprimir sejam sobre todas as viagens incluídas no ficheiro (1) ou apenas as que cumprem determinados critérios (2)? ");
    do
    {
        fgets(resposta, MAX_STR, stdin);
        ret = sscanf(resposta, "%d", todas_viagens); //converte para int e guarda na variável correta
        if(ret != 1 || *todas_viagens < 1 || *todas_viagens > 2)
        {
            printf("Resposta inválida\n");
            ret = 0;
        }

    }while(ret != 1);
    //se desejar filtrar as viagens:
    if(*todas_viagens == 2)
    {
        //enquanto não pedir para voltar atrás, é mostrado o menu seleção de dados
        while(menu != 0)
        {
            display_menus_selecao_dados(*periodo_tempo_enable, *tempo_inicial, *tempo_final, *dia_semana_enable, *dia_semana,
                                        *duracao_enable, *duracao, &menu);
            //período de tempo
            if(menu == 1)
                periodo_tempo_sel_dados(tempo_inicial, tempo_final, periodo_tempo_enable);
            //dia da semana
            else if(menu == 2)
                dia_semana_sel_dados(dia_semana, dia_semana_enable);
            //duração da viagem
            else if(menu == 3)
                duracao_sel_dados(duracao, duracao_enable);
            //apagar todos os critérios
            else if(menu == 4)
            {
                *periodo_tempo_enable = 0;
                *dia_semana_enable = 0;
                *duracao_enable = 0;
            }
        }
        //quando é feita a seleção, a lista de viagens é atualizada
        remover_nodes_viagens(head_viagens, tempo_inicial, tempo_final, duracao, dia_semana, duracao_enable, dia_semana_enable,
                              periodo_tempo_enable);
    }
    //caso o utilizador escolha todas as viagens ao início, o efeito é equivalente a apagar todos os critérios
    else if(*todas_viagens == 1)
    {
        *periodo_tempo_enable = 0;
        *dia_semana_enable = 0;
        *duracao_enable = 0;
    }
}

//função que imprime no ecrã os critérios do menu seleção de dados
void display_menus_selecao_dados(int periodo_tempo_enable, hora_data tempo_inicial, hora_data tempo_final, int dia_semana_enable,
                                int dia_semana, int duracao_enable, int duracao, int* criterio)
{
    char resposta[MAX_STR] = {0}; //string auxiliar
    int ret = 0; //retorno da função sscanf

    limparJanela();

    //se alguma das restrições tiver sido selecionada anteriormente, é impressa essa restrição à frente do respetivo critério
    if(periodo_tempo_enable == 0)
        printf("(1) Período de tempo");
    else
        printf("(1) Período de tempo: %.2d:%.2d:%.2d a %.2d:%.2d:%.2d", tempo_inicial.hora, tempo_inicial.minuto, tempo_inicial.segundo,
                                                                tempo_final.hora, tempo_final.minuto, tempo_final.segundo);
    if(dia_semana_enable == 0)
        printf("\n(2) Dia da semana");
    else
    {
        switch(dia_semana){
            case 1:
                printf("\n(2) Dia da semana: Domingo");
                break;
            case 2:
                printf("\n(2) Dia da semana: Segunda-Feira");
                break;
            case 3:
                printf("\n(2) Dia da semana: Terça-Feira");
                break;
            case 4:
                printf("\n(2) Dia da semana: Quarta-Feira");
                break;
            case 5:
                printf("\n(2) Dia da semana: Quinta-Feira");
                break;
            case 6:
                printf("\n(2) Dia da semana: Sexta-Feira");
                break;
            case 7:
                printf("\n(2) Dia da semana: Sábado");
                break;

            }
    }

    if(duracao_enable == 0)
        printf("\n(3) Duração");
    else
        printf("\n(3) Duração: %d segundos", duracao);

    printf("\n(4) Limpar todos os critérios\n(0) Voltar atrás\n\n");

    //ciclo que se certifica que o utilizador escolhe um critério e que esse critério é válido
    do
    {
        printf("Escolha um dos critérios acima: ");
        fgets(resposta, MAX_STR, stdin);
        ret = sscanf(resposta, "%d", criterio); //converte para int e guarda na variável correta
        if(ret != 1 || *criterio < 0 || *criterio > 4)
        {
            printf("Critério Inválido\n");
            ret = 0;
        }

    }while(ret != 1);
}

//critério de período de tempo no menu seleção de dados
void periodo_tempo_sel_dados(hora_data* tempo_inicial, hora_data* tempo_final, int *periodo_tempo_enable)
{
    char resposta[MAX_STR] = {0}; //string auxiliar
    int ret = 0;

    limparJanela();
    //ciclo que se certifica que o utilizador escolhe uma hora de inicio válida
    do
    {
        printf("Escolha a hora de inicio da viagem (hh:mm:ss): ");
        fgets(resposta, MAX_STR, stdin);
        ret = sscanf(resposta, "%d:%d:%d", &(tempo_inicial->hora), &(tempo_inicial->minuto), &(tempo_inicial->segundo));
        if(ret != 3 || tempo_inicial->hora < 0 || tempo_inicial->hora > 23 ||
                        tempo_inicial->minuto < 0 || tempo_inicial->minuto > 59 ||
                        tempo_inicial->segundo < 0 || tempo_inicial->segundo > 59)
        {
            printf("Hora inválida\n");
            ret = 0;
        }

    }while(ret != 3);
    //ciclo que se certifica que o utilizador escolhe uma hora de fim válida
    do
    {
        printf("Escolha a hora de fim da viagem (hh:mm:ss): ");
            fgets(resposta, MAX_STR, stdin);
            ret = sscanf(resposta, "%d:%d:%d", &(tempo_final->hora), &(tempo_final->minuto), &(tempo_final->segundo));
            if(ret != 3 || tempo_final->hora < 0 || tempo_final->hora > 23 ||
                            tempo_final->minuto < 0 || tempo_final->minuto > 59 ||
                            tempo_final->segundo < 0 || tempo_final->segundo > 59)
            {
                printf("Hora inválida\n");
                ret = 0;
            }
    }while(ret != 3);
    //os atributos dos tempos não utilizados são postos a zero
    tempo_final->ano = 0;
    tempo_final->mes = 0;
    tempo_final->dia = 0;
    tempo_inicial->ano = 0;
    tempo_inicial->mes = 0;
    tempo_inicial->dia = 0;
    //variável que indica que o utilizador fez uma escolha no período de tempo
    *periodo_tempo_enable = 1;
}

//critério de dia da semana no menu seleção de dados
void dia_semana_sel_dados(int *dia_semana, int *dia_semana_enable)
{
    char resposta[MAX_STR] = {0};
    int ret = 0;

    limparJanela();
    printf("(1) Domingo\n(2) Segunda-Feira\n(3) Terça-Feira\n(4) Quarta-Feira\n(5) Quinta-Feira\n(6) Sexta-Feira\n(7) Sábado\n");
    //ciclo que se certifica que o dia da semana escolhido pelo utilizador é válido
    do
    {
        printf("\nEscolha um dos dias da semana acima: ");
        fgets(resposta, MAX_STR, stdin);
        ret = sscanf(resposta, "%d", dia_semana);
        if(ret != 1 || *dia_semana < 1 || *dia_semana > 7)
        {
            printf("Resposta Inválida\n");
            ret = 0;
        }

    }while(ret != 1);
    //variável que indica que o utilizador fez uma escolha no dia da semana
    *dia_semana_enable = 1;
}

//critério de duração da viagem no menu seleção de dados
void duracao_sel_dados(int *duracao, int *duracao_enable)
{
    char resposta[MAX_STR] = {0};
    int ret = 0;

    limparJanela();
    //ciclo que se certifica que a resposta do utilizador é válida
    do
    {
        printf("Introduza a duração máxima das viagens (em segundos): ");
        fgets(resposta, MAX_STR, stdin);
        ret = sscanf(resposta, "%d", duracao);
        if(ret != 1 || *duracao < 0 )
        {
            printf("Resposta Inválida\n");
            ret = 0;
        }

    }while(ret != 1);
    //variável que indica que o utilizador fez uma escolha na duração
    *duracao_enable = 1;
}

//função que elimina viagens que não passem na filtragem feita pelo utilizador na seleção de dados
void remover_nodes_viagens(viagem_node **head_viagens, hora_data* tempo_inicial, hora_data* tempo_final, int* duracao,
                           int *dia_semana, int *duracao_enable,int* dia_semana_enable, int *periodo_tempo_enable)
{
    viagem_node *current = NULL;
    viagem_node *aux = NULL;
    //se a lista de viagens estiver não vazia e a head atual for para eliminar, a head avança um elemento e a head original
    //é eliminada
    while(*head_viagens != NULL && manter_viagem((*head_viagens)->viagem_info,*tempo_inicial,*tempo_final,*duracao,
                                                *dia_semana,*duracao_enable, *dia_semana_enable, *periodo_tempo_enable) == 0)
    {
        aux = *head_viagens;
        *head_viagens = (*head_viagens)->next;
        free(aux);
    }

    current = *head_viagens;
    //enquanto o final da lista não for atingido
    while(current != NULL)
    {
        //enquanto o nó seguinte for válido e for para eliminar, a current avança um elemento e o elemento
        //anterior é eliminado
        while(current->next != NULL && manter_viagem((current->next)->viagem_info,*tempo_inicial,*tempo_final,*duracao,
                                                *dia_semana,*duracao_enable, *dia_semana_enable, *periodo_tempo_enable) == 0 )
        {
            aux = current->next;
            current->next = aux->next;
            free(aux);
        }
        current = current->next;
    }
}

//função que retorna 1 se a viagem passada for para manter ou 0 se for para eliminar
int manter_viagem(viagem _viagem, hora_data tempo_inicial, hora_data tempo_final, int duracao, int dia_da_semana, int duracao_enable,
              int dia_semana_enable, int periodo_tempo_enable)
{
    //a viagem em questão é para eliminar se:
    //verificação do período de tempo
    if(periodo_tempo_enable == 1)
    {
        if(verificar_periodo_tempo(_viagem, tempo_inicial, tempo_final) == 0)
            return 0;
    }
    //a sua duração for menor que a duração escolhida pelo utilizador
    if(duracao < _viagem.duracao && duracao_enable == 1)
        return 0;
    //o seu dia da semana for diferente do dia da semana escolhido pelo utilizador
    if( dia_semana(_viagem.data_inicio.dia,_viagem.data_inicio.ano,_viagem.data_inicio.mes ) != dia_da_semana && dia_semana_enable == 1)
        return 0;
    //caso contrário, a viagem mantém-se na lista
    return 1;
}

//esta função verifica o critério período de tempo para decidir se uma viagem deve ser ou não mantida
int verificar_periodo_tempo(viagem _viagem, hora_data tempo_inicial, hora_data tempo_final)
{
    //se a hora inicial for menor que a final, assume-se que o utilizador pretende aceder a viagens que ocorreram
    //no espaço de um dia
    if(cmp_datas(tempo_inicial, tempo_final) <= 0)
    {
        tempo_inicial.ano = _viagem.data_inicio.ano;
        tempo_inicial.mes = _viagem.data_inicio.mes;
        tempo_inicial.dia = _viagem.data_inicio.dia;
        tempo_final.ano = _viagem.data_inicio.ano;
        tempo_final.mes = _viagem.data_inicio.mes;
        tempo_final.dia = _viagem.data_inicio.dia;
        //neste caso, a viagem é para eliminar se:
        //a sua hora de inicio for anterior à hora inicial escolhida pelo utilizador
        if(cmp_datas(_viagem.data_inicio, tempo_inicial) < 0 )
            return 0;
        //a sua hora de fim for posterior à hora final escolhida pelo utilizador
        if(cmp_datas(tempo_final, _viagem.data_fim) < 0 )
            return 0;
    }
    //caso contrário, as viagens podem ter ocorrido no espaço de dois dias
    else
    {
        tempo_inicial.ano = _viagem.data_inicio.ano;
        tempo_inicial.mes = _viagem.data_inicio.mes;
        tempo_inicial.dia = _viagem.data_inicio.dia;
        tempo_final.ano = _viagem.data_fim.ano;
        tempo_final.mes = _viagem.data_fim.mes;
        tempo_final.dia = _viagem.data_fim.dia;
        //devido às atribuições acima, quando o tempo final é inferior ao tempo inicial, significa que a
        //viagem em questão ocorreu no espaço de um dia:
        if(cmp_datas(tempo_final, tempo_inicial) < 0)
        {
            //neste caso a viagem é para manter se o tempo de inicio da viagem for superior ao tempo inicial
            //ou se o tempo de fim da viagem for inferior ao tempo final, ou seja, quando o complemento se verifica,
            //a viagem é para eliminar
            if(!(cmp_datas(_viagem.data_inicio, tempo_inicial) >= 0 || cmp_datas(_viagem.data_fim, tempo_final) <= 0))
                return 0;
        }
        //caso a viagem se realize dentro de um espaço superior a um dia, é para eliminar se:
        else
        {
            //a sua hora de inicio for anterior à hora inicial escolhida pelo utilizador
            if(cmp_datas(_viagem.data_inicio, tempo_inicial) < 0 )
                return 0;
            //a sua hora de fim for posterior à hora final escolhida pelo utilizador
            if(cmp_datas(tempo_final, _viagem.data_fim) < 0 )
                return 0;
        }
    }
    return 1;

}

//retorna o dia da semana (1 - Domingo, 2 - Segunda, ..., 7-Sábado) da data passada
int dia_semana(int dia, int mes, int ano)
{
    //algoritmo para determinar o dia da semana
    return (dia+=mes<3?ano--:ano-2,23*mes/9+dia+4+ano/4-ano/100+ano/400)%7 + 1;
}

//imprime no ecrã as viagens da secção listagem de viagens
void listagem_viagens(viagem_node *head_viagens)
{
    int numero_viagens_mostrar = 0;
    int i = 0;
    int ret = 0;
    int num_paginas = 0;
    char buffer[MAX_STR];
    viagem_node* current = head_viagens;
    //ciclo que se certifica que o numero de viagens a mostrar introduzido pelo utilizador é válido
    do
    {
        printf("Introduza o número de viagens a mostrar: ");
        fgets(buffer, MAX_STR, stdin);
        ret = sscanf(buffer, "%d", &numero_viagens_mostrar);
        if(ret != 1 || numero_viagens_mostrar < 0)
        {
            printf("Resposta inválida\n");
            ret = 0;
        }

    }while(ret != 1);
    //enquanto houver viagens na lista, o programa irá imprimi-las por páginas:
    do
    {
        limparJanela();
        printf("\nPágina %d:\n\n", num_paginas + 1);
        printf("\n|  ID Viagem  | Duração |    Data/Hora Início    | ID Estação Inicial |    Data/Hora Fim    | ID Estação Final | ID Bicicleta | Tipo Usuário | Nascimento |  Género  |");
        printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        num_paginas++;
        //impressão de uma página
        if(current != NULL)
        {
            for(i = 0; i < numero_viagens_mostrar; i++)
            {
                imprimir_viagem(current->viagem_info);
                current = current->next;
                if(current == NULL)
                    break;
            }
        }
        //pergunta ao utilizador se deseja mudar de página ou sair
        if(current != NULL)
        {
            if(avancar_pagina() == 0)
                break;
        }
        //quando não há mais viagens a imprimir, é pedido ao utilizador para voltar atrás
        else
        {
            esperar_resposta();
            break;
        }

    }while(current != NULL);
}

//imprime uma viagem em tabela, dentro do menu listagem de viagens
void imprimir_viagem(viagem _viagem)
{
    char buffer[MAX_STR] = {0}; //variável string auxiliar
    //Id da viagem:
    //snprintf converte a variável em questão para uma string, que é guardada no buffer
    snprintf(buffer, MAX_STR, "%d", _viagem.id_viagem);
    //o numero de espaços com que a string irá ser impressa é determinado pelo tamanho do titulo da coluna
    imprimir_em_tabela(buffer, strlen("  ID Viagem  "));
    //Duração:
    snprintf(buffer, MAX_STR, "%d", _viagem.duracao);
    imprimir_em_tabela(buffer, strlen(" Duracao "));
    //Data/Hora de inicio:
    snprintf(buffer, MAX_STR, "%d/%d/%d %.2d:%.2d:%.2d", _viagem.data_inicio.mes,
            _viagem.data_inicio.dia,  _viagem.data_inicio.ano,  _viagem.data_inicio.hora,
            _viagem.data_inicio.minuto, _viagem.data_inicio.segundo);
    imprimir_em_tabela(buffer, strlen("    Data/Hora Inicio    "));
    //ID da estação inicial:
    snprintf(buffer, MAX_STR, "%d", _viagem.id_inicial);
    imprimir_em_tabela(buffer, strlen(" ID Estacao Inicial "));
    //Data e hora de fim:
    snprintf(buffer, MAX_STR, "%d/%d/%d %.2d:%.2d:%.2d", _viagem.data_fim.mes,
            _viagem.data_fim.dia,  _viagem.data_fim.ano,  _viagem.data_fim.hora,
            _viagem.data_fim.minuto, _viagem.data_fim.segundo);
    imprimir_em_tabela(buffer, strlen("    Data/Hora Fim    "));
    //ID da estação final:
    snprintf(buffer, MAX_STR, "%d", _viagem.id_final);
    imprimir_em_tabela(buffer, strlen(" ID Estacao Final "));
    //ID da bicicleta:
    imprimir_em_tabela(_viagem.bicicleta, strlen(" ID Bicicleta "));
    //Tipo de usuário:
    if(_viagem.tipo_usuario == 'R')
    {
        imprimir_em_tabela("Registado", strlen(" Tipo Usuario "));
        //Ano de Nascimento:
        snprintf(buffer, MAX_STR, "%d", _viagem.ano_nascimento);
        imprimir_em_tabela(buffer, strlen(" Nascimento "));
        //Género:
        if(_viagem.genero == 'M')
            imprimir_em_tabela("Masculino", strlen("  Genero  "));
        else if(_viagem.genero == 'F')
            imprimir_em_tabela("Feminino", strlen("  Genero  "));

    }
    //caso o usuário seja casual, não tem data de nascimento nem género
    else if(_viagem.tipo_usuario == 'C')
    {
        imprimir_em_tabela("Casual", strlen(" Tipo Usuario "));

        imprimir_em_tabela("--", strlen(" Nascimento "));

        imprimir_em_tabela("--", strlen("  Genero  "));
    }
    printf("|\n");
}

//lida com o menu listagem de estações do modo textual
void listagem_estacoes(viagem_node *head_viagens, estacao_node *head_estacoes, int periodo_tempo_enable, hora_data tempo_inicial,
                       hora_data tempo_final)
{
    float num_horas_efetivo = 0; //numero real de horas entre o tempo_inicial e tempo_final
    int num_horas = 0; //quantidade de horas do dia durante as quais a viagem decorreu
    estacao_node *current_estacao = NULL;
    char buffer[MAX_STR] = {0};
    //caso o utilizador tenha selecionado o tempo, o num_horas_efetivo e num_horas tomam os valores determinados pela função contagem_horas
    if(periodo_tempo_enable == 1)
        contagem_horas(tempo_inicial, tempo_final, &num_horas_efetivo, &num_horas);
    //caso contrário tomam o valor 24
    else
    {
        num_horas = 24;
        num_horas_efetivo = 24.0f;
    }
    //impressão do cabeçalho
    printf("Listagem de estações:\n");
    printf("                                             | Bicicletas a chegar por hora  |  Bicicletas a sair por hora   |\n");
    printf("| ID Estação |  Nome  | Latitude | Longitude |  Mínimo  |  Máximo  |  Média  |  Mínimo  |  Máximo  |  Média  |\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");

    current_estacao = head_estacoes;
    while(current_estacao != NULL)
    {   //impressão das 4 primeiras colunas de cada linha:
        snprintf(buffer, MAX_STR, "%d", current_estacao->estacao_info.id_estacao);
        imprimir_em_tabela(buffer, strlen(" ID Estacao "));

        imprimir_em_tabela(current_estacao->estacao_info.nome_curto, strlen("  Nome  "));

        snprintf(buffer, MAX_STR, "%f", current_estacao->estacao_info.latitude);
        imprimir_em_tabela(buffer, strlen(" Latitude "));

        snprintf(buffer, MAX_STR, "%f", current_estacao->estacao_info.longitude);
        imprimir_em_tabela(buffer, strlen(" Longitude "));

        //calculo e impressão dos valores de chegada
        bicicletas_chegar_hora(num_horas, current_estacao, head_viagens, tempo_inicial, num_horas_efetivo);

        //calculo e impressão dos valores de partida
        bicicletas_partir_hora(num_horas, current_estacao, head_viagens, tempo_inicial, num_horas_efetivo);

        printf("|\n");
        current_estacao = current_estacao->next;
    }

    esperar_resposta();
}

//pede ao utilizador para selecionar uma estação através do id e verifica se essa estação é válida e existe
estacao_node* pedir_estacao(estacao_node *head_estacoes)
{
    char buffer[MAX_STR] = {0};
    int id_estacao = 0;
    int ret = 0;
    estacao_node* estacao_escolhida = NULL;
    estacao_node* current_estacao = NULL;
    //verifica se a estação introduzida é válida
    do
    {
        printf("Introduza o ID da estação a apresentar: ");
        fgets(buffer, MAX_STR, stdin);
        ret = sscanf(buffer, "%d", &id_estacao);
        if(ret != 1 || id_estacao < 0)
        {
            printf("Estação inválida\n");
            ret = 0;
        }
        //verifica se a estação existe na lista de estações
        else
        {
            current_estacao = head_estacoes;
            ret = 0;
            while (current_estacao != NULL)
            {
                if(current_estacao->estacao_info.id_estacao == id_estacao)
                {
                    ret = 1;
                    estacao_escolhida = current_estacao;
                    break;
                }
                current_estacao = current_estacao->next;
            }
            if (ret != 1)
                printf("Estação inexistente\n");
        }

    }while(ret != 1);
    //retorna um ponteiro para o nó que contém a estação escolhida
    return estacao_escolhida;
}

//num_horas efetivo é alterado para a diferença (em horas) entre o tempo_inicial e tempo_final introduzidos pelo
//utilizador e o num_horas para a quantidade de horas do dia durante as quais a viagem decorreu
void contagem_horas(hora_data tempo_inicial, hora_data tempo_final, float* num_horas_efetivo, int* num_horas)
{
    //se a hora inicial for menor que a final, assume-se que o utilizador pretende aceder a viagens que ocorreram
    //no espaço de um dia
    if(cmp_datas(tempo_inicial, tempo_final) <= 0)
    {
        *num_horas_efetivo = (tempo_final.hora - tempo_inicial.hora) + (float)(tempo_final.minuto - tempo_inicial.minuto)/60 + (float)(tempo_final.segundo - tempo_inicial.segundo)/(60*60);
        *num_horas = tempo_final.hora - tempo_inicial.hora + 1;
    }
    //caso contrário, assume-se que as viagens ocorreram no espaço de dois dias
    else
    {
        *num_horas_efetivo = (tempo_final.hora) + (float)(tempo_final.minuto)/60 + (float)(tempo_final.segundo)/(60*60)
                +(23 - tempo_inicial.hora) + (float)(59 - tempo_inicial.minuto)/60 + (float)(59 - tempo_inicial.segundo)/(60*60);
        *num_horas = 24 - tempo_inicial.hora + tempo_final.hora + 1;
    }


}

//calcula os valores mínimo, máximo e médio de bicicletas a chegar a uma estação, por hora
void bicicletas_chegar_hora(int num_horas, estacao_node* current_estacao, viagem_node* head_viagens, hora_data tempo_inicial, float num_horas_efetivo)
{
    char buffer[MAX_STR] = {0};
    int min_bicicletas_hora = -1;
    int max_bicicletas_hora = 0;
    float med_bicicletas_hora = 0;
    int i = 0;
    viagem_node* current_viagem = NULL;
    int hora_a_testar = 0;
    //vetor que guarda o número de bicicletas a chegar em cada hora
    int *num_bicicletas_hora = (int*)calloc(num_horas, sizeof(int));
    verificar_alocacao_memoria((void*)num_bicicletas_hora);
    //ciclo que preenche o vetor
    for(i = 0; i < num_horas; i++)
    {   //a hora_a_testar corresponde à hora do dia sobre a qual iremos analisar o número de bicicletas a chegar
        hora_a_testar = (i + tempo_inicial.hora)%24;
        num_bicicletas_hora[i] = 0;
        current_viagem = head_viagens;
        //determinação do valor de bicicletas a chegar em cada hora
        while(current_viagem != NULL)
        {
            if(current_viagem->viagem_info.id_final == current_estacao->estacao_info.id_estacao &&
               current_viagem->viagem_info.data_fim.hora == hora_a_testar)
            {
                num_bicicletas_hora[i]++;
            }

            current_viagem = current_viagem->next;
        }
    }

    med_bicicletas_hora = 0;
    max_bicicletas_hora = 0;
    min_bicicletas_hora = num_bicicletas_hora[0];
    //calculo dos valores mínimo, máximo e médio:
    for(i = 0; i  < num_horas; i++)
    {
        med_bicicletas_hora += num_bicicletas_hora[i];

        if(min_bicicletas_hora > num_bicicletas_hora[i])
            min_bicicletas_hora = num_bicicletas_hora[i];
        if(max_bicicletas_hora < num_bicicletas_hora[i])
            max_bicicletas_hora = num_bicicletas_hora[i];
    }
    med_bicicletas_hora = med_bicicletas_hora/(float)num_horas_efetivo;

    //impressão:
    snprintf(buffer, MAX_STR, "%d", min_bicicletas_hora);
    imprimir_em_tabela(buffer, strlen("  Minimo  "));

    snprintf(buffer, MAX_STR, "%d", max_bicicletas_hora);
    imprimir_em_tabela(buffer, strlen("  Maximo  "));

    snprintf(buffer, MAX_STR, "%.2f", med_bicicletas_hora);
    imprimir_em_tabela(buffer, strlen("  Media  "));
    //libertação do vetor alocado anteriormente
    free(num_bicicletas_hora);
}

//calcula os valores mínimo, máximo e médio de bicicletas a partir de uma estação, por hora
void bicicletas_partir_hora(int num_horas, estacao_node* current_estacao, viagem_node* head_viagens, hora_data tempo_inicial, float num_horas_efetivo)
{
    char buffer[MAX_STR] = {0};
    int min_bicicletas_hora = -1;
    int max_bicicletas_hora = 0;
    float med_bicicletas_hora = 0;
    int i = 0;
    viagem_node* current_viagem = NULL;
    int hora_a_testar = 0;
    //vetor que guarda o número de bicicletas a chegar em cada hora
    int* num_bicicletas_hora = (int*)calloc(num_horas, sizeof(int));
    verificar_alocacao_memoria((void*)num_bicicletas_hora);
    //ciclo que preenche o vetor
    for(i = 0; i < num_horas; i++)
    {
        //a hora_a_testar corresponde à hora do dia sobre a qual iremos analisar o número de bicicletas a sair
        hora_a_testar = (i + tempo_inicial.hora)%24;
        num_bicicletas_hora[i] = 0;
        current_viagem = head_viagens;
        //determinação do valor de bicicletas a sair em cada hora
        while(current_viagem != NULL)
        {
            if(current_viagem->viagem_info.id_inicial == current_estacao->estacao_info.id_estacao &&
               current_viagem->viagem_info.data_inicio.hora == hora_a_testar)
            {
                num_bicicletas_hora[i]++;
            }

            current_viagem = current_viagem->next;
        }
    }

    med_bicicletas_hora = 0;
    max_bicicletas_hora = 0;
    min_bicicletas_hora = num_bicicletas_hora[0];
    //calculo dos valores minimo, máximo e médio:
    for(i = 0; i  < num_horas; i++)
    {
        med_bicicletas_hora += num_bicicletas_hora[i];

        if(min_bicicletas_hora > num_bicicletas_hora[i])
            min_bicicletas_hora = num_bicicletas_hora[i];
        if(max_bicicletas_hora < num_bicicletas_hora[i])
            max_bicicletas_hora = num_bicicletas_hora[i];
    }

    med_bicicletas_hora = med_bicicletas_hora/(float)num_horas_efetivo;
    //impressão:
    snprintf(buffer, MAX_STR, "%d", min_bicicletas_hora);
    imprimir_em_tabela(buffer, strlen("  Minimo  "));

    snprintf(buffer, MAX_STR, "%d", max_bicicletas_hora);
    imprimir_em_tabela(buffer, strlen("  Maximo  "));

    snprintf(buffer, MAX_STR, "%.2f", med_bicicletas_hora);
    imprimir_em_tabela(buffer, strlen("  Media  "));
    //libertação do vetor alocado anteriormente
    free(num_bicicletas_hora);

}

//lida com o menu listagem de rotas do modo textual
void listagem_rotas(viagem_node *head_viagens, estacao_node *head_estacoes)
{
    int rotas_a_imprimir = 0;
    char buffer[MAX_STR] = {0};
    estacao_node *estacao_escolhida = NULL;
    int *num_viagens_partida = NULL;
    int *num_viagens_chegada = NULL;
    int num_impressoes = 0;
    int ret = 0;
    int contagem_estacoes = 0;
    int impressao_terminada = 0;
    int num_paginas = 0;
    int k = 0;
    //pede ao utilizador para selecionar uma estação
    estacao_escolhida = pedir_estacao(head_estacoes);
    //pergunta o número de rotas a imprimir por página
    do
    {
        printf("Introduza o número de rotas a imprimir: ");
        fgets(buffer, MAX_STR, stdin);
        ret = sscanf(buffer, "%d", &rotas_a_imprimir);
        if(ret != 1 || rotas_a_imprimir < 0)
        {
            printf("Valor inválido\n");
            ret = 0;
        }

    }while(ret != 1);

    //faz a contagem das rotas de todas as estações para a estação selecionada e guarda essa contagem nos vetores num_viagens_chegada
    //e num_viagens_partida
    contagem_viagens_rotas(&contagem_estacoes, head_estacoes, estacao_escolhida, head_viagens, &num_viagens_chegada,
                                &num_viagens_partida );

    //faz a impressão das rotas
    while(impressao_terminada == 0)
    {
        limparJanela();
        //imprime o cabeçalho
        printf("Estação escolhida: %d (%s)\n\n", estacao_escolhida->estacao_info.id_estacao, estacao_escolhida->estacao_info.nome_curto);
        printf("\nPágina %d:\n\n", num_paginas + 1);
        printf("|               Rota               | Número de viagens |\n");
        printf("--------------------------------------------------------\n");
        num_paginas++;
        for(k = 0; k < rotas_a_imprimir; k++)
        {
            //imprime a rota mais frequentada e mete a posição correspondente do vetor a zero
            impressao_terminada = imprimir_rota(&num_impressoes, num_viagens_chegada, num_viagens_partida, contagem_estacoes,
                                                head_estacoes, estacao_escolhida);
            if(impressao_terminada == 1)
                break;
        }
        //se a impressão não estiver terminada, pede ao utilizador para mudar de página/voltar atrás
        if(impressao_terminada == 0)
        {
            if(avancar_pagina() == 0)
                break;
        }
        //quando a impressão terminar, pede ao utilizar para voltar atrás
        else
        {
            esperar_resposta();
            break;
        }
    }

    //liberta os vetores alocados anteriormente
    if(num_viagens_chegada != NULL)
        free(num_viagens_chegada);

    if(num_viagens_partida != NULL)
        free(num_viagens_partida);

}

//faz a contagem das rotas de todas as estações para a estação selecionada e guarda essa contagem nos vetores num_viagens_chegada e num_viagens_partida
void contagem_viagens_rotas(int *contagem_estacoes, estacao_node* head_estacoes, estacao_node* estacao_escolhida,
                            viagem_node *head_viagens, int **num_viagens_chegada, int **num_viagens_partida )
{
    estacao_node *current_estacao = NULL;
    viagem_node *current_viagem = NULL;
    *contagem_estacoes = 0;
    current_estacao = head_estacoes;
    //rotas de chegada:
    //para cada estação na lista, aloca uma entrada no vetor num_viagens_chegada para guardar o número de viagens dessa rota
    while(current_estacao != NULL)
    {
        *num_viagens_chegada = (int*)realloc(*num_viagens_chegada, (*contagem_estacoes+1)*sizeof(int));
        verificar_alocacao_memoria((void*)num_viagens_chegada);
        (*num_viagens_chegada)[*contagem_estacoes] = 0;
        current_viagem = head_viagens;
        //percorre todo o ficheiro de viagens para verificar quais as viagens que pertencem a essa rota
        while(current_viagem != NULL)
        {
            if(current_viagem->viagem_info.id_final == estacao_escolhida->estacao_info.id_estacao &&
               current_viagem->viagem_info.id_inicial == current_estacao->estacao_info.id_estacao)
            {
                //incrementa a posição do vetor com o índice da estação de partida se uma viagem pertencer a essa rota
                ((*num_viagens_chegada)[*contagem_estacoes])++;
            }
            current_viagem = current_viagem->next;
        }
        current_estacao = current_estacao->next;
        (*contagem_estacoes)++;
    }

    *contagem_estacoes = 0;
    current_estacao = head_estacoes;
    //rotas de partida:
    //para cada estação na lista, aloca uma entrada no vetor num_viagens_partida para guardar o número de viagens dessa rota
    while(current_estacao != NULL)
    {
        *num_viagens_partida = (int*)realloc(*num_viagens_partida, (*contagem_estacoes+1)*sizeof(int));
        verificar_alocacao_memoria((void*)(*num_viagens_partida));
        (*num_viagens_partida)[*contagem_estacoes] = 0;
        current_viagem = head_viagens;
        //percorre todo o ficheiro de viagens para verificar quais as viagens que pertencem a essa rota
        while(current_viagem != NULL)
        {
            if(current_viagem->viagem_info.id_inicial == estacao_escolhida->estacao_info.id_estacao &&
               current_viagem->viagem_info.id_final == current_estacao->estacao_info.id_estacao)
            {
                //incrementa a posição do vetor com o índice da estação de destino se uma viagem pertencer a essa rota
                ((*num_viagens_partida)[*contagem_estacoes])++;
            }
            current_viagem = current_viagem->next;
        }
        current_estacao = current_estacao->next;
        (*contagem_estacoes)++;
    }
}

//imprime a rota mais frequentada com base na informação dos vetores
int imprimir_rota(int *num_impressoes, int *num_viagens_chegada,int *num_viagens_partida, int contagem_estacoes,
                  estacao_node* head_estacoes, estacao_node* estacao_escolhida)
{
    int j = 0;
    char buffer[MAX_STR] = {0};
    int maior_partida = 0;
    int maior_chegada = 0;
    int estacao_maior_chegada = 0;
    int estacao_maior_partida = 0;
    estacao_node* current_estacao = NULL;
    int impressao_terminada = 0;
    //calcula o maior numero de viagens numa rota de partida e de chegada e o índice da respetiva estação de destino/origem
    for(j = 0; j < contagem_estacoes; j++)
    {
        if(maior_chegada < num_viagens_chegada[j])
        {
            maior_chegada = num_viagens_chegada[j];
            estacao_maior_chegada = j;
        }

        if(maior_partida < num_viagens_partida[j])
        {
            maior_partida = num_viagens_partida[j];
            estacao_maior_partida = j;
        }
    }
    //entre a maior chegada e maior partida, imprime a maior, desde que pelo menos uma delas seja diferente de zero
    if(maior_chegada > maior_partida && (maior_chegada != 0 || maior_partida != 0))
    {
        current_estacao = head_estacoes;
        for(j = 0; j < estacao_maior_chegada; j++)
        {
            //current_estacao fica a apontar para a "estação de maior chegada"
            current_estacao = current_estacao->next;
        }
        //imprime a rota
        snprintf(buffer, MAX_STR, "%d(%s) --> %d(%s)", current_estacao->estacao_info.id_estacao, current_estacao->estacao_info.nome_curto,
                                                        estacao_escolhida->estacao_info.id_estacao, estacao_escolhida->estacao_info.nome_curto);
        imprimir_em_tabela(buffer, strlen("               Rota               "));

        snprintf(buffer, MAX_STR, "%d", maior_chegada);
        imprimir_em_tabela(buffer, strlen(" Numero de viagens "));
        printf("|\n");

        (*num_impressoes)++;
        //mete a entrada do vetor a zero para que essa rota não seja mais contabilizada
        num_viagens_chegada[estacao_maior_chegada] = 0;
        maior_chegada = 0;
    }
    else if(maior_chegada <= maior_partida && (maior_chegada != 0 || maior_partida != 0))
    {
        current_estacao = head_estacoes;
        for(j = 0; j < estacao_maior_partida; j++)

        {   //current_estacao fica a apontar para a "estação de maior partida"
            current_estacao = current_estacao->next;
        }
        //imprime a rota
        snprintf(buffer, MAX_STR, "%d(%s) --> %d(%s)", estacao_escolhida->estacao_info.id_estacao, estacao_escolhida->estacao_info.nome_curto,
                                                        current_estacao->estacao_info.id_estacao, current_estacao->estacao_info.nome_curto);
        imprimir_em_tabela(buffer, strlen("               Rota               "));

        snprintf(buffer, MAX_STR, "%d", maior_partida);
        imprimir_em_tabela(buffer, strlen(" Numero de viagens "));
        printf("|\n");


        (*num_impressoes)++;
        //mete a entrada do vetor a zero para que essa rota não seja mais contabilizada
        num_viagens_partida[estacao_maior_partida] = 0;
        maior_partida = 0;
    }
    else
    {
        impressao_terminada = 1;
    }
    return impressao_terminada;
}

//mostra no ecrã o menu de listagem de estatísticas e redireciona o programa para outras funções conforme a escolha do utilizador
void listagem_estatisticas(viagem_node *head_viagem, int periodo_tempo_enable, int dia_semana_enable, int duracao_enable, hora_data tempo_inicial,
                         hora_data tempo_final, int dia_semana, int duracao)
{
    int menu = -1;
    //retorno da função sscanf
    int ret = 0;

    while(menu != 0)
    {
        limparJanela();
        //pergunta ao utilizador o menu que pretende selecionar
        printf("(1) Estatísticas por género\n(2) Estatísticas da idade\n(3) Estatísticas da duração\n(4) Estatísticas da velocidade\n(0) Voltar atrás\n");
        do
        {
            ret = pedir_menu(&menu, 0 , 4);

        }while(ret != 1);

        limparJanela();
        //hipóteses de escolha do utilizador:
        if(menu == 1)
        {   //estatísticas de género
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            estatisticas_genero(head_viagem);
        }
        else if(menu == 2)
        {   //estatísticas de idade
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            estatisticas_idade(head_viagem);
        }
        else if(menu == 3)
        {   //estatísticas de duração
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            estatisticas_duracao(head_viagem);
        }
        else if(menu == 4)
        {   //estatísticas de velocidade
            imprimir_restricoes(periodo_tempo_enable, dia_semana_enable, duracao_enable, tempo_inicial, tempo_final, dia_semana, duracao);
            estatisticas_velocidade(head_viagem);
        }
    }
}

//lida com o sub-menu de estatísticas de género do menu listagem de estatísticas
void estatisticas_genero(viagem_node *head_viagens)
{
    viagem_node *current = head_viagens;
    int num_male = 0;
    int num_female = 0;
    int num_registered = 0;
    //percorre toda a lista de viagens e calcula o número de registados, de homens e de mulheres
    while(current != NULL)
    {
        if(current->viagem_info.tipo_usuario == 'R')
        {
            num_registered++;
            if(current->viagem_info.genero == 'F')
                num_female++;
            else if(current->viagem_info.genero == 'M')
                num_male++;
        }
        current = current->next;
    }
    //calcula e imprime as percentagens de cada um
    if(num_registered != 0)
    {
        printf("\nPercentagem de viagens efetuadas por utilizadores do género masculino: %.2f %%", num_male/(float)num_registered * 100);
        printf("\nPercentagem de viagens efetuadas por utilizadores do género feminino: %.2f %%\n", num_female/(float)num_registered * 100);
    }
    else
        printf("\nNão existem viagens realizadas por utilizadores registados\n");

    esperar_resposta();
}

//lida com o sub-menu de estatísticas de idade do menu listagem de estatísticas
void estatisticas_idade(viagem_node *head_viagens)
{
    int idade_max = 0;
    viagem_node *current = NULL;
    int i = 0;
    int num_pessoas = 0;
    int num_registados = 0;
    float percentagem = 0;
    char buffer[MAX_STR] = {0};
    printf("\n|     Idades     | Percentagens |");
    printf("\n---------------------------------\n");

    //percorre toda a lista para calcular a idade máxima e o numero de viagens de utilizadores registados
    current = head_viagens;
    while(current != NULL)
    {
        if(current->viagem_info.tipo_usuario == 'R')
        {
            num_registados++;
            if(idade_max < current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento)
                idade_max = current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento;
        }
        current = current->next;
    }
    //regista o número de pessoas com idades i ou i+1, em que i varia entre 0 e a idade máxima
    for(i = 0; i<= idade_max; i= i+2)
    {
        current = head_viagens;
        num_pessoas = 0;
        while(current != NULL)
        {
            if(current->viagem_info.tipo_usuario == 'R' && (i == current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento ||
                                                i+ 1 == current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento))
            {
                num_pessoas++;
            }
            current = current->next;
        }
        percentagem = num_pessoas/(float)num_registados *100;
        //imprime a percentagem de viagens de cada faixa etária (ignorando se for menor que 0.01%)
        if(percentagem >= 0.01)
        {
            snprintf(buffer, MAX_STR, "%d - %d", i, i+ 1);
            imprimir_em_tabela(buffer, strlen("     Idades     "));

            snprintf(buffer, MAX_STR, "%.2f%%", percentagem);
            imprimir_em_tabela(buffer, strlen(" Percentagens "));
            printf("|\n");
        }

    }
    esperar_resposta();
}

//lida com o sub-menu de estatísticas de duração do menu listagem de estatísticas
void estatisticas_duracao(viagem_node *head_viagens)
{
    int duracao_maxima = 6*60*60;//duração máxima: 6 horas
    int intervalo_tempo = 15*60;//intervalos de 15 minutos
    viagem_node *current = head_viagens;
    int i = 0;
    int num_viagens = 0;
    int num_viagens_total = 0;
    float percentagem = 0;
    char buffer[MAX_STR] = {0};

    printf("\n|      Duração      | Percentagem de viagens |");
    printf("\n----------------------------------------------\n");
    //calcula o número total de viagens
    while(current != NULL)
    {
        num_viagens_total++;
        current = current->next;
    }
    //percorre novamente a lista, desta vez para calcular o número de viagens cuja duração está entre i e i+intervalo de tempo
    for(i = 0; i < duracao_maxima; i = i + intervalo_tempo)
    {
        current = head_viagens;
        num_viagens = 0;
        while(current != NULL)
        {
            if(current->viagem_info.duracao >= i && current->viagem_info.duracao < i+intervalo_tempo)
            {
                num_viagens++;
            }
            current = current->next;
        }
        //calcula a percentagem de viagens efetuadas pelos utilizadores para cada intervalo de duração e imprime-a (se for < 0.01% é desprezada)
        percentagem = num_viagens/(float)num_viagens_total *100;
        if(percentagem >= 0.01)
        {
            snprintf(buffer, MAX_STR, "%.2dh%.2dm - %.2dh%.2dm", (i)/(60*60),((i)/(60))%60, (i+intervalo_tempo)/(60*60),
                                                                ((i+intervalo_tempo)/(60))%60); //conversão de segundos para horas e minutos
            imprimir_em_tabela(buffer, strlen("      Duracao      "));

            snprintf(buffer, MAX_STR, "%.2f%%", percentagem);
            imprimir_em_tabela(buffer, strlen(" Percentagem de viagens "));
            printf("|\n");

        }

    }
    esperar_resposta();

}

//lida com o sub-menu de estatísticas de velocidade do menu listagem de estatísticas
void estatisticas_velocidade(viagem_node *head_viagens)
{
    int idade_max = 0;
    viagem_node *current = head_viagens;
    int idade = 0;
    float num_km_masc = 0;
    float duracao_total_masc = 0;
    float num_km_fem = 0;
    float duracao_total_fem = 0;
    printf("\nVelocidade média em km/h:\n");
    printf("\n|     Idades     |   Masculino   |   Feminino   |");
    printf("\n-------------------------------------------------\n");

    //percorre a lista de viagens para determinar a idade máxima de utilizadores
    current = head_viagens;
    while(current != NULL)
    {
        if(current->viagem_info.tipo_usuario == 'R')
        {
            if(idade_max < current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento)
                idade_max = current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento;
        }
        current = current->next;
    }

    for(idade = 0; idade <= idade_max; idade++)
    {
        //calcula os valores necessários para cada um dos géneros
        velocidade_masculino(head_viagens, &num_km_masc, &duracao_total_masc, idade);

        velocidade_feminino(head_viagens, &num_km_fem, &duracao_total_fem, idade);

        //imprime os valores no ecrã, mas apenas se estes fizerem sentido (duração diferente de zero)
        if(duracao_total_masc != 0 || duracao_total_fem != 0)
        {
            imprimir_velocidade(idade, duracao_total_masc, duracao_total_fem, num_km_masc, num_km_fem);

            printf("|\n");
        }
    }
    esperar_resposta();
}

//converte um valor em graus para um valor em radianos e retorna esse valor
float graus_para_radianos(float angulo_em_graus)
{
    return (angulo_em_graus * M_PI)/(float)180;
}

//calcula a distância entre dois pontos da superfície terrestre pela formula de haversine com a aproximação equiretangular
//e retorna esse valor em km
float distancia(float lat1, float lon1, float lat2, float lon2)
{
    float x = (graus_para_radianos(lon2) - graus_para_radianos(lon1))* cosf((graus_para_radianos(lat1) + graus_para_radianos(lat2))/2);
    float y = graus_para_radianos(lat2) - graus_para_radianos(lat1);
    int R = 6371; //raio da terra em km
    return (R*sqrt(x*x + y*y));
}

//calcula o numero de km total e a duração para viagens de utilizadores do sexo masculino, de determinada idade
void velocidade_masculino(viagem_node *head_viagens, float* num_km_masc, float* duracao_total_masc, int idade)
{
    viagem_node* current = head_viagens;
    *num_km_masc = 0.0f;
    *duracao_total_masc = 0.0f;
    //para todas as viagens em que a idade e género coincidam com os valores de entrada, são acumulados o numero
    //de km e a duração da viagem
    while(current != NULL)
    {
        if(current->viagem_info.tipo_usuario == 'R' && idade == (current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento)
                                            && current->viagem_info.genero == 'M')
        {
            (*num_km_masc) += distancia(current->viagem_info.estacao_inicial->estacao_info.latitude, current->viagem_info.estacao_inicial->estacao_info.longitude,
                                current->viagem_info.estacao_final->estacao_info.latitude, current->viagem_info.estacao_final->estacao_info.longitude);
            (*duracao_total_masc) += current->viagem_info.duracao;
        }
        current = current->next;
    }
    //conversão da duração para horas
    (*duracao_total_masc) = (*duracao_total_masc)/(60*60);
}

//calcula o numero de km total e a duração para viagens de utilizadores do sexo feminino, de determinada idade
void velocidade_feminino(viagem_node *head_viagens, float* num_km_fem, float* duracao_total_fem, int idade)
{
    viagem_node* current = head_viagens;
    *num_km_fem = 0.0f;
    *duracao_total_fem = 0.0f;
    //para todas as viagens em que a idade e género coincidam com os valores de entrada, são acumulados o numero
    //de km e a duração da viagem
    while(current != NULL)
    {
        if(current->viagem_info.tipo_usuario == 'R' && idade == (current->viagem_info.data_inicio.ano - current->viagem_info.ano_nascimento)
                                            && current->viagem_info.genero == 'F')
        {
            (*num_km_fem) += distancia(current->viagem_info.estacao_inicial->estacao_info.latitude, current->viagem_info.estacao_inicial->estacao_info.longitude,
                                current->viagem_info.estacao_final->estacao_info.latitude, current->viagem_info.estacao_final->estacao_info.longitude);
            (*duracao_total_fem) += current->viagem_info.duracao;
        }
        current = current->next;
    }
    //conversão da duração para horas
    (*duracao_total_fem) = (*duracao_total_fem)/(60*60);
}

//imprime as velocidades do sexo feminino e masculino relativas a uma determinada idade
void imprimir_velocidade(int idade, float duracao_total_masc, float duracao_total_fem, float num_km_masc, float num_km_fem)
{
    float velocidade = 0.0f;
    char buffer[MAX_STR] = {0};
    snprintf(buffer, MAX_STR, "%d", idade);
    imprimir_em_tabela(buffer, strlen("     Idades     "));
    //apenas imprime se os valores fizerem sentido (duração diferente de 0)
    if(duracao_total_masc == 0)
    {
        imprimir_em_tabela("--", strlen("   Masculino   "));
    }
    else
    {
        velocidade = num_km_masc/duracao_total_masc;
        snprintf(buffer, MAX_STR, "%.2f km/h", velocidade);
        imprimir_em_tabela(buffer, strlen("   Masculino   "));
    }

    if(duracao_total_fem == 0)
    {
        imprimir_em_tabela("--", strlen("   Feminino   "));
    }
    else
    {
        velocidade = num_km_fem/duracao_total_fem;
        snprintf(buffer, MAX_STR, "%.2f km/h", velocidade);
        imprimir_em_tabela(buffer, strlen("   Feminino   "));
    }
}



//limpa a janela de texto
void limparJanela()
{
    printf("\e[1;1H\e[2J");//código que limpa uma janela de texto (em janelas ANSI)
}

//espera que o utilizador insira o valor '0' para voltar ao menu anterior (depois de mostrar alguma informação no ecrã)
void esperar_resposta()
{
    int ret = 0;
    int aux = 0;
    char resposta[MAX_STR] = {0};
    //ciclo que se certifica que o utilizador inseriu um '0'
    do
    {
        printf("\nIntroduza (0) para voltar: ");
        fgets(resposta, MAX_STR, stdin);
        ret = sscanf(resposta, "%d", &aux); //converte para int e guarda na variável correta
        if(ret != 1 || aux != 0)
        {
            ret = 0;
        }

    }while(ret != 1);
}

//função que imprime no ecrã o "texto" centrado numa entrada de uma tabela, adicionando os espaços necessários.
//o "tamanho" corresponde ao número de dígitos que o texto deve ocupar
void imprimir_em_tabela(char* texto, int tamanho)
{
    int numero_espacos = tamanho - strlen(texto); //número de espaços que são necessários adicionar
    int i = 0;
    fputc('|', stdout); //desenho da tabela
    //o texto fica centrado, sendo imprimidos metade dos espaços antes e outra metade depois do texto
    for(i = 0; i < numero_espacos/2; i++)
    {
        fputc(' ', stdout);
    }
    //impressão do texto
    printf("%s", texto);
    //segunda metade dos espaços
    for(i = 0; i < numero_espacos - numero_espacos/2; i++)
    {
        fputc(' ', stdout);
    }

}

//imprime as restrições impostas pelo jogador (utilizado antes da impressão de tabelas)
void imprimir_restricoes(int periodo_tempo_enable, int dia_semana_enable, int duracao_enable, hora_data tempo_inicial,
                         hora_data tempo_final, int dia_semana, int duracao)
{
    printf("\n----------------------------------------------------------------------------\n");
    if(periodo_tempo_enable || dia_semana_enable || duracao_enable)
    {
        printf("Restrições:\t");
        //imprime apenas as restrições que o utilizador selecionou
        if(periodo_tempo_enable == 1)
            printf("Período de tempo: %.2d:%.2d:%.2d a %.2d:%.2d:%.2d\t", tempo_inicial.hora, tempo_inicial.minuto, tempo_inicial.segundo,
                                                                    tempo_final.hora, tempo_final.minuto, tempo_final.segundo);
        if(dia_semana_enable == 1)
        {
            switch(dia_semana){
                case 1:
                    printf("Dia da semana: Domingo\t");
                    break;
                case 2:
                    printf("Dia da semana: Segunda-Feira\t");
                    break;
                case 3:
                    printf("Dia da semana: Terça-Feira\t");
                    break;
                case 4:
                    printf("Dia da semana: Quarta-Feira\t");
                    break;
                case 5:
                    printf("Dia da semana: Quinta-Feira\t");
                    break;
                case 6:
                    printf("Dia da semana: Sexta-Feira\t");
                    break;
                case 7:
                    printf("Dia da semana: Sábado\t");
                    break;

                    }
            }

        if(duracao_enable == 1)
            printf("Duração: %d segundos\t", duracao);
        printf("\n");
    }
    //caso não tenham sido selecionadas restrições
    else
        printf("Restrições: Nenhuma\n\n");

}

//pergunta ao utilizador se deseja avançar de página (1) ou sair do programa (0) e retorna o respetivo valor
int avancar_pagina()
{
    char buffer[MAX_STR] = {0};
    int aux = 0;
    int ret = 0;
    //ciclo que se certifica que a resposta do utilizador é válida
    do
    {
        printf("\nInsira (0) para voltar atrás ou (1) para avançar de página: ");
        fgets(buffer, MAX_STR, stdin);
        ret = sscanf(buffer, "%d", &aux); //converte para int e guarda na variável correta
        if(ret != 1 || (aux != 1 && aux != 0))
        {
            ret = 0;
        }
    }while(ret != 1);

    return aux;
}

//pergunta ao utilizador para introduzir um dos menus; o valor de retorno indica se o menu é válido (1) ou não (0)
int pedir_menu(int *menu, int lim_inferior, int lim_superior)
{
    char resposta[MAX_STR] = {0};
    int ret = 0;
    printf("\nEscolha um dos menus acima: ");
    fgets(resposta, MAX_STR, stdin);
    ret = sscanf(resposta, "%d", menu);
    if(ret != 1 || *menu < lim_inferior || *menu > lim_superior)
    {
        printf("Menu Inválido\n");
        ret = 0;
    }
    return ret;
}


