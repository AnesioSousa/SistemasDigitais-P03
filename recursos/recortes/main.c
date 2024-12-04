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
#define SIZE2 20
#define SIZE1 15
#define TRUE 1
#define FALSE 0

typedef enum {
    running = 1,
    finish,
    in_pause,
    in_menu,
    win,
    lose,
} states;

typedef struct TPacman {
    int status;
    int x, y, xi, yi, xj, yj;
    int direcao, passo, parcial;
    int pontos;
    int invencivel;
    int vivo;
    int animacao;
    int visibility;
    int data_register;
} Pacman;

typedef struct TPhantom {
    int status;
    int x, y, xi, yi, xj, yj;
    int direcao, passo, parcial;
    int pontos;
    int vivo;
    int animacao;
} Phantom;

pthread_mutex_t gpu_mutex;
pthread_mutex_t player_2_mutex;

// condicionais utilizadas
pthread_cond_t player_2_cond;

void invert_map(int size1, int size2, char map[size1][size2]);
void mudar_cor_fundo(int linhas, int colunas, char matriz[linhas][colunas], int cor);
void mudar_cor_generico(int linhas, int colunas, char matriz[linhas][colunas], int cor);
void iniciar_jogo(char map[SIZE1][SIZE2], char mapa2[SIZE1][SIZE2]);
void encerrar_jogo();

int pacman_vivo(Pacman *pac);
Pacman *pacman_create(int x, int y);
void desenhar_jogo(char mapa2[SIZE1][SIZE2]);
int pacman_vivo(Pacman *pac);
void pacman_destroy(Pacman *pac);
void posicionar_pacman(int x, int y, char mapa2[SIZE1][SIZE2]);
void pontua_verif(Pacman *pac, char mapa2[SIZE1][SIZE2]);
void pacman_altera_posicao(Pacman *pac, int direcao, char mapa2[SIZE1][SIZE2]);
void pacman_movimenta(Pacman *pac, char mapa2[SIZE1][SIZE2]);

Phantom *phantom_create(int x, int y);
void phantom_destroy(Phantom *ph);
void posicionar_phantom(int x, int y, char mapa2[SIZE1][SIZE2]);
void phantom_movimenta(Phantom *ph, char mapa2[SIZE1][SIZE2]);
void phantom_desenha(Phantom *ph, char mapa2[SIZE1][SIZE2]);
void trocar_status_phantom(Phantom *ph);

void inicializacao_accel();
void *player_1(void *args);
void *player_2(void *args);

void *pac_text_thread(void *args);
void *ph_text_thread(void *args);

void *buttons_thread(void *args);

/*Button*/
int LISTEN_BTN = 1, BUTTON = 0;
/*accel*/
int ACCEL = 1, fd;
I2C_Registers regs;
static int16_t X[1];
//---------------------------------
Pacman *pac;
Phantom *ph;

states state;

int MAX_POINTS;
/*mouse*/
int action = 0;
int power_amount = 1;

int pause_game = 0;
char pontuacao_matriz[5][36];

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
char mapa3[SIZE1][SIZE2] = {{0}};

