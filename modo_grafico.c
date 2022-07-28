#include "modo_grafico.h"

//inicio do modo gráfico
void modo_grafico(estacao_node *head_estacao, viagem_node **head_viagem)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *boston_map = NULL;
    SDL_Surface *ist_logo = NULL;
    SDL_Surface *BS_logo = NULL;
    SDL_Event event;
    int delay = 30;
    int quit = 0;
    int width = MAP_SIZE_X + RIGHT_BAR_SIZE; //largura da janela
    int height = MAP_SIZE_Y + BOTTOM_BAR_SIZE; //altura da janela
    int *posicao_x = NULL;  //vetor com as posições x de cada estação
    int *posicao_y = NULL;  //vetor com as posições y de cada estação
    estacao_node* estacao_selecionada = NULL;   //estação selecionada pelo utilizador
    int *dias_semana = (int*)calloc(7, sizeof(int));    //vetor com informação sobre os dias da semana selecionados pelo utilizador
    verificar_alocacao_memoria((void*)(dias_semana));
    //começa por pedir ao utilizador quais os dias da semana que pretende selecionar
    pedir_dias_semana(dias_semana);
    //remove as viagens que não pertencem aos dias da semana selecionados pelo utilizador
    remover_nodes_viagens_modo_grafico(head_viagem, dias_semana);
    //calcula as coordenadas xy de cada estação e guarda-as nos vetores posicao_x e posicao_y
    calcular_coordenadas(head_estacao, &posicao_x, &posicao_y);

	// inicializa os gráficos
	InitEverything(width, height, &serif, &boston_map, &ist_logo, &BS_logo, &window, &renderer);

  	while( quit == 0 )
    {
        // enquanto existirem eventos
        while( SDL_PollEvent( &event ) )
        {
            //quando a aplicação é fechada
            if( event.type == SDL_QUIT )
            {
                    quit = 1;
            }
            //quando o utilizador clica com o botão esquerdo do rato
            if( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {   //a estação selecionada irá corresponder à estação onde o utilizador clicou ou NULL se clicou num espaço vazio
                estacao_selecionada = clique_rato(head_estacao, posicao_x, posicao_y);
            }
            //quando um utilizador clica numa tecla
            if( event.type == SDL_KEYDOWN)
            {   //as setas permitem ao utilizador mudar de estação
                estacao_selecionada = tecla_premida(head_estacao, estacao_selecionada, event);
            }
        }
        //altera a cor para branco e faz clear ao renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); //branco
        SDL_RenderClear(renderer);
        //carrega o mapa de boston
        RenderMap( boston_map, renderer);
        //carrega os logotipos do ist e da bikestation
        RenderLogo(BS_logo, renderer, RenderLogo(ist_logo, renderer, 0));
        //mostra no ecrã os nomes, estação selecionada e legenda do mapa
        display_texto(estacao_selecionada, serif, renderer);
        //desenha as linhas correspondentes às rotas
        desenharRotas(head_estacao, *head_viagem, estacao_selecionada, renderer, posicao_x, posicao_y);
        //desenha os quadrados correspondentes às estações
        desenharEstacoes(head_estacao, estacao_selecionada, renderer, posicao_x, posicao_y);

        //render de tudo o que está acima
        SDL_RenderPresent(renderer);
    	//adiciona um delay
		SDL_Delay( delay );

    }

    // liberta memória alocada para imagens e texturas e fecha tudo incluindo fontes
    free(posicao_x);
    free(posicao_y);
    free(dias_semana);
    TTF_CloseFont(serif);
    SDL_FreeSurface(boston_map);
    SDL_FreeSurface(BS_logo);
    SDL_FreeSurface(ist_logo);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//pede ao utilizador para selecionar os dias da semana que pretende
void pedir_dias_semana(int* dias_semana)
{
    char buffer [MAX_STR];
    int ret = 0;
    int menu = -1;
    int i = 0;
    int nenhum_dia = 1;
    while(menu != 0)
    {
        nenhum_dia = 1;
        limparJanela();
        //imprime as opções de escolha do utilizador
        printf("(1) Domingo\n(2) Segunda-Feira\n(3) Terça-Feira\n(4) Quarta-Feira\n(5) Quinta-Feira\n(6) Sexta-Feira\n(7) Sábado\n");
        printf("Dias selecionados: ");
        //imprime as escolhas já feitas pelo utilizador
        for(i = 0; i < 7; i++)
        {
            if(dias_semana[i])
            {
                printf("(%d) ", i + 1);
                nenhum_dia = 0;
            }

        }
        if(nenhum_dia)
            printf("Nenhum");

        //ciclo que se certifica que as respostas do utilizador são válidas
        do
        {

            printf("\nSelecione os dias da semana que deseja ou (0) para iniciar o modo gráfico: ");
            fgets(buffer, MAX_STR, stdin);
            ret = sscanf(buffer, "%d", &menu);
            if(ret != 1 || menu < 0 || menu > 7)
            {
                printf("\nResposta inválida\n");
                ret = 0;
            }
        }while(ret != 1);
        //no vetor, a posição correspondente a cada dia da semana toma o valor 0 caso não tenha sido selecionado
        //pelo utilizador e um valor diferente de zero caso contrário
        if(menu != 0)
            dias_semana[menu - 1] = !dias_semana[menu - 1];
    }

}

//remove as viagens cujo dia da semana não foi selecionado pelo utilizador
void remover_nodes_viagens_modo_grafico(viagem_node **head_viagens, int* dias_semana)
{
    viagem_node *current = NULL;
    viagem_node *aux = NULL;
    //se a lista de viagens estiver não vazia e a head atual for para eliminar, a head avança um elemento e a head original
    //é eliminada
    while(*head_viagens != NULL && manter_viagem_modo_grafico((*head_viagens)->viagem_info, dias_semana) == 0)
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
        while(current->next != NULL && manter_viagem_modo_grafico((current->next)->viagem_info, dias_semana) == 0 )
        {
            aux = current->next;
            current->next = aux->next;
            free(aux);
        }
        current = current->next;
    }
}

