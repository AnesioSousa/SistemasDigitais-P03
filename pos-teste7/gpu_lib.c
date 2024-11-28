#include "gpu_lib.h"
#define LADO_QUADRADO 3
#define offsett_X 0
#define offsett_Y 0

/* Chamadas ao assembly */
extern void mem_map();
extern void set_background_block(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R);
extern void set_sprite(uint32_t reg, uint32_t x, uint32_t y, uint32_t offset, uint32_t sp);
extern void clear_background();
extern int time_is_up();
extern void reset_pulsecounter();
extern void mem_unmap();
extern int button();
extern void set_sprite_pixel_color();
extern void set_polygon();

void mapear_gpu() {
    mem_map();
}

void desmapear_gpu() {
    mem_unmap();
}

void desenhar_sprite(uint32_t reg, uint32_t x, uint32_t y, uint32_t offset, uint32_t sp) {
    set_sprite(reg, x, y, offset, sp);
}

void desenhar_quadrado(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R, int tamanho_quadrado) {
    int i, j;
    for (i = 0; i < tamanho_quadrado; i++) {
        for (j = 0; j < tamanho_quadrado; j++) {
            set_background_block(line + i, col + j, B, G, R);
        }
    }
}

// espaçamento deve ser maior ou igual ao tamanho do quadrado para evitar conflitos de sobreposição
void ler_matriz(int Max_linhas, int Max_colunas, char Matriz[Max_linhas][Max_colunas], int espacamento, int off_X, int off_Y, int tamanho_quadrado) {
    int i, j;
    for (i = 0; i < Max_linhas; i++)
        for (j = 0; j < Max_colunas; j++) {
            switch (Matriz[i][j]) { // posicionamento dos quadrados de acordo com suas coordenadas na matriz original,escolha de escala(tamanho_quadrado e espacamento) e escolha das cores com base no valor presente em Matriz[i][j]
            case 1:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 0, 0, 7, tamanho_quadrado);
                break;
            case 0:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 0, 0, 0, tamanho_quadrado);
                break;
            case 2:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 0, 7, 0, tamanho_quadrado);
                break;
            case 3:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 7, 0, 0, tamanho_quadrado);
                break;
            case 4:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 7, 7, 0, tamanho_quadrado);
                break;
            case 5:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 0, 7, 7, tamanho_quadrado);
                break;
            case 6:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 7, 0, 7, tamanho_quadrado);
                break;
            case 7:
                desenhar_quadrado((i * espacamento) + off_Y, (j * espacamento) + off_X, 7, 7, 7, tamanho_quadrado);
                break;
            }
        }
}

/*
todas as funções que escrevem palavras predefinidas seguem a mesma lógica:
definição das letras que serão utilizadas em matrizes 5x5 e preenchimento de uma matriz maior de acordo com a necessidade
a palavra TETRIS possui 6 letras logo necessita de no mínimo uma matriz de 5x(5x6) de tamanho para ser exibida corretamente
*/
void escreverTetris(int corT, int corE, int corR, int corI, int corS, int posX, int posY, int tamanho) {
    char tetris[5][36] = {{0}};
    char letraT[5][5] = {{0}}, letraE[5][5] = {{0}}, letraR[5][5] = {{0}}, letraI[5][5] = {{0}}, letraS[5][5] = {{0}};
    int i, j;
    // Definindo as letras
    definirLetraT(letraT);
    mudarCor(5, 5, letraT, corT);

    definirLetraE(letraE);
    mudarCor(5, 5, letraE, corE);

    definirLetraR(letraR);
    mudarCor(5, 5, letraR, corR);

    definirLetraI(letraI);
    mudarCor(5, 5, letraI, corI);

    definirLetraS(letraS);
    mudarCor(5, 5, letraS, corS);

    // Concatenando as letras na matriz
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            tetris[i][j] = letraT[i][j];      // T
            tetris[i][6 + j] = letraE[i][j];  // E
            tetris[i][12 + j] = letraT[i][j]; // T
            tetris[i][18 + j] = letraR[i][j]; // R
            tetris[i][24 + j] = letraI[i][j]; // I
            tetris[i][30 + j] = letraS[i][j]; // S
        }
    }
    ler_matriz(5, 36, tetris, 2, posX, posY, tamanho);
}

