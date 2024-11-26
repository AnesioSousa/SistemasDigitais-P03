
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

void invert_map(int size1, int size2, char map[size1][size2]);
void mudarCorFundo(int linhas, int colunas, char matriz[linhas][colunas], int cor);
void mudarCorGenerico(int linhas, int colunas, char matriz[linhas][colunas], int cor);
void iniciarJogo(char map[SIZE1][SIZE2], char mapa2[SIZE1][SIZE2]);
void encerrarJogo();

typedef struct TPacman
{
    int status;
    int x, y, xi, yi, xj, yj;
    int direcao, passo, parcial;
    int pontos;
    int invencivel;
    int vivo;
    int animacao;
} Pacman;

typedef struct TPhantom
{
    int status;
    int x, y, xi, yi, xj, yj;
    int direcao, passo, parcial;
    int pontos;
    int vivo;
    int animacao;
} Phantom;

int pacman_vivo(Pacman *pac);
Pacman *pacman_create(int x, int y);
void desenhar_jogo(char mapa2[SIZE1][SIZE2]);
int pacman_vivo(Pacman *pac);
void pacman_destroy(Pacman *pac);
void posicionarPacman(int x, int y, char mapa2[SIZE1][SIZE2]);
void pontuaVerif(Pacman *pac, char mapa2[SIZE1][SIZE2]);
void pacman_AlteraDirecao(Pacman *pac, int direcao, char mapa2[SIZE1][SIZE2]);
void pacman_movimenta(Pacman *pac, char mapa2[SIZE1][SIZE2]);

Phantom *phantom_create(int x, int y);
void phantom_destroy(Phantom *ph);
void posicionarPhantom(int x, int y, char mapa2[SIZE1][SIZE2]);
void phantom_movimenta(Phantom *ph, char mapa2[SIZE1][SIZE2]);
void phantom_desenha(Phantom *ph, char mapa2[SIZE1][SIZE2]);
void trocarStatusPhantom(Phantom *ph);

/*accel test*/
void inicializacao_accel();
void *accel_working(void *args);
int ACCEL = 1, fd;
I2C_Registers regs;
static int16_t X[1];
//---------------------------------
Pacman *pac;
Phantom *ph;
int gameState = 0;
int pacMaxPts;

char pontuacao_Matriz[5][36];
int main()
{
    pthread_t thread_accel;
    inicializacao_accel();
    pthread_create(&thread_accel, NULL, accel_working, NULL);

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
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
    char mapa2[SIZE1][SIZE2] = {{0}};
    int start, i, j;

    printf("iniciar jogo?\n");
    scanf("%d", &start);

    iniciarJogo(map, mapa2);
    pac = pacman_create(1, 1);
    posicionarPacman(1, 1, mapa2);
    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
    

    while (gameState!=2){
        desenhar_jogo(mapa2);
    }
 

    print_map(mapa2);
    encerrarJogo();

    pthread_join(thread_accel, NULL);
    desmapear_gpu();
    return 0;
}

// FUNCOES JOGO

void iniciarJogo(char map[SIZE1][SIZE2], char mapa2[SIZE1][SIZE2])
{
    mapear_gpu();
    limpar_tela();
    /*matriz de mapa/fundo*/
    mudarCorGenerico(SIZE1, SIZE2, map, 3);
    ler_matriz(SIZE1, SIZE2, map, 3, 0, 0, 3);

    /*matriz de controle/pontos*/
    copiarMatriz(SIZE1, SIZE2, mapa2, map);
    invert_map(SIZE1, SIZE2, mapa2);

    pacMaxPts = contarMaxPts(SIZE1,SIZE2,mapa2); /*define a condição de vitoria de pac*/
    
    mudarCorGenerico(SIZE1, SIZE2, mapa2, 2);
    mudarCorFundo(SIZE1, SIZE2, mapa2, 9);
    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

    gameState = 1;
}

void encerrarJogo()
{/*por enquanto nao diferencia quem ganhou*/
    
    gameState = 2;
}

