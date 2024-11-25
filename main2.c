
#include "acelerometro.c"
#include "gpu_lib.h"
#include "pthread.h"
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define SIZE2 20
#define SIZE1 15


void invert_map(int size1,int size2,char map[size1][size2]);
void mudarCorFundo(int linhas, int colunas,char matriz[linhas][colunas],int cor);
void mudarCorGenerico(int linhas, int colunas,char matriz[linhas][colunas],int cor);
void iniciarJogo(char map[SIZE1][SIZE2],char mapa2[SIZE1][SIZE2]);
void encerrarJogo();

typedef struct TPacman{
    int status;
    int x,y,xi,yi,xj,yj;
    int direcao,passo,parcial;
    int pontos;
    int invencivel;
    int vivo;
    int animacao;
}Pacman;


int pacman_vivo(Pacman *pac);
Pacman* pacman_create(int x, int y);
void desenhar_jogo(char mapa2[SIZE1][SIZE2]);
int pacman_vivo(Pacman *pac);
void pacman_destroy(Pacman *pac);
void posicionarPacman(int x, int y,char mapa2[SIZE1][SIZE2]);
void pontuaVerif(Pacman* pac,char mapa2[SIZE1][SIZE2]);
void pacman_AlteraDirecao(Pacman* pac,int direcao,char mapa2[SIZE1][SIZE2]);
void pacman_movimenta(Pacman* pac,char mapa2[SIZE1][SIZE2]);

Pacman *pac;
int gameState = 0;

int main(){
   
    char map[SIZE1][SIZE2] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
            {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };  
    char mapa2[SIZE1][SIZE2] = {{0}};
    int start,i,j;

    printf("iniciar jogo?\n");
    scanf("%d",&start);
    
    iniciarJogo(map,mapa2);
    pac = pacman_create(1,1);
    posicionarPacman(1,1,mapa2);
    ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1);
    pacman_AlteraDirecao(pac,1,mapa2);

    // while (gameState!=2)
    for (i = 0; i < 20; i++)
    {
        pacman_movimenta(pac, mapa2);

        pacman_desenha(pac, mapa2);
        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
        printf("x = %d,xj = %d,y = %d, yj = %d, direcao = %d \n", pac->x, pac->xj, pac->y, pac->yj, pac->direcao);
    }
    pacman_AlteraDirecao(pac, 1, mapa2);
    printf("trocou direcao %d \n", pac->direcao);

    for (i = 0; i < 6; i++)
    {
        pacman_movimenta(pac, mapa2);
        pacman_desenha(pac, mapa2);
        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
        printf("x = %d,xj = %d,y = %d, yj = %d \n, direcao = %d", pac->x, pac->xj, pac->y, pac->yj, pac->direcao);
    }

    pacman_AlteraDirecao(pac, 3, mapa2);
    printf("trocou direcao %d \n", pac->direcao);
    for (i = 0; i < 10; i++)
    {
        pacman_movimenta(pac, mapa2);

        pacman_desenha(pac, mapa2);
        usleep(80000);

        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
        printf("x = %d,xj = %d,y = %d, yj = %d,direcao = %d \n", pac->x, pac->xj, pac->y, pac->yj,pac->direcao);
    }
    pacman_AlteraDirecao(pac, 1, mapa2);
    printf("trocou direcao %d \n", pac->direcao);
    for (i = 0; i < 20; i++)
    {
        pacman_movimenta(pac, mapa2);
        pacman_desenha(pac, mapa2);
        usleep(80000);

        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
        printf("x = %d,xj = %d,y = %d, yj = %d, direcao =  %d \n", pac->x, pac->xj, pac->y, pac->yj,pac->direcao);
    }
    pacman_AlteraDirecao(pac, 3, mapa2);
    printf("trocou direcao %d \n", pac->direcao);
    for (i = 0; i < 20; i++)
    {
        pacman_movimenta(pac, mapa2);

        pacman_desenha(pac, mapa2);
        usleep(80000);

        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
        printf("x = %d,xj = %d,y = %d, yj = %d,direcao = %d \n", pac->x, pac->xj, pac->y, pac->yj,pac->direcao);
    }
    pacman_AlteraDirecao(pac, 2, mapa2);
    printf("trocou direcao %d \n", pac->direcao);
    for (i = 0; i < 20; i++)
    {
        pacman_movimenta(pac, mapa2);

        pacman_desenha(pac, mapa2);
        usleep(80000);

        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
        printf("x = %d,xj = %d,y = %d, yj = %d,direcao = %d \n", pac->x, pac->xj, pac->y, pac->yj,pac->direcao);
    }



    print_map(mapa2);
    encerrarJogo();
    
    desmapear_gpu();
    return 0;
}

