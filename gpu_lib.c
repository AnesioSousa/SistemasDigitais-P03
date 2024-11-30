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
void exibir_pontuacao(int pontuacao, int linhas, int colunas, char matriz[linhas][colunas]) {

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
					copiar_matriz(5, 5, digito1, temp);
					break;
				case 2:
					copiar_matriz(5, 5, digito2, temp);
					break;
				case 3:
					copiar_matriz(5, 5, digito3, temp);
					break;
				case 4:
					copiar_matriz(5, 5, digito4, temp);
					break;
				case 5:
					copiar_matriz(5, 5, digito5, temp);
					break;
				case 6:
					copiar_matriz(5, 5, digito6, temp);
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
					copiar_matriz(5, 5, digito1, temp);
					break;
				case 2:
					copiar_matriz(5, 5, digito2, temp);
					break;
				case 3:
					copiar_matriz(5, 5, digito3, temp);
					break;
				case 4:
					copiar_matriz(5, 5, digito4, temp);
					break;
				case 5:
					copiar_matriz(5, 5, digito5, temp);
					break;
				case 6:
					copiar_matriz(5, 5, digito6, temp);
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

void copiar_matriz(int linhas, int colunas, char matriz[linhas][colunas], char temp[linhas][colunas]) {
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

void escrever_PacMan(int corP, int corA, int corC, int corHifen, int corM, int corN, int posX, int posY, int espacamento, int tamanho)
{
    char pacman[5][41] = {{0}};
    char letraP[5][5] = {{0}}, letraA[5][5] = {{0}}, letraC[5][5] = {{0}}, hifen[5][5] = {{0}}, letraN[5][5] = {{0}}, letraM[5][5] = {{0}};
    int i, j;
    // Definindo as letras
    definirLetraP(letraP);
    mudarCor(5, 5, letraP, corP);

    definirLetraA(letraA);
    mudarCor(5, 5, letraA, corA);

    definirLetraC(letraC);
    mudarCor(5, 5, letraC, corC);

    definirHifen(hifen);
    mudarCor(5, 5, hifen, corHifen);

    definirLetraM(letraM);
    mudarCor(5, 5, letraM, corM);

    definirLetraN(letraN);
    mudarCor(5, 5, letraN, corN);

    // Concatenando as letras na matriz
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            pacman[i][j] = letraP[i][j];      // P
            pacman[i][6 + j] = letraA[i][j];  // A
            pacman[i][12 + j] = letraC[i][j]; // C
            pacman[i][18 + j] = hifen[i][j];  // -
            pacman[i][24 + j] = letraM[i][j]; // M
            pacman[i][30 + j] = letraA[i][j]; // A
            pacman[i][36 + j] = letraN[i][j]; // N
        }
    }
    ler_matriz(5, 41, pacman, espacamento, posX, posY, tamanho);
}

void escrever_Phantom(int corP, int corH, int corA, int corN, int corT, int corO, int corM, int posX, int posY, int espacamento, int tamanho)
{
    char phantom[5][41] = {{0}};
    char letraP[5][5] = {{0}}, letraH[5][5] = {{0}}, letraA[5][5] = {{0}}, letraN[5][5] = {{0}}, letraT[5][5] = {{0}}, letraO[5][5] = {{0}}, letraM[5][5] = {{0}};
    int i, j;
    // Definindo as letras
    definirLetraP(letraP);
    mudarCor(5, 5, letraP, corP);

    definirLetraH(letraH);
    mudarCor(5, 5, letraH, corH);

    definirLetraA(letraA);
    mudarCor(5, 5, letraA, corA);

    definirLetraN(letraN);
    mudarCor(5, 5, letraN, corN);

    definirLetraT(letraT);
    mudarCor(5, 5, letraT, corT);

    definirLetraO(letraO);
    mudarCor(5, 5, letraO, corO);

    definirLetraM(letraM);
    mudarCor(5, 5, letraM, corM);

    // Concatenando as letras na matriz
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            phantom[i][j] = letraP[i][j];      // P
            phantom[i][6 + j] = letraH[i][j];  // H
            phantom[i][12 + j] = letraA[i][j]; // A
            phantom[i][18 + j] = letraN[i][j]; // N
            phantom[i][24 + j] = letraT[i][j]; // T
            phantom[i][30 + j] = letraO[i][j]; // O
            phantom[i][36 + j] = letraM[i][j]; // M
        }
    }
    ler_matriz(5, 41, phantom, espacamento, posX, posY, tamanho);
}