int main() {
    mapear_gpu();
    pthread_t thread_player_1, thread_player_2, thread_button, text_pac, text_ph;

    inicializacao_accel();
    if (!open_mouse_device()) {
        return 1;
    }

    clear_poligonos();
    clear_sprites();
    setar_cor_pixel_sprite(1, 1, 1, 1);

    pthread_mutex_init(&gpu_mutex, NULL);
    pthread_mutex_init(&player_2_mutex, NULL);
    pthread_mutex_cond_init(&player_2_cond, &player_2_mutex);

    pthread_create(&thread_button, NULL, buttons_thread, NULL);
    pthread_create(&text_pac, NULL, pac_text_thread, NULL);
    pthread_create(&text_ph, NULL, ph_text_thread, NULL);
    pthread_create(&thread_player_1, NULL, player_1, NULL);
    pthread_create(&thread_player_2, NULL, player_2, NULL);

    limpar_tela();
    /*matriz de mapa/fundo*/
    mudar_cor_generico(SIZE1, SIZE2, map, 3);
    ler_matriz(SIZE1, SIZE2, map, 3, 0, 0, 3);

    /*matriz de controle/pontos*/
    copiar_matriz(SIZE1, SIZE2, mapa2, map);
    invert_map(SIZE1, SIZE2, mapa2);

    copiar_matriz(SIZE1, SIZE2, mapa3, mapa2);          /*criei essa matriz que é uma copia da matriz de controle para controlar o fantasma sem interferir na matriz de controle que será exibida*/
                                                        /*essa terceira matriz nao interfere na condição de vitoria do fantasma uma vez que ela se baseia nas cordenadas dentro da struct*/
    MAX_POINTS = count_max_points(SIZE1, SIZE2, mapa2); /*define a condição de vitoria de pac*/
    MAX_POINTS -= 10;                                   /*a primeira posicao que pacman ocupa ao iniciar o jogo nao é contabilizada
                                                          logo é necessário considerar essa diferenca ao tratar de sua condição de vitória
                                                        */
    mudar_cor_generico(SIZE1, SIZE2, mapa2, 2);
    mudar_cor_fundo(SIZE1, SIZE2, mapa2, 9);
    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

    state = running;

    pac = pacman_create(1, 1);
    posicionar_pacman(1, 1, mapa2);

    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

    ph = phantom_create(13, 18);
    posicionar_phantom(13, 18, mapa3);
    // teste de game_over
    /*
        pacman_altera_posicao(pac, 1, mapa2);
        phantom_altera_direcao(ph, 4, mapa3);

            for (i = 0; i < 20; i++)
        {
            pacman_movimenta(pac, mapa2);
            pacman_desenha(pac, mapa2);
            ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

            //phantom_movimenta(ph, mapa3);
            //phantom_desenha(ph, mapa3);


            printf("x = %d,xj = %d,y = %d, yj = %d, direcao = %d \n", pac->x, pac->xj, pac->y, pac->yj, pac->direcao);
        }
        pacman_altera_posicao(pac, 2, mapa2);

        phantom_altera_direcao(ph, 3, mapa3);

         for (i = 0; i < 20; i++)
         {
            pacman_movimenta(pac, mapa2);
            //pacman_desenha(pac, mapa2);

            ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

usleep(800000);
            phantom_desenha(ph, mapa3);

            //phantom_movimenta(ph, mapa3);
            //phantom_desenha(ph, mapa3);

        if(pacman_vivo(pac)){}
        else{
            pac->vivo =0;
            encerrar_jogo();
            break;
            }
            printf("x = %d,xj = %d,y = %d, yj = %d, direcao = %d \n", pac->x, pac->xj, pac->y, pac->yj, pac->direcao);
        }
*/  
    menu();

    print_map(mapa2);
    encerrar_jogo();

    pthread_join(thread_button, NULL);
    pthread_join(thread_player_1, NULL);
    pthread_join(thread_player_2, NULL);
    pthread_join(text_pac, NULL);
    pthread_join(text_ph, NULL);
    close_mouse_device();
    desmapear_gpu();
    pthread_mutex_destroy(&player_2_mutex);
    pthread_cond_destroy(&player_2_cond);
    return 0;
}

void menu() {
    state = in_menu;
    while(state != finish) {
        if (BUTTON == 1 && state == in_menu) {
            state=running;
            init_game();
        } else if (BUTTON == 2 && state == running) {
            state = in_pause;
            pause_screen();
        } else if (BUTTON == 2 && state == in_pause) {
            state = running;
            return_screen();
        }else if (BUTTON == 3) {
            state = finish;
        } else if (BUTTON == 4)  {
            state = in_menu;
            //pause_threads();
            //clear();
            //clean_all_obstacles(screen_obs, obstacle_on_screen_status);
            for(int i = 0; i < 10; i++){
                if (bullets[i]) {
                    bullets[i] = 0;
                }
            }
            pac->pontos = 0;
            ph->pontos = 0;
            draw_cover_art();
            set_menu();
        } 

    }
    return;
}


// FUNCOES JOGO

void encerrar_jogo() { /*por enquanto nao diferencia quem ganhou*/

    state = finish;

    if (pac->vivo == 0) { /*phantom ganhou*/
        clear_poligonos();
        clear_sprites();
        limpar_tela();
        escrever_PhantomWins(4, 10, 2, 2);
    } else if (pac->vivo == 1 && pac->pontos == MAX_POINTS) { /*pacman ganhou e atingiu o máximo de pontos*/
        clear_poligonos();
        clear_sprites();
        limpar_tela();
        escrever_PacWins(4, 10, 2, 2);
    } else { /*essa funcao é um teste e provavelmente será retirada antes da entrega, mas se a tela de game over aparecer
               provavelmente os pontos não estão sendo contados corretamente
           */
        clear_poligonos();
        clear_sprites();
        limpar_tela();
        escrever_GameOver(1, 1, 1, 1, 1, 1, 1, 14, 25, 2); /*provisorio*/
    }
}

