/*
 * Universidade Estadual de Feira de Santana
 * TEC499 - MI - SISTEMAS DIGITAIS - 2024.2
 * Engenharia de Computação - Departamento de Tecnologia (DTEC)
 * Discentes: Anésio Neto, Caick Wendell, Caike Dylon
 * Docente: Ângelo Duarte
 **/

#define LINHAS 20
#define COLUNAS 15
#define TRUE 1
#define FALSE 0

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

typedef struct {
    char **array;
    int largura, linha, coluna;
    int cor;
} Forma;

const int VetorDeCores[4] = {1, 2, 3, 4};
int fd, decrementar = 1000, pontuacao, pause_game = 0, LISTEN_BTN = 1, ACCEL = 1, BUTTON = 0;
static int16_t X[1];
volatile sig_atomic_t stop;
struct timeval before_now, now;
char pontuacao_Matriz[5][36], Matriz[LINHAS][COLUNAS], borda_Matriz[LINHAS + 1][COLUNAS + 1] = {{0}};
I2C_Registers regs;
Forma forma_atual;
suseconds_t temporizador = 400000; // é só diminuir esse valor pro jogo executar mais rápido

Forma CopiarForma(Forma forma);
void MovimentarForma(int direcao);
void RemoverLinhaEAtualizarPontuacao();
void SobrescreverMatriz();
void GerarNovaFormaAleatoriamente();
int ChecarPosicao(Forma forma);
void ApagarForma(Forma forma);
int temQueAtualizar();
void RotacionarForma(Forma forma);
void catchSIGINT(int signum);
void *button_threads(void *args);
void inicializacao_accel();
void *accel_working(void *args);
void limpar_matriz();
void pausar_game();
void rotacionar();
void mudarCorArray(int linhas, int colunas, Forma forma, int cor);

/* Definição das peças */
const Forma VetorDeFormas[7] = {
    {// formato S
     (char *[]){
         (char[]){0, 1, 1},
         (char[]){1, 1, 0},
         (char[]){0, 0, 0}},
     3}, // formato Z
    {
        (char *[]){
            (char[]){1, 1, 0},
            (char[]){0, 1, 1},
            (char[]){0, 0, 0}},
        3}, // formato T
    {
        (char *[]){
            (char[]){0, 1, 0},
            (char[]){1, 1, 1},
            (char[]){0, 0, 0}},
        3}, // formato L
    {
        (char *[]){
            (char[]){0, 0, 1},
            (char[]){1, 1, 1},
            (char[]){0, 0, 0}},
        3}, // formato L invertido
    {
        (char *[]){
            (char[]){1, 0, 0},
            (char[]){1, 1, 1},
            (char[]){0, 0, 0}},
        3}, // formato quadrado
    {
        (char *[]){
            (char[]){1, 1},
            (char[]){1, 1}},
        2}, // formato vareta
    {
        (char *[]){
            (char[]){0, 0, 0, 0},
            (char[]){1, 1, 1, 1},
            (char[]){0, 0, 0, 0},
            (char[]){0, 0, 0, 0}},
        4}};

