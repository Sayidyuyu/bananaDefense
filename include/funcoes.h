// Este header contem as funcoes para o programa principal, separar por tipo posteriormente?
#include "raylib.h"
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

//----------------------------------------------------------------------------------
// Protótipos das funções
//----------------------------------------------------------------------------------
void desenhaPlayer(TIPO_PLAYER *player, int posx, int posy);
void desenhaInimigo(TIPO_INIMIGO inimigo[MAX_INIMIGOS], int dx, int dy);
void move(COORDENADAS *entidade, char *matriz, char letra);
void sentidoAleatorioInimigo(TIPO_INIMIGO *inimigo);
int deveMover(COORDENADAS *entidade, char *matriz);
void controleJogador(TIPO_PLAYER *entidade, char *matriz);
void moveInimigo(TIPO_INIMIGO *inimigo, char *matriz, BASE *base, int *qtdInimigo);
void redefineDeslocamentoInimigo(TIPO_INIMIGO *inimigo, char *matriz, BASE *base);
void centerWindow(float windowWidth, float windowHeight);

//----------------------------------------------------------------------------------
// Definição das funções
//----------------------------------------------------------------------------------

// Funcao que atribui os valores iniciais ao player
void inicializaPlayer(TIPO_PLAYER *player)
{
    player->vidas = 3;   // Inicializa a vida do player
    player->cor = GREEN; // Atribui cor ao player
    player->letra = 'J'; // Letra que representa o player
    player->recursos = 0;
    player->vidas = 1;
}

// Funcao que atribui os valores inciciais ao inimigo
void inicializaInimigo(TIPO_INIMIGO *inimigo)
{
    inimigo->vidas = 1;   // Inicializa a vida do inimigo
    inimigo->cor = RED;   // Atribui cor ao inimigo
    inimigo->letra = 'M'; // Letra que representa o inimigo
    inimigo->timer = 0;   // Inicializa o timer do inimigo

    // Atribui os ultimos valores de movimento do inimigo
    inimigo->ultimoMovimentoX = 0;
    inimigo->ultimoMovimentoY = 0;
    inimigo->coordInimigo.dx = 0; // Inicializa o deslocamento x do inimigo
    inimigo->coordInimigo.dy = 0; // Inicializa o deslocamento y do inimigo
}

void inicializaBase(BASE *base)
{
    base->vidas = 3;  // Inicializa a vida da base
    base->cor = BLUE; // Atribui cor a base
}

void verificaVidas(BASE *base, TIPO_PLAYER *player, GAMESCREEN *tela)
{
    if (base->vidas == 0 || player->vidas == 0)
    {
        *tela = GAMEOVER;
    }
}

// Funcao que desenha o player na tela
void desenhaPlayer(TIPO_PLAYER *player, int posx, int posy)
{
    player->cor = GREEN; // Atribui cor ao player
    player->letra = 'J'; // Letra que representa o player

    // Coordenadas do player sao atribuidas
    player->coordPlayer.x = posx;
    player->coordPlayer.y = posy;

    DrawRectangle(player->coordPlayer.x * LADO, player->coordPlayer.y * LADO, LADO, LADO, player->cor); // Desenha player
}

// Funcao que desenha o inimigo na tela
void desenhaInimigo(TIPO_INIMIGO *inimigo, int posx, int posy)
{

    // Coordenadas do inimigo sao atribuidas
    inimigo->coordInimigo.x = posx;
    inimigo->coordInimigo.y = posy;
    DrawRectangle(inimigo->coordInimigo.x * LADO, inimigo->coordInimigo.y * LADO, LADO, LADO, inimigo->cor); // Desenha inimigo
}