void escrever_Pts(int corT, int corS, int corP, int corDoisP, int posX, int posY, int tamanho) {
    char pts[5][26] = {{0}};
    char letraT[5][5] = {{0}}, letraS[5][5] = {{0}}, letraP[5][5] = {{0}}, doisP[5][5] = {{0}};
    int i, j;

    definirLetraT(letraT);
    mudarCor(5, 5, letraT, corT);

    definirLetraS(letraS);
    mudarCor(5, 5, letraS, corS);

    definirLetraP(letraP);
    mudarCor(5, 5, letraP, corP);

    definirDoisPontos(doisP);
    mudarCor(5, 5, doisP, corDoisP);

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            pts[i][j] = letraP[i][j];
            pts[i][6 + j] = letraT[i][j];
            pts[i][12 + j] = letraS[i][j];
            pts[i][18 + j] = doisP[i][j];
        }
    }
    ler_matriz(5, 26, pts, 1, posX, posY, tamanho);
}
void escrever_Borda(int linhas, int colunas, char matriz[linhas][colunas], int cor) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (i == (linhas - 1) || j == 0 || j == (colunas - 1)) {
                matriz[i][j] = cor;
            }
        }
    }
}
void exibirPontuacao(int pontuacao, int linhas, int colunas, char matriz[linhas][colunas]) {

    int i, j;
    int c = 0;
    int p = pontuacao;
    char digito1[5][5] = {{0}}, digito2[5][5] = {{0}}, digito3[5][5] = {{0}}, digito4[5][5] = {{0}}, digito5[5][5] = {{0}}, digito6[5][5] = {{0}};
    while (p != 0) {
        p /= 10;
        c++;
    }

    int numberArray[c];

    c = 0;
    p = pontuacao;

    if (p != 0) {
        /* separação de cada dígito da pontuação */
        while (p != 0) {
            numberArray[c] = p % 10;
            p /= 10;
            c++;
        }
        for (i = 0; i < c; i++) {
            char temp[5][5] = {{0}};
            switch (numberArray[i]) { /*atribuição de uma matriz correspondente ao valor de numberArray[i] a variável temporária*/
            case 0:
                definirNumero0(temp);
                break;
            case 1:
                definirNumero1(temp);
                break;
            case 2:
                definirNumero2(temp);
                break;
            case 3:
                definirNumero3(temp);
                break;
            case 4:
                definirNumero4(temp);
                break;
            case 5:
                definirNumero5(temp);
                break;
            case 6:
                definirNumero6(temp);
                break;
            case 7:
                definirNumero7(temp);
                break;
            case 8:
                definirNumero8(temp);
                break;
            case 9:
                definirNumero9(temp);
                break;
            }
            switch (i + 1) { /*atribuição da variável temporária a submatriz que corresponde a sua posição no vetor*/
            case 1:
                copiarMatriz(5, 5, digito1, temp);
                break;
            case 2:
                copiarMatriz(5, 5, digito2, temp);
                break;
            case 3:
                copiarMatriz(5, 5, digito3, temp);
                break;
            case 4:
                copiarMatriz(5, 5, digito4, temp);
                break;
            case 5:
                copiarMatriz(5, 5, digito5, temp);
                break;
            case 6:
                copiarMatriz(5, 5, digito6, temp);
                break;
            }
        }
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) { /*preenchimento da matriz que será exibida*/
                matriz[i][j] = digito6[i][j];
                matriz[i][6 + j] = digito5[i][j];
                matriz[i][12 + j] = digito4[i][j];
                matriz[i][18 + j] = digito3[i][j];
                matriz[i][24 + j] = digito2[i][j];
                matriz[i][30 + j] = digito1[i][j];
            }
        }
        ler_matriz(linhas, colunas, matriz, 1, 30, 54, 1);
    } else {
        for (i = 0; i < 6; i++) {
            char temp[5][5] = {{0}};
            definirNumero0(temp);
            switch (i + 1) {
            case 1:
                copiarMatriz(5, 5, digito1, temp);
                break;
            case 2:
                copiarMatriz(5, 5, digito2, temp);
                break;
            case 3:
                copiarMatriz(5, 5, digito3, temp);
                break;
            case 4:
                copiarMatriz(5, 5, digito4, temp);
                break;
            case 5:
                copiarMatriz(5, 5, digito5, temp);
                break;
            case 6:
                copiarMatriz(5, 5, digito6, temp);
                break;
            }
        }
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                matriz[i][j] = digito6[i][j];
                matriz[i][6 + j] = digito5[i][j];
                matriz[i][12 + j] = digito4[i][j];
                matriz[i][18 + j] = digito3[i][j];
                matriz[i][24 + j] = digito2[i][j];
                matriz[i][30 + j] = digito1[i][j];
            }
        }
        ler_matriz(linhas, colunas, matriz, 1, 30, 54, 1);
    }
}

