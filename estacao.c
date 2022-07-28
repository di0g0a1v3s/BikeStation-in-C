#include "estacao.h"


//carrega todo o conteúdo do ficheiro de estações para uma lista
void load_estacoes(char* ficheiro_estacoes, estacao_node **head)
{

    char buffer[MAX_STR];
    estacao new_estacao;//nova estação onde será guardada a informação de cada linha
    estacao_node *last_node;
    int ignorar_estacao = 0;
    //abrir o ficheiro
    FILE* fp = fopen(ficheiro_estacoes, "r");

    if(fp == NULL)
    {
        printf("Não foi possível abrir o ficheiro de estações\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, MAX_STR, fp) != NULL)
    {   //ler cada linha
        new_estacao = ler_linha_estacao(buffer, &ignorar_estacao);

        if(ignorar_estacao != 1)
        {
            //se a lista estiver vazia, a nova estação é inserida no inicio
            if(*head == NULL)
            {
                *head = (estacao_node*)calloc(1, sizeof(estacao_node));
                verificar_alocacao_memoria((void*)(*head));
                (*head)->estacao_info = new_estacao;
                last_node = *head;
            }
            else
            {
                //inserir ordenadamente na lista
                inserir_estacao_ordenadamente(head, &last_node, new_estacao);
            }
        }
    }
    fclose(fp);
}


//insere estações na lista por ordem de id da estação
void inserir_estacao_ordenadamente(estacao_node **head, estacao_node **last_node, estacao new_estacao)
{
    estacao_node *aux = NULL;
    estacao_node *novo_node = (estacao_node*)calloc(1, sizeof(estacao_node));
    verificar_alocacao_memoria((void*)(novo_node));
    novo_node->estacao_info = new_estacao;
    //inserir na head
    if(new_estacao.id_estacao < (*head)->estacao_info.id_estacao)
    {
        novo_node->next = *head;
        *head = novo_node;

        return;
    }
    //como é mais habitual o ficheiro já estar parcialmente ordenado, tenta inserir primeiro no fim da lista
    if(new_estacao.id_estacao >= (*last_node)->estacao_info.id_estacao)
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
        if(new_estacao.id_estacao <= aux->next->estacao_info.id_estacao &&
           new_estacao.id_estacao >= aux->estacao_info.id_estacao)
        {
            novo_node->next = aux->next;
            aux->next = novo_node;
            return;
        }
        aux = aux->next;
    }

}

//lê uma linha do ficheiro estações e retorna uma estação com essas informações
estacao ler_linha_estacao(char* buffer, int* ignorar_estacao)
{
    char* p_aux = NULL;
    char* p_aux2 = NULL;
    int ret = 0;
    estacao new_estacao;
    *ignorar_estacao = 0;
    //o ponteiro p_aux2 estará a apontar para a porção de string que corresponde a um dado útil entre virgulas
    p_aux = buffer;
    //ID da estação
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_estacao = 1;
    else
    {
        ret = sscanf(p_aux2,"%d", &new_estacao.id_estacao);
        if(ret != 1)
            *ignorar_estacao = 1;
    }
    //Nome curto da estação
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        new_estacao.nome_curto[0] = '\0';
    else
    {
        ret = sscanf(p_aux2,"%s", new_estacao.nome_curto);
        if(ret != 1)
            *ignorar_estacao = 1;
    }
    //descrição da estação
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        new_estacao.descricao[0] = '\0';
    else
    {
        strcpy(new_estacao.descricao, p_aux2);
    }
    //Município
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        new_estacao.municipio[0] = '\0';
    else
    {
        strcpy(new_estacao.municipio, p_aux2);
    }
    //Latitude
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_estacao = 1;
    else
    {
        ret = sscanf(p_aux2,"%lf", &new_estacao.latitude);
        if(ret != 1)
            *ignorar_estacao = 1;
    }
    //Longitude
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_estacao = 1;
    else
    {
        ret = sscanf(p_aux2,"%lf", &new_estacao.longitude);
        if(ret != 1)
            *ignorar_estacao = 1;
    }
    //Estado
    proxima_virgula(&p_aux, &p_aux2);
    if(*p_aux2 == '\0')
        *ignorar_estacao = 1;
    else
    {
        if(strstr(p_aux2, EXISTING) != NULL)
        {
            new_estacao.estado = 'E';
        }
        else if(strstr(p_aux2, REMOVED) != NULL)
        {
            new_estacao.estado = 'R';
        }
        else
            *ignorar_estacao = 1;
    }
    return new_estacao;
}

//devolve a estação imediatamente anterior na lista ou a ultima estação, no caso de a atual ser a head
estacao_node* estacao_anterior(estacao_node *head, estacao_node *current)
{
    estacao_node *aux = head;
    if(current == head)
    {
        while(aux->next != NULL)
            aux = aux->next;

        return aux;
    }
    else
    {
        while(aux->next != current)
            aux = aux->next;

        return aux;
    }
}

//função que liberta toda a memória alocada pela lista de estações
void libertar_lista_estacoes(estacao_node **head_estacoes)
{
    estacao_node *current = *head_estacoes;
    estacao_node *aux = current;
    //os ponteiros current e aux vão iterando da cabeça para a cauda da lista e libertando a memória um nó de cada vez
    while(current != NULL)
    {
        aux = current->next;
        free(current);
        current = aux;
    }
    //no fim, a cabeça é posta a apontar para NULL
    *head_estacoes = NULL;
}

//assumindo que p_aux está a apontar para uma linha do ficheiro, uma chamada a esta função faz com que p_aux2 fique a apontar
//para a primeira porção de texto antes da primeira virgula e p_aux fique a apontar para a restante parte da linha
void proxima_virgula(char** p_aux, char** p_aux2)
{
    if(*p_aux != NULL)
    {
        *p_aux2 = *p_aux;
        *p_aux = strchr(*p_aux2, VIRGULA);
        if(*p_aux != NULL)
        {
        **p_aux = '\0';
        *p_aux = *p_aux + 1;
        }
    }

}

//verifica se a alocação de memória foi efetuada corretamente ou não
void verificar_alocacao_memoria(void* ponteiro)
{
    if(ponteiro == NULL)
    {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
}