// Funcao que define o sentido aleatorio do inimigo
void sentidoAleatorioInimigo(TIPO_INIMIGO *inimigo)
{
    // Gerar sentido aleatorio:

    // srand(time(NULL)); // seed atrelada ao tempo do computador, deixa mais devagar.

    // Ao subtrair -1 de um numero aleatorio entre 0 e 2, gera um numero entre -1 e 1
    // Variaveis contendo o sentido de deslocamento do inimigo.
    int numeroRandomDx = (rand() % 3) - 1; // Numero entre -1 e 1
    int numeroRandomDy = (rand() % 3) - 1; // Numero entre -1 e 1

    if (numeroRandomDx != 0)
        numeroRandomDy = 0;
    else if (numeroRandomDy != 0)
        numeroRandomDx = 0;

    // Se o numeroRandom for 0, deve-se retirar outro numero aleatorio
    while (numeroRandomDx == 0 && numeroRandomDy == 0)
    {
        numeroRandomDx = (rand() % 3) - 1;
        numeroRandomDy = (rand() % 3) - 1;

        if (numeroRandomDx != 0)
            numeroRandomDy = 0;
        else if (numeroRandomDy != 0)
            numeroRandomDx = 0;
    }

    // Define o deslocameto aleatorio do inimigo
    inimigo->coordInimigo.dx = numeroRandomDx;
    // inimigo->ultimoMovimentoX = numeroRandomDx;

    inimigo->coordInimigo.dy = numeroRandomDy;
    // inimigo->ultimoMovimentoY = numeroRandomDy;
}

void contaRecursos (TIPO_PLAYER *player) {
    (player->recursos)++;
}

// Funcao que verifica se a entidade deve mover.
int deveMoverPlayer (TIPO_PLAYER *entidade, char *matriz)
{
    // Verifica se a entidade está dentro dos limites da tela
    if (entidade->coordPlayer.x == (LARGURA / LADO - 1) && entidade->coordPlayer.dx == 1)
        return 0;
    if (entidade->coordPlayer.x == 0 && entidade->coordPlayer.dx == -1)
        return 0;
    if (entidade->coordPlayer.y == (ALTURA / LADO - 1) && entidade->coordPlayer.dy == 1)
        return 0;
    if (entidade->coordPlayer.y == 0 && entidade->coordPlayer.dy == -1)
        return 0;

    // Verifica se a entidade está parada
    if (entidade->coordPlayer.dx == 0 && entidade->coordPlayer.dy == 0)
        return 0;

    // Verifica se entidade está em movimento diagonal
    if (entidade->coordPlayer.dx != 0 && entidade->coordPlayer.dy != 0)
        return 0;

    // Verifica se a entidade colidiu com algum obstaculo
    if (*(matriz + (entidade->coordPlayer.x + entidade->coordPlayer.dx) + (entidade->coordPlayer.y + entidade->coordPlayer.dy) * (LARGURA / LADO)) == 'W')
    {
        return 0;
    }
    if (*(matriz + (entidade->coordPlayer.x + entidade->coordPlayer.dx) + (entidade->coordPlayer.y + entidade->coordPlayer.dy) * (LARGURA / LADO)) == 'M')
    {
        return 0;
    }
    if (*(matriz + (entidade->coordPlayer.x + entidade->coordPlayer.dx) + (entidade->coordPlayer.y + entidade->coordPlayer.dy) * (LARGURA / LADO)) == 'S')
        return 0;
    
    if (*(matriz + (entidade->coordPlayer.x + entidade->coordPlayer.dx) + (entidade->coordPlayer.y + entidade->coordPlayer.dy) * (LARGURA / LADO)) == 'O')
        return 0;

    if (*(matriz + (entidade->coordPlayer.x + entidade->coordPlayer.dx) + (entidade->coordPlayer.y + entidade->coordPlayer.dy) * (LARGURA / LADO)) == 'R') {
        contaRecursos(entidade);
    }

    if (*(matriz + (entidade->coordPlayer.x + entidade->coordPlayer.dx) + (entidade->coordPlayer.y + entidade->coordPlayer.dy) * (LARGURA / LADO)) == 'H') {
        int vaix = entidade->coordPlayer.x;
        int vaiy = entidade->coordPlayer.y;
        int i;
        //Deslocamento no eixo Y
        if (entidade->coordPlayer.dy == 1 || entidade->coordPlayer.dy == -1) {
            if (entidade->coordPlayer.dy == -1) {
                for (i=0; i<entidade->coordPlayer.y; i++) {
                    if (matriz[i * (LARGURA/LADO) + entidade->coordPlayer.x] == 'H') {
                        vaiy = i;
                    }
                }
            }
            else if (entidade->coordPlayer.dy == 1) {
                for (i=entidade->coordPlayer.y+1; i<ALTURA/LADO; i++) {
                    if (matriz[i * (LARGURA/LADO) + entidade->coordPlayer.x] == 'H') {
                        vaiy = i;
                    }
                }
            }
        }

        //Deslocamento no eixo X
        if (entidade->coordPlayer.dx == 1 || entidade->coordPlayer.dx == -1) {
            if (entidade->coordPlayer.dx == 1) {
                for (i=0; i<LARGURA/LADO; i++) {
                    if (matriz[entidade->coordPlayer.y * (LARGURA/LADO) + i] == 'H') {
                        vaix = i;
                    }
                }
            }
             else if (entidade->coordPlayer.dx == -1) {
                for (i=0; i<entidade->coordPlayer.x; i++){
                    if (matriz[entidade->coordPlayer.y * (LARGURA/LADO) + i] == 'H'){
                        vaix = i;
                    }
                }
            }
        }

        entidade->coordPlayer.y = vaiy;
        entidade->coordPlayer.x = vaix;
    }
    return 1;
}