void copiarMatriz(int linhas, int colunas, char matriz[linhas][colunas], char temp[linhas][colunas]) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            matriz[i][j] = temp[i][j];
        }
    }
}

void mudarCor(int linhas, int colunas, char matriz[5][5], int cor) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] == 1) {
                matriz[i][j] = cor;
            }
        }
    }
}

void desenhar_bloco(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R) {
    set_background_block(line, col, B, G, R);
}

void limpar_tela() {
    int i, j;
    for (i = 0; i <= 59; i++) {
        for (j = 0; j <= 79; j++) {
            set_background_block(i, j, 0, 0, 0);
        }
    }
}

int renderizou() {
    return time_is_up();
}

/*return void?*/
void renderizar_por_mais_tempo() {
    return reset_pulsecounter();
}

int botoes() {
    return button();
}

void escrever_GameOver(int corG, int corA, int corM, int corE, int corO, int corV, int corR, int posX, int posY, int tamanho) {
    char GameOver[5][60] = {{0}};
    char letraG[5][5] = {{0}}, letraA[5][5] = {{0}}, letraM[5][5] = {{0}}, letraO[5][5] = {{0}}, letraV[5][5] = {{0}}, letraE[5][5] = {{0}}, letraR[5][5] = {{0}};
    int i, j;

    definirLetraG(letraG);
    mudarCor(5, 5, letraG, corG);

    definirLetraA(letraA);
    mudarCor(5, 5, letraA, corA);

    definirLetraM(letraM);
    mudarCor(5, 5, letraM, corM);

    definirLetraE(letraE);
    mudarCor(5, 5, letraE, corE);

    definirLetraO(letraO);
    mudarCor(5, 5, letraO, corO);

    definirLetraV(letraV);
    mudarCor(5, 5, letraV, corV);

    definirLetraE(letraE);
    mudarCor(5, 5, letraE, corE);

    definirLetraR(letraR);
    mudarCor(5, 5, letraR, corR);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            GameOver[i][j] = letraG[i][j];
            GameOver[i][6 + j] = letraA[i][j];
            GameOver[i][12 + j] = letraM[i][j];
            GameOver[i][18 + j] = letraE[i][j];
            GameOver[i][30 + j] = letraO[i][j];
            GameOver[i][36 + j] = letraV[i][j];
            GameOver[i][42 + j] = letraE[i][j];
            GameOver[i][48 + j] = letraR[i][j];
        }
    }
    ler_matriz(5, 60, GameOver, 1, posX, posY, tamanho);
}