//FUNCOES JOGO
void iniciarJogo(char map[SIZE1][SIZE2],char mapa2[SIZE1][SIZE2]){
    mapear_gpu();
    limpar_tela();
    /*matriz de mapa/fundo*/
    mudarCorGenerico(SIZE1, SIZE2, map, 3);    
    ler_matriz(SIZE1, SIZE2, map, 3, 0, 0, 3); 
    
    /*matriz de controle/pontos*/
    copiarMatriz(SIZE1, SIZE2, mapa2, map);
    invert_map(SIZE1, SIZE2, mapa2);
    /*neste ponto havera uma funcao que varre a matriz de controle e calcula qual será a quantidade de pontos necessaria para que pacman ganhe*/
    mudarCorGenerico(SIZE1, SIZE2, mapa2, 2);
    mudarCorFundo(SIZE1, SIZE2, mapa2, 9);
    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
    
    ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1);
    gameState = 1;

}


void encerrarJogo(){
    gameState = 2;
}



void desenhar_jogo(char mapa2[SIZE1][SIZE2]){/*por enquanto sem implementação de sprites*/
   ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1); 
}

//FUNCOES PACMAN

// Função que inicializa os dados associados ao pacman
Pacman* pacman_create(int x, int y){

    Pacman* pac = malloc(sizeof(Pacman));
    if(pac != NULL){
        pac->invencivel = 0;
        pac->pontos = 0;
        pac->passo = 4;
        pac->vivo = 1;
        pac->status = 0;
        pac->direcao = 0;/*vou definir que 0 = parado; 1 = direita,2 = esquerda,3 = p/ cima,4 = p/baixo*/
        pac->parcial = 0;
        pac->x = x;
        pac->y = y;
        pac->xi = x;
        pac->yi = y;
        pac->xj = x;
        pac->yj = y;
    

}
    return pac;
}
int pacman_vivo(Pacman *pac){
    if(pac->vivo)
        return 1;
    else{
        return 0;
    }
}

void pacman_destroy(Pacman *pac){
    free(pac);
}

void posicionarPacman(int x, int y,char mapa2[SIZE1][SIZE2]){
   pac->x = x;
   pac->y = y;
   mapa2[x][y] = 6;/*Numero que representa o pacman na matriz de controle(mapa2)*/
}

void pontuaVerif(Pacman* pac,char mapa2[SIZE1][SIZE2]){
    if (mapa2[pac->xj][pac->yj]==2)
    {
        mapa2[pac->xi][pac->yi]=0;/*atualiza o valor da posicao anterior de pac caso ele pontue*/
        pac->pontos +=10;
    }
}

void pacman_AlteraDirecao(Pacman* pac,int direcao,char mapa2[SIZE1][SIZE2]){
    switch (pac->direcao)
    {
    case 0:
        pac->direcao = direcao;
        break;
    case 1:
        if (mapa2[(pac->x)+1][pac->y]<9)
        {
            pac->direcao = direcao;   
        }
        break;
    case 2:
        if (mapa2[(pac->x)-1][pac->y]<9)
        {
            pac->direcao = direcao;
        }
        break;
    case 3:
        if (mapa2[(pac->x)][pac->y-1]<9)
        {
            pac->direcao = direcao;
        }
        break;
    case 4:
        if (mapa2[(pac->x)][pac->y+1]<9)
        {
            pac->direcao = direcao;
        }
        break;
    default:
        pac->direcao =0;
        break;
    }
}
void pacman_movimenta(Pacman* pac,char mapa2[SIZE1][SIZE2]){
    
    if (pac->vivo == 0)
        return;

    switch (pac->direcao)
    {
    case 0:
            //pac->xi = pac->x;/*ainda nao sei a necessidade de zerar esses*/
            //pac->yi = pac->y;
            pac->xj = pac->x; 
            pac->yj = pac->y;
            pontuaVerif(pac, mapa2);
            posicionarPacman(((pac->x)), (pac->y), mapa2);

        break;
    case 3:
        if (mapa2[(pac->x) + 1][pac->y] < 9)
        {
            pac->xi = pac->x;
            pac->yi = pac->y;

            pac->xj = pac->x + 2;
            pac->yj = pac->y;

            pontuaVerif(pac, mapa2);
            posicionarPacman(((pac->x) + 1), (pac->y), mapa2);
        }
        else
        {
            pac->direcao = 0;

        }
        break;
    case 4:
        if (mapa2[(pac->x) - 1][pac->y] < 9)
        {
            pac->xi = pac->x;
            pac->yi = pac->y;
            pac->xj = pac->x - 2;
            pac->yj = pac->y;

            pontuaVerif(pac, mapa2);
            posicionarPacman(((pac->x) - 1), (pac->y), mapa2);
        }
        else
        {
            pac->direcao = 0;
        }
        break;
    case 2:
        if (mapa2[(pac->x)][(pac->y) - 1] < 9)
        {
            pac->xi = pac->x;
            pac->yi = pac->y;
            pac->xj = pac->x;
            pac->yj = pac->y - 2;

            pontuaVerif(pac, mapa2);
            posicionarPacman(((pac->x)), (pac->y - 1), mapa2);
        }
        else
        {
            pac->direcao = 0;
        }

        break;
    case 1:
        if (mapa2[(pac->x)][(pac->y) + 1] < 9)
        {
            pac->xi = pac->x;
            pac->yi = pac->y;
            pac->xj = pac->x;
            pac->yj = pac->y + 2;

            pontuaVerif(pac, mapa2);
            posicionarPacman(((pac->x)), (pac->y + 1), mapa2);
        }
        else
        {
            pac->direcao = 0;
        }
        break;
    }
    mapa2[(pac->xi)][pac->yi] = 0;
}