// Funcao que verifica se a entidade deve mover.
int deveMover(COORDENADAS *entidade, char *matriz)
{
    // Verifica se a entidade está dentro dos limites da tela
    if (entidade->x == (LARGURA / LADO - 1) && entidade->dx == 1)
        return 0;
    if (entidade->x == 0 && entidade->dx == -1)
        return 0;
    if (entidade->y == (ALTURA / LADO - 1) && entidade->dy == 1)
        return 0;
    if (entidade->y == 0 && entidade->dy == -1)
        return 0;

    // Verifica se a entidade está parada
    if (entidade->dx == 0 && entidade->dy == 0)
        return 0;

    // Verifica se entidade está em movimento diagonal
    if (entidade->dx != 0 && entidade->dy != 0)
        return 0;

    // Verifica se a entidade colidiu com algum obstaculo
    if (*(matriz + (entidade->x + entidade->dx) + (entidade->y + entidade->dy) * (LARGURA / LADO)) == 'W')
    {
        return 0;
    }

    // Verifica se a entidade colidiu com algum recurso
    if (*(matriz + (entidade->x + entidade->dx) + (entidade->y + entidade->dy) * (LARGURA / LADO)) == 'R')
    {
        return 1;
    }

    // Verifica se a entidade colidiu com algum inimigo
    if (*(matriz + (entidade->x + entidade->dx) + (entidade->y + entidade->dy) * (LARGURA / LADO)) == 'M')
    {
        return 0;
    }
    // Verifica se a entidade colidiu com a base
    if (*(matriz + (entidade->x + entidade->dx) + (entidade->y + entidade->dy) * (LARGURA / LADO)) == 'S')
        return 0;

    // Verifica se a entidade colidiu com jogador
    if (*(matriz + (entidade->x + entidade->dx) + (entidade->y + entidade->dy) * (LARGURA / LADO)) == 'J')
        return 0;
    return 1;
}

