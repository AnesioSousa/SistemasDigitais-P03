
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

    //while (gameState!=2)
    for(i = 0;i<20;i++)    
      {
      //pacman_desenha(pac);
      pacman_movimenta(pac,mapa2);
      pacman_desenha(pac);

      ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1);

    }
     pacman_AlteraDirecao(pac,2,mapa2);
      printf("trocou direcao %d \n",pac->direcao);
    for(i = 0;i<20;i++)    
      {
      //pacman_desenha(pac);
      pacman_movimenta(pac,mapa2);
       pacman_desenha(pac);
         usleep(8000);

      printf("desenha pra esquerda");
      ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1);
      printf("%d \n",pac->pontos);

    }



    print_map(mapa2);
    encerrarJogo();
    
    desmapear_gpu();
    return 0;
}
void iniciarJogo(char map[SIZE1][SIZE2],char mapa2[SIZE1][SIZE2]){
    mapear_gpu();
    limpar_tela();
    
    mudarCorGenerico(SIZE1,SIZE2,map,3);/*TESTAR FUNCIONAMENTO E ORDEM DAS FUNÇÕES DE TROCA DE COR*/
    ler_matriz(SIZE1,SIZE2,map,3,0,0,3);/*Testar leitura de Matriz */

    copiarMatriz(SIZE1,SIZE2,mapa2,map);
    
    invert_map(SIZE1,SIZE2,mapa2);
    mudarCorGenerico(SIZE1,SIZE2,mapa2,2);
    mudarCorFundo(SIZE1,SIZE2,mapa2,9);
    
    ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1);
    gameState = 1;

}


void encerrarJogo(){
    gameState = 2;
}

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

void desenhar_jogo(char mapa2[SIZE1][SIZE2]){/*por enquanto sem implementação de sprites*/
   ler_matriz(SIZE1,SIZE2,mapa2,3,1,1,1); 
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
        if (mapa2[(pac->x)+1][pac->y]!=9)
        {
            pac->direcao = direcao;   
        }
        break;
    case 2:
        if (mapa2[(pac->x)-1][pac->y]!=9)
        {
            pac->direcao = direcao;
        }
        break;
    case 3:
        if (mapa2[(pac->x)][pac->y-1]!=9)
        {
            pac->direcao = direcao;
        }
        break;
    case 4:
        if (mapa2[(pac->x)][pac->y+1]!=9)
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
    if(pac->vivo == 0)
        return;
    
    switch (pac->direcao)
    {
    case 0:
        break;
    case 3:
        if (mapa2[(pac->x)+1][pac->y]<9)
        {
            pac->xi=pac->x;
            pac->yi=pac->y;
            
            pac->xj=pac->x+1;
            pac->yj=pac->y;
            
            pontuaVerif(pac,mapa2);
            posicionarPacman(((pac->x)+1),(pac->y),mapa2);
        }
        else{
            pac->direcao = 0;
            }
        break;
    case 4:
        if (mapa2[(pac->x)-1][pac->y]<9)
        {
            pac->xi=pac->x;
            pac->yi=pac->y;
            pac->xj=pac->x-1;
            pac->yj=pac->y;
 

            pontuaVerif(pac,mapa2);
            posicionarPacman(((pac->x)-1),(pac->y),mapa2);
        }        
        else{
            pac->direcao = 0;
            }
        break;
    case 2:
        if (mapa2[(pac->x)][pac->y-1]<9)
        {
            pac->xi=pac->x;
            pac->yi=pac->y;
            pac->xj=pac->x;
            pac->yj=pac->y-1;
 

            pontuaVerif(pac,mapa2);
            posicionarPacman(((pac->x)),(pac->y-1),mapa2);
        }
        else{
            pac->direcao = 0;
        }

        break;
    case 1:
        if (mapa2[(pac->x)][pac->y+1]<9)
        {
            pac->xi=pac->x;
            pac->yi=pac->y;
            pac->xj=pac->x;
            pac->yj=pac->y+1;
 

            pontuaVerif(pac,mapa2);
            posicionarPacman(((pac->x)),(pac->y+1),mapa2);
        }
       else{
            pac->direcao = 0;
            }
        break;
        }
mapa2[(pac->xi)][pac->yi]=0;
}

void pacman_desenha(Pacman* pac){
      int i;

      if(pac->direcao==1){
      for(i=0;i<(pac->passo)-1;i++){      
        {
          desenhar_quadrado(((pac->x)*3)+1,(i+1)+(pac->yi)*3,0,0,0,1);
          desenhar_quadrado(((pac->x)*3)+1,(i+1)+(pac->yi+1)*3,7,0,7,1);
          usleep(80000);
          desenhar_quadrado(((pac->x)*3)+1,(i+1)+(pac->yi+1)*3,0,0,0,1); 
        }   
      }
}
      if(pac->direcao==2){
            printf("entrou \n");
       for(i=0;i<(pac->passo)-1;i++){      
        {
          desenhar_quadrado(((pac->x)*3)+1,(i+1)+(pac->yi)*3,0,0,0,1);
          desenhar_quadrado(((pac->x)*3)+1,(i+1)+(pac->yi-1)*3,7,0,7,1);
          usleep(800000);
          printf("delay");
          desenhar_quadrado(((pac->x)*3)+1,(i+1)+(pac->yi-1)*3,0,0,0,1); 
        }   
      }
    }
  }

void invert_map(int size1,int size2,char map[size1][size2]){
    int i,j;
    for ( i = 0; i < size1; i++) {
        for ( j = 0; j < size2; j++) {
            map[i][j] = 1 - map[i][j];
        }
    }
};
void mudarCorFundo(int linhas, int colunas,char matriz[linhas][colunas],int cor){
    int i,j;
    for ( i = 0; i < linhas; i++)
    {
        for ( j = 0; j < colunas; j++)
        {
           if (matriz[i][j]==0)
           {
            matriz[i][j]=cor;
           }  
        }    
    } 
}

void mudarCorGenerico(int linhas, int colunas,char matriz[linhas][colunas],int cor){
    int i,j;
    for ( i = 0; i < linhas; i++)
    {
        for ( j = 0; j < colunas; j++)
        {
           if (matriz[i][j]==1)
           {
            matriz[i][j]=cor;
           }  
        }    
    } 
}

void print_map(char map[SIZE1][SIZE2]) {
    for (int i = 0; i < SIZE1; i++) {
        for (int j = 0; j < SIZE2; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}