/*esse tipo de funcao pode ser replicado para o mouse*/
int pegar_direcao_pac() { /*no momento retorna somente o X*/
    int direcao;
    if (X[0] > 20) {
        direcao = 1;
    } else if (X[0] < -20) {
        direcao = 2;
    } else if (X[1] > 20) {
        direcao = 4;
    } else if (X[1] < -20) {
        direcao = 3;
    } else
        direcao = 0;
    printf("%d \n", X[1]);
    return direcao;
}

int pegar_direcao_phant() { /*no momento retorna somente o X*/
    int direcao;
    if (pos_x == 20) {
        direcao = 1;
    } else if (pos_x == -20) {
        direcao = 2;
    } else if (pos_y == 20) {
        direcao = 4;
    } else if (pos_y == -20) {
        direcao = 3;
    } else
        direcao = 0;
    printf("pos_x = %d, posy = %d \n, direcao = %d", pos_x, pos_y, ph->direcao);
    return direcao;
}

void desenhar_jogo(char mapa2[SIZE1][SIZE2]) { /*por enquanto sem implementação de sprites*/
    int direcao1 = pegar_direcao_pac();
    int direcao2 = pegar_direcao_phant();

    if (pacman_vivo(pac)) { /*a condição de morte de pacman esta implementada em phantom_movimenta*/
        pacman_altera_posicao(pac, direcao1, mapa2);
        pacman_movimenta(pac, mapa2);
        // pacman_desenha(pac, mapa2);

        ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

        phantom_altera_direcao(ph, direcao2, mapa3);
        phantom_movimenta(ph, mapa3);
        // phantom_desenha(ph, mapa3);

    } else { /*vitoria do fantasma nao implementada*/
        encerrar_jogo();
    }

    if (pac->pontos == MAX_POINTS) {
        encerrar_jogo();
    }

    // ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
    exibir_pontuacao(pac->pontos, 5, 36, pontuacao_matriz);
}

int count_max_points(int size1, int size2, char map[size1][size2]) { /*funcao que contabiliza a quantidade de pontos necessária para que pacman ganhe*/
    int i, j, count;
    for (i = 0; i < size1; i++) {
        for (j = 0; j < size2; j++) {
            if (map[i][j] == 1) {
                count++;
            }
        }
    }
    return count * 10;
};

// FUNCOES PACMAN
int pacman_vivo(Pacman *pac) {
    if (pac->vivo)
        return 1;
    else {
        return 0;
    }
}

// Função que inicializa os dados associados ao pacman
Pacman *pacman_create(int x, int y) {
    Pacman *pac = malloc(sizeof(Pacman));
    if (pac != NULL) {
        pac->invencivel = 0;
        pac->pontos = 0;
        pac->passo = 4;
        pac->vivo = 1;
        pac->status = 0;
        pac->direcao = 0; /*vou definir que 0 = parado; 1 = direita,2 = esquerda,3 = p/ baixo,4 = p/cima*/
        pac->parcial = 0;
        pac->x = x;
        pac->y = y;
        pac->xi = x;
        pac->yi = y;
        pac->xj = x;
        pac->yj = y;
        pac->data_register = 31;
    }
    return pac;
}

void pacman_destroy(Pacman *pac) {
    free(pac);
}

void posicionar_pacman(int x, int y, char mapa2[SIZE1][SIZE2]) {
    pac->x = x;
    pac->y = y;
    mapa2[x][y] = 6;                                                      /*Numero que representa o pacman na matriz de controle(mapa2)*/
    desenhar_sprite(1, 1 + ((pac->y) * 3) * 8, ((pac->x) * 3) * 8, 0, 1); /* A forma com que os sprites sao posicionados se assemelha muito com a forma que a função desenhar_quadrado
                                                                             é implementada na gpu_lib, faz-se necessário que o espaçamento de um quadrado para outro(para este jogo o valor é 3) seja respeitado
                                                                             e também e preciso que haja uma compensação tanto para o deslocamento utilizado para centralizar o sprite(+1) quanto para manter um
                                                                             deslocamento proporcional(*8)
                                                                        */
    // desenhar_sprite(1, ((i + 1) + (pac->yi + 1) * 3)*8,(((pac->x) * 3) + 1)*8-7,1,1);
}