void escrever_Wins(int corW, int corI, int corN, int corS,int corExclamacao, int posX, int posY,int espacamento, int tamanho)
{
    char wins[5][31] = {0};
    char letraW[5][5]= {{0}}, letraI[5][5]= {{0}}, letraN[5][5]= {{0}}, letraS[5][5]= {{0}},exclamacao[5][5] = {{0}};
    int i, j;

    definirLetraW(letraW);
    mudarCor(5, 5, letraW, corW);

    definirLetraI(letraI);
    mudarCor(5, 5, letraI, corI);

    definirLetraN(letraN);
    mudarCor(5, 5, letraN, corN);

    definirLetraS(letraS);
    mudarCor(5, 5, letraS, corS);
    
    definirExclamacao(exclamacao);
    mudarCor(5, 5, exclamacao, corExclamacao);

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            wins[i][j] = letraW[i][j]; //W
            wins[i][6 + j] = letraI[i][j];//I
            wins[i][12 + j] = letraN[i][j];//N
            wins[i][18 + j] = letraS[i][j];//S
            wins[i][24 + j] = exclamacao[i][j]; // ! ! !
        }
    }

    ler_matriz(5, 31, wins, espacamento, posX, posY, tamanho);
}

void escrever_PacWins(int posX, int posY,int espacamento, int tamanho){
    escrever_PacMan(3,3,3,2,3,3,posX,posY,espacamento,tamanho);
    escrever_Wins(1,1,1,1,7,posX+5,posY*espacamento,espacamento,tamanho); /*multipliquei o posY pelo espacamento na intenção de ser possível mudar o tamanho dos 2 sem problemas de sobreposição*/
    /*
        P A C - M A N
         W I N S !!! 
    */
}