//devolve 1 caso o dia da viagem passada tenha sido selecionado pelo utilizador e 0 caso contrário
int manter_viagem_modo_grafico(viagem _viagem, int* dias_semana)
{
    int dia_semana_viagem = dia_semana(_viagem.data_inicio.ano, _viagem.data_inicio.mes, _viagem.data_inicio.dia);
    if(!dias_semana[dia_semana_viagem - 1])
        return 0;
    else
        return 1;
}

//calcula as coordenadas xy de cada estação a partir da latitude e longitude e guarda-as nos vetores posicao_x e posicao_y
void calcular_coordenadas(estacao_node* head_estacao, int **_posicao_x,int **_posicao_y)
{
    estacao_node *current = head_estacao;
    int i = 0;
    while(current != NULL)
    {
        //redimensiona os vetores conforme o número de estações
        *_posicao_x = (int*)realloc(*_posicao_x, (i+1)*sizeof(int));
        verificar_alocacao_memoria((void*)(*_posicao_x));
        *_posicao_y = (int*)realloc(*_posicao_y, (i+1)*sizeof(int));
        verificar_alocacao_memoria((void*)(*_posicao_y));
        //calcula as posições x e y na janela do SDL a partir da latitude e longitude
        (*_posicao_x)[i] = (int)((current->estacao_info.longitude - MIN_LON)*MAP_SIZE_X /(MAX_LON - MIN_LON));
        (*_posicao_y)[i] = (int)((current->estacao_info.latitude - MAX_LAT)*MAP_SIZE_Y /(MIN_LAT - MAX_LAT));
        current = current->next;
        i++;
    }
}

//função chamada quando o utilizador carrega numa tecla do teclado
estacao_node* tecla_premida(estacao_node* head_estacao, estacao_node* estacao_selecionada, SDL_Event event)
{
    estacao_node* nova_estacao_selecionada = NULL;
    //se clicar na seta direita, a função retorna um ponteiro para a estação seguinte na lista (caso a estação atual seja
    //a última, retorna a head)
    if(event.key.keysym.sym == SDLK_RIGHT)
    {
       if(estacao_selecionada != NULL)
        {
            if(estacao_selecionada->next == NULL)
                nova_estacao_selecionada = head_estacao;
            else
                nova_estacao_selecionada = estacao_selecionada->next;
        }
        else
            nova_estacao_selecionada = head_estacao;

        return nova_estacao_selecionada;
    }
    //se clicar na seta esquerda, a função retorna um ponteiro para a estação anterior na lista (caso a estação atual seja
    //a head, retorna a última)
    else if(event.key.keysym.sym == SDLK_LEFT)
    {
        if(estacao_selecionada != NULL)
        {
            nova_estacao_selecionada = estacao_anterior(head_estacao, estacao_selecionada);
        }
        else
            nova_estacao_selecionada = estacao_anterior(head_estacao, head_estacao);

        return nova_estacao_selecionada;
    }
    //caso a tecla não seja nenhuma destas duas setas, retorna a própria estação selecionada
    return estacao_selecionada;

}