void pontua_verif(Pacman *pac, char mapa2[SIZE1][SIZE2]) {
    if (mapa2[pac->xj][pac->yj] == 2) {
        mapa2[pac->xi][pac->yi] = 0; /*atualiza o valor da posicao anterior de pac caso ele pontue*/
        pac->pontos += 10;
    }
}

void pacman_altera_posicao(Pacman *pac, int direcao, char mapa2[SIZE1][SIZE2]) {
    switch (pac->direcao) {
    case 0:
        pac->direcao = direcao;
        break;
    case 1:
        if (mapa2[(pac->x) + 1][pac->y] < 9) { /*caso não seja parede, altere a direcao*/
            pac->direcao = direcao;
        }
        break;
    case 2:
        if (mapa2[(pac->x) - 1][pac->y] < 9) {
            pac->direcao = direcao;
        }
        break;
    case 3:
        if (mapa2[(pac->x)][pac->y - 1] < 9) {
            pac->direcao = direcao;
        }
        break;
    case 4:
        if (mapa2[(pac->x)][pac->y + 1] < 9) {
            pac->direcao = direcao;
        }
        break;
    default:
        pac->direcao = 0;
        break;
    }
}
void pacman_movimenta(Pacman *pac, char mapa2[SIZE1][SIZE2]) {
    if (pac->vivo == 0)
        return;
    /*
    os elementos xj e yj representam as posicoes futuras de pac, elas se baseiam na sua posicao atual e na sua direcao para decidir as alteracoes necessarias
    ex: para a direcao == 3(ou seja,pac quer se movimentar para baixo) o xj deve ser atualizado
    o uso dos elementos xj e yj auxiliam na verificao de paredes(<9)

    os elementos xi e yi representam as posicoes anteriores de pac, tambem se baseando na sua posicao atual e na sua direcao
    sao muito utilizados na funcao pacman_desenha para auxiliar nos calculos necessarios da trajetoria de pac de um ponto da matriz a outro
    tambem sao utilizados para garantir que a posicao anterior a pacman seja apagada
    */
    switch (pac->direcao) {
    case 0:
        pac->xj = pac->x;
        pac->yj = pac->y;

        pontua_verif(pac, mapa2);
        posicionar_pacman(((pac->x)), (pac->y), mapa2);

        break;
    case 3:
        if (mapa2[(pac->x) + 1][pac->y] < 9) {
            pac->xi = pac->x;
            pac->yi = pac->y;

            pac->xj = pac->x + 2;
            pac->yj = pac->y;

            pontua_verif(pac, mapa2);
            posicionar_pacman(((pac->x) + 1), (pac->y), mapa2);
            pacman_desenha(pac, mapa2);

        } else {
            pac->direcao = 0;
        }
        break;
    case 4:
        if (mapa2[(pac->x) - 1][pac->y] < 9) {
            pac->xi = pac->x;
            pac->yi = pac->y;

            pac->xj = pac->x - 2;
            pac->yj = pac->y;

            pontua_verif(pac, mapa2);
            pacman_desenha(pac, mapa2); /*nos casos em que há uma subtração de linha ou coluna na matriz é necessario que haja uma mudança na ordem das funções
                                          para que a animação ocorra com fluidez
                                        */
            posicionar_pacman(((pac->x) - 1), (pac->y), mapa2);
        } else {
            pac->direcao = 0;
        }
        break;
    case 2:
        if (mapa2[(pac->x)][(pac->y) - 1] < 9) {
            pac->xi = pac->x;
            pac->yi = pac->y;

            pac->xj = pac->x;
            pac->yj = pac->y - 2;

            pontua_verif(pac, mapa2);
            pacman_desenha(pac, mapa2);

            posicionar_pacman(((pac->x)), (pac->y - 1), mapa2);
        } else {
            pac->direcao = 0;
        }

        break;
    case 1:
        if (mapa2[(pac->x)][(pac->y) + 1] < 9) {
            pac->xi = pac->x;
            pac->yi = pac->y;

            pac->xj = pac->x;
            pac->yj = pac->y + 2;

            pontua_verif(pac, mapa2);
            posicionar_pacman(((pac->x)), (pac->y + 1), mapa2);
            pacman_desenha(pac, mapa2);

        } else {
            pac->direcao = 0;
        }
        break;
    }
    mapa2[(pac->xi)][pac->yi] = 0;
}

