#include "acelerometro.c"
#include "gpu_lib.h"
#include "mouse_move.c"
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

int main(){
    mapear_gpu();
    limpar_tela();
    clear_poligonos();
    //desenhar_poligono(uint32_t endereco, uint32_t forma, uint32_t B, uint32_t G, uint32_t R, uint32_t tamanho, uint32_t ponto_ref_y, uint32_t ponto_ref_x)    
    int i;
    for(i = 0;i<400;i++){
    desenhar_poligono(0,0,7,7,0,1,200,10+i);
    desenhar_poligono(1,0,7,7,0,1,200,20+i);
    desenhar_poligono(2,0,7,7,0,1,200,30+i);
    desenhar_poligono(3,0,7,7,0,1,200,40+i);
    desenhar_poligono(4,0,7,7,0,1,200,50+i);
    desenhar_poligono(5,0,7,7,0,1,200,60+i);
    desenhar_poligono(6,0,7,7,0,1,200,70+i);
    desenhar_poligono(7,0,7,7,0,1,200,80+i);


usleep(800);



}
    for(i = 400;i>=170;i--){
    desenhar_poligono(8,0,7,7,0,1,200,10+i);
    desenhar_poligono(9,0,7,7,0,1,200,20+i);
    desenhar_poligono(10,0,7,7,0,1,200,30+i);
    desenhar_poligono(11,0,7,7,0,1,200,40+i);
    desenhar_poligono(12,0,7,7,0,1,200,50+i);
    desenhar_poligono(13,0,7,7,0,1,200,60+i);
    desenhar_poligono(14,0,7,7,0,1,200,70+i);


usleep(800);



}

    escrever_PacMan(3,3,3,3,3,3,21,28,1,1);
    usleep(800);

    escrever_PacMan(7,7,7,7,7,7,21,28,1,1);
    usleep(800);

    escrever_PacMan(3,3,3,3,3,3,21,28,1,1);
    usleep(800);

    escrever_PacMan(7,7,7,7,7,7,21,28,1,1);
    usleep(800);

    escrever_PacMan(3,3,3,3,3,3,21,28,1,1);
    usleep(800);

    escrever_PacMan(7,7,7,7,7,7,21,28,1,1);
    usleep(800);

    escrever_PacMan(3,3,3,3,3,3,21,28,1,1);
    usleep(800);

    escrever_PacMan(7,7,7,7,7,7,21,28,1,1);
    usleep(800);

    escrever_PacMan(3,3,3,3,3,3,21,28,1,1);
    usleep(800);

    for(i = 400;i>=170;i--){
    desenhar_poligono(0,0,7,7,0,1,200,10+i);
    desenhar_poligono(1,0,7,7,0,1,200,20+i);
    desenhar_poligono(2,0,7,7,0,1,200,30+i);
    desenhar_poligono(3,0,7,7,0,1,200,40+i);
    desenhar_poligono(4,0,7,7,0,1,200,50+i);
    desenhar_poligono(5,0,7,7,0,1,200,60+i);
    desenhar_poligono(6,0,7,7,0,1,200,70+i);
    desenhar_poligono(7,0,7,7,0,1,200,80+i);
    usleep(800);
}
    
    for(i = 170;i>=0;i--){
    desenhar_poligono(0,0,7,7,0,1,200,10+i);
    desenhar_poligono(1,0,7,7,0,1,200,20+i);
    desenhar_poligono(2,0,7,7,0,1,200,30+i);
    desenhar_poligono(3,0,7,7,0,1,200,40+i);
    desenhar_poligono(4,0,7,7,0,1,200,50+i);
    desenhar_poligono(5,0,7,7,0,1,200,60+i);
    desenhar_poligono(6,0,7,7,0,1,200,70+i);
    desenhar_poligono(7,0,7,7,0,1,200,80+i);


    desenhar_poligono(8,0,7,7,0,1,200,10+i);
    desenhar_poligono(9,0,7,7,0,1,200,20+i);
    desenhar_poligono(10,0,7,7,0,1,200,30+i);
    desenhar_poligono(11,0,7,7,0,1,200,40+i);
    desenhar_poligono(12,0,7,7,0,1,200,50+i);
    desenhar_poligono(13,0,7,7,0,1,200,60+i);
    desenhar_poligono(14,0,7,7,0,1,200,70+i);


usleep(800);



}
    desenhar_poligono(8,0,7,7,0,0,200,10+i);
    desenhar_poligono(9,0,7,7,0,0,200,20+i);
    desenhar_poligono(10,0,7,7,0,0,200,30+i);
    desenhar_poligono(11,0,7,7,0,0,200,40+i);
    desenhar_poligono(12,0,7,7,0,0,200,50+i);
    desenhar_poligono(13,0,7,7,0,0,200,60+i);
    desenhar_poligono(14,0,7,7,0,0,200,70+i);

    clear_poligonos();
    limpar_tela();
    escrever_PacMan(3,3,3,3,3,3,-1,10,2,2);
    //escreverPressionePB(2, 4, 2, 2, 2, 3, 2, 2, 2, 3, 1, 4, 30, 1);
    escrever_Menu(1,1,1,1,1,1,15,25);
    desmapear_gpu();
}

