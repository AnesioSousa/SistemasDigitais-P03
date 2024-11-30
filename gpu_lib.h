#ifndef GPU_LIB_H
#define GPU_LIB_H
#include <stdint.h>
#include "gpu_letters.h"


/*             API                */
void mapear_gpu();
void desmapear_gpu();
void limpar_tela();
void desenhar_quadrado(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R, int tamanho_quadrado);
void desenhar_sprite(uint32_t reg, uint32_t x, uint32_t y, uint32_t offset, uint32_t sp);
int time_is_up();
void reset_pulsecounter();
void setar_cor_pixel_sprite(uint32_t endereco, uint32_t B, uint32_t G, uint32_t R);
void clear_sprites();


void escreverPressionePB(int corP,int corR,int corE,int corS,int corI,int corO,int corN,int corB,int corA,int corJ,int corG,int posX, int posY,int tamanho);
void escreverPause(int corP,int corA,int corU,int corS,int corE,int posX, int posY,int tamanho);
void escrever_GameOver(int corG,int corA,int corM,int corE,int corO,int corV,int corR,int posX, int posY,int tamanho);
void escrever_PacMan(int corP, int corA, int corC, int corHifen, int corM, int corN, int posX, int posY, int espacamento, int tamanho);
void escrever_Phantom(int corP, int corH, int corA, int corN, int corT, int corO, int corM, int posX, int posY, int espacamento, int tamanho);
void escrever_Wins(int corW, int corI, int corN, int corS,int corExclamacao, int posX, int posY,int espacamento, int tamanho);
void escrever_PacWins(int posX, int posY,int espacamento, int tamanho);
void escrever_PhantomWins(int posX, int posY,int espacamento, int tamanho);
int botoes();
void escreverTetris(int corT,int corE,int corR,int corI,int corS,int posX, int posY,int tamanho);
void desenhar_tela_inicial();
void desenhar_T_vermelho();
void desenhar_E_laranja();
void desenhar_T_amarelo();
void escreverTetris(int corT,int corE,int corR,int corI,int corS,int posX, int posY,int tamanho);
void escrever_Pts(int corT,int corS,int corP,int corDoisP,int posX, int posY,int tamanho);
void exibirPontuacao(int pontuacao,int linhas,int colunas,char matriz[linhas][colunas]);
void escrever_Borda(int linhas,int colunas,char matriz[linhas][colunas],int cor);
void copiarMatriz(int linhas,int colunas,char matriz[linhas][colunas],char temp[linhas][colunas]);
void mudarCor(int linhas, int colunas,char matriz[5][5],int cor);
void ler_matriz(int Max_linhas, int Max_colunas,char Matriz[Max_linhas][Max_colunas],int espacamento,int off_X,int off_Y, int tamanho_quadrado);
#endif