int deveMoverInimigo(TIPO_INIMIGO *inimigo, char *matriz, BASE *base, int *qtdInimigo)
{
    // Verifica se a entidade está dentro dos limites da tela
    if (inimigo->coordInimigo.x == (LARGURA / LADO - 1) && inimigo->coordInimigo.dx == 1)
        return 0;
    if (inimigo->coordInimigo.x == 0 && inimigo->coordInimigo.dx == -1)
        return 0;
    if (inimigo->coordInimigo.y == (ALTURA / LADO - 1) && inimigo->coordInimigo.dy == 1)
        return 0;
    if (inimigo->coordInimigo.y == 0 && inimigo->coordInimigo.dy == -1)
        return 0;

    // Verifica se a entidade está parada
    if (inimigo->coordInimigo.dx == 0 && inimigo->coordInimigo.dy == 0)
        return 0;

    // Verifica se a entidade colidiu com algum obstaculo
    if (*(matriz + (inimigo->coordInimigo.x + inimigo->coordInimigo.dx) + (inimigo->coordInimigo.y + inimigo->coordInimigo.dy) * (LARGURA / LADO)) == 'W')
    {
        inimigo->vidas = 0;
        return 0;
    }
    // Verifica se a entidade colidiu com algum recurso
    if (*(matriz + (inimigo->coordInimigo.x + inimigo->coordInimigo.dx) + (inimigo->coordInimigo.y + inimigo->coordInimigo.dy) * (LARGURA / LADO)) == 'R')
        return 0;

    // Verifica se a entidade colidiu com algum inimigo
    if (*(matriz + (inimigo->coordInimigo.x + inimigo->coordInimigo.dx) + (inimigo->coordInimigo.y + inimigo->coordInimigo.dy) * (LARGURA / LADO)) == 'M')
        return 0;

    // Verifica se a entidade colidiu com a base
    if (*(matriz + (inimigo->coordInimigo.x + inimigo->coordInimigo.dx) + (inimigo->coordInimigo.y + inimigo->coordInimigo.dy) * (LARGURA / LADO)) == 'S')
    {
        *qtdInimigo = *qtdInimigo - 1; // Quantidade de inimigos diminui
        inimigo->vidas = 0;            // Inimigo desaparece
        base->vidas = base->vidas - 1; // Vida da base diminui
        matriz += inimigo->coordInimigo.x + inimigo->coordInimigo.y * (LARGURA / LADO);
        *matriz = ' ';                // Inimigo some
        inimigo->coordInimigo.x = -1; // Inimigo some
        inimigo->coordInimigo.y = -1; // Inimigo some
        moveInimigo(inimigo, matriz, base, qtdInimigo);

        return 0; // Inimigo nao se move para não apagar base
    }

    // Verifica se a entidade colidiu com jogador, inimigo perde vida?
    if (*(matriz + (inimigo->coordInimigo.x + inimigo->coordInimigo.dx) + (inimigo->coordInimigo.y + inimigo->coordInimigo.dy) * (LARGURA / LADO)) == 'J')
        return 0;

    return 1;
}

void move(COORDENADAS *entidade, char *matriz, char letra)
{
    //  Realiza a movimentação
    matriz += entidade->x + entidade->y * (LARGURA / LADO);
    if (*matriz != 'O')
        *matriz = ' ';

    entidade->x = entidade->x + entidade->dx;
    entidade->y += entidade->dy;

    matriz += entidade->dx + entidade->dy * (LARGURA / LADO);
    *matriz = letra;
}

// Funcao que move o inimigo
void moveInimigo(TIPO_INIMIGO *inimigo, char *matriz, BASE *base, int *qtdInimigo)
{

    double tempoAtual = GetTime(); // Captura o tempo atual

    double timerInimigo = inimigo->timer; // Captura o timer do inimigo

    // Se o tempo decorrido desde o ultimo movimento inimigo for maior ou igual ao delay
    if (tempoAtual - timerInimigo >= INIMIGO_DELAY)
    {

        inimigo->timer = tempoAtual; // Atualiza o timer do inimigo para o tempo atual

        // Verifica se inimigo deve mover
        if (deveMoverInimigo(inimigo, matriz, base, qtdInimigo))
        {
            move(&inimigo->coordInimigo, matriz, inimigo->letra);
            inimigo->ultimoMovimentoX = inimigo->coordInimigo.dx;
            inimigo->ultimoMovimentoY = inimigo->coordInimigo.dy;
        }
        else
        {

            redefineDeslocamentoInimigo(inimigo, matriz, base);
        }
    }
}