int main() {
    int i, j;
    pthread_t thread_button;
    pthread_t thread_accel;

    srand(time(0));
    gettimeofday(&before_now, NULL);

    mapear_gpu();
    inicializacao_accel();

    pthread_create(&thread_accel, NULL, accel_working, NULL);
    pthread_create(&thread_button, NULL, button_threads, NULL);

    limpar_tela();
    escreverTetris(1, 6, 5, 4, 3, 4, 10, 2);
    escreverPressionePB(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 30, 1);

    do {
        stop = 0;
        pontuacao = 0;
        if (BUTTON == 1) {
            limpar_matriz();
            limpar_tela();
            escrever_Borda(LINHAS, COLUNAS + 1, borda_Matriz, 3);
            ler_matriz(LINHAS + 1, COLUNAS + 1, borda_Matriz, 2, 0, 1, 2);
            escrever_Pts(1, 2, 3, 4, 0, 54, 1);
            exibirPontuacao(pontuacao, 5, 36, pontuacao_Matriz);

            GerarNovaFormaAleatoriamente();
            while (!stop && !pause_game) {
                if (BUTTON == 2)
                    pausar_game();
                if (BUTTON == 4)
                    rotacionar();

                char Buffer[LINHAS][COLUNAS] = {{0}};
                char matriz_aux[LINHAS][COLUNAS] = {{0}};

                for (i = 0; i < LINHAS; i++)
                    for (j = 0; j < COLUNAS; j++)
                        matriz_aux[i][j] = Matriz[i][j];

                for (i = 0; i < forma_atual.largura; i++)
                    for (j = 0; j < forma_atual.largura; j++)
                        if (forma_atual.array[i][j])
                            Buffer[forma_atual.linha + i][forma_atual.coluna + j] = forma_atual.array[i][j];

                for (i = 0; i < LINHAS; i++)
                    for (j = 0; j < COLUNAS; j++)
                        if (Buffer[i][j] != 0)
                            matriz_aux[i][j] = Buffer[i][j];

                ler_matriz(LINHAS, COLUNAS, matriz_aux, 2, 1, 0, 2);
                if (X[0] > 20) {
                    MovimentarForma('d');
                    usleep(80000);
                } else if (X[0] < -20) {
                    MovimentarForma('a');
                    usleep(80000);
                }

                gettimeofday(&now, NULL);
                if (temQueAtualizar()) {
                    MovimentarForma('s');
                    gettimeofday(&before_now, NULL);
                }
            }
            /* Saiu do laço principal, ou seja, game over! */
            limpar_tela();
            escrever_GameOver(1, 1, 1, 1, 1, 1, 1, 14, 25, 2);
            sleep(5);

            limpar_tela();
            escreverTetris(1, 6, 5, 4, 3, 4, 10, 2);
            escreverPressionePB(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 30, 1);
            BUTTON = 0;
            while (BUTTON != 1) {
            }
        }
    } while (BUTTON != 3);

    LISTEN_BTN = 0;
    ACCEL = 0;
    pthread_join(thread_button, NULL);
    pthread_join(thread_accel, NULL);
    desmapear_gpu();

    return 0;
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

void pausar_game() {
    while (1) { // pausa o jogo
        pause_game = 1;
        escreverPause(1, 1, 1, 1, 1, 14, 25, 2);
        if (BUTTON != 2) {
            BUTTON = 0;
            pause_game = 0;
            limpar_tela();
            escrever_Borda(LINHAS, COLUNAS + 1, borda_Matriz, 3);
            ler_matriz(LINHAS + 1, COLUNAS + 1, borda_Matriz, 2, 0, 1, 2);
            escrever_Pts(1, 2, 3, 4, 0, 54, 1);
            exibirPontuacao(pontuacao, 5, 36, pontuacao_Matriz);
            break;
        }
    }
}

void limpar_matriz() {
    int i, j;

    for (i = 0; i < LINHAS; i++)
        for (j = 0; j < COLUNAS; j++)
            Matriz[i][j] = 0;
}

void rotacionar() {
  while (1) {
    RotacionarForma(forma_atual);
    BUTTON = 1;
    break;
  }
}

/*
 * Função de utilidade responsável por copiar uma peça
 * @param forma - Peça a ser copiada
 **/
Forma CopiarForma(Forma forma) {
    Forma nova_forma = forma;
    char **aux = forma.array;
    nova_forma.array = (char **)malloc(nova_forma.largura * sizeof(char *));
    int i, j;
    for (i = 0; i < nova_forma.largura; i++) {
        nova_forma.array[i] = (char *)malloc(nova_forma.largura * sizeof(char));
        for (j = 0; j < nova_forma.largura; j++) {
            nova_forma.array[i][j] = aux[i][j];
        }
    }
    return nova_forma;
}

/*
 * Função responsável por realizar toda a movimentação da peça que cai
 * @param direcao - Esquerda, Baixo(descer mais rápido), Direita e Cima(rotação): 'a', 's', 'd' e 'w' respectivamente.
 * Aqui entra o acelerômetro.
 **/
void MovimentarForma(int direcao) {
    Forma temp = CopiarForma(forma_atual);
    switch (direcao) {
    case 'w':
        RotacionarForma(temp); // rotate clockwise
        if (ChecarPosicao(temp))
            RotacionarForma(forma_atual);
        break;
    case 's':
        temp.linha++; // move pra baixo
        if (ChecarPosicao(temp))
            forma_atual.linha++;
        else {
            SobrescreverMatriz();
            RemoverLinhaEAtualizarPontuacao();
            GerarNovaFormaAleatoriamente();
        }
        break;
    case 'd':
        temp.coluna++; // move pra direita
        if (ChecarPosicao(temp))
            forma_atual.coluna++;
        break;
    case 'a':
        temp.coluna--; // move pra esquerda
        if (ChecarPosicao(temp))
            forma_atual.coluna--;
        break;
    }
    ApagarForma(temp);
}

/*
 * Função responsável por realizar a remoção da linha completa e incrementação da pontuação do jogador
 **/
void RemoverLinhaEAtualizarPontuacao() {
    int i, j, soma, contagem = 0;
    for (i = 0; i < LINHAS; i++) {
        soma = 0;
        for (j = 0; j < COLUNAS; j++)
            if (Matriz[i][j] != 0)
                soma++;
        if (soma == COLUNAS) {
            contagem++;
            int l, k;
            for (k = i; k >= 1; k--)
                for (l = 0; l < COLUNAS; l++)
                    Matriz[k][l] = Matriz[k - 1][l];
            for (l = 0; l < COLUNAS; l++)
                Matriz[k][l] = 0;
            temporizador -= decrementar--;
        }
    }
    pontuacao += 100 * contagem;
    exibirPontuacao(pontuacao, 5, 36, pontuacao_Matriz);
}

/*
 * Função responsável por "repintar" a matriz do jogo, fazendo com que a peça atual se torne visível
 **/
void SobrescreverMatriz() {
    int i, j;
    for (i = 0; i < forma_atual.largura; i++) {
        for (j = 0; j < forma_atual.largura; j++) {
            if (forma_atual.array[i][j])
                Matriz[forma_atual.linha + i][forma_atual.coluna + j] = forma_atual.array[i][j];
        }
    }
    //  ler_matriz(LINHAS,COLUNAS,Matriz,2,1,0,2);
}

/*
 * Função responsável por gerar uma nova peça de forma randomica
 **/
void GerarNovaFormaAleatoriamente() {
    Forma nova_forma = CopiarForma(VetorDeFormas[rand() % 7]);
    nova_forma.coluna = rand() % (COLUNAS - nova_forma.largura + 1);
    nova_forma.linha = 0;
    // comentar as 2 linhas abaixo e a função mudar cor array em caso de problema no teste

    /* Tem que de alguma forma, mandar as cores randomizadas, pois não está colorindo corretamente quando pontua*/
    nova_forma.cor = VetorDeCores[rand() % 4];
    mudarCorArray(nova_forma.largura, nova_forma.largura, nova_forma, nova_forma.cor);

    ApagarForma(forma_atual);
    forma_atual = nova_forma;
    if (!ChecarPosicao(forma_atual)) {
        stop = TRUE;
    }
}

void mudarCorArray(int linhas, int colunas, Forma forma, int cor) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (forma.array[i][j] == 1) {
                forma.array[i][j] = cor;
            }
        }
    }
}