void escrever_PhantomWins(int posX, int posY,int espacamento, int tamanho){
    escrever_Phantom(7,7,7,7,7,7,7,posX,posY,espacamento,tamanho);
    escrever_Wins(1,1,1,1,3,posX+5,posY*espacamento,espacamento,tamanho); /*multipliquei o posY pelo espacamento na intenção de ser possível mudar o tamanho dos 2 sem problemas de sobreposição*/
    /*
        P H A N T O M
         W I N S !!! 
    */
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
	for(i; i < 800; i++) {
		set_sprite_pixel_color(i, 6, 7, 7);
	}
	set_sprite_pixel_color(400, 6, 7, 7);
	set_sprite_pixel_color(401, 6, 7, 7);
	set_sprite_pixel_color(402, 6, 7, 7);
	set_sprite_pixel_color(403, 6, 7, 7);
	set_sprite_pixel_color(404, 6, 7, 7);
	set_sprite_pixel_color(405, 6, 7, 7);
	set_sprite_pixel_color(406, 6, 7, 7);
	set_sprite_pixel_color(407, 6, 7, 7);
	set_sprite_pixel_color(408, 0, 0, 0);
	set_sprite_pixel_color(409, 0, 0, 0);
	set_sprite_pixel_color(410, 0, 0, 0);
	set_sprite_pixel_color(411, 0, 0, 0);
	set_sprite_pixel_color(412, 0, 0, 0);
	set_sprite_pixel_color(413, 6, 7, 7);
	set_sprite_pixel_color(414, 6, 7, 7);
	set_sprite_pixel_color(415, 6, 7, 7);
	set_sprite_pixel_color(416, 6, 7, 7);
	set_sprite_pixel_color(417, 6, 7, 7);
	set_sprite_pixel_color(418, 6, 7, 7);
	set_sprite_pixel_color(419, 6, 7, 7);
	set_sprite_pixel_color(420, 6, 7, 7);
	set_sprite_pixel_color(421, 6, 7, 7);
	set_sprite_pixel_color(422, 6, 7, 7);
	set_sprite_pixel_color(423, 6, 7, 7);
	set_sprite_pixel_color(424, 6, 7, 7);
	set_sprite_pixel_color(425, 6, 7, 7);
	set_sprite_pixel_color(426, 7, 7, 0);
	set_sprite_pixel_color(427, 7, 7, 0);
	set_sprite_pixel_color(428, 7, 7, 0);
	set_sprite_pixel_color(429, 7, 7, 0);
	set_sprite_pixel_color(430, 7, 7, 0);
	set_sprite_pixel_color(431, 7, 7, 0);
	set_sprite_pixel_color(432, 7, 7, 0);
	set_sprite_pixel_color(433, 7, 7, 0);
	set_sprite_pixel_color(434, 0, 0, 0);
	set_sprite_pixel_color(435, 6, 7, 7);
	set_sprite_pixel_color(436, 6, 7, 7);
	set_sprite_pixel_color(437, 6, 7, 7);
	set_sprite_pixel_color(438, 6, 7, 7);
	set_sprite_pixel_color(439, 6, 7, 7);
	set_sprite_pixel_color(440, 6, 7, 7);
	set_sprite_pixel_color(441, 6, 7, 7);
	set_sprite_pixel_color(442, 6, 7, 7);
	set_sprite_pixel_color(443, 6, 7, 7);
	set_sprite_pixel_color(444, 0, 0, 0);
	set_sprite_pixel_color(445, 7, 7, 0);
	set_sprite_pixel_color(446, 7, 7, 0);
	set_sprite_pixel_color(447, 7, 7, 0);
	set_sprite_pixel_color(448, 7, 7, 0);
	set_sprite_pixel_color(449, 7, 7, 0);
	set_sprite_pixel_color(450, 7, 7, 0);
	set_sprite_pixel_color(451, 7, 7, 0);
	set_sprite_pixel_color(452, 7, 7, 0);
	set_sprite_pixel_color(453, 7, 7, 0);
	set_sprite_pixel_color(454, 7, 7, 0);
	set_sprite_pixel_color(455, 7, 7, 0);
	set_sprite_pixel_color(456, 6, 7, 7);
	set_sprite_pixel_color(457, 6, 7, 7);
	set_sprite_pixel_color(458, 6, 7, 7);
	set_sprite_pixel_color(459, 6, 7, 7);
	set_sprite_pixel_color(460, 6, 7, 7);
	set_sprite_pixel_color(461, 6, 7, 7);
	set_sprite_pixel_color(462, 6, 7, 7);
	set_sprite_pixel_color(463, 0, 0, 0);
	set_sprite_pixel_color(464, 7, 7, 0);
	set_sprite_pixel_color(465, 7, 7, 0);
	set_sprite_pixel_color(466, 7, 7, 0);
	set_sprite_pixel_color(467, 7, 7, 0);
	set_sprite_pixel_color(468, 7, 7, 0);
	set_sprite_pixel_color(469, 7, 7, 0);
	set_sprite_pixel_color(470, 7, 7, 0);
	set_sprite_pixel_color(471, 7, 7, 0);
	set_sprite_pixel_color(472, 7, 7, 0);
	set_sprite_pixel_color(473, 7, 7, 0);
	set_sprite_pixel_color(474, 7, 7, 0);
	set_sprite_pixel_color(475, 7, 7, 0);
	set_sprite_pixel_color(476, 7, 7, 0);
	set_sprite_pixel_color(477, 6, 7, 7);
	set_sprite_pixel_color(478, 6, 7, 7);
	set_sprite_pixel_color(479, 6, 7, 7);
	set_sprite_pixel_color(480, 6, 7, 7);
	set_sprite_pixel_color(481, 6, 7, 7);
	set_sprite_pixel_color(482, 6, 7, 7);
	set_sprite_pixel_color(483, 7, 7, 0);
	set_sprite_pixel_color(484, 7, 7, 0);
	set_sprite_pixel_color(485, 7, 7, 0);
	set_sprite_pixel_color(486, 7, 7, 0);
	set_sprite_pixel_color(487, 7, 7, 0);
	set_sprite_pixel_color(488, 7, 7, 0);
	set_sprite_pixel_color(489, 7, 7, 0);
	set_sprite_pixel_color(490, 7, 7, 0);
	set_sprite_pixel_color(491, 7, 7, 0);
	set_sprite_pixel_color(492, 7, 7, 0);
	set_sprite_pixel_color(493, 7, 7, 0);
	set_sprite_pixel_color(494, 7, 7, 0);
	set_sprite_pixel_color(495, 7, 7, 0);
	set_sprite_pixel_color(496, 7, 7, 0);
	set_sprite_pixel_color(497, 3, 3, 0);
	set_sprite_pixel_color(498, 6, 7, 7);
	set_sprite_pixel_color(499, 6, 7, 7);
	set_sprite_pixel_color(500, 6, 7, 7);
	set_sprite_pixel_color(501, 6, 7, 7);
	set_sprite_pixel_color(502, 7, 7, 0);
	set_sprite_pixel_color(503, 7, 7, 0);
	set_sprite_pixel_color(504, 7, 7, 0);
	set_sprite_pixel_color(505, 7, 7, 0);
	set_sprite_pixel_color(506, 7, 7, 0);
	set_sprite_pixel_color(507, 7, 7, 0);
	set_sprite_pixel_color(508, 7, 7, 0);
	set_sprite_pixel_color(509, 7, 7, 0);
	set_sprite_pixel_color(510, 7, 7, 0);
	set_sprite_pixel_color(511, 7, 7, 0);
	set_sprite_pixel_color(512, 7, 7, 0);
	set_sprite_pixel_color(513, 7, 7, 0);
	set_sprite_pixel_color(514, 7, 7, 0);
	set_sprite_pixel_color(515, 7, 7, 0);
	set_sprite_pixel_color(516, 7, 7, 0);
	set_sprite_pixel_color(517, 7, 7, 0);
	set_sprite_pixel_color(518, 0, 0, 0);
	set_sprite_pixel_color(519, 6, 7, 7);
	set_sprite_pixel_color(520, 6, 7, 7);
	set_sprite_pixel_color(521, 6, 6, 6);
	set_sprite_pixel_color(522, 7, 7, 0);
	set_sprite_pixel_color(523, 7, 7, 0);
	set_sprite_pixel_color(524, 7, 7, 0);
	set_sprite_pixel_color(525, 7, 7, 0);
	set_sprite_pixel_color(526, 7, 7, 0);
	set_sprite_pixel_color(527, 7, 7, 0);
	set_sprite_pixel_color(528, 7, 7, 0);
	set_sprite_pixel_color(529, 7, 7, 0);
	set_sprite_pixel_color(530, 7, 7, 0);
	set_sprite_pixel_color(531, 7, 7, 0);
	set_sprite_pixel_color(532, 7, 7, 0);
	set_sprite_pixel_color(533, 7, 7, 0);
	set_sprite_pixel_color(534, 7, 7, 0);
	set_sprite_pixel_color(535, 7, 7, 0);
	set_sprite_pixel_color(536, 7, 7, 0);
	set_sprite_pixel_color(537, 7, 7, 0);
	set_sprite_pixel_color(538, 7, 7, 0);
	set_sprite_pixel_color(539, 6, 7, 7);
	set_sprite_pixel_color(540, 6, 7, 7);
	set_sprite_pixel_color(541, 0, 0, 0);
	set_sprite_pixel_color(542, 7, 7, 0);
	set_sprite_pixel_color(543, 7, 7, 0);
	set_sprite_pixel_color(544, 7, 7, 0);
	set_sprite_pixel_color(545, 7, 7, 0);
	set_sprite_pixel_color(546, 7, 7, 0);
	set_sprite_pixel_color(547, 7, 7, 0);
	set_sprite_pixel_color(548, 7, 7, 0);
	set_sprite_pixel_color(549, 7, 7, 0);
	set_sprite_pixel_color(550, 7, 7, 0);
	set_sprite_pixel_color(551, 7, 7, 0);
	set_sprite_pixel_color(552, 7, 7, 0);
	set_sprite_pixel_color(553, 7, 7, 0);
	set_sprite_pixel_color(554, 7, 7, 0);
	set_sprite_pixel_color(555, 7, 7, 0);
	set_sprite_pixel_color(556, 7, 7, 0);
	set_sprite_pixel_color(557, 7, 7, 0);
	set_sprite_pixel_color(558, 7, 7, 0);
	set_sprite_pixel_color(559, 6, 7, 7);
	set_sprite_pixel_color(560, 6, 7, 7);
	set_sprite_pixel_color(561, 7, 7, 0);
	set_sprite_pixel_color(562, 7, 7, 0);
	set_sprite_pixel_color(563, 7, 7, 0);
	set_sprite_pixel_color(564, 7, 7, 0);
	set_sprite_pixel_color(565, 7, 7, 0);
	set_sprite_pixel_color(566, 7, 7, 0);
	set_sprite_pixel_color(567, 7, 7, 0);
	set_sprite_pixel_color(568, 7, 7, 0);
	set_sprite_pixel_color(569, 7, 7, 0);
	set_sprite_pixel_color(570, 7, 7, 0);
	set_sprite_pixel_color(571, 7, 7, 0);
	set_sprite_pixel_color(572, 7, 7, 0);
	set_sprite_pixel_color(573, 7, 7, 0);
	set_sprite_pixel_color(574, 7, 7, 0);
	set_sprite_pixel_color(575, 7, 7, 0);
	set_sprite_pixel_color(576, 7, 7, 0);
	set_sprite_pixel_color(577, 7, 7, 0);
	set_sprite_pixel_color(578, 7, 7, 0);
	set_sprite_pixel_color(579, 0, 0, 0);
	set_sprite_pixel_color(580, 6, 7, 7);
	set_sprite_pixel_color(581, 7, 7, 0);
	set_sprite_pixel_color(582, 7, 7, 0);
	set_sprite_pixel_color(583, 7, 7, 0);
	set_sprite_pixel_color(584, 7, 7, 0);
	set_sprite_pixel_color(585, 7, 7, 0);
	set_sprite_pixel_color(586, 7, 7, 0);
	set_sprite_pixel_color(587, 7, 7, 0);
	set_sprite_pixel_color(588, 7, 7, 0);
	set_sprite_pixel_color(589, 7, 7, 0);
	set_sprite_pixel_color(590, 7, 7, 0);
	set_sprite_pixel_color(591, 7, 7, 0);
	set_sprite_pixel_color(592, 7, 7, 0);
	set_sprite_pixel_color(593, 7, 7, 0);
	set_sprite_pixel_color(594, 7, 7, 0);
	set_sprite_pixel_color(595, 7, 7, 0);
	set_sprite_pixel_color(596, 7, 7, 0);
	set_sprite_pixel_color(597, 7, 7, 0);
	set_sprite_pixel_color(598, 7, 7, 0);
	set_sprite_pixel_color(599, 7, 7, 0);
	set_sprite_pixel_color(600, 6, 7, 7);
	set_sprite_pixel_color(601, 7, 7, 0);
	set_sprite_pixel_color(602, 7, 7, 0);
	set_sprite_pixel_color(603, 7, 7, 0);
	set_sprite_pixel_color(604, 7, 7, 0);
	set_sprite_pixel_color(605, 7, 7, 0);
	set_sprite_pixel_color(606, 7, 7, 0);
	set_sprite_pixel_color(607, 7, 7, 0);
	set_sprite_pixel_color(608, 7, 7, 0);
	set_sprite_pixel_color(609, 7, 7, 0);
	set_sprite_pixel_color(610, 7, 7, 0);
	set_sprite_pixel_color(611, 7, 7, 0);
	set_sprite_pixel_color(612, 7, 7, 0);
	set_sprite_pixel_color(613, 7, 7, 0);
	set_sprite_pixel_color(614, 7, 7, 0);
	set_sprite_pixel_color(615, 7, 7, 0);
	set_sprite_pixel_color(616, 7, 7, 0);
	set_sprite_pixel_color(617, 7, 7, 0);
	set_sprite_pixel_color(618, 7, 7, 0);
	set_sprite_pixel_color(619, 7, 7, 0);
	set_sprite_pixel_color(620, 0, 0, 0);
	set_sprite_pixel_color(621, 7, 7, 0);
	set_sprite_pixel_color(622, 7, 7, 0);
	set_sprite_pixel_color(623, 7, 7, 0);
	set_sprite_pixel_color(624, 7, 7, 0);
	set_sprite_pixel_color(625, 7, 7, 0);
	set_sprite_pixel_color(626, 7, 7, 0);
	set_sprite_pixel_color(627, 7, 7, 0);
	set_sprite_pixel_color(628, 7, 7, 0);
	set_sprite_pixel_color(629, 7, 7, 0);
	set_sprite_pixel_color(630, 7, 7, 0);
	set_sprite_pixel_color(631, 7, 7, 0);
	set_sprite_pixel_color(632, 7, 7, 0);
	set_sprite_pixel_color(633, 0, 0, 0);
	set_sprite_pixel_color(634, 0, 0, 0);
	set_sprite_pixel_color(635, 7, 7, 0);
	set_sprite_pixel_color(636, 7, 7, 0);
	set_sprite_pixel_color(637, 7, 7, 0);
	set_sprite_pixel_color(638, 7, 7, 0);
	set_sprite_pixel_color(639, 7, 7, 0);
	set_sprite_pixel_color(640, 3, 3, 3);
	set_sprite_pixel_color(641, 7, 7, 0);
	set_sprite_pixel_color(642, 7, 7, 0);
	set_sprite_pixel_color(643, 7, 7, 0);
	set_sprite_pixel_color(644, 7, 7, 0);
	set_sprite_pixel_color(645, 7, 7, 0);
	set_sprite_pixel_color(646, 7, 7, 0);
	set_sprite_pixel_color(647, 7, 7, 0);
	set_sprite_pixel_color(648, 7, 7, 0);
	set_sprite_pixel_color(649, 0, 0, 0);
	set_sprite_pixel_color(650, 6, 7, 7);
	set_sprite_pixel_color(651, 7, 7, 0);
	set_sprite_pixel_color(652, 7, 7, 0);
	set_sprite_pixel_color(653, 0, 0, 0);
	set_sprite_pixel_color(654, 0, 0, 0);
	set_sprite_pixel_color(655, 7, 7, 0);
	set_sprite_pixel_color(656, 7, 7, 0);
	set_sprite_pixel_color(657, 7, 7, 0);
	set_sprite_pixel_color(658, 7, 7, 0);
	set_sprite_pixel_color(659, 7, 7, 0);
	set_sprite_pixel_color(660, 6, 7, 7);
	set_sprite_pixel_color(661, 7, 7, 0);
	set_sprite_pixel_color(662, 7, 7, 0);
	set_sprite_pixel_color(663, 7, 7, 0);
	set_sprite_pixel_color(664, 7, 7, 0);
	set_sprite_pixel_color(665, 7, 7, 0);
	set_sprite_pixel_color(666, 7, 7, 0);
	set_sprite_pixel_color(667, 7, 7, 0);
	set_sprite_pixel_color(668, 0, 0, 0);
	set_sprite_pixel_color(669, 6, 7, 7);
	set_sprite_pixel_color(670, 6, 7, 7);
	set_sprite_pixel_color(671, 0, 0, 0);
	set_sprite_pixel_color(672, 7, 7, 0);
	set_sprite_pixel_color(673, 7, 7, 0);
	set_sprite_pixel_color(674, 7, 7, 0);
	set_sprite_pixel_color(675, 7, 7, 0);
	set_sprite_pixel_color(676, 7, 7, 0);
	set_sprite_pixel_color(677, 7, 7, 0);
	set_sprite_pixel_color(678, 7, 7, 0);
	set_sprite_pixel_color(679, 7, 7, 0);
	set_sprite_pixel_color(680, 6, 7, 7);
	set_sprite_pixel_color(681, 7, 7, 0);
	set_sprite_pixel_color(682, 7, 7, 0);
	set_sprite_pixel_color(683, 7, 7, 0);
	set_sprite_pixel_color(684, 7, 7, 0);
	set_sprite_pixel_color(685, 7, 7, 0);
	set_sprite_pixel_color(686, 7, 7, 0);
	set_sprite_pixel_color(687, 0, 0, 0);
	set_sprite_pixel_color(688, 6, 7, 7);
	set_sprite_pixel_color(689, 6, 7, 7);
	set_sprite_pixel_color(690, 6, 7, 7);
	set_sprite_pixel_color(691, 6, 7, 7);
	set_sprite_pixel_color(692, 0, 0, 0);
	set_sprite_pixel_color(693, 7, 7, 0);
	set_sprite_pixel_color(694, 7, 7, 0);
	set_sprite_pixel_color(695, 7, 7, 0);
	set_sprite_pixel_color(696, 7, 7, 0);
	set_sprite_pixel_color(697, 7, 7, 0);
	set_sprite_pixel_color(698, 7, 7, 0);
	set_sprite_pixel_color(699, 0, 0, 0);
	set_sprite_pixel_color(700, 6, 7, 7);
	set_sprite_pixel_color(701, 7, 7, 0);
	set_sprite_pixel_color(702, 7, 7, 0);
	set_sprite_pixel_color(703, 7, 7, 0);
	set_sprite_pixel_color(704, 7, 7, 0);
	set_sprite_pixel_color(705, 7, 7, 0);
	set_sprite_pixel_color(706, 0, 0, 0);
	set_sprite_pixel_color(707, 6, 7, 7);
	set_sprite_pixel_color(708, 6, 7, 7);
	set_sprite_pixel_color(709, 6, 7, 7);
	set_sprite_pixel_color(710, 6, 7, 7);
	set_sprite_pixel_color(711, 6, 7, 7);
	set_sprite_pixel_color(712, 6, 7, 7);
	set_sprite_pixel_color(713, 0, 0, 0);
	set_sprite_pixel_color(714, 7, 7, 0);
	set_sprite_pixel_color(715, 7, 7, 0);
	set_sprite_pixel_color(716, 7, 7, 0);
	set_sprite_pixel_color(717, 7, 7, 0);
	set_sprite_pixel_color(718, 7, 7, 0);
	set_sprite_pixel_color(719, 0, 0, 0);
	set_sprite_pixel_color(720, 6, 7, 7);
	set_sprite_pixel_color(721, 0, 0, 0);
	set_sprite_pixel_color(722, 7, 7, 0);
	set_sprite_pixel_color(723, 7, 7, 0);
	set_sprite_pixel_color(724, 7, 7, 0);
	set_sprite_pixel_color(725, 0, 0, 0);
	set_sprite_pixel_color(726, 6, 7, 7);
	set_sprite_pixel_color(727, 6, 7, 7);
	set_sprite_pixel_color(728, 6, 7, 7);
	set_sprite_pixel_color(729, 6, 7, 7);
	set_sprite_pixel_color(730, 6, 7, 7);
	set_sprite_pixel_color(731, 6, 7, 7);
	set_sprite_pixel_color(732, 6, 7, 7);
	set_sprite_pixel_color(733, 6, 7, 7);
	set_sprite_pixel_color(734, 7, 7, 0);
	set_sprite_pixel_color(735, 7, 7, 0);
	set_sprite_pixel_color(736, 7, 7, 0);
	set_sprite_pixel_color(737, 7, 7, 0);
	set_sprite_pixel_color(738, 7, 7, 0);
	set_sprite_pixel_color(739, 6, 7, 7);
	set_sprite_pixel_color(740, 6, 7, 7);
	set_sprite_pixel_color(741, 6, 7, 7);
	set_sprite_pixel_color(742, 7, 7, 0);
	set_sprite_pixel_color(743, 7, 7, 0);
	set_sprite_pixel_color(744, 6, 6, 0);
	set_sprite_pixel_color(745, 6, 7, 7);
	set_sprite_pixel_color(746, 6, 7, 7);
	set_sprite_pixel_color(747, 6, 7, 7);
	set_sprite_pixel_color(748, 6, 7, 7);
	set_sprite_pixel_color(749, 6, 7, 7);
	set_sprite_pixel_color(750, 6, 7, 7);
	set_sprite_pixel_color(751, 6, 7, 7);
	set_sprite_pixel_color(752, 6, 7, 7);
	set_sprite_pixel_color(753, 6, 7, 7);
	set_sprite_pixel_color(754, 6, 7, 7);
	set_sprite_pixel_color(755, 7, 7, 0);
	set_sprite_pixel_color(756, 7, 7, 0);
	set_sprite_pixel_color(757, 7, 7, 0);
	set_sprite_pixel_color(758, 0, 0, 0);
	set_sprite_pixel_color(759, 6, 7, 7);
	set_sprite_pixel_color(760, 6, 7, 7);
	set_sprite_pixel_color(761, 6, 7, 7);
	set_sprite_pixel_color(762, 0, 0, 0);
	set_sprite_pixel_color(763, 7, 7, 0);
	set_sprite_pixel_color(764, 6, 7, 7);
	set_sprite_pixel_color(765, 6, 7, 7);
	set_sprite_pixel_color(766, 6, 7, 7);
	set_sprite_pixel_color(767, 6, 7, 7);
	set_sprite_pixel_color(768, 6, 7, 7);
	set_sprite_pixel_color(769, 6, 7, 7);
	set_sprite_pixel_color(770, 6, 7, 7);
	set_sprite_pixel_color(771, 6, 7, 7);
	set_sprite_pixel_color(772, 6, 7, 7);
	set_sprite_pixel_color(773, 6, 7, 7);
	set_sprite_pixel_color(774, 6, 7, 7);
	set_sprite_pixel_color(775, 0, 0, 0);
	set_sprite_pixel_color(776, 7, 7, 0);
	set_sprite_pixel_color(777, 7, 7, 0);
	set_sprite_pixel_color(778, 6, 7, 7);
	set_sprite_pixel_color(779, 6, 7, 7);
	set_sprite_pixel_color(780, 6, 7, 7);
	set_sprite_pixel_color(781, 6, 7, 7);
	set_sprite_pixel_color(782, 6, 7, 7);
	set_sprite_pixel_color(783, 6, 7, 7);
	set_sprite_pixel_color(784, 6, 7, 7);
	set_sprite_pixel_color(785, 6, 7, 7);
	set_sprite_pixel_color(786, 6, 7, 7);
	set_sprite_pixel_color(787, 6, 7, 7);
	set_sprite_pixel_color(788, 6, 7, 7);
	set_sprite_pixel_color(789, 6, 7, 7);
	set_sprite_pixel_color(790, 6, 7, 7);
	set_sprite_pixel_color(791, 6, 7, 7);
	set_sprite_pixel_color(792, 6, 7, 7);
	set_sprite_pixel_color(793, 6, 7, 7);
	set_sprite_pixel_color(794, 6, 7, 7);
	set_sprite_pixel_color(795, 6, 7, 7);
	set_sprite_pixel_color(796, 0, 0, 0);
	set_sprite_pixel_color(797, 0, 0, 0);
	set_sprite_pixel_color(798, 6, 7, 7);
	set_sprite_pixel_color(799, 6, 7, 7);
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