void escreverPause(int corP, int corA, int corU, int corS, int corE, int posX, int posY, int tamanho) {
    char Pause[5][36] = {{0}};
    char letraP[5][5] = {{0}}, letraA[5][5] = {{0}}, letraU[5][5] = {{0}}, letraS[5][5] = {{0}}, letraE[5][5] = {{0}};
    int i, j;
    // Definindo as letras
    definirLetraP(letraP);
    mudarCor(5, 5, letraP, corP);

    definirLetraA(letraA);
    mudarCor(5, 5, letraA, corA);

    definirLetraU(letraU);
    mudarCor(5, 5, letraU, corU);

    definirLetraS(letraS);
    mudarCor(5, 5, letraS, corS);

    definirLetraE(letraE);
    mudarCor(5, 5, letraE, corE);

    // Concatenando as letras na matriz
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            Pause[i][j] = letraP[i][j];      // T
            Pause[i][6 + j] = letraA[i][j];  // E
            Pause[i][12 + j] = letraU[i][j]; // T
            Pause[i][18 + j] = letraS[i][j]; // R
            Pause[i][24 + j] = letraE[i][j]; // I
        }
    }
    ler_matriz(5, 36, Pause, 2, posX, posY, tamanho);
}

void escreverPressionePB(int corP, int corR, int corE, int corS, int corI, int corO, int corN, int corB, int corA, int corJ, int corG, int posX, int posY, int tamanho) {
    int i, j;
    char Pressione[5][60] = {{0}};
    char PB[5][16] = {{0}};
    char Para[5][30] = {{0}};
    char Jogar[5][31] = {{0}};
    char letraP[5][5] = {{0}}, letraR[5][5] = {{0}}, letraE[5][5] = {{0}}, letraS[5][5] = {{0}}, letraI[5][5] = {{0}}, letraO[5][5] = {{0}}, letraN[5][5] = {{0}}, letraB[5][5] = {{0}}, letraA[5][5] = {{0}}, letraJ[5][5] = {{0}}, letraG[5][5] = {{0}};

    definirLetraP(letraP);
    mudarCor(5, 5, letraP, corP);

    definirLetraR(letraR);
    mudarCor(5, 5, letraR, corR);

    definirLetraE(letraE);
    mudarCor(5, 5, letraE, corE);

    definirLetraS(letraS);
    mudarCor(5, 5, letraS, corS);

    definirLetraI(letraI);
    mudarCor(5, 5, letraI, corI);

    definirLetraO(letraO);
    mudarCor(5, 5, letraO, corO);

    definirLetraN(letraN);
    mudarCor(5, 5, letraN, corN);

    definirLetraA(letraA);
    mudarCor(5, 5, letraA, corA);

    definirLetraB(letraB);
    mudarCor(5, 5, letraB, corB);

    definirLetraJ(letraJ);
    mudarCor(5, 5, letraJ, corJ);

    definirLetraG(letraG);
    mudarCor(5, 5, letraG, corG);

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

    // PB

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            PB[i][j] = letraP[i][j];
            PB[i][6 + j] = letraB[i][j];
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
    ler_matriz(5, 60, Pressione, 1, posX, posY, tamanho);
    ler_matriz(5, 16, PB, 1, posX + 60, posY, tamanho);
    ler_matriz(5, 30, Para, 1, posX + 5, posY + 6, tamanho);
    ler_matriz(5, 31, Jogar, 1, posX + 36, posY + 6, tamanho);
}