//função chamada quando o utilizador clica com o botão esquerdo do rato
estacao_node* clique_rato(estacao_node* head_estacao, int *posicao_x, int *posicao_y)
{
    int mouse_x = 0;
    int mouse_y = 0;
    //coordenadas xy do clique
    SDL_GetMouseState(&mouse_x, &mouse_y);
    estacao_node* current = head_estacao;
    estacao_node* estacao_selecionada = NULL;
    int i = 0;
    //procura na lista se o utilizador clicou nas coordenadas de alguma estação
    while(current != NULL)
    {
        if(abs(mouse_x - posicao_x[i]) <= GROSSURA_PONTO/2 && abs(mouse_y - posicao_y[i]) <= GROSSURA_PONTO/2)
        {   //caso sim, essa é a nova estação selecionada
            estacao_selecionada = current;
            break;
        }
        else
            //caso não, a estação selecionada é NULL
            estacao_selecionada = NULL;

        current = current->next;
        i++;
    }
    return estacao_selecionada;
}

//desenha na barra da direita um logotipo
int RenderLogo(SDL_Surface *logo, SDL_Renderer* _renderer, int pos_y)
{
    SDL_Texture *logo_texture;
    SDL_Rect mapDest;

    // space occupied by the logo
    mapDest.x = MAP_SIZE_X;
    mapDest.y = pos_y;
    mapDest.w = logo->w;
    mapDest.h = logo->h;

    // render it
    logo_texture = SDL_CreateTextureFromSurface(_renderer, logo);
    SDL_RenderCopy(_renderer, logo_texture, NULL, &mapDest);

    // destroy associated texture !
    SDL_DestroyTexture(logo_texture);
    return logo->h;
}

//carrega o mapa na janela do SDL
void RenderMap( SDL_Surface *_boston_map, SDL_Renderer* _renderer )
{

    SDL_Texture *map_texture;
    SDL_Rect mapDest;


    mapDest.x = 0;
    mapDest.y = 0;
    mapDest.w = MAP_SIZE_X;
    mapDest.h = MAP_SIZE_Y;

    // draws the map texture
    map_texture = SDL_CreateTextureFromSurface(_renderer, _boston_map);
    SDL_RenderCopy(_renderer, map_texture, NULL, &mapDest);

    // destroy everything
    SDL_DestroyTexture(map_texture);
}

//imprime na janela todo o texto: legenda da imagem, estação selecionada e nomes
void display_texto(estacao_node* estacao_selecionada, TTF_Font* font, SDL_Renderer *renderer)
{

    SDL_Color preto = {0,0,0,255};
    int paragrafo = 10;
    //nomes
    int height = 3*MAP_SIZE_Y/4;
    height += RenderText(MAP_SIZE_X + paragrafo, height,NOME1, font, &preto, renderer);
    height += RenderText(MAP_SIZE_X + paragrafo, height,NUMERO1, font, &preto, renderer);
    height += RenderText(MAP_SIZE_X + paragrafo, height,NOME2, font, &preto, renderer);
    RenderText(MAP_SIZE_X + paragrafo, height,NUMERO2, font, &preto, renderer);
    //estação selecionada
    height = MAP_SIZE_Y;
    char buffer[MAX_STR] = "Estacao escolhida: ";
    if(estacao_selecionada != NULL)
    {
        strcat(buffer, estacao_selecionada->estacao_info.nome_curto);
        strcat(buffer, ", ");
        strcat(buffer, estacao_selecionada->estacao_info.descricao);
        strcat(buffer, ", ");
        strcat(buffer, estacao_selecionada->estacao_info.municipio);
        height += RenderText(paragrafo, height, buffer, font, &preto, renderer);
    }
    else
        height += RenderText(paragrafo, height, "Estacao escolhida: Nenhuma", font, &preto, renderer);

    //legenda
    SDL_Color cor_chegada = {CORCHEGADA_R, CORCHEGADA_G, CORCHEGADA_B, CORCHEGADA_A};
    SDL_Color cor_partida = {CORPARTIDA_R, CORPARTIDA_G, CORPARTIDA_B, CORPARTIDA_A};
    height += RenderText(paragrafo, height, "Rotas de chegada", font, &cor_chegada, renderer);
    RenderText(paragrafo, height, "Rotas de partida", font, &cor_partida, renderer);

}