void pacman_desenha(Pacman *pac, char mapa2[SIZE1][SIZE2]) { /*funcao responsavel pela animacao de pacman*/
    int i;

    if (pac->direcao == 0) { /*nao move*/
        return;
    }

    /*as funcoes a seguir se baseiam na direcao que pacman deseja se locomover(pac->direcao), na sua posicao atual(x ou y ) e na sua posicao anterior(xi ou yi) para desenhar a trajetoria de pac*/
    /*ha algumas verificoes que sao feitas por conta de algumas peculiaridades da exibição da matriz de controle*/
    if (pac->direcao == 1 && pac->yj < 17 && pac->yj > 0 && pac->y != pac->yj && mapa2[pac->x][pac->yj] < 9) { /*move para frente*/
        for (i = 0; i < (pac->passo) - 1; i++) {
            {
                // desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi) * 3, 0, 0, 0, 1);
                // desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi + 1) * 3, 7, 0, 7, 1);
                usleep(80000);
                // desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi + 1) * 3, 0, 0, 0, 1);
                desenhar_sprite(1, ((i + 1) + (pac->yi + 1) * 3) * 8, (((pac->x) * 3) + 1) * 8 - 7, 1, 1);
                trocar_status(pac);
            }
        }
    }

    if (pac->direcao == 2 && pac->yj > 0 && pac->yj < 18 && pac->y != pac->yj && mapa2[pac->x][pac->yj] < 9) { /*move para tras*/
        for (i = (pac->passo) - 1; i > 0; i--) {
            {
                // desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi) * 3, 0, 0, 0, 1);
                // desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi - 1) * 3, 7, 0, 7, 1);
                usleep(80000);
                // desenhar_quadrado(((pac->x) * 3) + 1, (i + 1) + (pac->yi - 1) * 3, 0, 0, 0, 1);

                desenhar_sprite(1, ((i + 1) + (pac->yi - 1) * 3) * 8, (((pac->x) * 3) + 1) * 8 - 7, 1, 1);

                trocar_status(pac);
            }
        }
    }

    if (pac->direcao == 3 && pac->xj < 13 && pac->xj > 0 && pac->x != pac->xj && mapa2[pac->xj][pac->y] < 9) { /*move para baixo*/
        for (i = 0; i < (pac->passo) - 1; i++) {
            {
                // desenhar_quadrado((i + 1) + (pac->xi) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                // desenhar_quadrado((i + 1) + (pac->xi + 1) * 3, ((pac->y) * 3) + 1, 7, 0, 7, 1);
                usleep(80000);
                // desenhar_quadrado((i + 1) + (pac->xi + 1) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);

                desenhar_sprite(1, (((pac->y) * 3) + 1) * 8 - 7, ((i + 1) + (pac->xi + 1) * 3) * 8, 1, 1);
                trocar_status(pac);
            }
        }
    }

    if (pac->direcao == 4 && pac->xj > 0 && pac->xj < 13 && pac->x != pac->xj && mapa2[pac->xj][pac->y] < 9) { /*move para cima*/
        for (i = (pac->passo) - 1; i > 0; i--) {
            {
                // desenhar_quadrado((i + 1) + (pac->xi) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);
                // desenhar_quadrado((i + 1) + (pac->xi - 1) * 3, ((pac->y) * 3) + 1, 7, 0, 7, 1);
                usleep(80000);
                // desenhar_quadrado((i + 1) + (pac->xi - 1) * 3, ((pac->y) * 3) + 1, 0, 0, 0, 1);

                desenhar_sprite(1, (((pac->y) * 3) + 1) * 8 - 7, ((i + 1) + (pac->xi - 1) * 3) * 8, 1, 1);
                trocar_status(pac);
            }
        }
    }
}

void trocar_status(Pacman *pac) { /*status diz qual sprite sera utilizado*/
    pac->status = 1 - pac->status;
}