/*
 * Função responsável por verificar a posicao de uma forma
 * @param forma - Peça a ser apagada
 **/
int ChecarPosicao(Forma forma) {
    char **array = forma.array;
    int i, j;
    for (i = 0; i < forma.largura; i++) {
        for (j = 0; j < forma.largura; j++) {
            if ((forma.coluna + j < 0 || forma.coluna + j >= COLUNAS || forma.linha + i >= LINHAS)) {
                if (array[i][j])
                    return FALSE;
            } else if (Matriz[forma.linha + i][forma.coluna + j] && array[i][j])
                return FALSE;
        }
    }
    return TRUE;
}

/*
 * Função responsável por apagar uma forma
 * @param forma - Peça a ser apagada
 **/
void ApagarForma(Forma forma) {
    int i;
    for (i = 0; i < forma.largura; i++)
        free(forma.array[i]);
    free(forma.array);
}

/*
 * Função responsável por verificar se está na hora de atualizar a exibição
 **/
int temQueAtualizar() {
    return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > temporizador;
}

/*
 * Função responsável por rotacionar uma forma
 * @param forma - Peça a ser rotacionada
 **/
void RotacionarForma(Forma forma) {
    Forma temp = CopiarForma(forma);
    int i, j, k, width;
    width = forma.largura;
    for (i = 0; i < width; i++) {
        for (j = 0, k = width - 1; j < width; j++, k--) {
            forma.array[i][j] = temp.array[k][i];
        }
    }
    ApagarForma(temp);
}

void catchSIGINT(int signum) {
    printf("Unmapping\n");
    desmapear_gpu();
    stop = 1;
}

void *button_threads(void *args) {
    int btn = 0;
    while (LISTEN_BTN) {
        btn = botoes();
        btn = (~btn) & 0b1111;
        if (btn == 1) {
            BUTTON = 1;
            usleep(200000);
        } else if (btn == 2) {
            BUTTON = 2;
            usleep(200000);
        } else if (btn == 4) {
            BUTTON = 3;
            usleep(200000);
        } else if (btn == 8) {
            BUTTON = 4;
            usleep(200000);
        }
    }
    return NULL;
}