//desenha no mapa um quadrado correspondente à posição de cada estação
void desenharEstacoes(estacao_node* head_estacao,estacao_node* estacao_escolhida, SDL_Renderer *renderer, int *_posicao_x,int *_posicao_y)
{
    estacao_node *current = head_estacao;
    SDL_Rect ponto;
    int i = 0;

    SDL_SetRenderDrawColor(renderer, 176, 17, 21, 0); //vermelho
    current = head_estacao;
    //desenha um quadrado vermelho em cada estação
    while(current != NULL)
    {
        ponto.h = GROSSURA_PONTO;
        ponto.w = GROSSURA_PONTO;
        ponto.x = (_posicao_x)[i] - GROSSURA_PONTO/2;
        ponto.y= (_posicao_y)[i] - GROSSURA_PONTO/2;
        if(current == estacao_escolhida)
        {   //na estação escolhida desenha um quadrado verde
            SDL_SetRenderDrawColor(renderer, 50, 141, 44, 0); //verde
            SDL_RenderFillRect(renderer, &ponto);
            SDL_SetRenderDrawColor(renderer, 176, 17, 21, 0); //vermelho
        }
        else
            SDL_RenderFillRect(renderer, &ponto);
        current = current->next;
        i++;
    }
}

//lida com o desenho das rotas no mapa
void desenharRotas(estacao_node *head_estacao,viagem_node *head_viagem,estacao_node *estacao_selecionada,
                   SDL_Renderer *renderer, int *posicao_x, int *posicao_y)
{

    //rotas de e para a estação selecionada
    if(estacao_selecionada != NULL)
    {
        desenhar_rotas_estacao_selec(head_estacao, head_viagem, estacao_selecionada, renderer, posicao_x, posicao_y);
    }
    //rotas de e para todas as estações (comentado de propósito - o programa fica lento!)
    /*
    else
    {
        estacao_node *aux = NULL;
        aux = head_estacao;
        while(aux != NULL)
        {
            desenhar_rotas_estacao_selec(head_estacao, head_viagem, aux, renderer, posicao_x, posicao_y);
            aux = aux->next;
        }

    }*/

}

//desenha no ecrã retas correspondentes às rotas entre estações; a "espessura" da reta é determinado pelo numero de viagens
//dessa rota
void desenhar_rotas_estacao_selec(estacao_node *head_estacao,viagem_node *head_viagem,estacao_node *estacao_selecionada,
                   SDL_Renderer *renderer, int *posicao_x, int *posicao_y)
{
    int contagem_estacoes = 0;
    int *num_viagens_chegada = NULL;
    int *num_viagens_partida = NULL;
    estacao_node *current = head_estacao;
    int current_index = 0;
    int estacao_selecionada_index = 0;
    int i = 0;
    current = head_estacao;
    while(current != estacao_selecionada)
    {
       estacao_selecionada_index++;
       current = current->next;
    }

    contagem_viagens_rotas(&contagem_estacoes, head_estacao, estacao_selecionada, head_viagem, &num_viagens_chegada, &num_viagens_partida);
    current = head_estacao;

    while(current != NULL)
    {   //a "espessura" é igual ao logaritmo do número de viagens+1 arredondado por excesso.
        //desta forma, se houver pelo menos uma viagem numa rota, irá haver pelo menos uma reta; quantas mais retas,
        //mais viagens existem nessa rota e o ecrâ não fica demasiado cheio de retas
        for(i = 0; i < ceil(log(num_viagens_chegada[current_index] + 1)); i++)
        {
            //rotas de chegada
            SDL_SetRenderDrawColor(renderer, CORCHEGADA_R, CORCHEGADA_G, CORCHEGADA_B, CORCHEGADA_A);
            SDL_RenderDrawLine(renderer, posicao_x[current_index]  - i, posicao_y[current_index] - i,
                               posicao_x[estacao_selecionada_index]  - i, posicao_y[estacao_selecionada_index]  - i );
        }
        for(i = 0; i < ceil(log(num_viagens_partida[current_index] + 1)); i++)
        {
            //rotas de partida
            SDL_SetRenderDrawColor(renderer, CORPARTIDA_R, CORPARTIDA_G, CORPARTIDA_B, CORPARTIDA_A);
            SDL_RenderDrawLine(renderer, posicao_x[current_index]  + i, posicao_y[current_index] + i,
                               posicao_x[estacao_selecionada_index]  + i, posicao_y[estacao_selecionada_index]  + i );
        }
        current = current->next;
        current_index++;

    }
}


/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface **_boston_map, SDL_Surface **_logoIST, SDL_Surface **_logoBS, SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    *_boston_map = IMG_Load("boston_map.jpg");
    if (*_boston_map == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //load IST logo
    *_logoIST = SDL_LoadBMP("ist_logo.bmp");
    if (*_logoIST == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //load BS logo
    *_logoBS= SDL_LoadBMP("BS.bmp");
    if (*_logoBS == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "BikeStation", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width, height );

	return renderer;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderText_Blended(_font,text,*_color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}



