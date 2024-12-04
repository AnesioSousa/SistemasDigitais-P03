#ifndef GPU_LIB_H
#define GPU_LIB_H
#include <stdint.h>
#include "gpu_letters.h"

/**
 * \brief           dados de uma cor
 * \note            essa estrutura é utilizada para separar os campos de uma cor
 */
typedef struct {
    uint64_t red;           /*!< Intensidade cor vermelha*/
    uint64_t green;         /*!< Intensidade cor verde*/
    uint64_t blue;          /*!< Intensidade cor azul*/ 
} color_t;

/**
 * \brief           Estrutura de um sprite
 * \note            Essa struct é utilizada para separar os campos que um sprite deve conter
*/
typedef struct {
    uint64_t coord_x;               /*!< Coordenada do eixo x do centro do sprite:        0 a 639*/
    uint64_t coord_y;               /*!< Coordenada do eixo y do centro do sprite:        0 a 479*/
    uint64_t offset;                /*!< Identificação do sprite:                         0 a 24 */ 
    uint64_t data_register;         /*!< Localização do sprite na memoria:                1 a 31 */
    uint64_t visibility;            /*!< Visibilidade do sprite: 0. invisivel         1. visivel */ 
    uint64_t speed;                 /*!< Velocidade que o sprite se move, 0 se for um sprite fixo*/
} sprite_t;

/**
 * \brief           Estrutura de um polígono
 * \note            Essa estrutura é utilizada para definir os campos de um polígono
 */
typedef struct {
    uint64_t coord_x;               /*!< Eixo X:                    0 a 511 */ 
    uint64_t coord_y;               /*!< Eixo Y:                    0 a 479 */ 
    uint64_t mem_address;           /*!< Número referente a camada: 0 a 15 */ 
    uint64_t shape;                 /*!< Forma do poligno: 0 . quadrado  1 . triangulo */
    uint64_t size;                  /*!< Tamanho do poligno:        0 a 15 */ 
    color_t color;                  /*!< Campo de cor do polígono */
} polygon_t;

/**
 * \brief           Estrutura de Bloco de BackGround 
 * \note            Estrutura utilizada para definir os campos de um bloco do background 
 */
typedef struct {
    uint64_t coord_x;           /*!< Endereço do bloco: 0 a 79 */ 
    uint64_t coord_y;           /*!< Endereço do bloco: 0 a 59 */ 
    color_t color;              /*!< Cor referente ao bloco */ 
} background_block_t;


typedef struct{
    uint64_t coord_x;           /*!< Endereço do bloco: 0 a 79      */ 
    uint64_t coord_y;           /*!< Endereço do bloco: 0 a 59      */ 
    color_t color;              /*!< Cor referente ao bloco         */ 
    uint64_t speed;             /*!< Velocidade que o bloco se move */
} background_block_movel_t;

/**
 * \brief           Estrutura de um pixel 
 * \note            Essa Struct é utilizada para definir os campos que um pixel deve conter
 */
typedef struct {
    uint64_t mem_address;           /*!< Endereço do pixel: 0 a 16383 */ 
    color_t color;                  /*!< Cor do pixel */
} pixel_t;


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