void escrever_Menu(int corI,int corN,int corC,int corA,int corR,int corE,int pos_x,int pos_y){
  int i,j;
  char Iniciar[5][41] = {{0}};
  char Encerrar[5][48] = {{0}};
  char Pressione[5][60] = {{0}};
  char Para[5][30] = {{0}};
  char Jogar[5][31] = {{0}};
  char PB[5][16] = {{0}};

  char letraP[5][5] = {{0}}, letraR[5][5] = {{0}}, letraE[5][5] = {{0}}, letraS[5][5] = {{0}}, letraI[5][5] = {{0}}, letraO[5][5] = {{0}}, letraN[5][5] = {{0}};
  char letraC[5][5]={{0}},letraA[5][5]={{0}};
  char letraJ[5][5] = {{0}}, letraG[5][5] = {{0}};
  char letraB[5][5] = {{0}}, numero1[5][5] = {{0}}, numero3[5][5] = {{0}};
  definirLetraP(letraP);
  mudarCor(5, 5, letraP, 1);

  definirLetraR(letraR);
  mudarCor(5, 5, letraR, corR);

  definirLetraE(letraE);
  mudarCor(5, 5, letraE, corE);

  definirLetraS(letraS);
  mudarCor(5, 5, letraS, 1);

  definirLetraI(letraI);
  mudarCor(5, 5, letraI, corI);

  definirLetraO(letraO);
  mudarCor(5, 5, letraO, 1);

  definirLetraN(letraN);
  mudarCor(5, 5, letraN, corN);

  definirLetraC(letraC);
  mudarCor(5, 5, letraC, corC);

  definirLetraA(letraA);
  mudarCor(5, 5, letraA, corA);

  definirLetraJ(letraJ);
  mudarCor(5, 5, letraJ, 1);

  definirLetraG(letraG);
  mudarCor(5, 5, letraG, 1);

  definirLetraB(letraB);
  mudarCor(5, 5, letraB, 1);

  definirNumero1(numero1);
  definirNumero3(numero3);

  // PRESSIONE
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      Pressione[i][j] = letraP[i][j];
      Pressione[i][6 + j] = letraR[i][j];
      Pressione[i][12 + j] = letraE[i][j];
      Pressione[i][18 + j] = letraS[i][j];
      Pressione[i][24 + j] = letraS[i][j];
      Pressione[i][30 + j] = letraI[i][j];
      Pressione[i][36 + j] = letraO[i][j];
      Pressione[i][42 + j] = letraN[i][j];
      Pressione[i][48 + j] = letraE[i][j];
    }
  }

  // PARA
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      Para[i][j] = letraP[i][j];
      Para[i][6 + j] = letraA[i][j];
      Para[i][12 + j] = letraR[i][j];
      Para[i][18 + j] = letraA[i][j];
    }
  }
  // JOGAR
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      Jogar[i][j] = letraJ[i][j];
      Jogar[i][6 + j] = letraO[i][j];
      Jogar[i][12 + j] = letraG[i][j];
      Jogar[i][18 + j] = letraA[i][j];
      Jogar[i][24 + j] = letraR[i][j];
    }
  }

  // INICIAR
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      Iniciar[i][j] = letraI[i][j]; //I
      Iniciar[i][6 + j] = letraN[i][j];//N
      Iniciar[i][12 + j] = letraI[i][j];//I
      Iniciar[i][18 + j] = letraC[i][j];//C
      Iniciar[i][24 + j] = letraI[i][j];//I
      Iniciar[i][30 + j] = letraA[i][j];//A
      Iniciar[i][36 + j] = letraR[i][j];//R
    }
  }
  
  // ENCERRAR
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      Encerrar[i][j] = letraE[i][j];     //E
      Encerrar[i][6 + j] = letraN[i][j]; //N
      Encerrar[i][12 + j] = letraC[i][j];//C
      Encerrar[i][18 + j] = letraE[i][j];//E
      Encerrar[i][24 + j] = letraR[i][j];//R
      Encerrar[i][30 + j] = letraR[i][j];//R
      Encerrar[i][36 + j] = letraA[i][j];//A
      Encerrar[i][42 + j] = letraR[i][j];//R
    }
  }
  
  //PB
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      PB[i][j] = letraP[i][j];
      PB[i][6 + j] = letraB[i][j];
    }
  }
  /*
  char Iniciar[5][41] = {{0}};
  char Encerrar[5][46] = {{0}};
  char Pressione[5][60] = {{0}};
  char Para[5][30] = {{0}};
  char Jogar[5][31] = {{0}};
  char PB[5][16] = {{0}};

  ler_matriz(5, 60, Pressione, 1, posX, posY, tamanho);
  ler_matriz(5, 16, PB, 1, posX + 60, posY, tamanho);
  ler_matriz(5, 30, Para, 1, posX + 5, posY + 6, tamanho);
  ler_matriz(5, 31, Jogar, 1, posX + 36, posY + 6, tamanho);
}
  */
  ler_matriz(5,60,Pressione,1,pos_x,pos_y,1); //PRESSIONE

  ler_matriz(5,16,PB,1,pos_x,pos_y+6,1);  //PB
  ler_matriz(5,5,numero1,1,pos_x+10,pos_y+6,1);//1
  ler_matriz(5,30,Para,1,pos_x+30,pos_y+6,1);   //PARA
  ler_matriz(5,41,Iniciar,1,pos_x,pos_y+12,1);  //INICIAR

  ler_matriz(5,16,PB,1,pos_x,pos_y+21,1);  //PB
  ler_matriz(5,5,numero3,1,pos_x+11,pos_y+21,1);//3
  ler_matriz(5,30,Para,1,pos_x+30,pos_y+21,1);   //PARA
  ler_matriz(5,48,Encerrar,1,pos_x,pos_y+27,1); //ENCERRAR
  

}
