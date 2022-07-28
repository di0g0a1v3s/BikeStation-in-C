#include "viagem.h"

//carrega todo o conteúdo do ficheiro de viagens para uma lista
void load_viagens(char* ficheiro_viagens, viagem_node **head, estacao_node* head_estacao)
{
    char buffer[MAX_STR];
    viagem new_viagem; //nova viagem onde será guardada a informação de cada linha
    viagem_node *last_node = NULL;
    int ignorar_viagem = 0;
    //abrir o ficheiro
    FILE* fp = fopen(ficheiro_viagens, "r");
    if(fp == NULL)
    {
        printf("Não foi possível abrir o ficheiro de viagens\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, MAX_STR, fp) != NULL)
    {   //ler cada linha
        new_viagem = ler_linha_viagem(buffer, &ignorar_viagem);

        if(ignorar_viagem != 1)
        {
            //preenche os campos estacao_inicial e estacao_inicial de cada nova estacao criada
            ligar_viagens_estacoes(&new_viagem, head_estacao);
            //se a lista estiver vazia, a nova viagem é inserida no inicio
            if(*head == NULL)
            {
                *head = (viagem_node*)calloc(1, sizeof(viagem_node));
                verificar_alocacao_memoria((void*)(*head));
                (*head)->viagem_info = new_viagem;
                last_node = *head;
            }
            else
            {
                //inserir a viagem na lista ordenadamente
                inserir_viagem_ordenadamente(head, &last_node, new_viagem);
            }

        }
    }
    fclose(fp);
}

//insere viagens na lista por ordem de data de inicio da viagem
void inserir_viagem_ordenadamente(viagem_node **head, viagem_node **last_node, viagem new_viagem)
{
    viagem_node *aux = NULL;
    viagem_node *novo_node = (viagem_node*)calloc(1, sizeof(viagem_node));
    verificar_alocacao_memoria((void*)(novo_node));
    novo_node->viagem_info = new_viagem;
    //inserir na head
    if(cmp_datas(new_viagem.data_inicio, (*head)->viagem_info.data_inicio) < 0)
    {
        novo_node->next = *head;
        *head = novo_node;

        return;
    }
    //como é mais habitual o ficheiro já estar parcialmente ordenado, tenta inserir primeiro no fim da lista
    if(cmp_datas(new_viagem.data_inicio, (*last_node)->viagem_info.data_inicio) >= 0)
    {
        (*last_node)->next = novo_node;
        (*last_node) = (*last_node)->next;
        (*last_node)->next = NULL;
        return;
    }
    //inserir no meio da lista
    aux = *head;
    while(aux->next != NULL)
    {
        if(cmp_datas(new_viagem.data_inicio, aux->next->viagem_info.data_inicio) <= 0 &&
           cmp_datas(new_viagem.data_inicio, aux->viagem_info.data_inicio) >= 0)
        {
            novo_node->next = aux->next;
            aux->next = novo_node;
            return;
        }
        aux = aux->next;
    }


}

//preenche os campos estacao_inicial e estacao_inicial de cada nova estacao criada
void ligar_viagens_estacoes(viagem* new_viagem, estacao_node *head_estacao)
{
    estacao_node *current_estacao = NULL;
    new_viagem->estacao_inicial = NULL;
    current_estacao = head_estacao;
    //estacao_inicial
    while(current_estacao != NULL)
    {
        if(current_estacao->estacao_info.id_estacao == new_viagem->id_inicial)
        {
            new_viagem->estacao_inicial = current_estacao;
            break;
        }
        current_estacao = current_estacao->next;
    }

    new_viagem->estacao_final = NULL;
    current_estacao = head_estacao;
    //estacao_final
    while(current_estacao != NULL)
    {
        if(current_estacao->estacao_info.id_estacao == new_viagem->id_final)
        {
            new_viagem->estacao_final = current_estacao;
            break;
        }
        current_estacao = current_estacao->next;
    }
}

//lê uma linha do ficheiro viagens e retorna uma viagem com essas informações
viagem ler_linha_viagem(char* buffer, int* ignorar_viagem)
{
    char* p_aux = NULL;
    char* p_aux2 = NULL;
    int ret = 0;
    viagem new_viagem;
    *ignorar_viagem = 0;
    //o ponteiro p_aux2 estará a apontar para a porção de string que corresponde a um dado útil entre virgulas
    p_aux = buffer;
    //ID da viagem
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    else
    {
        if(sscanf(p_aux2,"%d", &new_viagem.id_viagem) != 1 || new_viagem.id_viagem < 0)
            *ignorar_viagem = 1;
    }
    //Duração
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    else
    {
        if(sscanf(p_aux2,"%d", &new_viagem.duracao) != 1 || new_viagem.duracao < 0)
            *ignorar_viagem = 1;
    }
    //Data/Hora de inicio
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    else
    {
        ret = sscanf(p_aux2,"%d/%d/%d %d:%d:%d", &new_viagem.data_inicio.mes,&new_viagem.data_inicio.dia, &new_viagem.data_inicio.ano,
           &new_viagem.data_inicio.hora, &new_viagem.data_inicio.minuto,&new_viagem.data_inicio.segundo);
        if(ret != 6 || new_viagem.data_inicio.mes > 12 || new_viagem.data_inicio.mes < 0 || new_viagem.data_inicio.dia > 31
           || new_viagem.data_inicio.dia < 0 || new_viagem.data_inicio.ano < 0 || new_viagem.data_inicio.hora > 23 ||
           new_viagem.data_inicio.hora < 0 || new_viagem.data_inicio.minuto < 0 || new_viagem.data_inicio.minuto > 59
           || new_viagem.data_inicio.segundo < 0 || new_viagem.data_inicio.segundo > 59)
            *ignorar_viagem = 1;
    }
    //ID Inicial
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    else
    {
        if(sscanf(p_aux2,"%d", &new_viagem.id_inicial) != 1 || new_viagem.id_inicial < 0)
            *ignorar_viagem = 1;
    }
    //Data/Hora Fim
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    else
    {
        ret = sscanf(p_aux2,"%d/%d/%d %d:%d:%d", &new_viagem.data_fim.mes,&new_viagem.data_fim.dia, &new_viagem.data_fim.ano,
           &new_viagem.data_fim.hora, &new_viagem.data_fim.minuto,&new_viagem.data_fim.segundo);
        if(ret != 6 || new_viagem.data_fim.mes > 12 || new_viagem.data_fim.mes < 0 || new_viagem.data_fim.dia > 31
           || new_viagem.data_fim.dia < 0 || new_viagem.data_fim.ano < 0 || new_viagem.data_fim.hora > 23 ||
           new_viagem.data_fim.hora < 0 || new_viagem.data_fim.minuto < 0 || new_viagem.data_fim.minuto > 59
           || new_viagem.data_fim.segundo < 0 || new_viagem.data_fim.segundo > 59)
            *ignorar_viagem = 1;
    }
    //ID Final
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    else
    {
        if(sscanf(p_aux2,"%d", &new_viagem.id_final) != 1 || new_viagem.id_final < 0)
            *ignorar_viagem = 1;
    }
    //ID Bicicleta
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
    {
        strcpy(new_viagem.bicicleta, "---");
    }
    else
    {
        if(sscanf(p_aux2,"%s", new_viagem.bicicleta) != 1)
            *ignorar_viagem = 1;
    }
    //Tipo de usuário
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_viagem = 1;
    if(strcmp(REGISTERED, p_aux2) == 0)
    {
        new_viagem.tipo_usuario = 'R';
    }
    else if(strcmp(CASUAL, p_aux2) == 0)
    {
        new_viagem.tipo_usuario = 'C';
    }
    else
        *ignorar_viagem = 1;

    if(new_viagem.tipo_usuario == 'R')
    {   //Ano Nascimento
        proxima_virgula(&p_aux, &p_aux2);
        if(*p_aux2 == '\0')
            *ignorar_viagem = 1;
        else
        {
            if(sscanf(p_aux2,"%d", &new_viagem.ano_nascimento) != 1)
                *ignorar_viagem = 1;
        }
        //Género
        p_aux2 = p_aux;
        p_aux = strchr(p_aux2, VIRGULA);
        if(p_aux != NULL)
            *p_aux = '\0';
        if(*p_aux2 == '\0')
            *ignorar_viagem = 1;
        if(strstr(p_aux2, MALE) != NULL)
        {
            new_viagem.genero = 'M';
        }
        else if(strstr(p_aux2, FEMALE) != NULL)
        {
            new_viagem.genero = 'F';
        }
        else
            *ignorar_viagem = 1;
    }
    else if(new_viagem.tipo_usuario == 'C')
    {
        new_viagem.ano_nascimento = -1;
        new_viagem.genero = '\0';
    }
    return new_viagem;
}

//função que liberta toda a memória alocada pela lista de viagens
void libertar_lista_viagens(viagem_node **head_viagens)
{
    viagem_node *current = *head_viagens;
    viagem_node *aux = current;
    //os ponteiros current e aux vão iterando da cabeça para a cauda da lista e libertando a memória um nó de cada vez
    while(current != NULL)
    {
        aux = current->next;
        free(current);
        current = aux;
    }
    //no fim, a cabeça é posta a apontar para NULL
    *head_viagens = NULL;
}

//função que compara duas datas e retorna um valor negativo, zero ou positivo se, respetivamente, a data1 for
//menor, igual ou menor que a data2
int cmp_datas(hora_data data1, hora_data data2)
{
    //se as datas fores diferentes, é retornada diferença entre a primeira unidade (ano, mes, dia...) diferente entre as duas
    if(data1.ano != data2.ano)
        return data1.ano - data2.ano;
    if(data1.mes != data2.mes)
        return data1.mes - data2.mes;
    if(data1.dia != data2.dia)
        return data1.dia - data2.dia;
    if(data1.hora != data2.hora)
        return data1.hora - data2.hora;
    if(data1.minuto != data2.minuto)
        return data1.minuto - data2.minuto;
    if(data1.segundo != data2.segundo)
        return data1.segundo - data2.segundo;
    //se forem iguais é retornado 0
    return 0;
}