// FUNCOES PHANTOM
//  Função que inicializa os dados associados ao phantom
Phantom *phantom_create(int x, int y) {

    Phantom *ph = malloc(sizeof(Phantom));
    if (ph != NULL) {
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

void phantom_destroy(Phantom *ph) {
    free(ph);
}

void posicionar_phantom(int x, int y, char mapa2[SIZE1][SIZE2]) {
    ph->x = x;
    ph->y = y;
    mapa2[x][y] = 7; /*Numero que representa o phantom na matriz de controle(mapa2)*/
    desenhar_sprite(2, 1 + ((ph->y) * 3) * 8, ((ph->x) * 3) * 8, 2, 1);
}

void phantom_altera_direcao(Phantom *ph, int direcao, char mapa2[SIZE1][SIZE2]) {
    switch (ph->direcao) {
    case 0:
        ph->direcao = direcao;
        break;
    case 1:
        if (mapa2[(ph->x) + 1][ph->y] < 9) {
            ph->direcao = direcao;
        }
        break;
    case 2:
        if (mapa2[(ph->x) - 1][ph->y] < 9) {
            ph->direcao = direcao;
        }
        break;
    case 3:
        if (mapa2[(ph->x)][ph->y - 1] < 9) {
            ph->direcao = direcao;
        }
        break;
    case 4:
        if (mapa2[(ph->x)][ph->y + 1] < 9) {
            ph->direcao = direcao;
        }
        break;
    default:
        ph->direcao = 0;
        break;
    }
}

void phantom_movimenta(Phantom *ph, char mapa2[SIZE1][SIZE2]) {
    if (ph->vivo == 0)
        return;

    switch (ph->direcao) {
    case 0:
        ph->xj = ph->x;
        ph->yj = ph->y;

        posicionar_phantom(((ph->x)), (ph->y), mapa2);

        break;
    case 3:
        if (mapa2[(ph->x) + 1][ph->y] < 9) {
            ph->xi = ph->x;
            ph->yi = ph->y;

            ph->xj = ph->x + 2;
            ph->yj = ph->y;

            posicionar_phantom(((ph->x) + 1), (ph->y), mapa2);
            phantom_desenha(ph, mapa2);
        } else {
            ph->direcao = 0;
        }
        break;
    case 4:
        if (mapa2[(ph->x) - 1][ph->y] < 9) {
            ph->xi = ph->x;
            ph->yi = ph->y;
            ph->xj = ph->x - 2;
            ph->yj = ph->y;

            phantom_desenha(ph, mapa2);
            posicionar_phantom(((ph->x) - 1), (ph->y), mapa2);
        } else {
            ph->direcao = 0;
        }
        break;
    case 2:
        if (mapa2[(ph->x)][(ph->y) - 1] < 9) {
            ph->xi = ph->x;
            ph->yi = ph->y;
            ph->xj = ph->x;
            ph->yj = ph->y - 2;

            phantom_desenha(ph, mapa2);
            posicionar_phantom(((ph->x)), (ph->y - 1), mapa2);
        } else {
            ph->direcao = 0;
        }

        break;
    case 1:
        if (mapa2[(ph->x)][(ph->y) + 1] < 9) {
            ph->xi = ph->x;
            ph->yi = ph->y;
            ph->xj = ph->x;
            ph->yj = ph->y + 2;

            posicionar_phantom(((ph->x)), (ph->y + 1), mapa2);
            phantom_desenha(ph, mapa2);
        } else {
            ph->direcao = 0;
        }
        break;
    }
    /*fantasma nao apaga blocos*/
    /* troquei essa funcao de lugar para deixar a animacao mais fluida
    if (ph->x == pac->x && ph ->y == pac->y)
    {
        pac->vivo = 0;
    }
    */
}

void phantom_desenha(Phantom *ph, char mapa2[SIZE1][SIZE2]) { /*funcao responsavel pela animacao de phantom*/
    /*por enquanto nao utiliza sprites*/
    int i;

    if (ph->direcao == 0) {
        return;
    }

    if (ph->direcao == 1 && ph->yj < 17 && ph->yj > 0 && ph->y != ph->yj && mapa2[ph->x][ph->yj] < 9) { /*move para frente*/
        for (i = 0; i < (ph->passo) - 1; i++) {
            {
                // desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi) * 3, 0, 0, 0, 1);
                // desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi + 1) * 3, 7, 7, 7, 1);
                usleep(80000);
                // desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi + 1) * 3, 0, 0, 0, 1);

                desenhar_sprite(2, ((i + 1) + (ph->yi + 1) * 3) * 8, (((ph->x) * 3) + 1) * 8 - 7, 2, 1);
                trocar_status_phantom(ph);
            }
        }
    }

    if (ph->direcao == 2 && ph->yj > 0 && ph->yj < 18 && ph->y != ph->yj && mapa2[ph->x][ph->yj] < 9) { /*move para tras*/
        for (i = (ph->passo) - 1; i > 0; i--) {
            {
                // desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi) * 3, 0, 0, 0, 1);
                // desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi - 1) * 3, 7, 7, 7, 1);
                usleep(80000);
                // desenhar_quadrado(((ph->x) * 3) + 1, (i + 1) + (ph->yi - 1) * 3, 0, 0, 0, 1);

                desenhar_sprite(2, ((i + 1) + (ph->yi - 1) * 3) * 8, (((ph->x) * 3) + 1) * 8 - 7, 2, 1);
                trocar_status_phantom(ph);
            }
        }
    }

    if (ph->direcao == 3 && ph->xj < 13 && ph->xj > 0 && ph->x != ph->xj && mapa2[ph->xj][ph->y] < 9) { /*move para baixo*/
        for (i = 0; i < (ph->passo) - 1; i++) {
            {
                // desenhar_quadrado((i + 1) + (ph->xi) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);
                // desenhar_quadrado((i + 1) + (ph->xi + 1) * 3, ((ph->y) * 3) + 1, 7, 7, 7, 1);
                usleep(80000);
                // desenhar_quadrado((i + 1) + (ph->xi + 1) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);

                desenhar_sprite(2, (((ph->y) * 3) + 1) * 8 - 7, ((i + 1) + (ph->xi + 1) * 3) * 8, 2, 1);
                trocar_status_phantom(ph);
            }
        }
    }

    if (ph->direcao == 4 && ph->xj > 0 && ph->xj < 13 && ph->x != ph->xj && mapa2[ph->xj][ph->y] < 9) { /*move para cima*/
        for (i = (ph->passo) - 1; i > 0; i--) {
            {
                // desenhar_quadrado((i + 1) + (ph->xi) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);
                // desenhar_quadrado((i + 1) + (ph->xi - 1) * 3, ((ph->y) * 3) + 1, 7, 7, 7, 1);
                usleep(80000);
                // desenhar_quadrado((i + 1) + (ph->xi - 1) * 3, ((ph->y) * 3) + 1, 0, 0, 0, 1);

                desenhar_sprite(2, (((ph->y) * 3) + 1) * 8 - 7, ((i + 1) + (ph->xi - 1) * 3) * 8, 2, 1);
                trocar_status_phantom(ph);
            }
        }
    }
    if (ph->x == pac->x && ph->y == pac->y) {
        pac->vivo = 0;
    }
}

void trocar_status_phantom(Phantom *ph) { /*status diz qual sprite sera utilizado*/
    ph->status = 1 - ph->status;
}

// FUNCOES AUXILIARES
void invert_map(int size1, int size2, char map[size1][size2]) {
    int i, j;
    for (i = 0; i < size1; i++) {
        for (j = 0; j < size2; j++) {
            map[i][j] = 1 - map[i][j];
        }
    }
}

void mudar_cor_fundo(int linhas, int colunas, char matriz[linhas][colunas], int cor) {
    int i, j;
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            if (!matriz[i][j])
                matriz[i][j] = cor;
}

void mudar_cor_generico(int linhas, int colunas, char matriz[linhas][colunas], int cor) {
    int i, j;
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            if (matriz[i][j] == 1)
                matriz[i][j] = cor;
}

void zerar_matriz(int linhas, int colunas, char matriz[linhas][colunas]) {
    int i, j;
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            matriz[i][j] = 0;
}

void print_map(char map[SIZE1][SIZE2]) {
    for (int i = 0; i < SIZE1; i++) {
        for (int j = 0; j < SIZE2; j++) {
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

void *player_1(void *args) {
    while (ACCEL) {
        if (accelerometer_is_data_ready(regs)) {
            accelerometer_x_read(X, regs); // lê os dados do eixo x
                                           // set_sprite(sprt_1.data_register, X[0], X[1], sprt_1.offset, sprt_1.active);
            while (state != running || pause_game == TRUE) {
            }; /*pausar thread enquanto o estado do jogo for diferente de 1(em jogo) ou variável pause game for acionada(botao)*/
            int di = pegar_direcao_pac();
            pacman_altera_posicao(pac, di, mapa2);
            pacman_movimenta(pac, mapa2);
        }
    }
}

void *player_2(void *args) {
    int dj;
    while (state != finish) {
        pthread_mutex_lock(&player_2_mutex);
        while (state == in_pause || state == in_menu || state == win || state == lose) {
            pthread_cond_wait(&player_2_cond, &player_2_mutex);
        }
        pthread_mutex_unlock(&player_2_mutex);
        mouse_movement(&action, &power_amount);

        while (state != running || pause_game == TRUE) {
        }; /*pausar thread enquanto o estado do jogo for diferente de 1(em jogo) ou variável pause game for acionada(botao)*/
        dj = pegar_direcao_phant();
        printf("X: %d, Y: %d, dir: %d\n", pos_x, pos_y, ph->direcao);
        // printf("X: %d, Y: %d\n", pos_x, pos_y);
        phantom_altera_direcao(ph, dj, mapa3);
        phantom_movimenta(ph, mapa3);
        phantom_desenha(ph, mapa3);
    }
}

void pausar_game() {
    while (1) { // pausa o jogo
        pause_game = 1;
        escreverPause(1, 1, 1, 1, 1, 14, 25, 2);
        if (BUTTON != 2) {
            BUTTON = 0;
            pause_game = 0;
            limpar_tela();
            ler_matriz(SIZE1, SIZE2, map, 3, 0, 0, 3);
            ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);
            exibir_pontuacao(pac->pontos, 5, 36, pontuacao_matriz);
            break;
        }
    }
}

void resetar_game() {
    BUTTON = 0;

    clear_poligonos();
    clear_sprites();
    limpar_tela();
    /*
    ao reiniciar o jogo e necessario que as matrizes 2 e 3 voltem ao seu estado inicial
    por isso elas sao zeradas,invertidas e copiadas novamente.
    */
    // mapa2 = {{0}};
    // mapa3 = {{0}};
    zerar_matriz(SIZE1, SIZE2, mapa2);
    zerar_matriz(SIZE1, SIZE2, mapa3);

    copiar_matriz(SIZE1, SIZE2, mapa2, map);
    invert_map(SIZE1, SIZE2, mapa2);
    copiar_matriz(SIZE1, SIZE2, mapa3, mapa2);

    MAX_POINTS = count_max_points(SIZE1, SIZE2, mapa2); /*define a condição de vitoria de pac*/
    MAX_POINTS -= 10;

    mudar_cor_generico(SIZE1, SIZE2, mapa2, 2);
    mudar_cor_fundo(SIZE1, SIZE2, mapa2, 9);
    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

    state = running;

    pac = pacman_create(1, 1);
    posicionar_pacman(1, 1, mapa2);

    ler_matriz(SIZE1, SIZE2, mapa2, 3, 1, 1, 1);

    ph = phantom_create(13, 18);
    posicionar_phantom(13, 18, mapa3);
}

void *buttons_thread(void *args) {
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
void *pac_text_thread(void *args) {
    int i;
    char vazia[5][41] = {{0}};
    while (1) {
        for (i = 0; i < 40; i++) {
            escrever_PacMan(3, 3, 3, 3, 3, 3, i, 49, 1, 1);
            usleep(80000);
            ler_matriz(5, 41, vazia, 1, i, 49, 1);
        }
        for (i = 39; i > -1; i--) {
            escrever_PacMan(3, 3, 3, 3, 3, 3, i, 49, 1, 1);
            usleep(80000);
            ler_matriz(5, 41, vazia, 1, i, 49, 1);
        }
    }
}

void *ph_text_thread(void *args) {
    int i;
    char vazia[5][41] = {{0}};
    while (1) {
        for (i = 39; i > -1; i--) {
            escrever_Phantom(1, 1, 1, 1, 1, 1, 1, i, 54, 1, 1);
            usleep(80000);
            ler_matriz(5, 41, vazia, 1, i, 54, 1);
        }
        for (i = 0; i < 40; i++) {
            escrever_Phantom(1, 1, 1, 1, 1, 1, 1, i, 54, 1, 1);
            usleep(80000);
            ler_matriz(5, 41, vazia, 1, i, 54, 1);
        }
    }