void desenhar_jogo(char mapa2[SIZE1][SIZE2])
{ /*por enquanto sem implementação de sprites*/
    int di;
    if (X[0] > 20) {
			di =1;
		} else if (X[0] < -20) {
			di=2;
		}
    if (pacman_vivo(pac))
    {/*a condição de morte de pacman esta implementada em phantom_movimenta*/

        pacman_AlteraDirecao(pac, di, mapa2);
        pacman_movimenta(pac, mapa2);
        pacman_desenha(pac, mapa2);

    }else{/*vitoria do fantasma nao implementada*/
        encerrarJogo();
    }

    if (pac->pontos == pacMaxPts)
    {
        encerrarJogo();
    }
    
    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
    exibirPontuacao(pac->pontos, 5, 36, pontuacao_Matriz);
}

int contarMaxPts(int size1, int size2, char map[size1][size2])
{/*funcao que contabiliza a quantidade de pontos necessária para que pacman ganhe*/
    int i, j,count;
    for (i = 0; i < size1; i++)
    {
        for (j = 0; j < size2; j++)
        {
            if (map[i][j]==1)
            {
                count++;
            }
            
        }
    }
    return count *10;
};
// FUNCOES PACMAN

int pacman_vivo(Pacman *pac)
{
    if (pac->vivo)
        return 1;
    else
    {
        return 0;
    }
}