void setar_cor_pixel_sprite(uint32_t endereco, uint32_t B, uint32_t G, uint32_t R) {
    int i = 400;
    for (i; i < 800; i++) {
        set_sprite_pixel_color(i, 6, 7, 7);
    }
    set_sprite_pixel_color(423, 1, 1, 1);
    set_sprite_pixel_color(424, 1, 1, 1);
    set_sprite_pixel_color(435, 1, 1, 1);
    set_sprite_pixel_color(436, 1, 1, 1);
    set_sprite_pixel_color(443, 1, 1, 1);
    set_sprite_pixel_color(444, 4, 4, 4);
    set_sprite_pixel_color(445, 1, 1, 1);
    set_sprite_pixel_color(454, 1, 1, 1);
    set_sprite_pixel_color(455, 4, 4, 4);
    set_sprite_pixel_color(456, 1, 1, 1);
    set_sprite_pixel_color(462, 1, 1, 1);
    set_sprite_pixel_color(463, 4, 4, 4);
    set_sprite_pixel_color(464, 4, 4, 4);
    set_sprite_pixel_color(465, 4, 4, 4);
    set_sprite_pixel_color(466, 1, 1, 1);
    set_sprite_pixel_color(473, 1, 1, 1);
    set_sprite_pixel_color(474, 4, 4, 4);
    set_sprite_pixel_color(475, 4, 4, 4);
    set_sprite_pixel_color(476, 4, 4, 4);
    set_sprite_pixel_color(477, 1, 1, 1);
    set_sprite_pixel_color(482, 1, 1, 1);
    set_sprite_pixel_color(483, 4, 4, 4);
    set_sprite_pixel_color(484, 4, 4, 4);
    set_sprite_pixel_color(485, 4, 4, 4);
    set_sprite_pixel_color(486, 4, 4, 4);
    set_sprite_pixel_color(487, 1, 1, 1);
    set_sprite_pixel_color(492, 1, 1, 1);
    set_sprite_pixel_color(493, 4, 4, 4);
    set_sprite_pixel_color(494, 4, 4, 4);
    set_sprite_pixel_color(495, 4, 4, 4);
    set_sprite_pixel_color(496, 4, 4, 4);
    set_sprite_pixel_color(497, 1, 1, 1);
    set_sprite_pixel_color(502, 1, 1, 1);
    set_sprite_pixel_color(503, 4, 4, 4);
    set_sprite_pixel_color(504, 7, 5, 5);
    set_sprite_pixel_color(505, 7, 5, 5);
    set_sprite_pixel_color(506, 4, 4, 4);
    set_sprite_pixel_color(507, 1, 1, 1);
    set_sprite_pixel_color(512, 1, 1, 1);
    set_sprite_pixel_color(513, 4, 4, 4);
    set_sprite_pixel_color(514, 7, 5, 5);
    set_sprite_pixel_color(515, 7, 5, 5);
    set_sprite_pixel_color(516, 4, 4, 4);
    set_sprite_pixel_color(517, 1, 1, 1);
    set_sprite_pixel_color(522, 1, 1, 1);
    set_sprite_pixel_color(523, 4, 4, 4);
    set_sprite_pixel_color(524, 7, 5, 5);
    set_sprite_pixel_color(525, 7, 5, 5);
    set_sprite_pixel_color(526, 4, 4, 4);
    set_sprite_pixel_color(527, 1, 1, 1);
    set_sprite_pixel_color(528, 1, 1, 1);
    set_sprite_pixel_color(529, 1, 1, 1);
    set_sprite_pixel_color(530, 1, 1, 1);
    set_sprite_pixel_color(531, 1, 1, 1);
    set_sprite_pixel_color(532, 1, 1, 1);
    set_sprite_pixel_color(533, 4, 4, 4);
    set_sprite_pixel_color(534, 7, 5, 5);
    set_sprite_pixel_color(535, 7, 5, 5);
    set_sprite_pixel_color(536, 4, 4, 4);
    set_sprite_pixel_color(537, 1, 1, 1);
    set_sprite_pixel_color(542, 1, 1, 1);
    set_sprite_pixel_color(543, 4, 4, 4);
    set_sprite_pixel_color(544, 4, 4, 4);
    set_sprite_pixel_color(545, 4, 4, 4);
    set_sprite_pixel_color(546, 4, 4, 4);
    set_sprite_pixel_color(547, 4, 4, 4);
    set_sprite_pixel_color(548, 3, 3, 3);
    set_sprite_pixel_color(549, 3, 3, 3);
    set_sprite_pixel_color(550, 3, 3, 3);
    set_sprite_pixel_color(551, 3, 3, 3);
    set_sprite_pixel_color(552, 4, 4, 4);
    set_sprite_pixel_color(553, 4, 4, 4);
    set_sprite_pixel_color(554, 4, 4, 4);
    set_sprite_pixel_color(555, 4, 4, 4);
    set_sprite_pixel_color(556, 4, 4, 4);
    set_sprite_pixel_color(557, 1, 1, 1);
    set_sprite_pixel_color(562, 1, 1, 1);
    set_sprite_pixel_color(563, 4, 4, 4);
    set_sprite_pixel_color(564, 4, 4, 4);
    set_sprite_pixel_color(565, 4, 4, 4);
    set_sprite_pixel_color(566, 4, 4, 4);
    set_sprite_pixel_color(567, 4, 4, 4);
    set_sprite_pixel_color(568, 4, 4, 4);
    set_sprite_pixel_color(569, 3, 3, 3);
    set_sprite_pixel_color(570, 3, 3, 3);
    set_sprite_pixel_color(571, 4, 4, 4);
    set_sprite_pixel_color(572, 4, 4, 4);
    set_sprite_pixel_color(573, 4, 4, 4);
    set_sprite_pixel_color(574, 4, 4, 4);
    set_sprite_pixel_color(575, 4, 4, 4);
    set_sprite_pixel_color(576, 4, 4, 4);
    set_sprite_pixel_color(577, 1, 1, 1);
    set_sprite_pixel_color(582, 1, 1, 1);
    set_sprite_pixel_color(583, 4, 4, 4);
    set_sprite_pixel_color(584, 4, 4, 4);
    set_sprite_pixel_color(585, 4, 4, 4);
    set_sprite_pixel_color(586, 4, 4, 4);
    set_sprite_pixel_color(587, 4, 4, 4);
    set_sprite_pixel_color(588, 4, 4, 4);
    set_sprite_pixel_color(589, 4, 4, 4);
    set_sprite_pixel_color(590, 4, 4, 4);
    set_sprite_pixel_color(591, 4, 4, 4);
    set_sprite_pixel_color(592, 4, 4, 4);
    set_sprite_pixel_color(593, 4, 4, 4);
    set_sprite_pixel_color(594, 4, 4, 4);
    set_sprite_pixel_color(595, 4, 4, 4);
    set_sprite_pixel_color(596, 4, 4, 4);
    set_sprite_pixel_color(597, 1, 1, 1);
    set_sprite_pixel_color(602, 1, 1, 1);
    set_sprite_pixel_color(603, 4, 4, 4);
    set_sprite_pixel_color(604, 4, 4, 4);
    set_sprite_pixel_color(605, 4, 4, 4);
    set_sprite_pixel_color(606, 4, 4, 4);
    set_sprite_pixel_color(607, 4, 4, 4);
    set_sprite_pixel_color(608, 4, 4, 4);
    set_sprite_pixel_color(609, 4, 4, 4);
    set_sprite_pixel_color(610, 4, 4, 4);
    set_sprite_pixel_color(611, 4, 4, 4);
    set_sprite_pixel_color(612, 4, 4, 4);
    set_sprite_pixel_color(613, 4, 4, 4);
    set_sprite_pixel_color(614, 4, 4, 4);
    set_sprite_pixel_color(615, 4, 4, 4);
    set_sprite_pixel_color(616, 4, 4, 4);
    set_sprite_pixel_color(617, 1, 1, 1);
    set_sprite_pixel_color(621, 1, 1, 1);
    set_sprite_pixel_color(622, 4, 4, 4);
    set_sprite_pixel_color(623, 4, 4, 4);
    set_sprite_pixel_color(624, 4, 4, 4);
    set_sprite_pixel_color(625, 4, 4, 4);
    set_sprite_pixel_color(626, 3, 3, 3);
    set_sprite_pixel_color(627, 3, 3, 3);
    set_sprite_pixel_color(628, 4, 4, 4);
    set_sprite_pixel_color(629, 4, 4, 4);
    set_sprite_pixel_color(630, 4, 4, 4);
    set_sprite_pixel_color(631, 4, 4, 4);
    set_sprite_pixel_color(632, 3, 3, 3);
    set_sprite_pixel_color(633, 3, 3, 3);
    set_sprite_pixel_color(634, 4, 4, 4);
    set_sprite_pixel_color(635, 4, 4, 4);
    set_sprite_pixel_color(636, 4, 4, 4);
    set_sprite_pixel_color(637, 4, 4, 4);
    set_sprite_pixel_color(638, 1, 1, 1);
    set_sprite_pixel_color(640, 1, 1, 1);
    set_sprite_pixel_color(641, 4, 4, 4);
    set_sprite_pixel_color(642, 4, 4, 4);
    set_sprite_pixel_color(643, 4, 4, 4);
    set_sprite_pixel_color(644, 4, 4, 4);
    set_sprite_pixel_color(645, 4, 4, 4);
    set_sprite_pixel_color(646, 3, 3, 3);
    set_sprite_pixel_color(647, 7, 7, 7);
    set_sprite_pixel_color(648, 4, 4, 4);
    set_sprite_pixel_color(649, 4, 4, 4);
    set_sprite_pixel_color(650, 4, 4, 4);
    set_sprite_pixel_color(651, 4, 4, 4);
    set_sprite_pixel_color(652, 3, 3, 3);
    set_sprite_pixel_color(653, 7, 7, 7);
    set_sprite_pixel_color(654, 4, 4, 4);
    set_sprite_pixel_color(655, 4, 4, 4);
    set_sprite_pixel_color(656, 4, 4, 4);
    set_sprite_pixel_color(657, 4, 4, 4);
    set_sprite_pixel_color(658, 4, 4, 4);
    set_sprite_pixel_color(659, 1, 1, 1);
    set_sprite_pixel_color(661, 1, 1, 1);
    set_sprite_pixel_color(662, 4, 4, 4);
    set_sprite_pixel_color(663, 4, 4, 4);
    set_sprite_pixel_color(664, 4, 4, 4);
    set_sprite_pixel_color(665, 4, 4, 4);
    set_sprite_pixel_color(666, 4, 4, 4);
    set_sprite_pixel_color(667, 4, 4, 4);
    set_sprite_pixel_color(668, 4, 4, 4);
    set_sprite_pixel_color(669, 4, 4, 4);
    set_sprite_pixel_color(670, 4, 4, 4);
    set_sprite_pixel_color(671, 4, 4, 4);
    set_sprite_pixel_color(672, 4, 4, 4);
    set_sprite_pixel_color(673, 4, 4, 4);
    set_sprite_pixel_color(674, 4, 4, 4);
    set_sprite_pixel_color(675, 4, 4, 4);
    set_sprite_pixel_color(676, 4, 4, 4);
    set_sprite_pixel_color(677, 4, 4, 4);
    set_sprite_pixel_color(678, 1, 1, 1);
    set_sprite_pixel_color(680, 1, 1, 1);
    set_sprite_pixel_color(681, 3, 3, 3);
    set_sprite_pixel_color(682, 3, 3, 3);
    set_sprite_pixel_color(683, 4, 4, 4);
    set_sprite_pixel_color(684, 4, 4, 4);
    set_sprite_pixel_color(685, 4, 4, 4);
    set_sprite_pixel_color(686, 4, 4, 4);
    set_sprite_pixel_color(687, 4, 4, 4);
    set_sprite_pixel_color(688, 4, 4, 4);
    set_sprite_pixel_color(689, 7, 5, 5);
    set_sprite_pixel_color(690, 7, 5, 5);
    set_sprite_pixel_color(691, 4, 4, 4);
    set_sprite_pixel_color(692, 4, 4, 4);
    set_sprite_pixel_color(693, 4, 4, 4);
    set_sprite_pixel_color(694, 4, 4, 4);
    set_sprite_pixel_color(695, 4, 4, 4);
    set_sprite_pixel_color(696, 4, 4, 4);
    set_sprite_pixel_color(697, 3, 3, 3);
    set_sprite_pixel_color(698, 3, 3, 3);
    set_sprite_pixel_color(699, 1, 1, 1);
    set_sprite_pixel_color(701, 1, 1, 1);
    set_sprite_pixel_color(702, 3, 3, 3);
    set_sprite_pixel_color(703, 4, 4, 4);
    set_sprite_pixel_color(704, 4, 4, 4);
    set_sprite_pixel_color(705, 4, 4, 4);
    set_sprite_pixel_color(706, 4, 4, 4);
    set_sprite_pixel_color(707, 4, 4, 4);
    set_sprite_pixel_color(708, 4, 4, 4);
    set_sprite_pixel_color(709, 4, 4, 4);
    set_sprite_pixel_color(710, 4, 4, 4);
    set_sprite_pixel_color(711, 4, 4, 4);
    set_sprite_pixel_color(712, 4, 4, 4);
    set_sprite_pixel_color(713, 4, 4, 4);
    set_sprite_pixel_color(714, 4, 4, 4);
    set_sprite_pixel_color(715, 4, 4, 4);
    set_sprite_pixel_color(716, 4, 4, 4);
    set_sprite_pixel_color(717, 3, 3, 3);
    set_sprite_pixel_color(718, 1, 1, 1);
    set_sprite_pixel_color(722, 1, 1, 1);
    set_sprite_pixel_color(723, 1, 1, 1);
    set_sprite_pixel_color(724, 4, 4, 4);
    set_sprite_pixel_color(725, 4, 4, 4);
    set_sprite_pixel_color(726, 4, 4, 4);
    set_sprite_pixel_color(727, 4, 4, 4);
    set_sprite_pixel_color(728, 4, 4, 4);
    set_sprite_pixel_color(729, 4, 4, 4);
    set_sprite_pixel_color(730, 4, 4, 4);
    set_sprite_pixel_color(731, 4, 4, 4);
    set_sprite_pixel_color(732, 4, 4, 4);
    set_sprite_pixel_color(733, 4, 4, 4);
    set_sprite_pixel_color(734, 4, 4, 4);
    set_sprite_pixel_color(735, 4, 4, 4);
    set_sprite_pixel_color(736, 1, 1, 1);
    set_sprite_pixel_color(737, 1, 1, 1);
    set_sprite_pixel_color(744, 1, 1, 1);
    set_sprite_pixel_color(745, 1, 1, 1);
    set_sprite_pixel_color(746, 4, 4, 4);
    set_sprite_pixel_color(747, 4, 4, 4);
    set_sprite_pixel_color(748, 4, 4, 4);
    set_sprite_pixel_color(749, 4, 4, 4);
    set_sprite_pixel_color(750, 4, 4, 4);
    set_sprite_pixel_color(751, 4, 4, 4);
    set_sprite_pixel_color(752, 4, 4, 4);
    set_sprite_pixel_color(753, 4, 4, 4);
    set_sprite_pixel_color(754, 1, 1, 1);
    set_sprite_pixel_color(755, 1, 1, 1);
    set_sprite_pixel_color(766, 1, 1, 1);
    set_sprite_pixel_color(767, 1, 1, 1);
    set_sprite_pixel_color(768, 1, 1, 1);
    set_sprite_pixel_color(769, 1, 1, 1);
    set_sprite_pixel_color(770, 1, 1, 1);
    set_sprite_pixel_color(771, 1, 1, 1);
    set_sprite_pixel_color(772, 1, 1, 1);
    set_sprite_pixel_color(773, 1, 1, 1);
}

void desenhar_poligono(uint32_t endereco, uint32_t forma, uint32_t B, uint32_t G, uint32_t R, uint32_t tamanho, uint32_t ponto_ref_y, uint32_t ponto_ref_x) {
    set_polygon(endereco, forma, B, G, R, tamanho, ponto_ref_y, ponto_ref_x);
}

void clear_poligonos()
{
    int i = 0;
    for (i; i < 15; i++)
    {
        desenhar_poligono(i, 0, 0, 0, 0, 0, 0, 0);
    }
}

void clear_sprites()
{
    int i = 1;
    for (i; i < 32; i++)
    {
        set_sprite(i, 0, 0, 0, 0);
    }
}