int temParede(Pacman *pac, char mapa2[SIZE1][SIZE2])
{/* essa funcao nao esta sendo utilizada */
    if (mapa2[pac->x + 1][pac->y] < 9 && pac->direcao == 3)
    {
        return 0;
    }
    else if (mapa2[pac->x][pac->y + 1] < 9 && pac->direcao == 1)
    {
        return 0;
    }
    else if (mapa2[pac->x - 1][pac->y] < 9 && pac->direcao == 4)
    {
        return 0;
    }
    else if (mapa2[pac->x][pac->y - 1] < 9 && pac->direcao == 2)
    {
        return 0;
    }
    printf("tem parede");
    return 1;
}

void pacman_desenha(Pacman* pac){
     /*funcao responsavel pela animacao de pacman*/
    /*por enquanto nao utiliza sprites*/
    int i;
   /* 
    if (temParede(pac, mapa2))
    {
        printf("achou parede e nao desenhou");
        return;
    }*/
    

    if (pac->direcao == 0)
    {/*espero que conserte o problema de atravessar parede*/
        return;
    }
    
    if (pac->direcao == 1 && pac->yj < 18 && pac->yj > 0 && pac->y != pac->yj && mapa2[pac->x][pac->yj]<9)
    { /*move para frente*/
        for (i = 0; i < (pac->passo) - 1; i++)
        {
            {
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi) * 3, 0, 0, 0, 1);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi + 1) * 3, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi + 1) * 3, 0, 0, 0, 1);
            }
        }
    }

    if (pac->direcao == 2 && pac->yj > 0 && pac->yj < 18 && pac->y != pac->yj && mapa2[pac->x][pac->yj]<9)
    { /*move para tras*/
        for (i = (pac->passo) - 1; i > 0; i--)
        {
            {
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi) * 3, 0, 0, 0, 1);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi - 1) * 3, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi - 1) * 3, 0, 0, 0, 1);
            }
        }
    }

    if (pac->direcao == 3 && pac->xj < 13 && pac->xj > 0 && pac->x != pac->xj && mapa2[pac->xj][pac->y]<9)
    { /*move para baixo*/
        for (i = 0; i < (pac->passo) - 1; i++)
        {
            {
                desenhar_quadrado((i + 1) + (pac->xi) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                desenhar_quadrado((i + 1) + (pac->xi + 1) * 3, ((pac->y) * 3) + 1, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado((i + 1) + (pac->xi + 1) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
            }
        }
    }

    if (pac->direcao == 4 && pac->xj > 0 && pac->xj < 13 && pac->x != pac->xj && mapa2[pac->xj-1][pac->y]<9)
    { /*move para cima*/
        for (i = (pac->passo) - 1; i > 0; i--)
        {
            {
                desenhar_quadrado((i + 1) + (pac->xi) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                desenhar_quadrado((i + 1) + (pac->xi - 1) * 3, ((pac->y) * 3) + 1, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado((i + 1) + (pac->xi - 1) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
            }
        }
    }
}

void trocarStatus(Pacman *pac){/*status diz qual sprite sera utilizado*/
    /*funcao ainda nao implementada*/
    pac->status = 1- pac->status;
}


//FUNCOES AUXILIARES
void print_map(char map[SIZE1][SIZE2]) {
    for (int i = 0; i < SIZE1; i++) {
        for (int j = 0; j < SIZE2; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}
void mudarCorFundo(int linhas, int colunas, char matriz[linhas][colunas], int cor)
{
    int i, j;
    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            if (matriz[i][j] == 0)
            {
                matriz[i][j] = cor;
            }
        }
    }
}

void mudarCorGenerico(int linhas, int colunas, char matriz[linhas][colunas], int cor)
{
    int i, j;
    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            if (matriz[i][j] == 1)
            {
                matriz[i][j] = cor;
            }
        }
    }
}

void print_map(char map[SIZE1][SIZE2])
{
    for (int i = 0; i < SIZE1; i++)
    {
        for (int j = 0; j < SIZE2; j++)
        {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}