// Função que inicializa os dados associados ao pacman
Pacman *pacman_create(int x, int y)
{

    Pacman *pac = malloc(sizeof(Pacman));
    if (pac != NULL)
    {
        pac->invencivel = 0;
        pac->pontos = 0;
        pac->passo = 4;
        pac->vivo = 1;
        pac->status = 0;
        pac->direcao = 0; /*vou definir que 0 = parado; 1 = direita,2 = esquerda,3 = p/ cima,4 = p/baixo*/
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

void pacman_destroy(Pacman *pac)
{
    free(pac);
}

void posicionarPacman(int x, int y, char mapa2[SIZE1][SIZE2])
{
    pac->x = x;
    pac->y = y;
    mapa2[x][y] = 6; /*Numero que representa o pacman na matriz de controle(mapa2)*/
}

void pontuaVerif(Pacman *pac, char mapa2[SIZE1][SIZE2])
{
    if (mapa2[pac->xj][pac->yj] == 2)
    {
        mapa2[pac->xi][pac->yi] = 0; /*atualiza o valor da posicao anterior de pac caso ele pontue*/
        pac->pontos += 10;
    }
}

void pacman_AlteraDirecao(Pacman *pac, int direcao, char mapa2[SIZE1][SIZE2])
{
    switch (pac->direcao)
    {
    case 0:
        pac->direcao = direcao;
        break;
    case 1:
        if (mapa2[(pac->x) + 1][pac->y] < 9)
        {
            pac->direcao = direcao;
        }
        break;
    case 2:
        if (mapa2[(pac->x) - 1][pac->y] < 9)
        {
            pac->direcao = direcao;
        }
        break;
    case 3:
        if (mapa2[(pac->x)][pac->y - 1] < 9)
        {
            pac->direcao = direcao;
        }
        break;
    case 4:
        if (mapa2[(pac->x)][pac->y + 1] < 9)
        {
            pac->direcao = direcao;
        }
        break;
    default:
        pac->direcao = 0;
        break;
    }
}
void pacman_movimenta(Pacman *pac, char mapa2[SIZE1][SIZE2])
{
    if (pac->vivo == 0)
        return;

    switch (pac->direcao)
    {
    case 0:
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
{/*nao esta sendo utilizada*/
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
    //printf("tem parede");
    return 1;
}

void pacman_desenha(Pacman *pac, char mapa2[SIZE1][SIZE2])
{ /*funcao responsavel pela animacao de pacman*/
    /*por enquanto nao utiliza sprites*/
    int i;
    /*
     if (temParede(pac, mapa2))
     {
         printf("achou parede e nao desenhou");
         return;
     }*/

    if (pac->direcao == 0)
    { /*nao move*/ 
        return;
    }

    if (pac->direcao == 1 && pac->yj < 18 && pac->yj > 0 && pac->y != pac->yj && mapa2[pac->x][pac->yj] < 9)
    { /*move para frente*/
        for (i = 0; i < (pac->passo) - 1; i++)
        {
            {
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi) * 3, 0, 0, 0, 1);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi + 1) * 3, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi + 1) * 3, 0, 0, 0, 1);
                trocarStatus(pac);
            }
        }
    }

    if (pac->direcao == 2 && pac->yj > 0 && pac->yj < 18 && pac->y != pac->yj && mapa2[pac->x][pac->yj] < 9)
    { /*move para tras*/
        for (i = (pac->passo) - 1; i > 0; i--)
        {
            {
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi) * 3, 0, 0, 0, 1);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi - 1) * 3, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi - 1) * 3, 0, 0, 0, 1);
                trocarStatus(pac);
            }
        }
    }

    if (pac->direcao == 3 && pac->xj < 13 && pac->xj > 0 && pac->x != pac->xj && mapa2[pac->xj][pac->y] < 9)
    { /*move para baixo*/
        for (i = 0; i < (pac->passo) - 1; i++)
        {
            {
                desenhar_quadrado((i + 1) + (pac->xi) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                desenhar_quadrado((i + 1) + (pac->xi + 1) * 3, ((pac->y) * 3) + 1, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado((i + 1) + (pac->xi + 1) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                trocarStatus(pac);
            }
        }
    }

    if (pac->direcao == 4 && pac->xj > 0 && pac->xj < 13 && pac->x != pac->xj && mapa2[pac->xj - 1][pac->y] < 9)
    { /*move para cima*/
        for (i = (pac->passo) - 1; i > 0; i--)
        {
            {
                desenhar_quadrado((i + 1) + (pac->xi) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                desenhar_quadrado((i + 1) + (pac->xi - 1) * 3, ((pac->y) * 3) + 1, 7, 0, 7, 1);
                usleep(80000);
                desenhar_quadrado((i + 1) + (pac->xi - 1) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                trocarStatus(pac);
            }
        }
    }
}

void trocarStatus(Pacman *pac)
{ /*status diz qual sprite sera utilizado*/
    pac->status = 1 - pac->status;
}

// FUNCOES PHANTOM
//  Função que inicializa os dados associados ao phantom
Phantom *phantom_create(int x, int y)
{

    Phantom *ph = malloc(sizeof(Phantom));
    if (ph != NULL)
    {
        ph->pontos = 0;
        ph->passo = 4;
        ph->vivo = 1;
        ph->status = 0;
        ph->direcao = 0; /*vou definir que 0 = parado; 1 = direita,2 = esquerda,3 = p/ cima,4 = p/baixo*/
        ph->parcial = 0;
        ph->x = x;
        ph->y = y;
        ph->xi = x;
        ph->yi = y;
        ph->xj = x;
        ph->yj = y;
    }
    return ph;
}

void phantom_destroy(Phantom *ph)
{
    free(ph);
}

void posicionarPhantom(int x, int y, char mapa2[SIZE1][SIZE2])
{
    ph->x = x;
    ph->y = y;
    mapa2[x][y] = 7; /*Numero que representa o phantom na matriz de controle(mapa2)*/
}

void phantom_movimenta(Phantom *ph, char mapa2[SIZE1][SIZE2])
{
    if (ph->vivo == 0)
        return;

    switch (ph->direcao)
    {
    case 0:
        ph->xj = ph->x;
        ph->yj = ph->y;

        posicionarPhantom(((ph->x)), (ph->y), mapa2);

        break;
    case 3:
        if (mapa2[(ph->x) + 1][ph->y] < 9)
        {
            ph->xi = ph->x;
            ph->yi = ph->y;

            ph->xj = ph->x + 2;
            ph->yj = ph->y;

            posicionarPhantom(((ph->x) + 1), (ph->y), mapa2);
        }
        else
        {
            ph->direcao = 0;
        }
        break;
    case 4:
        if (mapa2[(ph->x) - 1][ph->y] < 9)
        {
            ph->xi = ph->x;
            ph->yi = ph->y;
            ph->xj = ph->x - 2;
            ph->yj = ph->y;

            posicionarPhantom(((ph->x) - 1), (ph->y), mapa2);
        }
        else
        {
            ph->direcao = 0;
        }
        break;
    case 2:
        if (mapa2[(ph->x)][(ph->y) - 1] < 9)
        {
            ph->xi = ph->x;
            ph->yi = ph->y;
            ph->xj = ph->x;
            ph->yj = ph->y - 2;

            posicionarPhantom(((ph->x)), (ph->y - 1), mapa2);
        }
        else
        {
            ph->direcao = 0;
        }

        break;
    case 1:
        if (mapa2[(ph->x)][(ph->y) + 1] < 9)
        {
            ph->xi = ph->x;
            ph->yi = ph->y;
            ph->xj = ph->x;
            ph->yj = ph->y + 2;

            posicionarPhantom(((ph->x)), (ph->y + 1), mapa2);
        }
        else
        {
            ph->direcao = 0;
        }
        break;
    }
    /*fantasma nao apaga blocos*/
    if (ph->x == pac->x && ph ->y == pac->y)
    {
        pac->vivo = 0;
    }
    
}

void phantom_desenha(Phantom *ph, char mapa2[SIZE1][SIZE2])
{ /*funcao responsavel pela animacao de phantom*/
    /*por enquanto nao utiliza sprites*/
    int i;

    if (ph->direcao == 0)
    {
        return;
    }

    if (ph->direcao == 1 && ph->yj < 18 && ph->yj > 0 && ph->y != ph->yj && mapa2[ph->x][ph->yj] < 9)
    { /*move para frente*/
        for (i = 0; i < (ph->passo) - 1; i++)
        {
            {
                desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi) * 3, 0, 0, 0, 1);
                desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi + 1) * 3, 7, 7, 7, 1);
                usleep(80000);
                desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi + 1) * 3, 0, 0, 0, 1);
                trocarStatusPhantom(ph);
            }
        }
    }

    if (ph->direcao == 2 && ph->yj > 0 && ph->yj < 18 && ph->y != ph->yj && mapa2[ph->x][ph->yj] < 9)
    { /*move para tras*/
        for (i = (ph->passo) - 1; i > 0; i--)
        {
            {
                desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi) * 3, 0, 0, 0, 1);
                desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi - 1) * 3, 7, 7, 7, 1);
                usleep(80000);
                desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi - 1) * 3, 0, 0, 0, 1);
                trocarStatusPhantom(ph);
            }
        }
    }

    if (ph->direcao == 3 && ph->xj < 13 && ph->xj > 0 && ph->x != ph->xj && mapa2[ph->xj][ph->y] < 9)
    { /*move para baixo*/
        for (i = 0; i < (ph->passo) - 1; i++)
        {
            {
                desenhar_quadrado((i + 1) + (ph->xi) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);
                desenhar_quadrado((i + 1) + (ph->xi + 1) * 3, ((ph->y) * 3) + 1, 7, 7, 7, 1);
                usleep(80000);
                desenhar_quadrado((i + 1) + (ph->xi + 1) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);
                trocarStatusPhantom(ph);
            }
        }
    }

    if (ph->direcao == 4 && ph->xj > 0 && ph->xj < 13 && ph->x != ph->xj && mapa2[ph->xj - 1][ph->y] < 9)
    { /*move para cima*/
        for (i = (ph->passo) - 1; i > 0; i--)
        {
            {
                desenhar_quadrado((i + 1) + (ph->xi) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);
                desenhar_quadrado((i + 1) + (ph->xi - 1) * 3, ((ph->y) * 3) + 1, 7, 7, 7, 1);
                usleep(80000);
                desenhar_quadrado((i + 1) + (ph->xi - 1) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);
                trocarStatusPhantom(ph);
            }
        }
    }

}

void trocarStatusPhantom(Phantom *ph)
{ /*status diz qual sprite sera utilizado*/
    ph->status = 1 - ph->status;
}

// FUNCOES AUXILIARES
void invert_map(int size1, int size2, char map[size1][size2])
{
    int i, j;
    for (i = 0; i < size1; i++)
    {
        for (j = 0; j < size2; j++)
        {
            map[i][j] = 1 - map[i][j];
        }
    }
};
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

void inicializacao_accel() {
    fd = open_fd();
    regs = map_i2c(fd);
    I2C0_Init(&regs);         // Estabelece a comunicação com o acelerômetro
    accelerometer_init(regs); // Configura o acelerômetro
}

void *accel_working(void *args) {
	while (ACCEL)
		if (accelereometer_isDataReady(regs))
			accelerometer_x_read(X, regs); // lê os dados do eixo x
	return NULL;
}