float distanciaAteBase(COORDENADAS *entidade, BASE *base)
{
    int distanciaX = 0;
    int distanciaY = 0;

    distanciaX = abs(entidade->x - base->coordBase.x);
    distanciaY = abs(entidade->y - base->coordBase.y);

    return (float)hypot(distanciaX, distanciaY);
}

// Funcao que redefine o deslocamento do inimigo
void redefineDeslocamentoInimigo(TIPO_INIMIGO *inimigo, char *matriz, BASE *base)
{

    // Verificar negativos

    //  caso dx e dy sejam iguais a 0, o inimigo está parado
    if (inimigo->coordInimigo.dx == 0 && inimigo->coordInimigo.dy == 0)
    {
        inimigo->coordInimigo.dx = 1; // Inimigo deslocamento x igual a -1
        if (!deveMover(&inimigo->coordInimigo, matriz))
            inimigo->coordInimigo.dx = -1;
    }

    if (inimigo->ultimoMovimentoX == -1) // Se ultimo movimento foi para esquerda
    {
        // Inimigo deslocamento x igual a 0, pois inimigo nao pode ir para direita e houve uma colisao na esquerda
        inimigo->coordInimigo.dx = 0;
        inimigo->coordInimigo.dy = 1;                   // Inimigo deslocamento y igual a 1
        if (!deveMover(&inimigo->coordInimigo, matriz)) // Verifica se inimigo pode mover para cima
            inimigo->coordInimigo.dy = -1;              // Se nao, move para baixo
    }
    if (inimigo->ultimoMovimentoY == -1) // ultimo movimento pra baixo
    {
        inimigo->coordInimigo.dx = 1;
        inimigo->coordInimigo.dy = 0;
        if (!deveMover(&inimigo->coordInimigo, matriz))
            inimigo->coordInimigo.dx = -1;
    }
    // Verifica positivos
    if (inimigo->ultimoMovimentoX == 1)
    {
        inimigo->coordInimigo.dx = 0;
        inimigo->coordInimigo.dy = 1;
        if (!deveMover(&inimigo->coordInimigo, matriz))
            inimigo->coordInimigo.dy = -1;
    }
    if (inimigo->ultimoMovimentoY == 1)
    {
        inimigo->coordInimigo.dx = -1;
        inimigo->coordInimigo.dy = 0;
        if (!deveMover(&inimigo->coordInimigo, matriz))
            inimigo->coordInimigo.dx = 1;
    }
}

// Funcao que trata controle do jogador
void controleJogador(TIPO_PLAYER *entidade, char *matriz)
{
    entidade->cor;

    if (IsKeyPressed(KEY_RIGHT)) // Verifica tecla pressionada
    {
        entidade->coordPlayer.dx = 1;
        entidade->coordPlayer.dy = 0;
        if (deveMoverPlayer(entidade, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }

    if (IsKeyPressed(KEY_LEFT))
    {
        entidade->coordPlayer.dx = -1;
        entidade->coordPlayer.dy = 0;
        if (deveMoverPlayer(entidade, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }

    if (IsKeyPressed(KEY_UP))
    {
        entidade->coordPlayer.dx = 0;
        entidade->coordPlayer.dy = -1;
        if (deveMoverPlayer(entidade, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        entidade->coordPlayer.dx = 0;
        entidade->coordPlayer.dy = 1;
        if (deveMoverPlayer(entidade, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }
}

/*void controleJogador(TIPO_PLAYER *entidade, char *matriz)
{
    entidade->cor;

    if (IsKeyPressed(KEY_RIGHT)) // Verifica tecla pressionada
    {
        entidade->coordPlayer.dx = 1;
        entidade->coordPlayer.dy = 0;
        if (deveMover(&entidade->coordPlayer, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }

    if (IsKeyPressed(KEY_LEFT))
    {
        entidade->coordPlayer.dx = -1;
        entidade->coordPlayer.dy = 0;
        if (deveMover(&entidade->coordPlayer, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }

    if (IsKeyPressed(KEY_UP))
    {
        entidade->coordPlayer.dx = 0;
        entidade->coordPlayer.dy = -1;
        if (deveMover(&entidade->coordPlayer, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        entidade->coordPlayer.dx = 0;
        entidade->coordPlayer.dy = 1;
        if (deveMover(&entidade->coordPlayer, matriz))
            move(&entidade->coordPlayer, matriz, entidade->letra);
    }
}*/

// Funcao que centraliza a janela ao centro da tela
void centerWindow(float windowWidth, float windowHeight)
{
    int monitor = GetCurrentMonitor();             // Captura o monitor usado
    int monitorWidth = GetMonitorWidth(monitor);   //  Verifica a largura do monitor usado
    int monitorHeight = GetMonitorHeight(monitor); //  Verifica a altura do monitor usado

    //  Define a posição da janela (window) na tela
    SetWindowPosition((int)(monitorWidth / 2) - (int)(windowWidth / 2), (int)(monitorHeight / 2) - (int)(windowHeight / 2));
}

// Funcao que verifica a tela atual do jogo e muda de acordo com a situacao
void verificaTelaJogo(GAMESCREEN *telaAtual, int *deveFechar, GAMESTATUS *estadoDoJogo)
{
    // Modifica a tela atual de acordo com a situacao do jogo
    switch (*telaAtual)
    {

    case TITULO: // Tela titulo do jogo, onde o jogador pode iniciar o jogo
    {
        if (IsKeyPressed(KEY_N)) // Inicia um novo jogo
        {
            // inicializaPlayer(&player);
            *telaAtual = GAMEPLAY;
        }

        if (IsKeyPressed(KEY_C)) // Carrega o jogo
        {
            *telaAtual = GAMEPLAY;
        }
        if (IsKeyPressed(KEY_O)) // Tela de game over
        {
            *telaAtual = GAMEOVER;
        }
        if (IsKeyPressed(KEY_Q)) // Sai do jogo
        {
            *deveFechar = 1;
        }
    }
    break;

    case GAMEPLAY: // Tela de gameplay, onde o jogador pode jogar
    {
        if (IsKeyPressed(KEY_TAB)) // Abre o menu
        {
            *telaAtual = MENU;
        }
        if (IsKeyPressed(KEY_ESCAPE)) // Abre o menu
        {
            *telaAtual = MENU;
        }
        
    }
    break;

    case MENU: // Tela de menu ou pause, onde o jogador pode retornar ao titulo
    {
        if (IsKeyPressed(KEY_J)) /// Continua o jogo
        {
            *telaAtual = GAMEPLAY;
        }
        if (IsKeyPressed(KEY_C)) // Carrega o jogo
        {
            *telaAtual = GAMEPLAY;
        }
        if (IsKeyPressed(KEY_S)) // Salva o jogo
        {
        }

        if (IsKeyPressed(KEY_V)) // Volta ao titulo
        {
            *telaAtual = TITULO;
        }
    }
    break;

    case GAMEOVER: // Tela de game over, onde o jogador pode retornar ao titulo
    {
        if (IsKeyPressed(KEY_L)) // Carrega o jogo
        {
            *telaAtual = GAMEPLAY;
        }
        if (IsKeyPressed(KEY_R)) // Reinicia o jogo
        {
            *telaAtual = GAMEPLAY;
        }

        if (IsKeyPressed(KEY_V)) // Volta ao titulo
        {
            *telaAtual = TITULO;
        }
    }
    break;

    case VITORIA: // Tela de vitoria, onde o jogador pode retornar ao titulo
    {
        if (IsKeyPressed(KEY_N)) // Carrega o proximo nivel
        {
            estadoDoJogo->nivel++;
            *telaAtual = GAMEPLAY;
        }

        if (IsKeyPressed(KEY_V)) // Volta ao titulo
        {
            *telaAtual = TITULO;
        }
    }
    break;

    default:
        break;
    }
}

int salvaEstado(char nomeDoArquivo[30], GAMESTATUS gameStatus)
{
    int erro = 1;
    FILE *arq;

    if (!(arq = fopen(nomeDoArquivo, "w")))
        erro = 0;
    else if (!(fwrite(&gameStatus, sizeof(GAMESTATUS), 1, arq)))
        erro = 0;
    else
        fclose(arq);

    return erro;
}

int leEstado(char nomeDoArquivo[30], GAMESTATUS *gameStatus)
{
    int erro = 1;
    FILE *arq;

    if (!(arq = fopen(nomeDoArquivo, "r")))
    {
        perror("Erro ao abrir o arquivo para leitura");
        erro = 0;
    }
    else if (!(fread(gameStatus, sizeof(GAMESTATUS), 1, arq)))
    {
        perror("Erro ao ler do arquivo");
        erro = 0;
    }
    else
        fclose(arq);

    return erro;
}

void passaNivel(char fase[], char* nivel){

    strcpy(fase, "src/fases/mapa");		   // Copia o nome do arquivo para a variavel
	strncat(fase, nivel, 1); // Concatena o nome do arquivo com o nivel
	strcat(fase, ".txt");				   // Concatena o nome do arquivo com a extensao
}

// Funcao que desenha na tela
void desenha(int coordX, int coordY, Color cor)
{

    DrawRectangle(coordX * LADO, coordY * LADO, LADO, LADO, cor); // Desenha um retangulo
}

// Funcao que le o mapa do arquivo e conta a quantidade de inimigos
int leMapa(char nomeDoArquivo[30], char *matriz, int *qtdInimigos)
{
    int erro = 1;
    FILE *arq;
    char caractere;

    if (!(arq = fopen(nomeDoArquivo, "r")))
    {
        perror("Erro ao abrir o arquivo para leitura");
        erro = 0;
    }
    else
    {
        while (!(feof(arq)))
        {
            caractere = getc(arq);
            if (caractere != '\n' && caractere != '\0')
            {

                *matriz = caractere;

                // Conta a quantidade de inimigos
                if (caractere == 'M')
                    *qtdInimigos = *qtdInimigos + 1;

                matriz += sizeof(char);
            }
        }
        fclose(arq);
    }

    return erro;
}

void desenhaMapa(char *matriz, TIPO_PLAYER *player, TIPO_INIMIGO *inimigo, BASE *base)
{

    for (int i = 0; i < (ALTURA / 20); i++)
        for (int j = 0; j < LARGURA / 20; j++)
        {
            switch (*matriz)
            {
            case 'J':                        // Player
                desenhaPlayer(player, j, i); // Desenha o player
                break;

            case 'M': // Inimigo
                desenhaInimigo(inimigo, j, i);
                inimigo++; // Quantidade de inimigos soma 1
                break;

            case 'R': // Recurso
                desenha(j, i, PINK);
                break;

            case 'H': // Buraco/Portal
                desenha(j, i, YELLOW);
                break;

            case 'S': // Base
                desenha(j, i, BLUE);
                base->coordBase.x = j;
                base->coordBase.y = i;
                break;

            case 'W': // Parede
                desenha(j, i, PURPLE);
                break;

            case ' ': // Área de trânsito
                desenha(j, i, WHITE);
                break;

            case 'O': // Obstáculo
                desenha(j, i, BLACK);
                break;
            }
            matriz++;
        }
}