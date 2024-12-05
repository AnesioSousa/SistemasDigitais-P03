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

void escrever_Menu(int corI, int corN, int corC, int corA, int corR, int corE, int pos_x, int pos_y)
{
	int i, j;
	char Iniciar[5][41] = {{0}};
	char Encerrar[5][48] = {{0}};
	char Pressione[5][60] = {{0}};
	char Para[5][30] = {{0}};
	char Jogar[5][31] = {{0}};
	char PB[5][16] = {{0}};

	char letraP[5][5] = {{0}}, letraR[5][5] = {{0}}, letraE[5][5] = {{0}}, letraS[5][5] = {{0}}, letraI[5][5] = {{0}}, letraO[5][5] = {{0}}, letraN[5][5] = {{0}};
	char letraC[5][5] = {{0}}, letraA[5][5] = {{0}};
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
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
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
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			Para[i][j] = letraP[i][j];
			Para[i][6 + j] = letraA[i][j];
			Para[i][12 + j] = letraR[i][j];
			Para[i][18 + j] = letraA[i][j];
		}
	}
	// JOGAR
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			Jogar[i][j] = letraJ[i][j];
			Jogar[i][6 + j] = letraO[i][j];
			Jogar[i][12 + j] = letraG[i][j];
			Jogar[i][18 + j] = letraA[i][j];
			Jogar[i][24 + j] = letraR[i][j];
		}
	}

	// INICIAR
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			Iniciar[i][j] = letraI[i][j];	   // I
			Iniciar[i][6 + j] = letraN[i][j];  // N
			Iniciar[i][12 + j] = letraI[i][j]; // I
			Iniciar[i][18 + j] = letraC[i][j]; // C
			Iniciar[i][24 + j] = letraI[i][j]; // I
			Iniciar[i][30 + j] = letraA[i][j]; // A
			Iniciar[i][36 + j] = letraR[i][j]; // R
		}
	}

	// ENCERRAR
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			Encerrar[i][j] = letraE[i][j];		// E
			Encerrar[i][6 + j] = letraN[i][j];	// N
			Encerrar[i][12 + j] = letraC[i][j]; // C
			Encerrar[i][18 + j] = letraE[i][j]; // E
			Encerrar[i][24 + j] = letraR[i][j]; // R
			Encerrar[i][30 + j] = letraR[i][j]; // R
			Encerrar[i][36 + j] = letraA[i][j]; // A
			Encerrar[i][42 + j] = letraR[i][j]; // R
		}
	}

	// PB
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			PB[i][j] = letraP[i][j];
			PB[i][6 + j] = letraB[i][j];
		}
	}

	ler_matriz(5, 60, Pressione, 1, pos_x, pos_y, 1); // PRESSIONE

	ler_matriz(5, 16, PB, 1, pos_x, pos_y + 6, 1);			// PB
	ler_matriz(5, 5, numero1, 1, pos_x + 10, pos_y + 6, 1); // 1
	ler_matriz(5, 30, Para, 1, pos_x + 30, pos_y + 6, 1);	// PARA
	ler_matriz(5, 41, Iniciar, 1, pos_x, pos_y + 12, 1);	// INICIAR

	ler_matriz(5, 16, PB, 1, pos_x, pos_y + 21, 1);			 // PB
	ler_matriz(5, 5, numero3, 1, pos_x + 11, pos_y + 21, 1); // 3
	ler_matriz(5, 30, Para, 1, pos_x + 30, pos_y + 21, 1);	 // PARA
	ler_matriz(5, 48, Encerrar, 1, pos_x, pos_y + 27, 1);	 // ENCERRAR
}

void animacao_inicial()
{
	int i;
	for (i = 0; i < 400; i++)
	{
		desenhar_poligono(0, 0, 7, 7, 0, 1, 200, 10 + i);
		desenhar_poligono(1, 0, 7, 7, 0, 1, 200, 20 + i);
		desenhar_poligono(2, 0, 7, 7, 0, 1, 200, 30 + i);
		desenhar_poligono(3, 0, 7, 7, 0, 1, 200, 40 + i);
		desenhar_poligono(4, 0, 7, 7, 0, 1, 200, 50 + i);
		desenhar_poligono(5, 0, 7, 7, 0, 1, 200, 60 + i);
		desenhar_poligono(6, 0, 7, 7, 0, 1, 200, 70 + i);
		desenhar_poligono(7, 0, 7, 7, 0, 1, 200, 80 + i);

		usleep(800);
	}
	for (i = 400; i >= 170; i--)
	{
		desenhar_poligono(8, 0, 7, 7, 0, 1, 200, 10 + i);
		desenhar_poligono(9, 0, 7, 7, 0, 1, 200, 20 + i);
		desenhar_poligono(10, 0, 7, 7, 0, 1, 200, 30 + i);
		desenhar_poligono(11, 0, 7, 7, 0, 1, 200, 40 + i);
		desenhar_poligono(12, 0, 7, 7, 0, 1, 200, 50 + i);
		desenhar_poligono(13, 0, 7, 7, 0, 1, 200, 60 + i);
		desenhar_poligono(14, 0, 7, 7, 0, 1, 200, 70 + i);

		usleep(800);
	}

	escrever_PacMan(3, 3, 3, 3, 3, 3, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(7, 7, 7, 7, 7, 7, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(3, 3, 3, 3, 3, 3, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(7, 7, 7, 7, 7, 7, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(3, 3, 3, 3, 3, 3, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(7, 7, 7, 7, 7, 7, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(3, 3, 3, 3, 3, 3, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(7, 7, 7, 7, 7, 7, 21, 28, 1, 1);
	usleep(800);

	escrever_PacMan(3, 3, 3, 3, 3, 3, 21, 28, 1, 1);
	usleep(800);

	for (i = 400; i >= 170; i--)
	{
		desenhar_poligono(0, 0, 7, 7, 0, 1, 200, 10 + i);
		desenhar_poligono(1, 0, 7, 7, 0, 1, 200, 20 + i);
		desenhar_poligono(2, 0, 7, 7, 0, 1, 200, 30 + i);
		desenhar_poligono(3, 0, 7, 7, 0, 1, 200, 40 + i);
		desenhar_poligono(4, 0, 7, 7, 0, 1, 200, 50 + i);
		desenhar_poligono(5, 0, 7, 7, 0, 1, 200, 60 + i);
		desenhar_poligono(6, 0, 7, 7, 0, 1, 200, 70 + i);
		desenhar_poligono(7, 0, 7, 7, 0, 1, 200, 80 + i);
		usleep(800);
	}

	for (i = 170; i >= 0; i--)
	{
		desenhar_poligono(0, 0, 7, 7, 0, 1, 200, 10 + i);
		desenhar_poligono(1, 0, 7, 7, 0, 1, 200, 20 + i);
		desenhar_poligono(2, 0, 7, 7, 0, 1, 200, 30 + i);
		desenhar_poligono(3, 0, 7, 7, 0, 1, 200, 40 + i);
		desenhar_poligono(4, 0, 7, 7, 0, 1, 200, 50 + i);
		desenhar_poligono(5, 0, 7, 7, 0, 1, 200, 60 + i);
		desenhar_poligono(6, 0, 7, 7, 0, 1, 200, 70 + i);
		desenhar_poligono(7, 0, 7, 7, 0, 1, 200, 80 + i);

		desenhar_poligono(8, 0, 7, 7, 0, 1, 200, 10 + i);
		desenhar_poligono(9, 0, 7, 7, 0, 1, 200, 20 + i);
		desenhar_poligono(10, 0, 7, 7, 0, 1, 200, 30 + i);
		desenhar_poligono(11, 0, 7, 7, 0, 1, 200, 40 + i);
		desenhar_poligono(12, 0, 7, 7, 0, 1, 200, 50 + i);
		desenhar_poligono(13, 0, 7, 7, 0, 1, 200, 60 + i);
		desenhar_poligono(14, 0, 7, 7, 0, 1, 200, 70 + i);

		usleep(800);
	}
	desenhar_poligono(8, 0, 7, 7, 0, 0, 200, 10 + i);
	desenhar_poligono(9, 0, 7, 7, 0, 0, 200, 20 + i);
	desenhar_poligono(10, 0, 7, 7, 0, 0, 200, 30 + i);
	desenhar_poligono(11, 0, 7, 7, 0, 0, 200, 40 + i);
	desenhar_poligono(12, 0, 7, 7, 0, 0, 200, 50 + i);
	desenhar_poligono(13, 0, 7, 7, 0, 0, 200, 60 + i);
	desenhar_poligono(14, 0, 7, 7, 0, 0, 200, 70 + i);

	clear_poligonos();
	limpar_tela();
	escrever_PacMan(3, 3, 3, 3, 3, 3, -1, 10, 2, 2);
	escrever_Menu(1, 1, 1, 1, 1, 1, 15, 25);
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

void setar_cor_pixel_sprite(){
   int i = 0;
    for (i; i < 400; i++) {
        set_sprite_pixel_color(i, 6, 7, 7);
    }
   // Ordem do sprites, 1- ghosts para baixo 2- ghosts para cima 3- ghosts para a direita 4- ghosts para a esquerda 5- pac boca para cima 6- boca para baixo 7- boca para a direita 8- boca para a esquerda 9- bola

    set_sprite_pixel_color(0, 0, 0, 0); //sprite ghosts para baixo
    set_sprite_pixel_color(1, 0, 0, 0);
    set_sprite_pixel_color(2, 0, 0, 0);
    set_sprite_pixel_color(3, 0, 0, 0);
    set_sprite_pixel_color(4, 0, 0, 0);
    set_sprite_pixel_color(5, 0, 0, 0);
    set_sprite_pixel_color(6, 0, 0, 0);
    set_sprite_pixel_color(7, 0, 0, 0);
    set_sprite_pixel_color(8, 0, 0, 0);
    set_sprite_pixel_color(9, 0, 0, 0);
    set_sprite_pixel_color(10, 0, 0, 0);
    set_sprite_pixel_color(11, 0, 0, 0);
    set_sprite_pixel_color(12, 0, 0, 0);
    set_sprite_pixel_color(13, 0, 0, 0);
    set_sprite_pixel_color(14, 0, 0, 0);
    set_sprite_pixel_color(15, 0, 0, 0);
    set_sprite_pixel_color(16, 0, 0, 0);
    set_sprite_pixel_color(17, 0, 0, 0);
    set_sprite_pixel_color(18, 0, 0, 0);
    set_sprite_pixel_color(19, 0, 0, 0);
    set_sprite_pixel_color(20, 0, 0, 0);
    set_sprite_pixel_color(21, 0, 0, 0);
    set_sprite_pixel_color(22, 0, 0, 0);
    set_sprite_pixel_color(23, 0, 0, 0);
    set_sprite_pixel_color(24, 0, 0, 0);
    set_sprite_pixel_color(25, 0, 0, 0);
    set_sprite_pixel_color(26, 0, 0, 0);
    set_sprite_pixel_color(27, 0, 0, 0);
    set_sprite_pixel_color(28, 0, 0, 0);
    set_sprite_pixel_color(29, 0, 0, 0);
    set_sprite_pixel_color(30, 0, 0, 0);
    set_sprite_pixel_color(31, 0, 0, 0);
    set_sprite_pixel_color(32, 0, 0, 0);
    set_sprite_pixel_color(33, 0, 0, 0);
    set_sprite_pixel_color(34, 0, 0, 0);
    set_sprite_pixel_color(35, 0, 0, 0);
    set_sprite_pixel_color(36, 0, 0, 0);
    set_sprite_pixel_color(37, 0, 0, 0);
    set_sprite_pixel_color(38, 0, 0, 0);
    set_sprite_pixel_color(39, 0, 0, 0);
    set_sprite_pixel_color(40, 0, 0, 0);
    set_sprite_pixel_color(41, 0, 0, 0);
    set_sprite_pixel_color(42, 7, 5, 2);
    set_sprite_pixel_color(43, 7, 5, 2);
    set_sprite_pixel_color(44, 7, 5, 2);
    set_sprite_pixel_color(45, 7, 5, 2);
    set_sprite_pixel_color(46, 7, 5, 2);
    set_sprite_pixel_color(47, 7, 5, 2);
    set_sprite_pixel_color(48, 7, 5, 2);
    set_sprite_pixel_color(49, 7, 5, 2);
    set_sprite_pixel_color(50, 7, 5, 2);
    set_sprite_pixel_color(51, 0, 0, 0);
    set_sprite_pixel_color(52, 0, 0, 0);
    set_sprite_pixel_color(53, 0, 0, 0);
    set_sprite_pixel_color(54, 0, 0, 0);
    set_sprite_pixel_color(55, 0, 0, 0);
    set_sprite_pixel_color(56, 0, 0, 0);
    set_sprite_pixel_color(57, 0, 0, 0);
    set_sprite_pixel_color(58, 0, 0, 0);
    set_sprite_pixel_color(59, 0, 0, 0);
    set_sprite_pixel_color(60, 0, 0, 0);
    set_sprite_pixel_color(61, 0, 0, 0);
    set_sprite_pixel_color(62, 6, 6, 6);
    set_sprite_pixel_color(63, 7, 5, 2);
    set_sprite_pixel_color(64, 7, 5, 2);
    set_sprite_pixel_color(65, 7, 5, 2);
    set_sprite_pixel_color(66, 7, 5, 2);
    set_sprite_pixel_color(67, 7, 5, 2);
    set_sprite_pixel_color(68, 7, 5, 2);
    set_sprite_pixel_color(69, 7, 5, 2);
    set_sprite_pixel_color(70, 7, 5, 2);
    set_sprite_pixel_color(71, 7, 5, 2);
    set_sprite_pixel_color(72, 7, 5, 2);
    set_sprite_pixel_color(73, 7, 5, 2);
    set_sprite_pixel_color(74, 6, 6, 6);
    set_sprite_pixel_color(75, 0, 0, 0);
    set_sprite_pixel_color(76, 0, 0, 0);
    set_sprite_pixel_color(77, 0, 0, 0);
    set_sprite_pixel_color(78, 0, 0, 0);
    set_sprite_pixel_color(79, 0, 0, 0);
    set_sprite_pixel_color(80, 0, 0, 0);
    set_sprite_pixel_color(81, 0, 0, 0);
    set_sprite_pixel_color(82, 0, 0, 0);
    set_sprite_pixel_color(83, 7, 5, 2);
    set_sprite_pixel_color(84, 7, 5, 2);
    set_sprite_pixel_color(85, 7, 5, 2);
    set_sprite_pixel_color(86, 7, 5, 2);
    set_sprite_pixel_color(87, 7, 5, 2);
    set_sprite_pixel_color(88, 7, 5, 2);
    set_sprite_pixel_color(89, 7, 5, 2);
    set_sprite_pixel_color(90, 7, 5, 2);
    set_sprite_pixel_color(91, 7, 5, 2);
    set_sprite_pixel_color(92, 7, 5, 2);
    set_sprite_pixel_color(93, 7, 5, 2);
    set_sprite_pixel_color(94, 5, 5, 5);
    set_sprite_pixel_color(95, 5, 5, 5);
    set_sprite_pixel_color(96, 0, 0, 0);
    set_sprite_pixel_color(97, 0, 0, 0);
    set_sprite_pixel_color(98, 0, 0, 0);
    set_sprite_pixel_color(99, 0, 0, 0);
    set_sprite_pixel_color(100, 0, 0, 0);
    set_sprite_pixel_color(101, 0, 0, 0);
    set_sprite_pixel_color(102, 0, 0, 0);
    set_sprite_pixel_color(103, 7, 7, 7);
    set_sprite_pixel_color(104, 4, 4, 5);
    set_sprite_pixel_color(105, 7, 5, 2);
    set_sprite_pixel_color(106, 7, 5, 2);
    set_sprite_pixel_color(107, 7, 5, 2);
    set_sprite_pixel_color(108, 7, 5, 2);
    set_sprite_pixel_color(109, 7, 5, 2);
    set_sprite_pixel_color(110, 7, 5, 2);
    set_sprite_pixel_color(111, 7, 5, 2);
    set_sprite_pixel_color(112, 7, 5, 2);
    set_sprite_pixel_color(113, 7, 5, 2);
    set_sprite_pixel_color(114, 7, 5, 2);
    set_sprite_pixel_color(115, 7, 5, 2);
    set_sprite_pixel_color(116, 0, 0, 0);
    set_sprite_pixel_color(117, 0, 0, 0);
    set_sprite_pixel_color(118, 0, 0, 0);
    set_sprite_pixel_color(119, 0, 0, 0);
    set_sprite_pixel_color(120, 0, 0, 0);
    set_sprite_pixel_color(121, 0, 0, 0);
    set_sprite_pixel_color(122, 0, 0, 0);
    set_sprite_pixel_color(123, 7, 5, 2);
    set_sprite_pixel_color(124, 7, 5, 2);
    set_sprite_pixel_color(125, 7, 5, 2);
    set_sprite_pixel_color(126, 7, 5, 2);
    set_sprite_pixel_color(127, 7, 5, 2);
    set_sprite_pixel_color(128, 7, 5, 2);
    set_sprite_pixel_color(129, 7, 5, 2);
    set_sprite_pixel_color(130, 7, 7, 7);
    set_sprite_pixel_color(131, 7, 7, 7);
    set_sprite_pixel_color(132, 7, 7, 7);
    set_sprite_pixel_color(133, 7, 5, 2);
    set_sprite_pixel_color(134, 7, 5, 2);
    set_sprite_pixel_color(135, 7, 5, 2);
    set_sprite_pixel_color(136, 7, 5, 2);
    set_sprite_pixel_color(137, 0, 0, 0);
    set_sprite_pixel_color(138, 0, 0, 0);
    set_sprite_pixel_color(139, 0, 0, 0);
    set_sprite_pixel_color(140, 0, 0, 0);
    set_sprite_pixel_color(141, 0, 0, 0);
    set_sprite_pixel_color(142, 7, 5, 2);
    set_sprite_pixel_color(143, 7, 5, 2);
    set_sprite_pixel_color(144, 7, 5, 2);
    set_sprite_pixel_color(145, 7, 5, 2);
    set_sprite_pixel_color(146, 7, 5, 2);
    set_sprite_pixel_color(147, 7, 5, 2);
    set_sprite_pixel_color(148, 7, 5, 2);
    set_sprite_pixel_color(149, 7, 7, 7);
    set_sprite_pixel_color(150, 7, 7, 7);
    set_sprite_pixel_color(151, 7, 7, 7);
    set_sprite_pixel_color(152, 7, 7, 7);
    set_sprite_pixel_color(153, 7, 7, 7);
    set_sprite_pixel_color(154, 7, 5, 2);
    set_sprite_pixel_color(155, 7, 5, 2);
    set_sprite_pixel_color(156, 7, 5, 2);
    set_sprite_pixel_color(157, 0, 0, 0);
    set_sprite_pixel_color(158, 0, 0, 0);
    set_sprite_pixel_color(159, 0, 0, 0);
    set_sprite_pixel_color(160, 0, 0, 0);
    set_sprite_pixel_color(161, 0, 0, 0);
    set_sprite_pixel_color(162, 7, 5, 2);
    set_sprite_pixel_color(163, 7, 5, 2);
    set_sprite_pixel_color(164, 7, 5, 2);
    set_sprite_pixel_color(165, 7, 5, 2);
    set_sprite_pixel_color(166, 7, 5, 2);
    set_sprite_pixel_color(167, 7, 5, 2);
    set_sprite_pixel_color(168, 7, 5, 2);
    set_sprite_pixel_color(169, 7, 7, 7);
    set_sprite_pixel_color(170, 2, 3, 5);
    set_sprite_pixel_color(171, 2, 3, 5);
    set_sprite_pixel_color(172, 7, 7, 7);
    set_sprite_pixel_color(173, 7, 7, 7);
    set_sprite_pixel_color(174, 7, 5, 2);
    set_sprite_pixel_color(175, 7, 5, 2);
    set_sprite_pixel_color(176, 7, 5, 2);
    set_sprite_pixel_color(177, 7, 5, 2);
    set_sprite_pixel_color(178, 0, 0, 0);
    set_sprite_pixel_color(179, 0, 0, 0);
    set_sprite_pixel_color(180, 0, 0, 0);
    set_sprite_pixel_color(181, 0, 0, 0);
    set_sprite_pixel_color(182, 0, 0, 0);
    set_sprite_pixel_color(183, 0, 0, 0);
    set_sprite_pixel_color(184, 4, 4, 5);
    set_sprite_pixel_color(185, 7, 5, 2);
    set_sprite_pixel_color(186, 7, 5, 2);
    set_sprite_pixel_color(187, 7, 5, 2);
    set_sprite_pixel_color(188, 7, 5, 2);
    set_sprite_pixel_color(189, 7, 5, 2);
    set_sprite_pixel_color(190, 2, 3, 5);
    set_sprite_pixel_color(191, 2, 3, 5);
    set_sprite_pixel_color(192, 7, 7, 7);
    set_sprite_pixel_color(193, 7, 5, 2);
    set_sprite_pixel_color(194, 7, 5, 2);
    set_sprite_pixel_color(195, 7, 5, 2);
    set_sprite_pixel_color(196, 7, 5, 2);
    set_sprite_pixel_color(197, 7, 5, 2);
    set_sprite_pixel_color(198, 0, 0, 0);
    set_sprite_pixel_color(199, 0, 0, 0);
    set_sprite_pixel_color(200, 0, 0, 0);
    set_sprite_pixel_color(201, 0, 0, 0);
    set_sprite_pixel_color(202, 0, 0, 0);
    set_sprite_pixel_color(203, 0, 0, 0);
    set_sprite_pixel_color(204, 4, 4, 5);
    set_sprite_pixel_color(205, 7, 5, 2);
    set_sprite_pixel_color(206, 7, 5, 2);
    set_sprite_pixel_color(207, 7, 5, 2);
    set_sprite_pixel_color(208, 7, 5, 2);
    set_sprite_pixel_color(209, 7, 5, 2);
    set_sprite_pixel_color(210, 7, 5, 2);
    set_sprite_pixel_color(211, 7, 5, 2);
    set_sprite_pixel_color(212, 7, 5, 2);
    set_sprite_pixel_color(213, 7, 5, 2);
    set_sprite_pixel_color(214, 7, 5, 2);
    set_sprite_pixel_color(215, 7, 5, 2);
    set_sprite_pixel_color(216, 7, 5, 2);
    set_sprite_pixel_color(217, 7, 5, 2);
    set_sprite_pixel_color(218, 0, 0, 0);
    set_sprite_pixel_color(219, 0, 0, 0);
    set_sprite_pixel_color(220, 0, 0, 0);
    set_sprite_pixel_color(221, 0, 0, 0);
    set_sprite_pixel_color(222, 7, 5, 2);
    set_sprite_pixel_color(223, 7, 5, 2);
    set_sprite_pixel_color(224, 7, 5, 2);
    set_sprite_pixel_color(225, 7, 5, 2);
    set_sprite_pixel_color(226, 7, 5, 2);
    set_sprite_pixel_color(227, 7, 5, 2);
    set_sprite_pixel_color(228, 7, 5, 2);
    set_sprite_pixel_color(229, 7, 5, 2);
    set_sprite_pixel_color(230, 7, 5, 2);
    set_sprite_pixel_color(231, 7, 5, 2);
    set_sprite_pixel_color(232, 7, 5, 2);
    set_sprite_pixel_color(233, 7, 5, 2);
    set_sprite_pixel_color(234, 7, 5, 2);
    set_sprite_pixel_color(235, 7, 5, 2);
    set_sprite_pixel_color(236, 7, 5, 2);
    set_sprite_pixel_color(237, 7, 5, 2);
    set_sprite_pixel_color(238, 0, 0, 0);
    set_sprite_pixel_color(239, 0, 0, 0);
    set_sprite_pixel_color(240, 0, 0, 0);
    set_sprite_pixel_color(241, 0, 0, 0);
    set_sprite_pixel_color(242, 7, 5, 2);
    set_sprite_pixel_color(243, 7, 5, 2);
    set_sprite_pixel_color(244, 7, 5, 2);
    set_sprite_pixel_color(245, 7, 5, 2);
    set_sprite_pixel_color(246, 7, 5, 2);
    set_sprite_pixel_color(247, 7, 5, 2);
    set_sprite_pixel_color(248, 7, 5, 2);
    set_sprite_pixel_color(249, 7, 5, 2);
    set_sprite_pixel_color(250, 7, 7, 7);
    set_sprite_pixel_color(251, 7, 7, 7);
    set_sprite_pixel_color(252, 7, 7, 7);
    set_sprite_pixel_color(253, 7, 5, 2);
    set_sprite_pixel_color(254, 7, 5, 2);
    set_sprite_pixel_color(255, 7, 5, 2);
    set_sprite_pixel_color(256, 7, 5, 2);
    set_sprite_pixel_color(257, 0, 0, 0);
    set_sprite_pixel_color(258, 0, 0, 0);
    set_sprite_pixel_color(259, 0, 0, 0);
    set_sprite_pixel_color(260, 0, 0, 0);
    set_sprite_pixel_color(261, 0, 0, 0);
    set_sprite_pixel_color(262, 0, 0, 0);
    set_sprite_pixel_color(263, 7, 5, 2);
    set_sprite_pixel_color(264, 7, 5, 2);
    set_sprite_pixel_color(265, 7, 5, 2);
    set_sprite_pixel_color(266, 7, 5, 2);
    set_sprite_pixel_color(267, 7, 5, 2);
    set_sprite_pixel_color(268, 7, 5, 2);
    set_sprite_pixel_color(269, 7, 7, 7);
    set_sprite_pixel_color(270, 7, 7, 7);
    set_sprite_pixel_color(271, 7, 7, 7);
    set_sprite_pixel_color(272, 7, 7, 7);
    set_sprite_pixel_color(273, 7, 7, 7);
    set_sprite_pixel_color(274, 7, 5, 2);
    set_sprite_pixel_color(275, 7, 5, 2);
    set_sprite_pixel_color(276, 7, 5, 2);
    set_sprite_pixel_color(277, 0, 0, 0);
    set_sprite_pixel_color(278, 0, 0, 0);
    set_sprite_pixel_color(279, 0, 0, 0);
    set_sprite_pixel_color(280, 0, 0, 0);
    set_sprite_pixel_color(281, 0, 0, 0);
    set_sprite_pixel_color(282, 0, 0, 0);
    set_sprite_pixel_color(283, 6, 6, 6);
    set_sprite_pixel_color(284, 4, 4, 5);
    set_sprite_pixel_color(285, 7, 5, 2);
    set_sprite_pixel_color(286, 7, 5, 2);
    set_sprite_pixel_color(287, 7, 5, 2);
    set_sprite_pixel_color(288, 7, 5, 2);
    set_sprite_pixel_color(289, 7, 7, 7);
    set_sprite_pixel_color(290, 1, 2, 5);
    set_sprite_pixel_color(291, 2, 3, 5);
    set_sprite_pixel_color(292, 7, 7, 7);
    set_sprite_pixel_color(293, 7, 7, 7);
    set_sprite_pixel_color(294, 7, 5, 2);
    set_sprite_pixel_color(295, 7, 5, 2);
    set_sprite_pixel_color(296, 6, 6, 6);
    set_sprite_pixel_color(297, 0, 0, 0);
    set_sprite_pixel_color(298, 0, 0, 0);
    set_sprite_pixel_color(299, 0, 0, 0);
    set_sprite_pixel_color(300, 0, 0, 0);
    set_sprite_pixel_color(301, 0, 0, 0);
    set_sprite_pixel_color(302, 0, 0, 0);
    set_sprite_pixel_color(303, 6, 5, 3);
    set_sprite_pixel_color(304, 6, 5, 3);
    set_sprite_pixel_color(305, 7, 5, 2);
    set_sprite_pixel_color(306, 7, 5, 2);
    set_sprite_pixel_color(307, 7, 5, 2);
    set_sprite_pixel_color(308, 7, 5, 2);
    set_sprite_pixel_color(309, 7, 6, 4);
    set_sprite_pixel_color(310, 2, 3, 5);
    set_sprite_pixel_color(311, 2, 3, 5);
    set_sprite_pixel_color(312, 7, 7, 7);
    set_sprite_pixel_color(313, 7, 6, 4);
    set_sprite_pixel_color(314, 6, 5, 3);
    set_sprite_pixel_color(315, 7, 6, 4);
    set_sprite_pixel_color(316, 0, 0, 0);
    set_sprite_pixel_color(317, 0, 0, 0);
    set_sprite_pixel_color(318, 0, 0, 0);
    set_sprite_pixel_color(319, 0, 0, 0);
    set_sprite_pixel_color(320, 0, 0, 0);
    set_sprite_pixel_color(321, 0, 0, 0);
    set_sprite_pixel_color(322, 5, 5, 5);
    set_sprite_pixel_color(323, 7, 5, 2);
    set_sprite_pixel_color(324, 7, 5, 2);
    set_sprite_pixel_color(325, 7, 5, 2);
    set_sprite_pixel_color(326, 7, 5, 2);
    set_sprite_pixel_color(327, 7, 5, 2);
    set_sprite_pixel_color(328, 7, 5, 2);
    set_sprite_pixel_color(329, 7, 5, 2);
    set_sprite_pixel_color(330, 2, 3, 5);
    set_sprite_pixel_color(331, 1, 2, 5);
    set_sprite_pixel_color(332, 7, 7, 7);
    set_sprite_pixel_color(333, 7, 5, 2);
    set_sprite_pixel_color(334, 0, 0, 0);
    set_sprite_pixel_color(335, 0, 0, 0);
    set_sprite_pixel_color(336, 0, 0, 0);
    set_sprite_pixel_color(337, 0, 0, 0);
    set_sprite_pixel_color(338, 0, 0, 0);
    set_sprite_pixel_color(339, 0, 0, 0);
    set_sprite_pixel_color(340, 0, 0, 0);
    set_sprite_pixel_color(341, 0, 0, 0);
    set_sprite_pixel_color(342, 7, 5, 2);
    set_sprite_pixel_color(343, 7, 5, 2);
    set_sprite_pixel_color(344, 7, 5, 2);
    set_sprite_pixel_color(345, 7, 5, 2);
    set_sprite_pixel_color(346, 7, 5, 2);
    set_sprite_pixel_color(347, 7, 5, 2);
    set_sprite_pixel_color(348, 7, 5, 2);
    set_sprite_pixel_color(349, 7, 5, 2);
    set_sprite_pixel_color(350, 7, 5, 2);
    set_sprite_pixel_color(351, 0, 0, 0);
    set_sprite_pixel_color(352, 0, 0, 0);
    set_sprite_pixel_color(353, 0, 0, 0);
    set_sprite_pixel_color(354, 0, 0, 0);
    set_sprite_pixel_color(355, 0, 0, 0);
    set_sprite_pixel_color(356, 0, 0, 0);
    set_sprite_pixel_color(357, 0, 0, 0);
    set_sprite_pixel_color(358, 0, 0, 0);
    set_sprite_pixel_color(359, 0, 0, 0);
    set_sprite_pixel_color(360, 0, 0, 0);
    set_sprite_pixel_color(361, 0, 0, 0);
    set_sprite_pixel_color(362, 0, 0, 0);
    set_sprite_pixel_color(363, 0, 0, 0);
    set_sprite_pixel_color(364, 0, 0, 0);
    set_sprite_pixel_color(365, 0, 0, 0);
    set_sprite_pixel_color(366, 0, 0, 0);
    set_sprite_pixel_color(367, 0, 0, 0);
    set_sprite_pixel_color(368, 0, 0, 0);
    set_sprite_pixel_color(369, 0, 0, 0);
    set_sprite_pixel_color(370, 0, 0, 0);
    set_sprite_pixel_color(371, 0, 0, 0);
    set_sprite_pixel_color(372, 0, 0, 0);
    set_sprite_pixel_color(373, 0, 0, 0);
    set_sprite_pixel_color(374, 0, 0, 0);
    set_sprite_pixel_color(375, 0, 0, 0);
    set_sprite_pixel_color(376, 0, 0, 0);
    set_sprite_pixel_color(377, 0, 0, 0);
    set_sprite_pixel_color(378, 0, 0, 0);
    set_sprite_pixel_color(379, 0, 0, 0);
    set_sprite_pixel_color(380, 0, 0, 0);
    set_sprite_pixel_color(381, 0, 0, 0);
    set_sprite_pixel_color(382, 0, 0, 0);
    set_sprite_pixel_color(383, 0, 0, 0);
    set_sprite_pixel_color(384, 0, 0, 0);
    set_sprite_pixel_color(385, 0, 0, 0);
    set_sprite_pixel_color(386, 0, 0, 0);
    set_sprite_pixel_color(387, 0, 0, 0);
    set_sprite_pixel_color(388, 0, 0, 0);
    set_sprite_pixel_color(389, 0, 0, 0);
    set_sprite_pixel_color(390, 0, 0, 0);
    set_sprite_pixel_color(391, 0, 0, 0);
    set_sprite_pixel_color(392, 0, 0, 0);
    set_sprite_pixel_color(393, 0, 0, 0);
    set_sprite_pixel_color(394, 0, 0, 0);
    set_sprite_pixel_color(395, 0, 0, 0);
    set_sprite_pixel_color(396, 0, 0, 0);
    set_sprite_pixel_color(397, 0, 0, 0);
    set_sprite_pixel_color(398, 0, 0, 0);
    set_sprite_pixel_color(399, 0, 0, 0);

    set_sprite_pixel_color(400, 0, 0, 0); // sprite ghosts para cima
    set_sprite_pixel_color(401, 0, 0, 0);
    set_sprite_pixel_color(402, 0, 0, 0);
    set_sprite_pixel_color(403, 0, 0, 0);
    set_sprite_pixel_color(404, 0, 0, 0);
    set_sprite_pixel_color(405, 0, 0, 0);
    set_sprite_pixel_color(406, 0, 0, 0);
    set_sprite_pixel_color(407, 0, 0, 0);
    set_sprite_pixel_color(408, 0, 0, 0);
    set_sprite_pixel_color(409, 0, 0, 0);
    set_sprite_pixel_color(410, 0, 0, 0);
    set_sprite_pixel_color(411, 0, 0, 0);
    set_sprite_pixel_color(412, 0, 0, 0);
    set_sprite_pixel_color(413, 0, 0, 0);
    set_sprite_pixel_color(414, 0, 0, 0);
    set_sprite_pixel_color(415, 0, 0, 0);
    set_sprite_pixel_color(416, 0, 0, 0);
    set_sprite_pixel_color(417, 0, 0, 0);
    set_sprite_pixel_color(418, 0, 0, 0);
    set_sprite_pixel_color(419, 0, 0, 0);
    set_sprite_pixel_color(420, 0, 0, 0);
    set_sprite_pixel_color(421, 0, 0, 0);
    set_sprite_pixel_color(422, 0, 0, 0);
    set_sprite_pixel_color(423, 0, 0, 0);
    set_sprite_pixel_color(424, 0, 0, 0);
    set_sprite_pixel_color(425, 0, 0, 0);
    set_sprite_pixel_color(426, 0, 0, 0);
    set_sprite_pixel_color(427, 0, 0, 0);
    set_sprite_pixel_color(428, 0, 0, 0);
    set_sprite_pixel_color(429, 0, 0, 0);
    set_sprite_pixel_color(430, 0, 0, 0);
    set_sprite_pixel_color(431, 0, 0, 0);
    set_sprite_pixel_color(432, 0, 0, 0);
    set_sprite_pixel_color(433, 0, 0, 0);
    set_sprite_pixel_color(434, 0, 0, 0);
    set_sprite_pixel_color(435, 0, 0, 0);
    set_sprite_pixel_color(436, 0, 0, 0);
    set_sprite_pixel_color(437, 0, 0, 0);
    set_sprite_pixel_color(438, 0, 0, 0);
    set_sprite_pixel_color(439, 0, 0, 0);
    set_sprite_pixel_color(440, 0, 0, 0);
    set_sprite_pixel_color(441, 0, 0, 0);
    set_sprite_pixel_color(442, 7, 5, 2);
    set_sprite_pixel_color(443, 7, 5, 2);
    set_sprite_pixel_color(444, 7, 5, 2);
    set_sprite_pixel_color(445, 7, 5, 2);
    set_sprite_pixel_color(446, 7, 5, 2);
    set_sprite_pixel_color(447, 7, 5, 2);
    set_sprite_pixel_color(448, 7, 5, 2);
    set_sprite_pixel_color(449, 7, 5, 2);
    set_sprite_pixel_color(450, 7, 5, 2);
    set_sprite_pixel_color(451, 0, 0, 0);
    set_sprite_pixel_color(452, 0, 0, 0);
    set_sprite_pixel_color(453, 0, 0, 0);
    set_sprite_pixel_color(454, 0, 0, 0);
    set_sprite_pixel_color(455, 0, 0, 0);
    set_sprite_pixel_color(456, 0, 0, 0);
    set_sprite_pixel_color(457, 0, 0, 0);
    set_sprite_pixel_color(458, 0, 0, 0);
    set_sprite_pixel_color(459, 0, 0, 0);
    set_sprite_pixel_color(460, 0, 0, 0);
    set_sprite_pixel_color(461, 0, 0, 0);
    set_sprite_pixel_color(462, 6, 6, 6);
    set_sprite_pixel_color(463, 7, 5, 2);
    set_sprite_pixel_color(464, 7, 5, 2);
    set_sprite_pixel_color(465, 7, 5, 2);
    set_sprite_pixel_color(466, 7, 5, 2);
    set_sprite_pixel_color(467, 7, 5, 2);
    set_sprite_pixel_color(468, 7, 5, 2);
    set_sprite_pixel_color(469, 7, 5, 2);
    set_sprite_pixel_color(470, 2, 3, 5);
    set_sprite_pixel_color(471, 2, 3, 5);
    set_sprite_pixel_color(472, 7, 7, 7);
    set_sprite_pixel_color(473, 7, 5, 2);
    set_sprite_pixel_color(474, 0, 0, 0);
    set_sprite_pixel_color(475, 0, 0, 0);
    set_sprite_pixel_color(476, 0, 0, 0);
    set_sprite_pixel_color(477, 0, 0, 0);
    set_sprite_pixel_color(478, 0, 0, 0);
    set_sprite_pixel_color(479, 0, 0, 0);
    set_sprite_pixel_color(480, 0, 0, 0);
    set_sprite_pixel_color(481, 0, 0, 0);
    set_sprite_pixel_color(482, 0, 0, 0);
    set_sprite_pixel_color(483, 5, 5, 5);
    set_sprite_pixel_color(484, 4, 4, 5);
    set_sprite_pixel_color(485, 7, 5, 2);
    set_sprite_pixel_color(486, 7, 5, 2);
    set_sprite_pixel_color(487, 7, 5, 2);
    set_sprite_pixel_color(488, 7, 5, 2);
    set_sprite_pixel_color(489, 7, 7, 7);
    set_sprite_pixel_color(490, 2, 3, 5);
    set_sprite_pixel_color(491, 2, 3, 5);
    set_sprite_pixel_color(492, 7, 7, 7);
    set_sprite_pixel_color(493, 7, 7, 7);
    set_sprite_pixel_color(494, 7, 5, 2);
    set_sprite_pixel_color(495, 7, 5, 2);
    set_sprite_pixel_color(496, 0, 0, 0);
    set_sprite_pixel_color(497, 0, 0, 0);
    set_sprite_pixel_color(498, 0, 0, 0);
    set_sprite_pixel_color(499, 0, 0, 0);
    set_sprite_pixel_color(500, 0, 0, 0);
    set_sprite_pixel_color(501, 0, 0, 0);
    set_sprite_pixel_color(502, 0, 0, 0);
    set_sprite_pixel_color(503, 5, 5, 5);
    set_sprite_pixel_color(504, 4, 4, 5);
    set_sprite_pixel_color(505, 7, 5, 2);
    set_sprite_pixel_color(506, 7, 5, 2);
    set_sprite_pixel_color(507, 7, 5, 2);
    set_sprite_pixel_color(508, 7, 5, 2);
    set_sprite_pixel_color(509, 7, 7, 7);
    set_sprite_pixel_color(510, 1, 2, 5);
    set_sprite_pixel_color(511, 1, 2, 5);
    set_sprite_pixel_color(512, 7, 7, 7);
    set_sprite_pixel_color(513, 7, 7, 7);
    set_sprite_pixel_color(514, 7, 5, 2);
    set_sprite_pixel_color(515, 6, 5, 3);
    set_sprite_pixel_color(516, 6, 6, 6);
    set_sprite_pixel_color(517, 0, 0, 0);
    set_sprite_pixel_color(518, 0, 0, 0);
    set_sprite_pixel_color(519, 0, 0, 0);
    set_sprite_pixel_color(520, 0, 0, 0);
    set_sprite_pixel_color(521, 0, 0, 0);
    set_sprite_pixel_color(522, 7, 7, 7);
    set_sprite_pixel_color(523, 7, 5, 2);
    set_sprite_pixel_color(524, 7, 5, 2);
    set_sprite_pixel_color(525, 7, 5, 2);
    set_sprite_pixel_color(526, 7, 5, 2);
    set_sprite_pixel_color(527, 7, 5, 2);
    set_sprite_pixel_color(528, 7, 5, 2);
    set_sprite_pixel_color(529, 7, 7, 7);
    set_sprite_pixel_color(530, 7, 7, 7);
    set_sprite_pixel_color(531, 7, 7, 7);
    set_sprite_pixel_color(532, 7, 7, 7);
    set_sprite_pixel_color(533, 7, 7, 7);
    set_sprite_pixel_color(534, 7, 5, 2);
    set_sprite_pixel_color(535, 7, 5, 2);
    set_sprite_pixel_color(536, 7, 5, 2);
    set_sprite_pixel_color(537, 0, 0, 0);
    set_sprite_pixel_color(538, 0, 0, 0);
    set_sprite_pixel_color(539, 0, 0, 0);
    set_sprite_pixel_color(540, 0, 0, 0);
    set_sprite_pixel_color(541, 0, 0, 0);
    set_sprite_pixel_color(542, 7, 5, 2);
    set_sprite_pixel_color(543, 7, 5, 2);
    set_sprite_pixel_color(544, 7, 5, 2);
    set_sprite_pixel_color(545, 7, 5, 2);
    set_sprite_pixel_color(546, 7, 5, 2);
    set_sprite_pixel_color(547, 7, 5, 2);
    set_sprite_pixel_color(548, 7, 5, 2);
    set_sprite_pixel_color(549, 7, 5, 2);
    set_sprite_pixel_color(550, 7, 7, 7);
    set_sprite_pixel_color(551, 7, 7, 7);
    set_sprite_pixel_color(552, 7, 7, 7);
    set_sprite_pixel_color(553, 7, 5, 2);
    set_sprite_pixel_color(554, 7, 5, 2);
    set_sprite_pixel_color(555, 7, 5, 2);
    set_sprite_pixel_color(556, 7, 5, 2);
    set_sprite_pixel_color(557, 0, 0, 0);
    set_sprite_pixel_color(558, 0, 0, 0);
    set_sprite_pixel_color(559, 0, 0, 0);
    set_sprite_pixel_color(560, 0, 0, 0);
    set_sprite_pixel_color(561, 0, 0, 0);
    set_sprite_pixel_color(562, 7, 5, 2);
    set_sprite_pixel_color(563, 7, 5, 2);
    set_sprite_pixel_color(564, 7, 5, 2);
    set_sprite_pixel_color(565, 7, 5, 2);
    set_sprite_pixel_color(566, 7, 5, 2);
    set_sprite_pixel_color(567, 7, 5, 2);
    set_sprite_pixel_color(568, 7, 5, 2);
    set_sprite_pixel_color(569, 7, 5, 2);
    set_sprite_pixel_color(570, 7, 5, 2);
    set_sprite_pixel_color(571, 7, 5, 2);
    set_sprite_pixel_color(572, 7, 5, 2);
    set_sprite_pixel_color(573, 7, 5, 2);
    set_sprite_pixel_color(574, 7, 5, 2);
    set_sprite_pixel_color(575, 7, 5, 2);
    set_sprite_pixel_color(576, 7, 5, 2);
    set_sprite_pixel_color(577, 7, 5, 2);
    set_sprite_pixel_color(578, 0, 0, 0);
    set_sprite_pixel_color(579, 0, 0, 0);
    set_sprite_pixel_color(580, 0, 0, 0);
    set_sprite_pixel_color(581, 0, 0, 0);
    set_sprite_pixel_color(582, 0, 0, 0);
    set_sprite_pixel_color(583, 0, 0, 0);
    set_sprite_pixel_color(584, 4, 4, 5);
    set_sprite_pixel_color(585, 7, 5, 2);
    set_sprite_pixel_color(586, 7, 5, 2);
    set_sprite_pixel_color(587, 7, 5, 2);
    set_sprite_pixel_color(588, 7, 5, 2);
    set_sprite_pixel_color(589, 7, 5, 2);
    set_sprite_pixel_color(590, 7, 5, 2);
    set_sprite_pixel_color(591, 7, 5, 2);
    set_sprite_pixel_color(592, 7, 5, 2);
    set_sprite_pixel_color(593, 7, 5, 2);
    set_sprite_pixel_color(594, 7, 5, 2);
    set_sprite_pixel_color(595, 7, 5, 2);
    set_sprite_pixel_color(596, 7, 5, 2);
    set_sprite_pixel_color(597, 7, 5, 2);
    set_sprite_pixel_color(598, 0, 0, 0);
    set_sprite_pixel_color(599, 0, 0, 0);
    set_sprite_pixel_color(600, 0, 0, 0);
    set_sprite_pixel_color(601, 0, 0, 0);
    set_sprite_pixel_color(602, 0, 0, 0);
    set_sprite_pixel_color(603, 0, 0, 0);
    set_sprite_pixel_color(604, 4, 4, 5);
    set_sprite_pixel_color(605, 7, 5, 2);
    set_sprite_pixel_color(606, 7, 5, 2);
    set_sprite_pixel_color(607, 7, 5, 2);
    set_sprite_pixel_color(608, 7, 5, 2);
    set_sprite_pixel_color(609, 7, 5, 2);
    set_sprite_pixel_color(610, 2, 3, 5);
    set_sprite_pixel_color(611, 2, 3, 5);
    set_sprite_pixel_color(612, 7, 7, 7);
    set_sprite_pixel_color(613, 7, 5, 2);
    set_sprite_pixel_color(614, 7, 5, 2);
    set_sprite_pixel_color(615, 7, 5, 2);
    set_sprite_pixel_color(616, 7, 5, 2);
    set_sprite_pixel_color(617, 7, 5, 2);
    set_sprite_pixel_color(618, 0, 0, 0);
    set_sprite_pixel_color(619, 0, 0, 0);
    set_sprite_pixel_color(620, 0, 0, 0);
    set_sprite_pixel_color(621, 0, 0, 0);
    set_sprite_pixel_color(622, 7, 5, 2);
    set_sprite_pixel_color(623, 7, 5, 2);
    set_sprite_pixel_color(624, 7, 5, 2);
    set_sprite_pixel_color(625, 7, 5, 2);
    set_sprite_pixel_color(626, 7, 5, 2);
    set_sprite_pixel_color(627, 7, 5, 2);
    set_sprite_pixel_color(628, 7, 5, 2);
    set_sprite_pixel_color(629, 7, 7, 7);
    set_sprite_pixel_color(630, 2, 3, 5);
    set_sprite_pixel_color(631, 2, 3, 5);
    set_sprite_pixel_color(632, 7, 7, 7);
    set_sprite_pixel_color(633, 7, 7, 7);
    set_sprite_pixel_color(634, 7, 5, 2);
    set_sprite_pixel_color(635, 7, 5, 2);
    set_sprite_pixel_color(636, 7, 5, 2);
    set_sprite_pixel_color(637, 7, 5, 2);
    set_sprite_pixel_color(638, 0, 0, 0);
    set_sprite_pixel_color(639, 0, 0, 0);
    set_sprite_pixel_color(640, 0, 0, 0);
    set_sprite_pixel_color(641, 0, 0, 0);
    set_sprite_pixel_color(642, 7, 5, 2);
    set_sprite_pixel_color(643, 7, 5, 2);
    set_sprite_pixel_color(644, 7, 5, 2);
    set_sprite_pixel_color(645, 7, 5, 2);
    set_sprite_pixel_color(646, 7, 5, 2);
    set_sprite_pixel_color(647, 7, 5, 2);
    set_sprite_pixel_color(648, 7, 5, 2);
    set_sprite_pixel_color(649, 7, 7, 7);
    set_sprite_pixel_color(650, 7, 7, 7);
    set_sprite_pixel_color(651, 7, 7, 7);
    set_sprite_pixel_color(652, 7, 7, 7);
    set_sprite_pixel_color(653, 7, 7, 7);
    set_sprite_pixel_color(654, 7, 5, 2);
    set_sprite_pixel_color(655, 7, 5, 2);
    set_sprite_pixel_color(656, 7, 5, 2);
    set_sprite_pixel_color(657, 0, 0, 0);
    set_sprite_pixel_color(658, 0, 0, 0);
    set_sprite_pixel_color(659, 0, 0, 0);
    set_sprite_pixel_color(660, 0, 0, 0);
    set_sprite_pixel_color(661, 0, 0, 0);
    set_sprite_pixel_color(662, 0, 0, 0);
    set_sprite_pixel_color(663, 7, 5, 2);
    set_sprite_pixel_color(664, 7, 5, 2);
    set_sprite_pixel_color(665, 7, 5, 2);
    set_sprite_pixel_color(666, 7, 5, 2);
    set_sprite_pixel_color(667, 7, 5, 2);
    set_sprite_pixel_color(668, 7, 5, 2);
    set_sprite_pixel_color(669, 7, 5, 2);
    set_sprite_pixel_color(670, 7, 7, 7);
    set_sprite_pixel_color(671, 7, 7, 7);
    set_sprite_pixel_color(672, 7, 7, 7);
    set_sprite_pixel_color(673, 7, 5, 2);
    set_sprite_pixel_color(674, 7, 5, 2);
    set_sprite_pixel_color(675, 7, 5, 2);
    set_sprite_pixel_color(676, 7, 5, 2);
    set_sprite_pixel_color(677, 0, 0, 0);
    set_sprite_pixel_color(678, 0, 0, 0);
    set_sprite_pixel_color(679, 0, 0, 0);
    set_sprite_pixel_color(680, 0, 0, 0);
    set_sprite_pixel_color(681, 0, 0, 0);
    set_sprite_pixel_color(682, 0, 0, 0);
    set_sprite_pixel_color(683, 0, 0, 0);
    set_sprite_pixel_color(684, 4, 4, 5);
    set_sprite_pixel_color(685, 7, 5, 2);
    set_sprite_pixel_color(686, 7, 5, 2);
    set_sprite_pixel_color(687, 7, 5, 2);
    set_sprite_pixel_color(688, 7, 5, 2);
    set_sprite_pixel_color(689, 7, 5, 2);
    set_sprite_pixel_color(690, 7, 5, 2);
    set_sprite_pixel_color(691, 7, 5, 2);
    set_sprite_pixel_color(692, 7, 5, 2);
    set_sprite_pixel_color(693, 7, 5, 2);
    set_sprite_pixel_color(694, 7, 5, 2);
    set_sprite_pixel_color(695, 7, 5, 2);
    set_sprite_pixel_color(696, 0, 0, 0);
    set_sprite_pixel_color(697, 0, 0, 0);
    set_sprite_pixel_color(698, 0, 0, 0);
    set_sprite_pixel_color(699, 0, 0, 0);
    set_sprite_pixel_color(700, 0, 0, 0);
    set_sprite_pixel_color(701, 0, 0, 0);
    set_sprite_pixel_color(702, 0, 0, 0);
    set_sprite_pixel_color(703, 7, 5, 2);
    set_sprite_pixel_color(704, 7, 5, 2);
    set_sprite_pixel_color(705, 7, 5, 2);
    set_sprite_pixel_color(706, 7, 5, 2);
    set_sprite_pixel_color(707, 7, 5, 2);
    set_sprite_pixel_color(708, 7, 5, 2);
    set_sprite_pixel_color(709, 7, 5, 2);
    set_sprite_pixel_color(710, 7, 5, 2);
    set_sprite_pixel_color(711, 7, 5, 2);
    set_sprite_pixel_color(712, 7, 5, 2);
    set_sprite_pixel_color(713, 7, 5, 2);
    set_sprite_pixel_color(714, 6, 6, 6);
    set_sprite_pixel_color(715, 5, 5, 5);
    set_sprite_pixel_color(716, 0, 0, 0);
    set_sprite_pixel_color(717, 0, 0, 0);
    set_sprite_pixel_color(718, 0, 0, 0);
    set_sprite_pixel_color(719, 0, 0, 0);
    set_sprite_pixel_color(720, 0, 0, 0);
    set_sprite_pixel_color(721, 0, 0, 0);
    set_sprite_pixel_color(722, 6, 5, 3);
    set_sprite_pixel_color(723, 7, 5, 2);
    set_sprite_pixel_color(724, 7, 5, 2);
    set_sprite_pixel_color(725, 7, 5, 2);
    set_sprite_pixel_color(726, 7, 5, 2);
    set_sprite_pixel_color(727, 7, 5, 2);
    set_sprite_pixel_color(728, 7, 5, 2);
    set_sprite_pixel_color(729, 7, 5, 2);
    set_sprite_pixel_color(730, 7, 5, 2);
    set_sprite_pixel_color(731, 6, 5, 3);
    set_sprite_pixel_color(732, 6, 5, 3);
    set_sprite_pixel_color(733, 6, 5, 3);
    set_sprite_pixel_color(734, 5, 5, 5);
    set_sprite_pixel_color(735, 0, 0, 0);
    set_sprite_pixel_color(736, 0, 0, 0);
    set_sprite_pixel_color(737, 0, 0, 0);
    set_sprite_pixel_color(738, 0, 0, 0);
    set_sprite_pixel_color(739, 0, 0, 0);
    set_sprite_pixel_color(740, 0, 0, 0);
    set_sprite_pixel_color(741, 0, 0, 0);
    set_sprite_pixel_color(742, 7, 5, 2);
    set_sprite_pixel_color(743, 7, 5, 2);
    set_sprite_pixel_color(744, 7, 5, 2);
    set_sprite_pixel_color(745, 7, 5, 2);
    set_sprite_pixel_color(746, 7, 5, 2);
    set_sprite_pixel_color(747, 7, 5, 2);
    set_sprite_pixel_color(748, 7, 5, 2);
    set_sprite_pixel_color(749, 7, 5, 2);
    set_sprite_pixel_color(750, 7, 5, 2);
    set_sprite_pixel_color(751, 0, 0, 0);
    set_sprite_pixel_color(752, 0, 0, 0);
    set_sprite_pixel_color(753, 0, 0, 0);
    set_sprite_pixel_color(754, 0, 0, 0);
    set_sprite_pixel_color(755, 0, 0, 0);
    set_sprite_pixel_color(756, 0, 0, 0);
    set_sprite_pixel_color(757, 0, 0, 0);
    set_sprite_pixel_color(758, 0, 0, 0);
    set_sprite_pixel_color(759, 0, 0, 0);
    set_sprite_pixel_color(760, 0, 0, 0);
    set_sprite_pixel_color(761, 0, 0, 0);
    set_sprite_pixel_color(762, 0, 0, 0);
    set_sprite_pixel_color(763, 0, 0, 0);
    set_sprite_pixel_color(764, 0, 0, 0);
    set_sprite_pixel_color(765, 0, 0, 0);
    set_sprite_pixel_color(766, 0, 0, 0);
    set_sprite_pixel_color(767, 0, 0, 0);
    set_sprite_pixel_color(768, 0, 0, 0);
    set_sprite_pixel_color(769, 0, 0, 0);
    set_sprite_pixel_color(770, 0, 0, 0);
    set_sprite_pixel_color(771, 0, 0, 0);
    set_sprite_pixel_color(772, 0, 0, 0);
    set_sprite_pixel_color(773, 0, 0, 0);
    set_sprite_pixel_color(774, 0, 0, 0);
    set_sprite_pixel_color(775, 0, 0, 0);
    set_sprite_pixel_color(776, 0, 0, 0);
    set_sprite_pixel_color(777, 0, 0, 0);
    set_sprite_pixel_color(778, 0, 0, 0);
    set_sprite_pixel_color(779, 0, 0, 0);
    set_sprite_pixel_color(780, 0, 0, 0);
    set_sprite_pixel_color(781, 0, 0, 0);
    set_sprite_pixel_color(782, 0, 0, 0);
    set_sprite_pixel_color(783, 0, 0, 0);
    set_sprite_pixel_color(784, 0, 0, 0);
    set_sprite_pixel_color(785, 0, 0, 0);
    set_sprite_pixel_color(786, 0, 0, 0);
    set_sprite_pixel_color(787, 0, 0, 0);
    set_sprite_pixel_color(788, 0, 0, 0);
    set_sprite_pixel_color(789, 0, 0, 0);
    set_sprite_pixel_color(790, 0, 0, 0);
    set_sprite_pixel_color(791, 0, 0, 0);
    set_sprite_pixel_color(792, 0, 0, 0);
    set_sprite_pixel_color(793, 0, 0, 0);
    set_sprite_pixel_color(794, 0, 0, 0);
    set_sprite_pixel_color(795, 0, 0, 0);
    set_sprite_pixel_color(796, 0, 0, 0);
    set_sprite_pixel_color(797, 0, 0, 0);
    set_sprite_pixel_color(798, 0, 0, 0);
    set_sprite_pixel_color(799, 0, 0, 0);

    set_sprite_pixel_color(800, 0, 0, 0); // sprite ghosts para a direita
    set_sprite_pixel_color(801, 0, 0, 0);
    set_sprite_pixel_color(802, 0, 0, 0);
    set_sprite_pixel_color(803, 0, 0, 0);
    set_sprite_pixel_color(804, 0, 0, 0);
    set_sprite_pixel_color(805, 0, 0, 0);
    set_sprite_pixel_color(806, 0, 0, 0);
    set_sprite_pixel_color(807, 0, 0, 0);
    set_sprite_pixel_color(808, 0, 0, 0);
    set_sprite_pixel_color(809, 0, 0, 0);
    set_sprite_pixel_color(810, 0, 0, 0);
    set_sprite_pixel_color(811, 0, 0, 0);
    set_sprite_pixel_color(812, 0, 0, 0);
    set_sprite_pixel_color(813, 0, 0, 0);
    set_sprite_pixel_color(814, 0, 0, 0);
    set_sprite_pixel_color(815, 0, 0, 0);
    set_sprite_pixel_color(816, 0, 0, 0);
    set_sprite_pixel_color(817, 0, 0, 0);
    set_sprite_pixel_color(818, 0, 0, 0);
    set_sprite_pixel_color(819, 0, 0, 0);
    set_sprite_pixel_color(820, 0, 0, 0);
    set_sprite_pixel_color(821, 0, 0, 0);
    set_sprite_pixel_color(822, 0, 0, 0);
    set_sprite_pixel_color(823, 0, 0, 0);
    set_sprite_pixel_color(824, 0, 0, 0);
    set_sprite_pixel_color(825, 0, 0, 0);
    set_sprite_pixel_color(826, 0, 0, 0);
    set_sprite_pixel_color(827, 0, 0, 0);
    set_sprite_pixel_color(828, 0, 0, 0);
    set_sprite_pixel_color(829, 0, 0, 0);
    set_sprite_pixel_color(830, 0, 0, 0);
    set_sprite_pixel_color(831, 0, 0, 0);
    set_sprite_pixel_color(832, 0, 0, 0);
    set_sprite_pixel_color(833, 0, 0, 0);
    set_sprite_pixel_color(834, 0, 0, 0);
    set_sprite_pixel_color(835, 0, 0, 0);
    set_sprite_pixel_color(836, 0, 0, 0);
    set_sprite_pixel_color(837, 0, 0, 0);
    set_sprite_pixel_color(838, 0, 0, 0);
    set_sprite_pixel_color(839, 0, 0, 0);
    set_sprite_pixel_color(840, 0, 0, 0);
    set_sprite_pixel_color(841, 0, 0, 0);
    set_sprite_pixel_color(842, 0, 0, 0);
    set_sprite_pixel_color(843, 0, 0, 0);
    set_sprite_pixel_color(844, 0, 0, 0);
    set_sprite_pixel_color(845, 0, 0, 0);
    set_sprite_pixel_color(846, 0, 0, 0);
    set_sprite_pixel_color(847, 0, 0, 0);
    set_sprite_pixel_color(848, 7, 5, 2);
    set_sprite_pixel_color(849, 7, 5, 2);
    set_sprite_pixel_color(850, 7, 5, 2);
    set_sprite_pixel_color(851, 7, 5, 2);
    set_sprite_pixel_color(852, 0, 0, 0);
    set_sprite_pixel_color(853, 0, 0, 0);
    set_sprite_pixel_color(854, 0, 0, 0);
    set_sprite_pixel_color(855, 0, 0, 0);
    set_sprite_pixel_color(856, 0, 0, 0);
    set_sprite_pixel_color(857, 0, 0, 0);
    set_sprite_pixel_color(858, 0, 0, 0);
    set_sprite_pixel_color(859, 0, 0, 0);
    set_sprite_pixel_color(860, 0, 0, 0);
    set_sprite_pixel_color(861, 0, 0, 0);
    set_sprite_pixel_color(862, 0, 0, 0);
    set_sprite_pixel_color(863, 0, 0, 0);
    set_sprite_pixel_color(864, 0, 0, 0);
    set_sprite_pixel_color(865, 0, 0, 0);
    set_sprite_pixel_color(866, 7, 5, 2);
    set_sprite_pixel_color(867, 7, 5, 2);
    set_sprite_pixel_color(868, 7, 5, 2);
    set_sprite_pixel_color(869, 7, 5, 2);
    set_sprite_pixel_color(870, 7, 5, 2);
    set_sprite_pixel_color(871, 7, 5, 2);
    set_sprite_pixel_color(872, 7, 5, 2);
    set_sprite_pixel_color(873, 7, 5, 2);
    set_sprite_pixel_color(874, 6, 6, 6);
    set_sprite_pixel_color(875, 0, 0, 0);
    set_sprite_pixel_color(876, 0, 0, 0);
    set_sprite_pixel_color(877, 0, 0, 0);
    set_sprite_pixel_color(878, 0, 0, 0);
    set_sprite_pixel_color(879, 0, 0, 0);
    set_sprite_pixel_color(880, 0, 0, 0);
    set_sprite_pixel_color(881, 0, 0, 0);
    set_sprite_pixel_color(882, 0, 0, 0);
    set_sprite_pixel_color(883, 0, 0, 0);
    set_sprite_pixel_color(884, 5, 5, 5);
    set_sprite_pixel_color(885, 7, 5, 2);
    set_sprite_pixel_color(886, 7, 5, 2);
    set_sprite_pixel_color(887, 7, 5, 2);
    set_sprite_pixel_color(888, 7, 5, 2);
    set_sprite_pixel_color(889, 7, 5, 2);
    set_sprite_pixel_color(890, 7, 5, 2);
    set_sprite_pixel_color(891, 7, 5, 2);
    set_sprite_pixel_color(892, 7, 5, 2);
    set_sprite_pixel_color(893, 7, 5, 2);
    set_sprite_pixel_color(894, 7, 5, 2);
    set_sprite_pixel_color(895, 7, 6, 4);
    set_sprite_pixel_color(896, 0, 0, 0);
    set_sprite_pixel_color(897, 0, 0, 0);
    set_sprite_pixel_color(898, 0, 0, 0);
    set_sprite_pixel_color(899, 0, 0, 0);
    set_sprite_pixel_color(900, 0, 0, 0);
    set_sprite_pixel_color(901, 0, 0, 0);
    set_sprite_pixel_color(902, 0, 0, 0);
    set_sprite_pixel_color(903, 5, 5, 5);
    set_sprite_pixel_color(904, 5, 5, 5);
    set_sprite_pixel_color(905, 7, 5, 2);
    set_sprite_pixel_color(906, 7, 5, 2);
    set_sprite_pixel_color(907, 7, 5, 2);
    set_sprite_pixel_color(908, 7, 5, 2);
    set_sprite_pixel_color(909, 7, 5, 2);
    set_sprite_pixel_color(910, 7, 5, 2);
    set_sprite_pixel_color(911, 7, 5, 2);
    set_sprite_pixel_color(912, 7, 5, 2);
    set_sprite_pixel_color(913, 7, 5, 2);
    set_sprite_pixel_color(914, 7, 5, 2);
    set_sprite_pixel_color(915, 6, 5, 3);
    set_sprite_pixel_color(916, 6, 6, 6);
    set_sprite_pixel_color(917, 0, 0, 0);
    set_sprite_pixel_color(918, 0, 0, 0);
    set_sprite_pixel_color(919, 0, 0, 0);
    set_sprite_pixel_color(920, 0, 0, 0);
    set_sprite_pixel_color(921, 0, 0, 0);
    set_sprite_pixel_color(922, 0, 0, 0);
    set_sprite_pixel_color(923, 7, 5, 2);
    set_sprite_pixel_color(924, 7, 5, 2);
    set_sprite_pixel_color(925, 7, 5, 2);
    set_sprite_pixel_color(926, 7, 5, 2);
    set_sprite_pixel_color(927, 7, 7, 7);
    set_sprite_pixel_color(928, 7, 7, 7);
    set_sprite_pixel_color(929, 7, 5, 2);
    set_sprite_pixel_color(930, 7, 5, 2);
    set_sprite_pixel_color(931, 7, 5, 2);
    set_sprite_pixel_color(932, 7, 5, 2);
    set_sprite_pixel_color(933, 7, 7, 7);
    set_sprite_pixel_color(934, 7, 7, 7);
    set_sprite_pixel_color(935, 7, 6, 4);
    set_sprite_pixel_color(936, 7, 5, 2);
    set_sprite_pixel_color(937, 0, 0, 0);
    set_sprite_pixel_color(938, 0, 0, 0);
    set_sprite_pixel_color(939, 0, 0, 0);
    set_sprite_pixel_color(940, 0, 0, 0);
    set_sprite_pixel_color(941, 0, 0, 0);
    set_sprite_pixel_color(942, 0, 0, 0);
    set_sprite_pixel_color(943, 7, 5, 2);
    set_sprite_pixel_color(944, 7, 5, 2);
    set_sprite_pixel_color(945, 7, 5, 2);
    set_sprite_pixel_color(946, 7, 7, 7);
    set_sprite_pixel_color(947, 7, 7, 7);
    set_sprite_pixel_color(948, 7, 7, 7);
    set_sprite_pixel_color(949, 7, 7, 7);
    set_sprite_pixel_color(950, 7, 5, 2);
    set_sprite_pixel_color(951, 7, 5, 2);
    set_sprite_pixel_color(952, 7, 7, 7);
    set_sprite_pixel_color(953, 7, 7, 7);
    set_sprite_pixel_color(954, 7, 7, 7);
    set_sprite_pixel_color(955, 7, 7, 7);
    set_sprite_pixel_color(956, 7, 7, 7);
    set_sprite_pixel_color(957, 0, 0, 0);
    set_sprite_pixel_color(958, 0, 0, 0);
    set_sprite_pixel_color(959, 0, 0, 0);
    set_sprite_pixel_color(960, 0, 0, 0);
    set_sprite_pixel_color(961, 0, 0, 0);
    set_sprite_pixel_color(962, 0, 0, 0);
    set_sprite_pixel_color(963, 7, 5, 2);
    set_sprite_pixel_color(964, 7, 5, 2);
    set_sprite_pixel_color(965, 7, 5, 2);
    set_sprite_pixel_color(966, 7, 7, 7);
    set_sprite_pixel_color(967, 7, 7, 7);
    set_sprite_pixel_color(968, 2, 3, 5);
    set_sprite_pixel_color(969, 2, 3, 5);
    set_sprite_pixel_color(970, 7, 5, 2);
    set_sprite_pixel_color(971, 7, 5, 2);
    set_sprite_pixel_color(972, 7, 7, 7);
    set_sprite_pixel_color(973, 7, 7, 7);
    set_sprite_pixel_color(974, 2, 3, 5);
    set_sprite_pixel_color(975, 2, 3, 5);
    set_sprite_pixel_color(976, 2, 3, 5);
    set_sprite_pixel_color(977, 0, 0, 0);
    set_sprite_pixel_color(978, 0, 0, 0);
    set_sprite_pixel_color(979, 0, 0, 0);
    set_sprite_pixel_color(980, 0, 0, 0);
    set_sprite_pixel_color(981, 0, 0, 0);
    set_sprite_pixel_color(982, 7, 5, 2);
    set_sprite_pixel_color(983, 7, 5, 2);
    set_sprite_pixel_color(984, 7, 5, 2);
    set_sprite_pixel_color(985, 7, 5, 2);
    set_sprite_pixel_color(986, 7, 7, 7);
    set_sprite_pixel_color(987, 7, 7, 7);
    set_sprite_pixel_color(988, 2, 3, 5);
    set_sprite_pixel_color(989, 2, 3, 5);
    set_sprite_pixel_color(990, 7, 5, 2);
    set_sprite_pixel_color(991, 7, 5, 2);
    set_sprite_pixel_color(992, 7, 7, 7);
    set_sprite_pixel_color(993, 7, 7, 7);
    set_sprite_pixel_color(994, 2, 3, 5);
    set_sprite_pixel_color(995, 2, 3, 5);
    set_sprite_pixel_color(996, 2, 3, 5);
    set_sprite_pixel_color(997, 7, 5, 2);
    set_sprite_pixel_color(998, 0, 0, 0);
    set_sprite_pixel_color(999, 0, 0, 0);
    set_sprite_pixel_color(1000, 0, 0, 0);
    set_sprite_pixel_color(1001, 0, 0, 0);
    set_sprite_pixel_color(1002, 7, 5, 2);
    set_sprite_pixel_color(1003, 7, 5, 2);
    set_sprite_pixel_color(1004, 7, 5, 2);
    set_sprite_pixel_color(1005, 7, 5, 2);
    set_sprite_pixel_color(1006, 7, 5, 2);
    set_sprite_pixel_color(1007, 7, 7, 7);
    set_sprite_pixel_color(1008, 7, 7, 7);
    set_sprite_pixel_color(1009, 7, 5, 2);
    set_sprite_pixel_color(1010, 7, 5, 2);
    set_sprite_pixel_color(1011, 7, 5, 2);
    set_sprite_pixel_color(1012, 7, 5, 2);
    set_sprite_pixel_color(1013, 7, 7, 7);
    set_sprite_pixel_color(1014, 7, 7, 7);
    set_sprite_pixel_color(1015, 7, 6, 4);
    set_sprite_pixel_color(1016, 7, 5, 2);
    set_sprite_pixel_color(1017, 7, 5, 2);
    set_sprite_pixel_color(1018, 0, 0, 0);
    set_sprite_pixel_color(1019, 0, 0, 0);
    set_sprite_pixel_color(1020, 0, 0, 0);
    set_sprite_pixel_color(1021, 0, 0, 0);
    set_sprite_pixel_color(1022, 7, 5, 2);
    set_sprite_pixel_color(1023, 7, 5, 2);
    set_sprite_pixel_color(1024, 7, 5, 2);
    set_sprite_pixel_color(1025, 7, 5, 2);
    set_sprite_pixel_color(1026, 7, 5, 2);
    set_sprite_pixel_color(1027, 7, 5, 2);
    set_sprite_pixel_color(1028, 7, 5, 2);
    set_sprite_pixel_color(1029, 7, 5, 2);
    set_sprite_pixel_color(1030, 7, 5, 2);
    set_sprite_pixel_color(1031, 7, 5, 2);
    set_sprite_pixel_color(1032, 7, 5, 2);
    set_sprite_pixel_color(1033, 7, 5, 2);
    set_sprite_pixel_color(1034, 7, 5, 2);
    set_sprite_pixel_color(1035, 7, 5, 2);
    set_sprite_pixel_color(1036, 7, 5, 2);
    set_sprite_pixel_color(1037, 7, 5, 2);
    set_sprite_pixel_color(1038, 0, 0, 0);
    set_sprite_pixel_color(1039, 0, 0, 0);
    set_sprite_pixel_color(1040, 0, 0, 0);
    set_sprite_pixel_color(1041, 0, 0, 0);
    set_sprite_pixel_color(1042, 7, 5, 2);
    set_sprite_pixel_color(1043, 7, 5, 2);
    set_sprite_pixel_color(1044, 7, 5, 2);
    set_sprite_pixel_color(1045, 7, 5, 2);
    set_sprite_pixel_color(1046, 7, 5, 2);
    set_sprite_pixel_color(1047, 7, 5, 2);
    set_sprite_pixel_color(1048, 7, 5, 2);
    set_sprite_pixel_color(1049, 7, 5, 2);
    set_sprite_pixel_color(1050, 7, 5, 2);
    set_sprite_pixel_color(1051, 7, 5, 2);
    set_sprite_pixel_color(1052, 7, 5, 2);
    set_sprite_pixel_color(1053, 7, 5, 2);
    set_sprite_pixel_color(1054, 7, 5, 2);
    set_sprite_pixel_color(1055, 7, 5, 2);
    set_sprite_pixel_color(1056, 7, 5, 2);
    set_sprite_pixel_color(1057, 7, 5, 2);
    set_sprite_pixel_color(1058, 0, 0, 0);
    set_sprite_pixel_color(1059, 0, 0, 0);
    set_sprite_pixel_color(1060, 0, 0, 0);
    set_sprite_pixel_color(1061, 0, 0, 0);
    set_sprite_pixel_color(1062, 7, 5, 2);
    set_sprite_pixel_color(1063, 7, 5, 2);
    set_sprite_pixel_color(1064, 7, 5, 2);
    set_sprite_pixel_color(1065, 7, 5, 2);
    set_sprite_pixel_color(1066, 7, 5, 2);
    set_sprite_pixel_color(1067, 7, 5, 2);
    set_sprite_pixel_color(1068, 7, 5, 2);
    set_sprite_pixel_color(1069, 7, 5, 2);
    set_sprite_pixel_color(1070, 7, 5, 2);
    set_sprite_pixel_color(1071, 7, 5, 2);
    set_sprite_pixel_color(1072, 7, 5, 2);
    set_sprite_pixel_color(1073, 7, 5, 2);
    set_sprite_pixel_color(1074, 7, 5, 2);
    set_sprite_pixel_color(1075, 7, 5, 2);
    set_sprite_pixel_color(1076, 7, 5, 2);
    set_sprite_pixel_color(1077, 7, 5, 2);
    set_sprite_pixel_color(1078, 0, 0, 0);
    set_sprite_pixel_color(1079, 0, 0, 0);
    set_sprite_pixel_color(1080, 0, 0, 0);
    set_sprite_pixel_color(1081, 0, 0, 0);
    set_sprite_pixel_color(1082, 7, 5, 2);
    set_sprite_pixel_color(1083, 7, 5, 2);
    set_sprite_pixel_color(1084, 7, 5, 2);
    set_sprite_pixel_color(1085, 7, 5, 2);
    set_sprite_pixel_color(1086, 7, 5, 2);
    set_sprite_pixel_color(1087, 7, 5, 2);
    set_sprite_pixel_color(1088, 7, 5, 2);
    set_sprite_pixel_color(1089, 7, 5, 2);
    set_sprite_pixel_color(1090, 7, 5, 2);
    set_sprite_pixel_color(1091, 7, 5, 2);
    set_sprite_pixel_color(1092, 7, 5, 2);
    set_sprite_pixel_color(1093, 7, 5, 2);
    set_sprite_pixel_color(1094, 7, 5, 2);
    set_sprite_pixel_color(1095, 7, 5, 2);
    set_sprite_pixel_color(1096, 7, 5, 2);
    set_sprite_pixel_color(1097, 7, 5, 2);
    set_sprite_pixel_color(1098, 0, 0, 0);
    set_sprite_pixel_color(1099, 0, 0, 0);
    set_sprite_pixel_color(1100, 0, 0, 0);
    set_sprite_pixel_color(1101, 0, 0, 0);
    set_sprite_pixel_color(1102, 7, 5, 2);
    set_sprite_pixel_color(1103, 7, 5, 2);
    set_sprite_pixel_color(1104, 7, 5, 2);
    set_sprite_pixel_color(1105, 4, 4, 5);
    set_sprite_pixel_color(1106, 7, 5, 2);
    set_sprite_pixel_color(1107, 7, 5, 2);
    set_sprite_pixel_color(1108, 7, 5, 2);
    set_sprite_pixel_color(1109, 5, 5, 5);
    set_sprite_pixel_color(1110, 4, 4, 5);
    set_sprite_pixel_color(1111, 7, 5, 2);
    set_sprite_pixel_color(1112, 7, 5, 2);
    set_sprite_pixel_color(1113, 7, 5, 2);
    set_sprite_pixel_color(1114, 5, 5, 5);
    set_sprite_pixel_color(1115, 6, 5, 3);
    set_sprite_pixel_color(1116, 7, 5, 2);
    set_sprite_pixel_color(1117, 7, 5, 2);
    set_sprite_pixel_color(1118, 0, 0, 0);
    set_sprite_pixel_color(1119, 0, 0, 0);
    set_sprite_pixel_color(1120, 0, 0, 0);
    set_sprite_pixel_color(1121, 0, 0, 0);
    set_sprite_pixel_color(1122, 7, 5, 2);
    set_sprite_pixel_color(1123, 4, 4, 5);
    set_sprite_pixel_color(1124, 4, 4, 5);
    set_sprite_pixel_color(1125, 7, 7, 7);
    set_sprite_pixel_color(1126, 4, 4, 5);
    set_sprite_pixel_color(1127, 7, 5, 2);
    set_sprite_pixel_color(1128, 7, 5, 2);
    set_sprite_pixel_color(1129, 0, 0, 0);
    set_sprite_pixel_color(1130, 0, 0, 0);
    set_sprite_pixel_color(1131, 7, 5, 2);
    set_sprite_pixel_color(1132, 7, 5, 2);
    set_sprite_pixel_color(1133, 4, 4, 5);
    set_sprite_pixel_color(1134, 5, 5, 5);
    set_sprite_pixel_color(1135, 5, 5, 5);
    set_sprite_pixel_color(1136, 4, 4, 5);
    set_sprite_pixel_color(1137, 7, 5, 2);
    set_sprite_pixel_color(1138, 0, 0, 0);
    set_sprite_pixel_color(1139, 0, 0, 0);
    set_sprite_pixel_color(1140, 0, 0, 0);
    set_sprite_pixel_color(1141, 0, 0, 0);
    set_sprite_pixel_color(1142, 7, 5, 2);
    set_sprite_pixel_color(1143, 5, 5, 5);
    set_sprite_pixel_color(1144, 0, 0, 0);
    set_sprite_pixel_color(1145, 0, 0, 0);
    set_sprite_pixel_color(1146, 0, 0, 0);
    set_sprite_pixel_color(1147, 7, 5, 2);
    set_sprite_pixel_color(1148, 7, 5, 2);
    set_sprite_pixel_color(1149, 0, 0, 0);
    set_sprite_pixel_color(1150, 0, 0, 0);
    set_sprite_pixel_color(1151, 7, 5, 2);
    set_sprite_pixel_color(1152, 7, 5, 2);
    set_sprite_pixel_color(1153, 0, 0, 0);
    set_sprite_pixel_color(1154, 0, 0, 0);
    set_sprite_pixel_color(1155, 0, 0, 0);
    set_sprite_pixel_color(1156, 5, 5, 5);
    set_sprite_pixel_color(1157, 7, 5, 2);
    set_sprite_pixel_color(1158, 0, 0, 0);
    set_sprite_pixel_color(1159, 0, 0, 0);
    set_sprite_pixel_color(1160, 0, 0, 0);
    set_sprite_pixel_color(1161, 0, 0, 0);
    set_sprite_pixel_color(1162, 0, 0, 0);
    set_sprite_pixel_color(1163, 0, 0, 0);
    set_sprite_pixel_color(1164, 0, 0, 0);
    set_sprite_pixel_color(1165, 0, 0, 0);
    set_sprite_pixel_color(1166, 0, 0, 0);
    set_sprite_pixel_color(1167, 0, 0, 0);
    set_sprite_pixel_color(1168, 0, 0, 0);
    set_sprite_pixel_color(1169, 0, 0, 0);
    set_sprite_pixel_color(1170, 0, 0, 0);
    set_sprite_pixel_color(1171, 0, 0, 0);
    set_sprite_pixel_color(1172, 0, 0, 0);
    set_sprite_pixel_color(1173, 0, 0, 0);
    set_sprite_pixel_color(1174, 0, 0, 0);
    set_sprite_pixel_color(1175, 0, 0, 0);
    set_sprite_pixel_color(1176, 0, 0, 0);
    set_sprite_pixel_color(1177, 0, 0, 0);
    set_sprite_pixel_color(1178, 0, 0, 0);
    set_sprite_pixel_color(1179, 0, 0, 0);
    set_sprite_pixel_color(1180, 0, 0, 0);
    set_sprite_pixel_color(1181, 0, 0, 0);
    set_sprite_pixel_color(1182, 0, 0, 0);
    set_sprite_pixel_color(1183, 0, 0, 0);
    set_sprite_pixel_color(1184, 0, 0, 0);
    set_sprite_pixel_color(1185, 0, 0, 0);
    set_sprite_pixel_color(1186, 0, 0, 0);
    set_sprite_pixel_color(1187, 0, 0, 0);
    set_sprite_pixel_color(1188, 0, 0, 0);
    set_sprite_pixel_color(1189, 0, 0, 0);
    set_sprite_pixel_color(1190, 0, 0, 0);
    set_sprite_pixel_color(1191, 0, 0, 0);
    set_sprite_pixel_color(1192, 0, 0, 0);
    set_sprite_pixel_color(1193, 0, 0, 0);
    set_sprite_pixel_color(1194, 0, 0, 0);
    set_sprite_pixel_color(1195, 0, 0, 0);
    set_sprite_pixel_color(1196, 0, 0, 0);
    set_sprite_pixel_color(1197, 0, 0, 0);
    set_sprite_pixel_color(1198, 0, 0, 0);
    set_sprite_pixel_color(1199, 0, 0, 0);

    set_sprite_pixel_color(1200, 0, 0, 0); // sprite ghosts esquerda
    set_sprite_pixel_color(1201, 0, 0, 0);
    set_sprite_pixel_color(1202, 0, 0, 0);
    set_sprite_pixel_color(1203, 0, 0, 0);
    set_sprite_pixel_color(1204, 0, 0, 0);
    set_sprite_pixel_color(1205, 0, 0, 0);
    set_sprite_pixel_color(1206, 0, 0, 0);
    set_sprite_pixel_color(1207, 0, 0, 0);
    set_sprite_pixel_color(1208, 0, 0, 0);
    set_sprite_pixel_color(1209, 0, 0, 0);
    set_sprite_pixel_color(1210, 0, 0, 0);
    set_sprite_pixel_color(1211, 0, 0, 0);
    set_sprite_pixel_color(1212, 0, 0, 0);
    set_sprite_pixel_color(1213, 0, 0, 0);
    set_sprite_pixel_color(1214, 0, 0, 0);
    set_sprite_pixel_color(1215, 0, 0, 0);
    set_sprite_pixel_color(1216, 0, 0, 0);
    set_sprite_pixel_color(1217, 0, 0, 0);
    set_sprite_pixel_color(1218, 0, 0, 0);
    set_sprite_pixel_color(1219, 0, 0, 0);
    set_sprite_pixel_color(1220, 0, 0, 0);
    set_sprite_pixel_color(1221, 0, 0, 0);
    set_sprite_pixel_color(1222, 0, 0, 0);
    set_sprite_pixel_color(1223, 0, 0, 0);
    set_sprite_pixel_color(1224, 0, 0, 0);
    set_sprite_pixel_color(1225, 0, 0, 0);
    set_sprite_pixel_color(1226, 0, 0, 0);
    set_sprite_pixel_color(1227, 0, 0, 0);
    set_sprite_pixel_color(1228, 0, 0, 0);
    set_sprite_pixel_color(1229, 0, 0, 0);
    set_sprite_pixel_color(1230, 0, 0, 0);
    set_sprite_pixel_color(1231, 0, 0, 0);
    set_sprite_pixel_color(1232, 0, 0, 0);
    set_sprite_pixel_color(1233, 0, 0, 0);
    set_sprite_pixel_color(1234, 0, 0, 0);
    set_sprite_pixel_color(1235, 0, 0, 0);
    set_sprite_pixel_color(1236, 0, 0, 0);
    set_sprite_pixel_color(1237, 0, 0, 0);
    set_sprite_pixel_color(1238, 0, 0, 0);
    set_sprite_pixel_color(1239, 0, 0, 0);
    set_sprite_pixel_color(1240, 0, 0, 0);
    set_sprite_pixel_color(1241, 0, 0, 0);
    set_sprite_pixel_color(1242, 0, 0, 0);
    set_sprite_pixel_color(1243, 0, 0, 0);
    set_sprite_pixel_color(1244, 0, 0, 0);
    set_sprite_pixel_color(1245, 0, 0, 0);
    set_sprite_pixel_color(1246, 0, 0, 0);
    set_sprite_pixel_color(1247, 0, 0, 0);
    set_sprite_pixel_color(1248, 7, 5, 2);
    set_sprite_pixel_color(1249, 7, 5, 2);
    set_sprite_pixel_color(1250, 7, 5, 2);
    set_sprite_pixel_color(1251, 7, 5, 2);
    set_sprite_pixel_color(1252, 0, 0, 0);
    set_sprite_pixel_color(1253, 0, 0, 0);
    set_sprite_pixel_color(1254, 0, 0, 0);
    set_sprite_pixel_color(1255, 0, 0, 0);
    set_sprite_pixel_color(1256, 0, 0, 0);
    set_sprite_pixel_color(1257, 0, 0, 0);
    set_sprite_pixel_color(1258, 0, 0, 0);
    set_sprite_pixel_color(1259, 0, 0, 0);
    set_sprite_pixel_color(1260, 0, 0, 0);
    set_sprite_pixel_color(1261, 0, 0, 0);
    set_sprite_pixel_color(1262, 0, 0, 0);
    set_sprite_pixel_color(1263, 0, 0, 0);
    set_sprite_pixel_color(1264, 0, 0, 0);
    set_sprite_pixel_color(1265, 6, 6, 6);
    set_sprite_pixel_color(1266, 7, 5, 2);
    set_sprite_pixel_color(1267, 7, 5, 2);
    set_sprite_pixel_color(1268, 7, 5, 2);
    set_sprite_pixel_color(1269, 7, 5, 2);
    set_sprite_pixel_color(1270, 7, 5, 2);
    set_sprite_pixel_color(1271, 7, 5, 2);
    set_sprite_pixel_color(1272, 7, 5, 2);
    set_sprite_pixel_color(1273, 7, 5, 2);
    set_sprite_pixel_color(1274, 0, 0, 0);
    set_sprite_pixel_color(1275, 0, 0, 0);
    set_sprite_pixel_color(1276, 0, 0, 0);
    set_sprite_pixel_color(1277, 0, 0, 0);
    set_sprite_pixel_color(1278, 0, 0, 0);
    set_sprite_pixel_color(1279, 0, 0, 0);
    set_sprite_pixel_color(1280, 0, 0, 0);
    set_sprite_pixel_color(1281, 0, 0, 0);
    set_sprite_pixel_color(1282, 0, 0, 0);
    set_sprite_pixel_color(1283, 0, 0, 0);
    set_sprite_pixel_color(1284, 7, 5, 2);
    set_sprite_pixel_color(1285, 7, 5, 2);
    set_sprite_pixel_color(1286, 7, 5, 2);
    set_sprite_pixel_color(1287, 7, 5, 2);
    set_sprite_pixel_color(1288, 7, 5, 2);
    set_sprite_pixel_color(1289, 7, 5, 2);
    set_sprite_pixel_color(1290, 7, 5, 2);
    set_sprite_pixel_color(1291, 7, 5, 2);
    set_sprite_pixel_color(1292, 7, 5, 2);
    set_sprite_pixel_color(1293, 7, 5, 2);
    set_sprite_pixel_color(1294, 7, 5, 2);
    set_sprite_pixel_color(1295, 5, 5, 5);
    set_sprite_pixel_color(1296, 0, 0, 0);
    set_sprite_pixel_color(1297, 0, 0, 0);
    set_sprite_pixel_color(1298, 0, 0, 0);
    set_sprite_pixel_color(1299, 0, 0, 0);
    set_sprite_pixel_color(1300, 0, 0, 0);
    set_sprite_pixel_color(1301, 0, 0, 0);
    set_sprite_pixel_color(1302, 0, 0, 0);
    set_sprite_pixel_color(1303, 6, 6, 6);
    set_sprite_pixel_color(1304, 7, 5, 2);
    set_sprite_pixel_color(1305, 7, 5, 2);
    set_sprite_pixel_color(1306, 7, 5, 2);
    set_sprite_pixel_color(1307, 7, 5, 2);
    set_sprite_pixel_color(1308, 7, 5, 2);
    set_sprite_pixel_color(1309, 7, 5, 2);
    set_sprite_pixel_color(1310, 7, 5, 2);
    set_sprite_pixel_color(1311, 7, 5, 2);
    set_sprite_pixel_color(1312, 7, 5, 2);
    set_sprite_pixel_color(1313, 7, 5, 2);
    set_sprite_pixel_color(1314, 7, 5, 2);
    set_sprite_pixel_color(1315, 6, 6, 6);
    set_sprite_pixel_color(1316, 6, 6, 6);
    set_sprite_pixel_color(1317, 0, 0, 0);
    set_sprite_pixel_color(1318, 0, 0, 0);
    set_sprite_pixel_color(1319, 0, 0, 0);
    set_sprite_pixel_color(1320, 0, 0, 0);
    set_sprite_pixel_color(1321, 0, 0, 0);
    set_sprite_pixel_color(1322, 0, 0, 0);
    set_sprite_pixel_color(1323, 7, 5, 2);
    set_sprite_pixel_color(1324, 7, 7, 7);
    set_sprite_pixel_color(1325, 7, 7, 7);
    set_sprite_pixel_color(1326, 7, 7, 7);
    set_sprite_pixel_color(1327, 7, 5, 2);
    set_sprite_pixel_color(1328, 7, 5, 2);
    set_sprite_pixel_color(1329, 7, 5, 2);
    set_sprite_pixel_color(1330, 7, 5, 2);
    set_sprite_pixel_color(1331, 7, 7, 7);
    set_sprite_pixel_color(1332, 7, 7, 7);
    set_sprite_pixel_color(1333, 7, 5, 2);
    set_sprite_pixel_color(1334, 7, 5, 2);
    set_sprite_pixel_color(1335, 7, 5, 2);
    set_sprite_pixel_color(1336, 6, 5, 3);
    set_sprite_pixel_color(1337, 0, 0, 0);
    set_sprite_pixel_color(1338, 0, 0, 0);
    set_sprite_pixel_color(1339, 0, 0, 0);
    set_sprite_pixel_color(1340, 0, 0, 0);
    set_sprite_pixel_color(1341, 0, 0, 0);
    set_sprite_pixel_color(1342, 0, 0, 0);
    set_sprite_pixel_color(1343, 7, 7, 7);
    set_sprite_pixel_color(1344, 7, 7, 7);
    set_sprite_pixel_color(1345, 7, 7, 7);
    set_sprite_pixel_color(1346, 7, 7, 7);
    set_sprite_pixel_color(1347, 7, 7, 7);
    set_sprite_pixel_color(1348, 7, 5, 2);
    set_sprite_pixel_color(1349, 7, 5, 2);
    set_sprite_pixel_color(1350, 7, 7, 7);
    set_sprite_pixel_color(1351, 7, 7, 7);
    set_sprite_pixel_color(1352, 7, 7, 7);
    set_sprite_pixel_color(1353, 7, 7, 7);
    set_sprite_pixel_color(1354, 7, 5, 2);
    set_sprite_pixel_color(1355, 7, 5, 2);
    set_sprite_pixel_color(1356, 6, 5, 3);
    set_sprite_pixel_color(1357, 0, 0, 0);
    set_sprite_pixel_color(1358, 0, 0, 0);
    set_sprite_pixel_color(1359, 0, 0, 0);
    set_sprite_pixel_color(1360, 0, 0, 0);
    set_sprite_pixel_color(1361, 0, 0, 0);
    set_sprite_pixel_color(1362, 0, 0, 0);
    set_sprite_pixel_color(1363, 2, 3, 5);
    set_sprite_pixel_color(1364, 2, 3, 5);
    set_sprite_pixel_color(1365, 1, 2, 5);
    set_sprite_pixel_color(1366, 7, 7, 7);
    set_sprite_pixel_color(1367, 7, 7, 7);
    set_sprite_pixel_color(1368, 7, 5, 2);
    set_sprite_pixel_color(1369, 7, 5, 2);
    set_sprite_pixel_color(1370, 2, 3, 5);
    set_sprite_pixel_color(1371, 2, 3, 5);
    set_sprite_pixel_color(1372, 7, 7, 7);
    set_sprite_pixel_color(1373, 7, 7, 7);
    set_sprite_pixel_color(1374, 7, 5, 2);
    set_sprite_pixel_color(1375, 7, 5, 2);
    set_sprite_pixel_color(1376, 6, 5, 3);
    set_sprite_pixel_color(1377, 0, 0, 0);
    set_sprite_pixel_color(1378, 0, 0, 0);
    set_sprite_pixel_color(1379, 0, 0, 0);
    set_sprite_pixel_color(1380, 0, 0, 0);
    set_sprite_pixel_color(1381, 0, 0, 0);
    set_sprite_pixel_color(1382, 7, 5, 2);
    set_sprite_pixel_color(1383, 2, 3, 5);
    set_sprite_pixel_color(1384, 2, 3, 5);
    set_sprite_pixel_color(1385, 1, 2, 5);
    set_sprite_pixel_color(1386, 7, 7, 7);
    set_sprite_pixel_color(1387, 7, 7, 7);
    set_sprite_pixel_color(1388, 7, 5, 2);
    set_sprite_pixel_color(1389, 7, 5, 2);
    set_sprite_pixel_color(1390, 2, 3, 5);
    set_sprite_pixel_color(1391, 2, 3, 5);
    set_sprite_pixel_color(1392, 7, 7, 7);
    set_sprite_pixel_color(1393, 7, 7, 7);
    set_sprite_pixel_color(1394, 7, 5, 2);
    set_sprite_pixel_color(1395, 7, 5, 2);
    set_sprite_pixel_color(1396, 7, 5, 2);
    set_sprite_pixel_color(1397, 7, 5, 2);
    set_sprite_pixel_color(1398, 0, 0, 0);
    set_sprite_pixel_color(1399, 0, 0, 0);
    set_sprite_pixel_color(1400, 0, 0, 0);
    set_sprite_pixel_color(1401, 0, 0, 0);
    set_sprite_pixel_color(1402, 7, 5, 2);
    set_sprite_pixel_color(1403, 7, 5, 2);
    set_sprite_pixel_color(1404, 7, 7, 7);
    set_sprite_pixel_color(1405, 7, 7, 7);
    set_sprite_pixel_color(1406, 7, 7, 7);
    set_sprite_pixel_color(1407, 7, 5, 2);
    set_sprite_pixel_color(1408, 7, 5, 2);
    set_sprite_pixel_color(1409, 7, 5, 2);
    set_sprite_pixel_color(1410, 7, 5, 2);
    set_sprite_pixel_color(1411, 7, 7, 7);
    set_sprite_pixel_color(1412, 7, 7, 7);
    set_sprite_pixel_color(1413, 7, 5, 2);
    set_sprite_pixel_color(1414, 7, 5, 2);
    set_sprite_pixel_color(1415, 7, 5, 2);
    set_sprite_pixel_color(1416, 7, 5, 2);
    set_sprite_pixel_color(1417, 7, 5, 2);
    set_sprite_pixel_color(1418, 0, 0, 0);
    set_sprite_pixel_color(1419, 0, 0, 0);
    set_sprite_pixel_color(1420, 0, 0, 0);
    set_sprite_pixel_color(1421, 0, 0, 0);
    set_sprite_pixel_color(1422, 7, 5, 2);
    set_sprite_pixel_color(1423, 7, 5, 2);
    set_sprite_pixel_color(1424, 7, 5, 2);
    set_sprite_pixel_color(1425, 7, 5, 2);
    set_sprite_pixel_color(1426, 7, 5, 2);
    set_sprite_pixel_color(1427, 7, 5, 2);
    set_sprite_pixel_color(1428, 7, 5, 2);
    set_sprite_pixel_color(1429, 7, 5, 2);
    set_sprite_pixel_color(1430, 7, 5, 2);
    set_sprite_pixel_color(1431, 7, 5, 2);
    set_sprite_pixel_color(1432, 7, 5, 2);
    set_sprite_pixel_color(1433, 7, 5, 2);
    set_sprite_pixel_color(1434, 7, 5, 2);
    set_sprite_pixel_color(1435, 7, 5, 2);
    set_sprite_pixel_color(1436, 7, 5, 2);
    set_sprite_pixel_color(1437, 7, 5, 2);
    set_sprite_pixel_color(1438, 0, 0, 0);
    set_sprite_pixel_color(1439, 0, 0, 0);
    set_sprite_pixel_color(1440, 0, 0, 0);
    set_sprite_pixel_color(1441, 0, 0, 0);
    set_sprite_pixel_color(1442, 7, 5, 2);
    set_sprite_pixel_color(1443, 7, 5, 2);
    set_sprite_pixel_color(1444, 7, 5, 2);
    set_sprite_pixel_color(1445, 7, 5, 2);
    set_sprite_pixel_color(1446, 7, 5, 2);
    set_sprite_pixel_color(1447, 7, 5, 2);
    set_sprite_pixel_color(1448, 7, 5, 2);
    set_sprite_pixel_color(1449, 7, 5, 2);
    set_sprite_pixel_color(1450, 7, 5, 2);
    set_sprite_pixel_color(1451, 7, 5, 2);
    set_sprite_pixel_color(1452, 7, 5, 2);
    set_sprite_pixel_color(1453, 7, 5, 2);
    set_sprite_pixel_color(1454, 7, 5, 2);
    set_sprite_pixel_color(1455, 7, 5, 2);
    set_sprite_pixel_color(1456, 7, 5, 2);
    set_sprite_pixel_color(1457, 7, 5, 2);
    set_sprite_pixel_color(1458, 0, 0, 0);
    set_sprite_pixel_color(1459, 0, 0, 0);
    set_sprite_pixel_color(1460, 0, 0, 0);
    set_sprite_pixel_color(1461, 0, 0, 0);
    set_sprite_pixel_color(1462, 7, 5, 2);
    set_sprite_pixel_color(1463, 7, 5, 2);
    set_sprite_pixel_color(1464, 7, 5, 2);
    set_sprite_pixel_color(1465, 7, 5, 2);
    set_sprite_pixel_color(1466, 7, 5, 2);
    set_sprite_pixel_color(1467, 7, 5, 2);
    set_sprite_pixel_color(1468, 7, 5, 2);
    set_sprite_pixel_color(1469, 7, 5, 2);
    set_sprite_pixel_color(1470, 7, 5, 2);
    set_sprite_pixel_color(1471, 7, 5, 2);
    set_sprite_pixel_color(1472, 7, 5, 2);
    set_sprite_pixel_color(1473, 7, 5, 2);
    set_sprite_pixel_color(1474, 7, 5, 2);
    set_sprite_pixel_color(1475, 7, 5, 2);
    set_sprite_pixel_color(1476, 7, 5, 2);
    set_sprite_pixel_color(1477, 7, 5, 2);
    set_sprite_pixel_color(1478, 0, 0, 0);
    set_sprite_pixel_color(1479, 0, 0, 0);
    set_sprite_pixel_color(1480, 0, 0, 0);
    set_sprite_pixel_color(1481, 0, 0, 0);
    set_sprite_pixel_color(1482, 7, 5, 2);
    set_sprite_pixel_color(1483, 7, 5, 2);
    set_sprite_pixel_color(1484, 7, 5, 2);
    set_sprite_pixel_color(1485, 7, 5, 2);
    set_sprite_pixel_color(1486, 7, 5, 2);
    set_sprite_pixel_color(1487, 7, 5, 2);
    set_sprite_pixel_color(1488, 7, 5, 2);
    set_sprite_pixel_color(1489, 7, 5, 2);
    set_sprite_pixel_color(1490, 7, 5, 2);
    set_sprite_pixel_color(1491, 7, 5, 2);
    set_sprite_pixel_color(1492, 7, 5, 2);
    set_sprite_pixel_color(1493, 7, 5, 2);
    set_sprite_pixel_color(1494, 7, 5, 2);
    set_sprite_pixel_color(1495, 7, 5, 2);
    set_sprite_pixel_color(1496, 7, 5, 2);
    set_sprite_pixel_color(1497, 7, 5, 2);
    set_sprite_pixel_color(1498, 0, 0, 0);
    set_sprite_pixel_color(1499, 0, 0, 0);
    set_sprite_pixel_color(1500, 0, 0, 0);
    set_sprite_pixel_color(1501, 0, 0, 0);
    set_sprite_pixel_color(1502, 7, 5, 2);
    set_sprite_pixel_color(1503, 7, 5, 2);
    set_sprite_pixel_color(1504, 4, 4, 5);
    set_sprite_pixel_color(1505, 4, 4, 5);
    set_sprite_pixel_color(1506, 7, 5, 2);
    set_sprite_pixel_color(1507, 7, 5, 2);
    set_sprite_pixel_color(1508, 7, 5, 2);
    set_sprite_pixel_color(1509, 4, 4, 5);
    set_sprite_pixel_color(1510, 5, 5, 5);
    set_sprite_pixel_color(1511, 7, 5, 2);
    set_sprite_pixel_color(1512, 7, 5, 2);
    set_sprite_pixel_color(1513, 7, 5, 2);
    set_sprite_pixel_color(1514, 4, 4, 5);
    set_sprite_pixel_color(1515, 7, 5, 2);
    set_sprite_pixel_color(1516, 7, 5, 2);
    set_sprite_pixel_color(1517, 7, 5, 2);
    set_sprite_pixel_color(1518, 0, 0, 0);
    set_sprite_pixel_color(1519, 0, 0, 0);
    set_sprite_pixel_color(1520, 0, 0, 0);
    set_sprite_pixel_color(1521, 0, 0, 0);
    set_sprite_pixel_color(1522, 7, 5, 2);
    set_sprite_pixel_color(1523, 4, 4, 5);
    set_sprite_pixel_color(1524, 5, 5, 5);
    set_sprite_pixel_color(1525, 5, 5, 5);
    set_sprite_pixel_color(1526, 4, 4, 5);
    set_sprite_pixel_color(1527, 7, 5, 2);
    set_sprite_pixel_color(1528, 7, 5, 2);
    set_sprite_pixel_color(1529, 0, 0, 0);
    set_sprite_pixel_color(1530, 0, 0, 0);
    set_sprite_pixel_color(1531, 7, 5, 2);
    set_sprite_pixel_color(1532, 7, 5, 2);
    set_sprite_pixel_color(1533, 4, 4, 5);
    set_sprite_pixel_color(1534, 0, 0, 0);
    set_sprite_pixel_color(1535, 4, 4, 5);
    set_sprite_pixel_color(1536, 6, 5, 3);
    set_sprite_pixel_color(1537, 7, 5, 2);
    set_sprite_pixel_color(1538, 0, 0, 0);
    set_sprite_pixel_color(1539, 0, 0, 0);
    set_sprite_pixel_color(1540, 0, 0, 0);
    set_sprite_pixel_color(1541, 0, 0, 0);
    set_sprite_pixel_color(1542, 7, 5, 2);
    set_sprite_pixel_color(1543, 6, 6, 6);
    set_sprite_pixel_color(1544, 0, 0, 0);
    set_sprite_pixel_color(1545, 0, 0, 0);
    set_sprite_pixel_color(1546, 7, 7, 7);
    set_sprite_pixel_color(1547, 7, 5, 2);
    set_sprite_pixel_color(1548, 7, 5, 2);
    set_sprite_pixel_color(1549, 0, 0, 0);
    set_sprite_pixel_color(1550, 0, 0, 0);
    set_sprite_pixel_color(1551, 7, 5, 2);
    set_sprite_pixel_color(1552, 7, 5, 2);
    set_sprite_pixel_color(1553, 0, 0, 0);
    set_sprite_pixel_color(1554, 0, 0, 0);
    set_sprite_pixel_color(1555, 0, 0, 0);
    set_sprite_pixel_color(1556, 7, 6, 4);
    set_sprite_pixel_color(1557, 7, 5, 2);
    set_sprite_pixel_color(1558, 0, 0, 0);
    set_sprite_pixel_color(1559, 0, 0, 0);
    set_sprite_pixel_color(1560, 0, 0, 0);
    set_sprite_pixel_color(1561, 0, 0, 0);
    set_sprite_pixel_color(1562, 0, 0, 0);
    set_sprite_pixel_color(1563, 0, 0, 0);
    set_sprite_pixel_color(1564, 0, 0, 0);
    set_sprite_pixel_color(1565, 0, 0, 0);
    set_sprite_pixel_color(1566, 0, 0, 0);
    set_sprite_pixel_color(1567, 0, 0, 0);
    set_sprite_pixel_color(1568, 0, 0, 0);
    set_sprite_pixel_color(1569, 0, 0, 0);
    set_sprite_pixel_color(1570, 0, 0, 0);
    set_sprite_pixel_color(1571, 0, 0, 0);
    set_sprite_pixel_color(1572, 0, 0, 0);
    set_sprite_pixel_color(1573, 0, 0, 0);
    set_sprite_pixel_color(1574, 0, 0, 0);
    set_sprite_pixel_color(1575, 0, 0, 0);
    set_sprite_pixel_color(1576, 0, 0, 0);
    set_sprite_pixel_color(1577, 0, 0, 0);
    set_sprite_pixel_color(1578, 0, 0, 0);
    set_sprite_pixel_color(1579, 0, 0, 0);
    set_sprite_pixel_color(1580, 0, 0, 0);
    set_sprite_pixel_color(1581, 0, 0, 0);
    set_sprite_pixel_color(1582, 0, 0, 0);
    set_sprite_pixel_color(1583, 0, 0, 0);
    set_sprite_pixel_color(1584, 0, 0, 0);
    set_sprite_pixel_color(1585, 0, 0, 0);
    set_sprite_pixel_color(1586, 0, 0, 0);
    set_sprite_pixel_color(1587, 0, 0, 0);
    set_sprite_pixel_color(1588, 0, 0, 0);
    set_sprite_pixel_color(1589, 0, 0, 0);
    set_sprite_pixel_color(1590, 0, 0, 0);
    set_sprite_pixel_color(1591, 0, 0, 0);
    set_sprite_pixel_color(1592, 0, 0, 0);
    set_sprite_pixel_color(1593, 0, 0, 0);
    set_sprite_pixel_color(1594, 0, 0, 0);
    set_sprite_pixel_color(1595, 0, 0, 0);
    set_sprite_pixel_color(1596, 0, 0, 0);
    set_sprite_pixel_color(1597, 0, 0, 0);
    set_sprite_pixel_color(1598, 0, 0, 0);
    set_sprite_pixel_color(1599, 0, 0, 0);

    set_sprite_pixel_color(1600, 0, 0, 0); // sprite pac boca para cima
    set_sprite_pixel_color(1601, 0, 0, 0);
    set_sprite_pixel_color(1602, 0, 0, 0);
    set_sprite_pixel_color(1603, 0, 0, 0);
    set_sprite_pixel_color(1604, 0, 0, 0);
    set_sprite_pixel_color(1605, 0, 0, 0);
    set_sprite_pixel_color(1606, 0, 0, 0);
    set_sprite_pixel_color(1607, 0, 0, 0);
    set_sprite_pixel_color(1608, 0, 0, 0);
    set_sprite_pixel_color(1609, 0, 0, 0);
    set_sprite_pixel_color(1610, 0, 0, 0);
    set_sprite_pixel_color(1611, 0, 0, 0);
    set_sprite_pixel_color(1612, 0, 0, 0);
    set_sprite_pixel_color(1613, 0, 0, 0);
    set_sprite_pixel_color(1614, 0, 0, 0);
    set_sprite_pixel_color(1615, 0, 0, 0);
    set_sprite_pixel_color(1616, 0, 0, 0);
    set_sprite_pixel_color(1617, 0, 0, 0);
    set_sprite_pixel_color(1618, 0, 0, 0);
    set_sprite_pixel_color(1619, 0, 0, 0);
    set_sprite_pixel_color(1620, 0, 0, 0);
    set_sprite_pixel_color(1621, 0, 0, 0);
    set_sprite_pixel_color(1622, 0, 0, 0);
    set_sprite_pixel_color(1623, 7, 7, 0);
    set_sprite_pixel_color(1624, 0, 0, 0);
    set_sprite_pixel_color(1625, 0, 0, 0);
    set_sprite_pixel_color(1626, 0, 0, 0);
    set_sprite_pixel_color(1627, 0, 0, 0);
    set_sprite_pixel_color(1628, 0, 0, 0);
    set_sprite_pixel_color(1629, 0, 0, 0);
    set_sprite_pixel_color(1630, 0, 0, 0);
    set_sprite_pixel_color(1631, 0, 0, 0);
    set_sprite_pixel_color(1632, 0, 0, 0);
    set_sprite_pixel_color(1633, 0, 0, 0);
    set_sprite_pixel_color(1634, 0, 0, 0);
    set_sprite_pixel_color(1635, 0, 0, 0);
    set_sprite_pixel_color(1636, 0, 0, 0);
    set_sprite_pixel_color(1637, 0, 0, 0);
    set_sprite_pixel_color(1638, 0, 0, 0);
    set_sprite_pixel_color(1639, 0, 0, 0);
    set_sprite_pixel_color(1640, 0, 0, 0);
    set_sprite_pixel_color(1641, 0, 0, 0);
    set_sprite_pixel_color(1642, 7, 7, 0);
    set_sprite_pixel_color(1643, 7, 7, 0);
    set_sprite_pixel_color(1644, 7, 7, 0);
    set_sprite_pixel_color(1645, 0, 0, 0);
    set_sprite_pixel_color(1646, 0, 0, 0);
    set_sprite_pixel_color(1647, 0, 0, 0);
    set_sprite_pixel_color(1648, 0, 0, 0);
    set_sprite_pixel_color(1649, 0, 0, 0);
    set_sprite_pixel_color(1650, 0, 0, 0);
    set_sprite_pixel_color(1651, 0, 0, 0);
    set_sprite_pixel_color(1652, 0, 0, 0);
    set_sprite_pixel_color(1653, 0, 0, 0);
    set_sprite_pixel_color(1654, 0, 0, 0);
    set_sprite_pixel_color(1655, 0, 0, 0);
    set_sprite_pixel_color(1656, 7, 7, 0);
    set_sprite_pixel_color(1657, 2, 2, 0);
    set_sprite_pixel_color(1658, 0, 0, 0);
    set_sprite_pixel_color(1659, 0, 0, 0);
    set_sprite_pixel_color(1660, 0, 0, 0);
    set_sprite_pixel_color(1661, 0, 0, 0);
    set_sprite_pixel_color(1662, 7, 7, 0);
    set_sprite_pixel_color(1663, 7, 7, 0);
    set_sprite_pixel_color(1664, 7, 7, 0);
    set_sprite_pixel_color(1665, 0, 0, 0);
    set_sprite_pixel_color(1666, 0, 0, 0);
    set_sprite_pixel_color(1667, 0, 0, 0);
    set_sprite_pixel_color(1668, 0, 0, 0);
    set_sprite_pixel_color(1669, 0, 0, 0);
    set_sprite_pixel_color(1670, 0, 0, 0);
    set_sprite_pixel_color(1671, 0, 0, 0);
    set_sprite_pixel_color(1672, 0, 0, 0);
    set_sprite_pixel_color(1673, 0, 0, 0);
    set_sprite_pixel_color(1674, 0, 0, 0);
    set_sprite_pixel_color(1675, 7, 7, 0);
    set_sprite_pixel_color(1676, 7, 7, 0);
    set_sprite_pixel_color(1677, 7, 7, 0);
    set_sprite_pixel_color(1678, 0, 0, 0);
    set_sprite_pixel_color(1679, 0, 0, 0);
    set_sprite_pixel_color(1680, 0, 0, 0);
    set_sprite_pixel_color(1681, 7, 7, 0);
    set_sprite_pixel_color(1682, 7, 7, 0);
    set_sprite_pixel_color(1683, 7, 7, 0);
    set_sprite_pixel_color(1684, 7, 7, 0);
    set_sprite_pixel_color(1685, 7, 7, 0);
    set_sprite_pixel_color(1686, 0, 0, 0);
    set_sprite_pixel_color(1687, 0, 0, 0);
    set_sprite_pixel_color(1688, 0, 0, 0);
    set_sprite_pixel_color(1689, 0, 0, 0);
    set_sprite_pixel_color(1690, 0, 0, 0);
    set_sprite_pixel_color(1691, 0, 0, 0);
    set_sprite_pixel_color(1692, 0, 0, 0);
    set_sprite_pixel_color(1693, 1, 1, 1);
    set_sprite_pixel_color(1694, 7, 7, 0);
    set_sprite_pixel_color(1695, 7, 7, 0);
    set_sprite_pixel_color(1696, 7, 7, 0);
    set_sprite_pixel_color(1697, 7, 7, 0);
    set_sprite_pixel_color(1698, 0, 0, 0);
    set_sprite_pixel_color(1699, 0, 0, 0);
    set_sprite_pixel_color(1700, 0, 0, 0);
    set_sprite_pixel_color(1701, 7, 7, 0);
    set_sprite_pixel_color(1702, 7, 7, 0);
    set_sprite_pixel_color(1703, 7, 7, 0);
    set_sprite_pixel_color(1704, 7, 7, 0);
    set_sprite_pixel_color(1705, 7, 7, 0);
    set_sprite_pixel_color(1706, 7, 7, 0);
    set_sprite_pixel_color(1707, 0, 0, 0);
    set_sprite_pixel_color(1708, 0, 0, 0);
    set_sprite_pixel_color(1709, 0, 0, 0);
    set_sprite_pixel_color(1710, 0, 0, 0);
    set_sprite_pixel_color(1711, 0, 0, 0);
    set_sprite_pixel_color(1712, 0, 0, 0);
    set_sprite_pixel_color(1713, 7, 7, 0);
    set_sprite_pixel_color(1714, 7, 7, 0);
    set_sprite_pixel_color(1715, 7, 7, 0);
    set_sprite_pixel_color(1716, 7, 7, 0);
    set_sprite_pixel_color(1717, 7, 7, 0);
    set_sprite_pixel_color(1718, 7, 7, 0);
    set_sprite_pixel_color(1719, 0, 0, 0);
    set_sprite_pixel_color(1720, 0, 0, 0);
    set_sprite_pixel_color(1721, 7, 7, 0);
    set_sprite_pixel_color(1722, 7, 7, 0);
    set_sprite_pixel_color(1723, 7, 7, 0);
    set_sprite_pixel_color(1724, 7, 7, 0);
    set_sprite_pixel_color(1725, 7, 7, 0);
    set_sprite_pixel_color(1726, 7, 7, 0);
    set_sprite_pixel_color(1727, 7, 7, 0);
    set_sprite_pixel_color(1728, 0, 0, 0);
    set_sprite_pixel_color(1729, 0, 0, 0);
    set_sprite_pixel_color(1730, 0, 0, 0);
    set_sprite_pixel_color(1731, 0, 0, 0);
    set_sprite_pixel_color(1732, 7, 7, 0);
    set_sprite_pixel_color(1733, 7, 7, 0);
    set_sprite_pixel_color(1734, 7, 7, 0);
    set_sprite_pixel_color(1735, 7, 7, 0);
    set_sprite_pixel_color(1736, 7, 7, 0);
    set_sprite_pixel_color(1737, 7, 7, 0);
    set_sprite_pixel_color(1738, 7, 7, 0);
    set_sprite_pixel_color(1739, 0, 0, 0);
    set_sprite_pixel_color(1740, 2, 2, 0);
    set_sprite_pixel_color(1741, 7, 7, 0);
    set_sprite_pixel_color(1742, 7, 7, 0);
    set_sprite_pixel_color(1743, 7, 7, 0);
    set_sprite_pixel_color(1744, 7, 7, 0);
    set_sprite_pixel_color(1745, 0, 0, 0);
    set_sprite_pixel_color(1746, 7, 7, 0);
    set_sprite_pixel_color(1747, 7, 7, 0);
    set_sprite_pixel_color(1748, 3, 3, 0);
    set_sprite_pixel_color(1749, 0, 0, 0);
    set_sprite_pixel_color(1750, 0, 0, 0);
    set_sprite_pixel_color(1751, 7, 7, 0);
    set_sprite_pixel_color(1752, 7, 7, 0);
    set_sprite_pixel_color(1753, 7, 7, 0);
    set_sprite_pixel_color(1754, 7, 7, 0);
    set_sprite_pixel_color(1755, 7, 7, 0);
    set_sprite_pixel_color(1756, 7, 7, 0);
    set_sprite_pixel_color(1757, 7, 7, 0);
    set_sprite_pixel_color(1758, 7, 7, 0);
    set_sprite_pixel_color(1759, 0, 0, 0);
    set_sprite_pixel_color(1760, 7, 7, 0);
    set_sprite_pixel_color(1761, 7, 7, 0);
    set_sprite_pixel_color(1762, 7, 7, 0);
    set_sprite_pixel_color(1763, 7, 7, 0);
    set_sprite_pixel_color(1764, 7, 7, 0);
    set_sprite_pixel_color(1765, 0, 0, 0);
    set_sprite_pixel_color(1766, 0, 0, 0);
    set_sprite_pixel_color(1767, 7, 7, 0);
    set_sprite_pixel_color(1768, 7, 7, 0);
    set_sprite_pixel_color(1769, 0, 0, 0);
    set_sprite_pixel_color(1770, 7, 7, 0);
    set_sprite_pixel_color(1771, 7, 7, 0);
    set_sprite_pixel_color(1772, 7, 7, 0);
    set_sprite_pixel_color(1773, 7, 7, 0);
    set_sprite_pixel_color(1774, 7, 7, 0);
    set_sprite_pixel_color(1775, 7, 7, 0);
    set_sprite_pixel_color(1776, 7, 7, 0);
    set_sprite_pixel_color(1777, 7, 7, 0);
    set_sprite_pixel_color(1778, 7, 7, 0);
    set_sprite_pixel_color(1779, 0, 0, 0);
    set_sprite_pixel_color(1780, 7, 7, 0);
    set_sprite_pixel_color(1781, 7, 7, 0);
    set_sprite_pixel_color(1782, 7, 7, 0);
    set_sprite_pixel_color(1783, 7, 7, 0);
    set_sprite_pixel_color(1784, 7, 7, 0);
    set_sprite_pixel_color(1785, 0, 0, 0);
    set_sprite_pixel_color(1786, 5, 5, 0);
    set_sprite_pixel_color(1787, 7, 7, 0);
    set_sprite_pixel_color(1788, 7, 7, 0);
    set_sprite_pixel_color(1789, 7, 7, 0);
    set_sprite_pixel_color(1790, 7, 7, 0);
    set_sprite_pixel_color(1791, 7, 7, 0);
    set_sprite_pixel_color(1792, 7, 7, 0);
    set_sprite_pixel_color(1793, 7, 7, 0);
    set_sprite_pixel_color(1794, 7, 7, 0);
    set_sprite_pixel_color(1795, 7, 7, 0);
    set_sprite_pixel_color(1796, 7, 7, 0);
    set_sprite_pixel_color(1797, 7, 7, 0);
    set_sprite_pixel_color(1798, 7, 7, 0);
    set_sprite_pixel_color(1799, 0, 0, 0);
    set_sprite_pixel_color(1800, 0, 0, 0);
    set_sprite_pixel_color(1801, 7, 7, 0);
    set_sprite_pixel_color(1802, 7, 7, 0);
    set_sprite_pixel_color(1803, 7, 7, 0);
    set_sprite_pixel_color(1804, 7, 7, 0);
    set_sprite_pixel_color(1805, 7, 7, 0);
    set_sprite_pixel_color(1806, 7, 7, 0);
    set_sprite_pixel_color(1807, 7, 7, 0);
    set_sprite_pixel_color(1808, 7, 7, 0);
    set_sprite_pixel_color(1809, 7, 7, 0);
    set_sprite_pixel_color(1810, 7, 7, 0);
    set_sprite_pixel_color(1811, 7, 7, 0);
    set_sprite_pixel_color(1812, 7, 7, 0);
    set_sprite_pixel_color(1813, 7, 7, 0);
    set_sprite_pixel_color(1814, 7, 7, 0);
    set_sprite_pixel_color(1815, 7, 7, 0);
    set_sprite_pixel_color(1816, 7, 7, 0);
    set_sprite_pixel_color(1817, 7, 7, 0);
    set_sprite_pixel_color(1818, 7, 7, 0);
    set_sprite_pixel_color(1819, 0, 0, 0);
    set_sprite_pixel_color(1820, 0, 0, 0);
    set_sprite_pixel_color(1821, 7, 7, 0);
    set_sprite_pixel_color(1822, 7, 7, 0);
    set_sprite_pixel_color(1823, 7, 7, 0);
    set_sprite_pixel_color(1824, 7, 7, 0);
    set_sprite_pixel_color(1825, 7, 7, 0);
    set_sprite_pixel_color(1826, 7, 7, 0);
    set_sprite_pixel_color(1827, 7, 7, 0);
    set_sprite_pixel_color(1828, 7, 7, 0);
    set_sprite_pixel_color(1829, 7, 7, 0);
    set_sprite_pixel_color(1830, 7, 7, 0);
    set_sprite_pixel_color(1831, 7, 7, 0);
    set_sprite_pixel_color(1832, 7, 7, 0);
    set_sprite_pixel_color(1833, 7, 7, 0);
    set_sprite_pixel_color(1834, 7, 7, 0);
    set_sprite_pixel_color(1835, 7, 7, 0);
    set_sprite_pixel_color(1836, 7, 7, 0);
    set_sprite_pixel_color(1837, 7, 7, 0);
    set_sprite_pixel_color(1838, 7, 7, 0);
    set_sprite_pixel_color(1839, 0, 0, 0);
    set_sprite_pixel_color(1840, 0, 0, 0);
    set_sprite_pixel_color(1841, 7, 7, 0);
    set_sprite_pixel_color(1842, 7, 7, 0);
    set_sprite_pixel_color(1843, 7, 7, 0);
    set_sprite_pixel_color(1844, 7, 7, 0);
    set_sprite_pixel_color(1845, 7, 7, 0);
    set_sprite_pixel_color(1846, 7, 7, 0);
    set_sprite_pixel_color(1847, 7, 7, 0);
    set_sprite_pixel_color(1848, 7, 7, 0);
    set_sprite_pixel_color(1849, 7, 7, 0);
    set_sprite_pixel_color(1850, 7, 7, 0);
    set_sprite_pixel_color(1851, 7, 7, 0);
    set_sprite_pixel_color(1852, 7, 7, 0);
    set_sprite_pixel_color(1853, 7, 7, 0);
    set_sprite_pixel_color(1854, 7, 7, 0);
    set_sprite_pixel_color(1855, 7, 7, 0);
    set_sprite_pixel_color(1856, 7, 7, 0);
    set_sprite_pixel_color(1857, 7, 7, 0);
    set_sprite_pixel_color(1858, 7, 7, 0);
    set_sprite_pixel_color(1859, 0, 0, 0);
    set_sprite_pixel_color(1860, 0, 0, 0);
    set_sprite_pixel_color(1861, 7, 7, 0);
    set_sprite_pixel_color(1862, 7, 7, 0);
    set_sprite_pixel_color(1863, 7, 7, 0);
    set_sprite_pixel_color(1864, 7, 7, 0);
    set_sprite_pixel_color(1865, 7, 7, 0);
    set_sprite_pixel_color(1866, 7, 7, 0);
    set_sprite_pixel_color(1867, 7, 7, 0);
    set_sprite_pixel_color(1868, 7, 7, 0);
    set_sprite_pixel_color(1869, 7, 7, 0);
    set_sprite_pixel_color(1870, 7, 7, 0);
    set_sprite_pixel_color(1871, 7, 7, 0);
    set_sprite_pixel_color(1872, 7, 7, 0);
    set_sprite_pixel_color(1873, 7, 7, 0);
    set_sprite_pixel_color(1874, 7, 7, 0);
    set_sprite_pixel_color(1875, 7, 7, 0);
    set_sprite_pixel_color(1876, 7, 7, 0);
    set_sprite_pixel_color(1877, 7, 7, 0);
    set_sprite_pixel_color(1878, 0, 0, 0);
    set_sprite_pixel_color(1879, 0, 0, 0);
    set_sprite_pixel_color(1880, 0, 0, 0);
    set_sprite_pixel_color(1881, 0, 0, 0);
    set_sprite_pixel_color(1882, 7, 7, 0);
    set_sprite_pixel_color(1883, 7, 7, 0);
    set_sprite_pixel_color(1884, 7, 7, 0);
    set_sprite_pixel_color(1885, 7, 7, 0);
    set_sprite_pixel_color(1886, 7, 7, 0);
    set_sprite_pixel_color(1887, 7, 7, 0);
    set_sprite_pixel_color(1888, 7, 7, 0);
    set_sprite_pixel_color(1889, 7, 7, 0);
    set_sprite_pixel_color(1890, 7, 7, 0);
    set_sprite_pixel_color(1891, 7, 7, 0);
    set_sprite_pixel_color(1892, 7, 7, 0);
    set_sprite_pixel_color(1893, 7, 7, 0);
    set_sprite_pixel_color(1894, 7, 7, 0);
    set_sprite_pixel_color(1895, 7, 7, 0);
    set_sprite_pixel_color(1896, 7, 7, 0);
    set_sprite_pixel_color(1897, 7, 7, 0);
    set_sprite_pixel_color(1898, 0, 0, 0);
    set_sprite_pixel_color(1899, 0, 0, 0);
    set_sprite_pixel_color(1900, 0, 0, 0);
    set_sprite_pixel_color(1901, 0, 0, 0);
    set_sprite_pixel_color(1902, 7, 7, 0);
    set_sprite_pixel_color(1903, 7, 7, 0);
    set_sprite_pixel_color(1904, 7, 7, 0);
    set_sprite_pixel_color(1905, 7, 7, 0);
    set_sprite_pixel_color(1906, 7, 7, 0);
    set_sprite_pixel_color(1907, 7, 7, 0);
    set_sprite_pixel_color(1908, 7, 7, 0);
    set_sprite_pixel_color(1909, 7, 7, 0);
    set_sprite_pixel_color(1910, 7, 7, 0);
    set_sprite_pixel_color(1911, 7, 7, 0);
    set_sprite_pixel_color(1912, 7, 7, 0);
    set_sprite_pixel_color(1913, 7, 7, 0);
    set_sprite_pixel_color(1914, 7, 7, 0);
    set_sprite_pixel_color(1915, 7, 7, 0);
    set_sprite_pixel_color(1916, 7, 7, 0);
    set_sprite_pixel_color(1917, 0, 0, 0);
    set_sprite_pixel_color(1918, 0, 0, 0);
    set_sprite_pixel_color(1919, 0, 0, 0);
    set_sprite_pixel_color(1920, 0, 0, 0);
    set_sprite_pixel_color(1921, 0, 0, 0);
    set_sprite_pixel_color(1922, 1, 1, 1);
    set_sprite_pixel_color(1923, 7, 7, 0);
    set_sprite_pixel_color(1924, 7, 7, 0);
    set_sprite_pixel_color(1925, 7, 7, 0);
    set_sprite_pixel_color(1926, 7, 7, 0);
    set_sprite_pixel_color(1927, 7, 7, 0);
    set_sprite_pixel_color(1928, 7, 7, 0);
    set_sprite_pixel_color(1929, 7, 7, 0);
    set_sprite_pixel_color(1930, 7, 7, 0);
    set_sprite_pixel_color(1931, 7, 7, 0);
    set_sprite_pixel_color(1932, 7, 7, 0);
    set_sprite_pixel_color(1933, 7, 7, 0);
    set_sprite_pixel_color(1934, 7, 7, 0);
    set_sprite_pixel_color(1935, 7, 7, 0);
    set_sprite_pixel_color(1936, 6, 6, 0);
    set_sprite_pixel_color(1937, 0, 0, 0);
    set_sprite_pixel_color(1938, 0, 0, 0);
    set_sprite_pixel_color(1939, 0, 0, 0);
    set_sprite_pixel_color(1940, 0, 0, 0);
    set_sprite_pixel_color(1941, 0, 0, 0);
    set_sprite_pixel_color(1942, 0, 0, 0);
    set_sprite_pixel_color(1943, 0, 0, 0);
    set_sprite_pixel_color(1944, 7, 7, 0);
    set_sprite_pixel_color(1945, 7, 7, 0);
    set_sprite_pixel_color(1946, 7, 7, 0);
    set_sprite_pixel_color(1947, 7, 7, 0);
    set_sprite_pixel_color(1948, 7, 7, 0);
    set_sprite_pixel_color(1949, 7, 7, 0);
    set_sprite_pixel_color(1950, 7, 7, 0);
    set_sprite_pixel_color(1951, 7, 7, 0);
    set_sprite_pixel_color(1952, 7, 7, 0);
    set_sprite_pixel_color(1953, 7, 7, 0);
    set_sprite_pixel_color(1954, 7, 7, 0);
    set_sprite_pixel_color(1955, 7, 7, 0);
    set_sprite_pixel_color(1956, 0, 0, 0);
    set_sprite_pixel_color(1957, 0, 0, 0);
    set_sprite_pixel_color(1958, 0, 0, 0);
    set_sprite_pixel_color(1959, 0, 0, 0);
    set_sprite_pixel_color(1960, 0, 0, 0);
    set_sprite_pixel_color(1961, 0, 0, 0);
    set_sprite_pixel_color(1962, 0, 0, 0);
    set_sprite_pixel_color(1963, 0, 0, 0);
    set_sprite_pixel_color(1964, 0, 0, 0);
    set_sprite_pixel_color(1965, 7, 7, 0);
    set_sprite_pixel_color(1966, 7, 7, 0);
    set_sprite_pixel_color(1967, 7, 7, 0);
    set_sprite_pixel_color(1968, 7, 7, 0);
    set_sprite_pixel_color(1969, 7, 7, 0);
    set_sprite_pixel_color(1970, 7, 7, 0);
    set_sprite_pixel_color(1971, 7, 7, 0);
    set_sprite_pixel_color(1972, 7, 7, 0);
    set_sprite_pixel_color(1973, 7, 7, 0);
    set_sprite_pixel_color(1974, 0, 0, 0);
    set_sprite_pixel_color(1975, 0, 0, 0);
    set_sprite_pixel_color(1976, 0, 0, 0);
    set_sprite_pixel_color(1977, 0, 0, 0);
    set_sprite_pixel_color(1978, 0, 0, 0);
    set_sprite_pixel_color(1979, 0, 0, 0);
    set_sprite_pixel_color(1980, 0, 0, 0);
    set_sprite_pixel_color(1981, 0, 0, 0);
    set_sprite_pixel_color(1982, 0, 0, 0);
    set_sprite_pixel_color(1983, 0, 0, 0);
    set_sprite_pixel_color(1984, 0, 0, 0);
    set_sprite_pixel_color(1985, 0, 0, 0);
    set_sprite_pixel_color(1986, 0, 0, 0);
    set_sprite_pixel_color(1987, 7, 7, 0);
    set_sprite_pixel_color(1988, 7, 7, 0);
    set_sprite_pixel_color(1989, 7, 7, 0);
    set_sprite_pixel_color(1990, 7, 7, 0);
    set_sprite_pixel_color(1991, 7, 7, 0);
    set_sprite_pixel_color(1992, 2, 2, 0);
    set_sprite_pixel_color(1993, 0, 0, 0);
    set_sprite_pixel_color(1994, 0, 0, 0);
    set_sprite_pixel_color(1995, 0, 0, 0);
    set_sprite_pixel_color(1996, 0, 0, 0);
    set_sprite_pixel_color(1997, 0, 0, 0);
    set_sprite_pixel_color(1998, 0, 0, 0);
    set_sprite_pixel_color(1999, 0, 0, 0);

    set_sprite_pixel_color(2000, 0, 0, 0); // sprite boca para baixo
    set_sprite_pixel_color(2001, 0, 0, 0);
    set_sprite_pixel_color(2002, 0, 0, 0);
    set_sprite_pixel_color(2003, 0, 0, 0);
    set_sprite_pixel_color(2004, 0, 0, 0);
    set_sprite_pixel_color(2005, 0, 0, 0);
    set_sprite_pixel_color(2006, 0, 0, 0);
    set_sprite_pixel_color(2007, 0, 0, 0);
    set_sprite_pixel_color(2008, 0, 0, 0);
    set_sprite_pixel_color(2009, 0, 0, 0);
    set_sprite_pixel_color(2010, 0, 0, 0);
    set_sprite_pixel_color(2011, 0, 0, 0);
    set_sprite_pixel_color(2012, 0, 0, 0);
    set_sprite_pixel_color(2013, 0, 0, 0);
    set_sprite_pixel_color(2014, 0, 0, 0);
    set_sprite_pixel_color(2015, 0, 0, 0);
    set_sprite_pixel_color(2016, 0, 0, 0);
    set_sprite_pixel_color(2017, 0, 0, 0);
    set_sprite_pixel_color(2018, 0, 0, 0);
    set_sprite_pixel_color(2019, 0, 0, 0);
    set_sprite_pixel_color(2020, 0, 0, 0);
    set_sprite_pixel_color(2021, 0, 0, 0);
    set_sprite_pixel_color(2022, 0, 0, 0);
    set_sprite_pixel_color(2023, 0, 0, 0);
    set_sprite_pixel_color(2024, 0, 0, 0);
    set_sprite_pixel_color(2025, 0, 0, 0);
    set_sprite_pixel_color(2026, 7, 7, 0);
    set_sprite_pixel_color(2027, 7, 7, 0);
    set_sprite_pixel_color(2028, 7, 7, 0);
    set_sprite_pixel_color(2029, 7, 7, 0);
    set_sprite_pixel_color(2030, 7, 7, 0);
    set_sprite_pixel_color(2031, 7, 7, 0);
    set_sprite_pixel_color(2032, 7, 7, 0);
    set_sprite_pixel_color(2033, 7, 7, 0);
    set_sprite_pixel_color(2034, 0, 0, 0);
    set_sprite_pixel_color(2035, 0, 0, 0);
    set_sprite_pixel_color(2036, 0, 0, 0);
    set_sprite_pixel_color(2037, 0, 0, 0);
    set_sprite_pixel_color(2038, 0, 0, 0);
    set_sprite_pixel_color(2039, 0, 0, 0);
    set_sprite_pixel_color(2040, 0, 0, 0);
    set_sprite_pixel_color(2041, 0, 0, 0);
    set_sprite_pixel_color(2042, 0, 0, 0);
    set_sprite_pixel_color(2043, 0, 0, 0);
    set_sprite_pixel_color(2044, 0, 0, 0);
    set_sprite_pixel_color(2045, 7, 7, 0);
    set_sprite_pixel_color(2046, 7, 7, 0);
    set_sprite_pixel_color(2047, 7, 7, 0);
    set_sprite_pixel_color(2048, 7, 7, 0);
    set_sprite_pixel_color(2049, 7, 7, 0);
    set_sprite_pixel_color(2050, 7, 7, 0);
    set_sprite_pixel_color(2051, 7, 7, 0);
    set_sprite_pixel_color(2052, 7, 7, 0);
    set_sprite_pixel_color(2053, 7, 7, 0);
    set_sprite_pixel_color(2054, 7, 7, 0);
    set_sprite_pixel_color(2055, 7, 7, 0);
    set_sprite_pixel_color(2056, 0, 0, 0);
    set_sprite_pixel_color(2057, 0, 0, 0);
    set_sprite_pixel_color(2058, 0, 0, 0);
    set_sprite_pixel_color(2059, 0, 0, 0);
    set_sprite_pixel_color(2060, 0, 0, 0);
    set_sprite_pixel_color(2061, 0, 0, 0);
    set_sprite_pixel_color(2062, 0, 0, 0);
    set_sprite_pixel_color(2063, 0, 0, 0);
    set_sprite_pixel_color(2064, 7, 7, 0);
    set_sprite_pixel_color(2065, 7, 7, 0);
    set_sprite_pixel_color(2066, 7, 7, 0);
    set_sprite_pixel_color(2067, 7, 7, 0);
    set_sprite_pixel_color(2068, 7, 7, 0);
    set_sprite_pixel_color(2069, 7, 7, 0);
    set_sprite_pixel_color(2070, 7, 7, 0);
    set_sprite_pixel_color(2071, 7, 7, 0);
    set_sprite_pixel_color(2072, 7, 7, 0);
    set_sprite_pixel_color(2073, 7, 7, 0);
    set_sprite_pixel_color(2074, 7, 7, 0);
    set_sprite_pixel_color(2075, 7, 7, 0);
    set_sprite_pixel_color(2076, 7, 7, 0);
    set_sprite_pixel_color(2077, 0, 0, 0);
    set_sprite_pixel_color(2078, 0, 0, 0);
    set_sprite_pixel_color(2079, 0, 0, 0);
    set_sprite_pixel_color(2080, 0, 0, 0);
    set_sprite_pixel_color(2081, 0, 0, 0);
    set_sprite_pixel_color(2082, 0, 0, 0);
    set_sprite_pixel_color(2083, 7, 7, 0);
    set_sprite_pixel_color(2084, 7, 7, 0);
    set_sprite_pixel_color(2085, 7, 7, 0);
    set_sprite_pixel_color(2086, 7, 7, 0);
    set_sprite_pixel_color(2087, 7, 7, 0);
    set_sprite_pixel_color(2088, 7, 7, 0);
    set_sprite_pixel_color(2089, 7, 7, 0);
    set_sprite_pixel_color(2090, 7, 7, 0);
    set_sprite_pixel_color(2091, 7, 7, 0);
    set_sprite_pixel_color(2092, 7, 7, 0);
    set_sprite_pixel_color(2093, 7, 7, 0);
    set_sprite_pixel_color(2094, 7, 7, 0);
    set_sprite_pixel_color(2095, 7, 7, 0);
    set_sprite_pixel_color(2096, 7, 7, 0);
    set_sprite_pixel_color(2097, 3, 3, 0);
    set_sprite_pixel_color(2098, 0, 0, 0);
    set_sprite_pixel_color(2099, 0, 0, 0);
    set_sprite_pixel_color(2100, 0, 0, 0);
    set_sprite_pixel_color(2101, 0, 0, 0);
    set_sprite_pixel_color(2102, 7, 7, 0);
    set_sprite_pixel_color(2103, 7, 7, 0);
    set_sprite_pixel_color(2104, 7, 7, 0);
    set_sprite_pixel_color(2105, 7, 7, 0);
    set_sprite_pixel_color(2106, 7, 7, 0);
    set_sprite_pixel_color(2107, 7, 7, 0);
    set_sprite_pixel_color(2108, 7, 7, 0);
    set_sprite_pixel_color(2109, 7, 7, 0);
    set_sprite_pixel_color(2110, 7, 7, 0);
    set_sprite_pixel_color(2111, 7, 7, 0);
    set_sprite_pixel_color(2112, 7, 7, 0);
    set_sprite_pixel_color(2113, 7, 7, 0);
    set_sprite_pixel_color(2114, 7, 7, 0);
    set_sprite_pixel_color(2115, 7, 7, 0);
    set_sprite_pixel_color(2116, 7, 7, 0);
    set_sprite_pixel_color(2117, 7, 7, 0);
    set_sprite_pixel_color(2118, 0, 0, 0);
    set_sprite_pixel_color(2119, 0, 0, 0);
    set_sprite_pixel_color(2120, 0, 0, 0);
    set_sprite_pixel_color(2121, 6, 6, 6);
    set_sprite_pixel_color(2122, 7, 7, 0);
    set_sprite_pixel_color(2123, 7, 7, 0);
    set_sprite_pixel_color(2124, 7, 7, 0);
    set_sprite_pixel_color(2125, 7, 7, 0);
    set_sprite_pixel_color(2126, 7, 7, 0);
    set_sprite_pixel_color(2127, 7, 7, 0);
    set_sprite_pixel_color(2128, 7, 7, 0);
    set_sprite_pixel_color(2129, 7, 7, 0);
    set_sprite_pixel_color(2130, 7, 7, 0);
    set_sprite_pixel_color(2131, 7, 7, 0);
    set_sprite_pixel_color(2132, 7, 7, 0);
    set_sprite_pixel_color(2133, 7, 7, 0);
    set_sprite_pixel_color(2134, 7, 7, 0);
    set_sprite_pixel_color(2135, 7, 7, 0);
    set_sprite_pixel_color(2136, 7, 7, 0);
    set_sprite_pixel_color(2137, 7, 7, 0);
    set_sprite_pixel_color(2138, 7, 7, 0);
    set_sprite_pixel_color(2139, 0, 0, 0);
    set_sprite_pixel_color(2140, 0, 0, 0);
    set_sprite_pixel_color(2141, 0, 0, 0);
    set_sprite_pixel_color(2142, 7, 7, 0);
    set_sprite_pixel_color(2143, 7, 7, 0);
    set_sprite_pixel_color(2144, 7, 7, 0);
    set_sprite_pixel_color(2145, 7, 7, 0);
    set_sprite_pixel_color(2146, 7, 7, 0);
    set_sprite_pixel_color(2147, 7, 7, 0);
    set_sprite_pixel_color(2148, 7, 7, 0);
    set_sprite_pixel_color(2149, 7, 7, 0);
    set_sprite_pixel_color(2150, 7, 7, 0);
    set_sprite_pixel_color(2151, 7, 7, 0);
    set_sprite_pixel_color(2152, 7, 7, 0);
    set_sprite_pixel_color(2153, 7, 7, 0);
    set_sprite_pixel_color(2154, 7, 7, 0);
    set_sprite_pixel_color(2155, 7, 7, 0);
    set_sprite_pixel_color(2156, 7, 7, 0);
    set_sprite_pixel_color(2157, 7, 7, 0);
    set_sprite_pixel_color(2158, 7, 7, 0);
    set_sprite_pixel_color(2159, 0, 0, 0);
    set_sprite_pixel_color(2160, 0, 0, 0);
    set_sprite_pixel_color(2161, 7, 7, 0);
    set_sprite_pixel_color(2162, 7, 7, 0);
    set_sprite_pixel_color(2163, 7, 7, 0);
    set_sprite_pixel_color(2164, 7, 7, 0);
    set_sprite_pixel_color(2165, 7, 7, 0);
    set_sprite_pixel_color(2166, 7, 7, 0);
    set_sprite_pixel_color(2167, 7, 7, 0);
    set_sprite_pixel_color(2168, 7, 7, 0);
    set_sprite_pixel_color(2169, 7, 7, 0);
    set_sprite_pixel_color(2170, 7, 7, 0);
    set_sprite_pixel_color(2171, 7, 7, 0);
    set_sprite_pixel_color(2172, 7, 7, 0);
    set_sprite_pixel_color(2173, 7, 7, 0);
    set_sprite_pixel_color(2174, 7, 7, 0);
    set_sprite_pixel_color(2175, 7, 7, 0);
    set_sprite_pixel_color(2176, 7, 7, 0);
    set_sprite_pixel_color(2177, 7, 7, 0);
    set_sprite_pixel_color(2178, 7, 7, 0);
    set_sprite_pixel_color(2179, 0, 0, 0);
    set_sprite_pixel_color(2180, 0, 0, 0);
    set_sprite_pixel_color(2181, 7, 7, 0);
    set_sprite_pixel_color(2182, 7, 7, 0);
    set_sprite_pixel_color(2183, 7, 7, 0);
    set_sprite_pixel_color(2184, 7, 7, 0);
    set_sprite_pixel_color(2185, 7, 7, 0);
    set_sprite_pixel_color(2186, 7, 7, 0);
    set_sprite_pixel_color(2187, 7, 7, 0);
    set_sprite_pixel_color(2188, 7, 7, 0);
    set_sprite_pixel_color(2189, 7, 7, 0);
    set_sprite_pixel_color(2190, 7, 7, 0);
    set_sprite_pixel_color(2191, 7, 7, 0);
    set_sprite_pixel_color(2192, 7, 7, 0);
    set_sprite_pixel_color(2193, 7, 7, 0);
    set_sprite_pixel_color(2194, 7, 7, 0);
    set_sprite_pixel_color(2195, 7, 7, 0);
    set_sprite_pixel_color(2196, 7, 7, 0);
    set_sprite_pixel_color(2197, 7, 7, 0);
    set_sprite_pixel_color(2198, 7, 7, 0);
    set_sprite_pixel_color(2199, 7, 7, 0);
    set_sprite_pixel_color(2200, 0, 0, 0);
    set_sprite_pixel_color(2201, 7, 7, 0);
    set_sprite_pixel_color(2202, 7, 7, 0);
    set_sprite_pixel_color(2203, 7, 7, 0);
    set_sprite_pixel_color(2204, 7, 7, 0);
    set_sprite_pixel_color(2205, 7, 7, 0);
    set_sprite_pixel_color(2206, 7, 7, 0);
    set_sprite_pixel_color(2207, 7, 7, 0);
    set_sprite_pixel_color(2208, 7, 7, 0);
    set_sprite_pixel_color(2209, 7, 7, 0);
    set_sprite_pixel_color(2210, 7, 7, 0);
    set_sprite_pixel_color(2211, 7, 7, 0);
    set_sprite_pixel_color(2212, 7, 7, 0);
    set_sprite_pixel_color(2213, 7, 7, 0);
    set_sprite_pixel_color(2214, 7, 7, 0);
    set_sprite_pixel_color(2215, 7, 7, 0);
    set_sprite_pixel_color(2216, 7, 7, 0);
    set_sprite_pixel_color(2217, 7, 7, 0);
    set_sprite_pixel_color(2218, 7, 7, 0);
    set_sprite_pixel_color(2219, 7, 7, 0);
    set_sprite_pixel_color(2220, 0, 0, 0);
    set_sprite_pixel_color(2221, 7, 7, 0);
    set_sprite_pixel_color(2222, 7, 7, 0);
    set_sprite_pixel_color(2223, 7, 7, 0);
    set_sprite_pixel_color(2224, 7, 7, 0);
    set_sprite_pixel_color(2225, 7, 7, 0);
    set_sprite_pixel_color(2226, 7, 7, 0);
    set_sprite_pixel_color(2227, 7, 7, 0);
    set_sprite_pixel_color(2228, 7, 7, 0);
    set_sprite_pixel_color(2229, 7, 7, 0);
    set_sprite_pixel_color(2230, 7, 7, 0);
    set_sprite_pixel_color(2231, 7, 7, 0);
    set_sprite_pixel_color(2232, 7, 7, 0);
    set_sprite_pixel_color(2233, 0, 0, 0);
    set_sprite_pixel_color(2234, 0, 0, 0);
    set_sprite_pixel_color(2235, 7, 7, 0);
    set_sprite_pixel_color(2236, 7, 7, 0);
    set_sprite_pixel_color(2237, 7, 7, 0);
    set_sprite_pixel_color(2238, 7, 7, 0);
    set_sprite_pixel_color(2239, 7, 7, 0);
    set_sprite_pixel_color(2240, 3, 3, 3);
    set_sprite_pixel_color(2241, 7, 7, 0);
    set_sprite_pixel_color(2242, 7, 7, 0);
    set_sprite_pixel_color(2243, 7, 7, 0);
    set_sprite_pixel_color(2244, 7, 7, 0);
    set_sprite_pixel_color(2245, 7, 7, 0);
    set_sprite_pixel_color(2246, 7, 7, 0);
    set_sprite_pixel_color(2247, 7, 7, 0);
    set_sprite_pixel_color(2248, 7, 7, 0);
    set_sprite_pixel_color(2249, 0, 0, 0);
    set_sprite_pixel_color(2250, 0, 0, 0);
    set_sprite_pixel_color(2251, 7, 7, 0);
    set_sprite_pixel_color(2252, 7, 7, 0);
    set_sprite_pixel_color(2253, 0, 0, 0);
    set_sprite_pixel_color(2254, 0, 0, 0);
    set_sprite_pixel_color(2255, 7, 7, 0);
    set_sprite_pixel_color(2256, 7, 7, 0);
    set_sprite_pixel_color(2257, 7, 7, 0);
    set_sprite_pixel_color(2258, 7, 7, 0);
    set_sprite_pixel_color(2259, 7, 7, 0);
    set_sprite_pixel_color(2260, 0, 0, 0);
    set_sprite_pixel_color(2261, 7, 7, 0);
    set_sprite_pixel_color(2262, 7, 7, 0);
    set_sprite_pixel_color(2263, 7, 7, 0);
    set_sprite_pixel_color(2264, 7, 7, 0);
    set_sprite_pixel_color(2265, 7, 7, 0);
    set_sprite_pixel_color(2266, 7, 7, 0);
    set_sprite_pixel_color(2267, 7, 7, 0);
    set_sprite_pixel_color(2268, 0, 0, 0);
    set_sprite_pixel_color(2269, 0, 0, 0);
    set_sprite_pixel_color(2270, 0, 0, 0);
    set_sprite_pixel_color(2271, 0, 0, 0);
    set_sprite_pixel_color(2272, 7, 7, 0);
    set_sprite_pixel_color(2273, 7, 7, 0);
    set_sprite_pixel_color(2274, 7, 7, 0);
    set_sprite_pixel_color(2275, 7, 7, 0);
    set_sprite_pixel_color(2276, 7, 7, 0);
    set_sprite_pixel_color(2277, 7, 7, 0);
    set_sprite_pixel_color(2278, 7, 7, 0);
    set_sprite_pixel_color(2279, 7, 7, 0);
    set_sprite_pixel_color(2280, 0, 0, 0);
    set_sprite_pixel_color(2281, 7, 7, 0);
    set_sprite_pixel_color(2282, 7, 7, 0);
    set_sprite_pixel_color(2283, 7, 7, 0);
    set_sprite_pixel_color(2284, 7, 7, 0);
    set_sprite_pixel_color(2285, 7, 7, 0);
    set_sprite_pixel_color(2286, 7, 7, 0);
    set_sprite_pixel_color(2287, 0, 0, 0);
    set_sprite_pixel_color(2288, 0, 0, 0);
    set_sprite_pixel_color(2289, 0, 0, 0);
    set_sprite_pixel_color(2290, 0, 0, 0);
    set_sprite_pixel_color(2291, 0, 0, 0);
    set_sprite_pixel_color(2292, 0, 0, 0);
    set_sprite_pixel_color(2293, 7, 7, 0);
    set_sprite_pixel_color(2294, 7, 7, 0);
    set_sprite_pixel_color(2295, 7, 7, 0);
    set_sprite_pixel_color(2296, 7, 7, 0);
    set_sprite_pixel_color(2297, 7, 7, 0);
    set_sprite_pixel_color(2298, 7, 7, 0);
    set_sprite_pixel_color(2299, 0, 0, 0);
    set_sprite_pixel_color(2300, 0, 0, 0);
    set_sprite_pixel_color(2301, 7, 7, 0);
    set_sprite_pixel_color(2302, 7, 7, 0);
    set_sprite_pixel_color(2303, 7, 7, 0);
    set_sprite_pixel_color(2304, 7, 7, 0);
    set_sprite_pixel_color(2305, 7, 7, 0);
    set_sprite_pixel_color(2306, 0, 0, 0);
    set_sprite_pixel_color(2307, 0, 0, 0);
    set_sprite_pixel_color(2308, 0, 0, 0);
    set_sprite_pixel_color(2309, 0, 0, 0);
    set_sprite_pixel_color(2310, 0, 0, 0);
    set_sprite_pixel_color(2311, 0, 0, 0);
    set_sprite_pixel_color(2312, 0, 0, 0);
    set_sprite_pixel_color(2313, 0, 0, 0);
    set_sprite_pixel_color(2314, 7, 7, 0);
    set_sprite_pixel_color(2315, 7, 7, 0);
    set_sprite_pixel_color(2316, 7, 7, 0);
    set_sprite_pixel_color(2317, 7, 7, 0);
    set_sprite_pixel_color(2318, 7, 7, 0);
    set_sprite_pixel_color(2319, 0, 0, 0);
    set_sprite_pixel_color(2320, 0, 0, 0);
    set_sprite_pixel_color(2321, 0, 0, 0);
    set_sprite_pixel_color(2322, 7, 7, 0);
    set_sprite_pixel_color(2323, 7, 7, 0);
    set_sprite_pixel_color(2324, 7, 7, 0);
    set_sprite_pixel_color(2325, 0, 0, 0);
    set_sprite_pixel_color(2326, 0, 0, 0);
    set_sprite_pixel_color(2327, 0, 0, 0);
    set_sprite_pixel_color(2328, 0, 0, 0);
    set_sprite_pixel_color(2329, 0, 0, 0);
    set_sprite_pixel_color(2330, 0, 0, 0);
    set_sprite_pixel_color(2331, 0, 0, 0);
    set_sprite_pixel_color(2332, 0, 0, 0);
    set_sprite_pixel_color(2333, 0, 0, 0);
    set_sprite_pixel_color(2334, 7, 7, 0);
    set_sprite_pixel_color(2335, 7, 7, 0);
    set_sprite_pixel_color(2336, 7, 7, 0);
    set_sprite_pixel_color(2337, 7, 7, 0);
    set_sprite_pixel_color(2338, 7, 7, 0);
    set_sprite_pixel_color(2339, 0, 0, 0);
    set_sprite_pixel_color(2340, 0, 0, 0);
    set_sprite_pixel_color(2341, 0, 0, 0);
    set_sprite_pixel_color(2342, 7, 7, 0);
    set_sprite_pixel_color(2343, 7, 7, 0);
    set_sprite_pixel_color(2344, 6, 6, 0);
    set_sprite_pixel_color(2345, 0, 0, 0);
    set_sprite_pixel_color(2346, 0, 0, 0);
    set_sprite_pixel_color(2347, 0, 0, 0);
    set_sprite_pixel_color(2348, 0, 0, 0);
    set_sprite_pixel_color(2349, 0, 0, 0);
    set_sprite_pixel_color(2350, 0, 0, 0);
    set_sprite_pixel_color(2351, 0, 0, 0);
    set_sprite_pixel_color(2352, 0, 0, 0);
    set_sprite_pixel_color(2353, 0, 0, 0);
    set_sprite_pixel_color(2354, 0, 0, 0);
    set_sprite_pixel_color(2355, 7, 7, 0);
    set_sprite_pixel_color(2356, 7, 7, 0);
    set_sprite_pixel_color(2357, 7, 7, 0);
    set_sprite_pixel_color(2358, 0, 0, 0);
    set_sprite_pixel_color(2359, 0, 0, 0);
    set_sprite_pixel_color(2360, 0, 0, 0);
    set_sprite_pixel_color(2361, 0, 0, 0);
    set_sprite_pixel_color(2362, 0, 0, 0);
    set_sprite_pixel_color(2363, 7, 7, 0);
    set_sprite_pixel_color(2364, 0, 0, 0);
    set_sprite_pixel_color(2365, 0, 0, 0);
    set_sprite_pixel_color(2366, 0, 0, 0);
    set_sprite_pixel_color(2367, 0, 0, 0);
    set_sprite_pixel_color(2368, 0, 0, 0);
    set_sprite_pixel_color(2369, 0, 0, 0);
    set_sprite_pixel_color(2370, 0, 0, 0);
    set_sprite_pixel_color(2371, 0, 0, 0);
    set_sprite_pixel_color(2372, 0, 0, 0);
    set_sprite_pixel_color(2373, 0, 0, 0);
    set_sprite_pixel_color(2374, 0, 0, 0);
    set_sprite_pixel_color(2375, 0, 0, 0);
    set_sprite_pixel_color(2376, 7, 7, 0);
    set_sprite_pixel_color(2377, 7, 7, 0);
    set_sprite_pixel_color(2378, 0, 0, 0);
    set_sprite_pixel_color(2379, 0, 0, 0);
    set_sprite_pixel_color(2380, 0, 0, 0);
    set_sprite_pixel_color(2381, 0, 0, 0);
    set_sprite_pixel_color(2382, 0, 0, 0);
    set_sprite_pixel_color(2383, 0, 0, 0);
    set_sprite_pixel_color(2384, 0, 0, 0);
    set_sprite_pixel_color(2385, 0, 0, 0);
    set_sprite_pixel_color(2386, 0, 0, 0);
    set_sprite_pixel_color(2387, 0, 0, 0);
    set_sprite_pixel_color(2388, 0, 0, 0);
    set_sprite_pixel_color(2389, 0, 0, 0);
    set_sprite_pixel_color(2390, 0, 0, 0);
    set_sprite_pixel_color(2391, 0, 0, 0);
    set_sprite_pixel_color(2392, 0, 0, 0);
    set_sprite_pixel_color(2393, 0, 0, 0);
    set_sprite_pixel_color(2394, 0, 0, 0);
    set_sprite_pixel_color(2395, 0, 0, 0);
    set_sprite_pixel_color(2396, 0, 0, 0);
    set_sprite_pixel_color(2397, 0, 0, 0);
    set_sprite_pixel_color(2398, 0, 0, 0);
    set_sprite_pixel_color(2399, 0, 0, 0);

    set_sprite_pixel_color(2400, 0, 0, 0); // sprite boca para a direita
    set_sprite_pixel_color(2401, 0, 0, 0);
    set_sprite_pixel_color(2402, 0, 0, 0);
    set_sprite_pixel_color(2403, 0, 0, 0);
    set_sprite_pixel_color(2404, 0, 0, 0);
    set_sprite_pixel_color(2405, 0, 0, 0);
    set_sprite_pixel_color(2406, 0, 0, 0);
    set_sprite_pixel_color(2407, 0, 0, 0);
    set_sprite_pixel_color(2408, 0, 0, 0);
    set_sprite_pixel_color(2409, 0, 0, 0);
    set_sprite_pixel_color(2410, 0, 0, 0);
    set_sprite_pixel_color(2411, 7, 7, 0);
    set_sprite_pixel_color(2412, 7, 7, 0);
    set_sprite_pixel_color(2413, 0, 0, 0);
    set_sprite_pixel_color(2414, 0, 0, 0);
    set_sprite_pixel_color(2415, 0, 0, 0);
    set_sprite_pixel_color(2416, 0, 0, 0);
    set_sprite_pixel_color(2417, 0, 0, 0);
    set_sprite_pixel_color(2418, 0, 0, 0);
    set_sprite_pixel_color(2419, 0, 0, 0);
    set_sprite_pixel_color(2420, 0, 0, 0);
    set_sprite_pixel_color(2421, 0, 0, 0);
    set_sprite_pixel_color(2422, 0, 0, 0);
    set_sprite_pixel_color(2423, 0, 0, 0);
    set_sprite_pixel_color(2424, 0, 0, 0);
    set_sprite_pixel_color(2425, 0, 0, 0);
    set_sprite_pixel_color(2426, 0, 0, 0);
    set_sprite_pixel_color(2427, 7, 7, 0);
    set_sprite_pixel_color(2428, 7, 7, 0);
    set_sprite_pixel_color(2429, 7, 7, 0);
    set_sprite_pixel_color(2430, 7, 7, 0);
    set_sprite_pixel_color(2431, 7, 7, 0);
    set_sprite_pixel_color(2432, 7, 7, 0);
    set_sprite_pixel_color(2433, 7, 7, 0);
    set_sprite_pixel_color(2434, 7, 7, 0);
    set_sprite_pixel_color(2435, 7, 7, 0);
    set_sprite_pixel_color(2436, 7, 7, 0);
    set_sprite_pixel_color(2437, 0, 0, 0);
    set_sprite_pixel_color(2438, 0, 0, 0);
    set_sprite_pixel_color(2439, 0, 0, 0);
    set_sprite_pixel_color(2440, 0, 0, 0);
    set_sprite_pixel_color(2441, 0, 0, 0);
    set_sprite_pixel_color(2442, 0, 0, 0);
    set_sprite_pixel_color(2443, 0, 0, 0);
    set_sprite_pixel_color(2444, 0, 0, 0);
    set_sprite_pixel_color(2445, 7, 7, 0);
    set_sprite_pixel_color(2446, 7, 7, 0);
    set_sprite_pixel_color(2447, 7, 7, 0);
    set_sprite_pixel_color(2448, 7, 7, 0);
    set_sprite_pixel_color(2449, 7, 7, 0);
    set_sprite_pixel_color(2450, 7, 7, 0);
    set_sprite_pixel_color(2451, 7, 7, 0);
    set_sprite_pixel_color(2452, 7, 7, 0);
    set_sprite_pixel_color(2453, 7, 7, 0);
    set_sprite_pixel_color(2454, 7, 7, 0);
    set_sprite_pixel_color(2455, 7, 7, 0);
    set_sprite_pixel_color(2456, 7, 7, 0);
    set_sprite_pixel_color(2457, 7, 7, 0);
    set_sprite_pixel_color(2458, 7, 7, 0);
    set_sprite_pixel_color(2459, 0, 0, 0);
    set_sprite_pixel_color(2460, 0, 0, 0);
    set_sprite_pixel_color(2461, 0, 0, 0);
    set_sprite_pixel_color(2462, 0, 0, 0);
    set_sprite_pixel_color(2463, 5, 5, 0);
    set_sprite_pixel_color(2464, 7, 7, 0);
    set_sprite_pixel_color(2465, 7, 7, 0);
    set_sprite_pixel_color(2466, 7, 7, 0);
    set_sprite_pixel_color(2467, 7, 7, 0);
    set_sprite_pixel_color(2468, 7, 7, 0);
    set_sprite_pixel_color(2469, 7, 7, 0);
    set_sprite_pixel_color(2470, 7, 7, 0);
    set_sprite_pixel_color(2471, 7, 7, 0);
    set_sprite_pixel_color(2472, 7, 7, 0);
    set_sprite_pixel_color(2473, 7, 7, 0);
    set_sprite_pixel_color(2474, 7, 7, 0);
    set_sprite_pixel_color(2475, 7, 7, 0);
    set_sprite_pixel_color(2476, 7, 7, 0);
    set_sprite_pixel_color(2477, 7, 7, 0);
    set_sprite_pixel_color(2478, 7, 7, 0);
    set_sprite_pixel_color(2479, 0, 0, 0);
    set_sprite_pixel_color(2480, 0, 0, 0);
    set_sprite_pixel_color(2481, 0, 0, 0);
    set_sprite_pixel_color(2482, 0, 0, 0);
    set_sprite_pixel_color(2483, 7, 7, 0);
    set_sprite_pixel_color(2484, 7, 7, 0);
    set_sprite_pixel_color(2485, 7, 7, 0);
    set_sprite_pixel_color(2486, 7, 7, 0);
    set_sprite_pixel_color(2487, 7, 7, 0);
    set_sprite_pixel_color(2488, 7, 7, 0);
    set_sprite_pixel_color(2489, 7, 7, 0);
    set_sprite_pixel_color(2490, 7, 7, 0);
    set_sprite_pixel_color(2491, 7, 7, 0);
    set_sprite_pixel_color(2492, 7, 7, 0);
    set_sprite_pixel_color(2493, 7, 7, 0);
    set_sprite_pixel_color(2494, 7, 7, 0);
    set_sprite_pixel_color(2495, 7, 7, 0);
    set_sprite_pixel_color(2496, 7, 7, 0);
    set_sprite_pixel_color(2497, 7, 7, 0);
    set_sprite_pixel_color(2498, 0, 0, 0);
    set_sprite_pixel_color(2499, 0, 0, 0);
    set_sprite_pixel_color(2500, 0, 0, 0);
    set_sprite_pixel_color(2501, 0, 0, 0);
    set_sprite_pixel_color(2502, 7, 7, 0);
    set_sprite_pixel_color(2503, 7, 7, 0);
    set_sprite_pixel_color(2504, 7, 7, 0);
    set_sprite_pixel_color(2505, 7, 7, 0);
    set_sprite_pixel_color(2506, 7, 7, 0);
    set_sprite_pixel_color(2507, 7, 7, 0);
    set_sprite_pixel_color(2508, 7, 7, 0);
    set_sprite_pixel_color(2509, 7, 7, 0);
    set_sprite_pixel_color(2510, 7, 7, 0);
    set_sprite_pixel_color(2511, 0, 0, 0);
    set_sprite_pixel_color(2512, 0, 0, 0);
    set_sprite_pixel_color(2513, 7, 7, 0);
    set_sprite_pixel_color(2514, 7, 7, 0);
    set_sprite_pixel_color(2515, 7, 7, 0);
    set_sprite_pixel_color(2516, 7, 7, 0);
    set_sprite_pixel_color(2517, 0, 0, 0);
    set_sprite_pixel_color(2518, 0, 0, 0);
    set_sprite_pixel_color(2519, 0, 0, 0);
    set_sprite_pixel_color(2520, 0, 0, 0);
    set_sprite_pixel_color(2521, 7, 7, 0);
    set_sprite_pixel_color(2522, 7, 7, 0);
    set_sprite_pixel_color(2523, 7, 7, 0);
    set_sprite_pixel_color(2524, 7, 7, 0);
    set_sprite_pixel_color(2525, 7, 7, 0);
    set_sprite_pixel_color(2526, 7, 7, 0);
    set_sprite_pixel_color(2527, 7, 7, 0);
    set_sprite_pixel_color(2528, 7, 7, 0);
    set_sprite_pixel_color(2529, 7, 7, 0);
    set_sprite_pixel_color(2530, 7, 7, 0);
    set_sprite_pixel_color(2531, 0, 0, 0);
    set_sprite_pixel_color(2532, 0, 0, 0);
    set_sprite_pixel_color(2533, 7, 7, 0);
    set_sprite_pixel_color(2534, 7, 7, 0);
    set_sprite_pixel_color(2535, 7, 7, 0);
    set_sprite_pixel_color(2536, 0, 0, 0);
    set_sprite_pixel_color(2537, 0, 0, 0);
    set_sprite_pixel_color(2538, 0, 0, 0);
    set_sprite_pixel_color(2539, 0, 0, 0);
    set_sprite_pixel_color(2540, 0, 0, 0);
    set_sprite_pixel_color(2541, 7, 7, 0);
    set_sprite_pixel_color(2542, 7, 7, 0);
    set_sprite_pixel_color(2543, 7, 7, 0);
    set_sprite_pixel_color(2544, 7, 7, 0);
    set_sprite_pixel_color(2545, 7, 7, 0);
    set_sprite_pixel_color(2546, 7, 7, 0);
    set_sprite_pixel_color(2547, 7, 7, 0);
    set_sprite_pixel_color(2548, 7, 7, 0);
    set_sprite_pixel_color(2549, 7, 7, 0);
    set_sprite_pixel_color(2550, 7, 7, 0);
    set_sprite_pixel_color(2551, 7, 7, 0);
    set_sprite_pixel_color(2552, 7, 7, 0);
    set_sprite_pixel_color(2553, 7, 7, 0);
    set_sprite_pixel_color(2554, 0, 0, 0);
    set_sprite_pixel_color(2555, 0, 0, 0);
    set_sprite_pixel_color(2556, 0, 0, 0);
    set_sprite_pixel_color(2557, 0, 0, 0);
    set_sprite_pixel_color(2558, 0, 0, 0);
    set_sprite_pixel_color(2559, 0, 0, 0);
    set_sprite_pixel_color(2560, 0, 0, 0);
    set_sprite_pixel_color(2561, 7, 7, 0);
    set_sprite_pixel_color(2562, 7, 7, 0);
    set_sprite_pixel_color(2563, 7, 7, 0);
    set_sprite_pixel_color(2564, 7, 7, 0);
    set_sprite_pixel_color(2565, 7, 7, 0);
    set_sprite_pixel_color(2566, 7, 7, 0);
    set_sprite_pixel_color(2567, 7, 7, 0);
    set_sprite_pixel_color(2568, 7, 7, 0);
    set_sprite_pixel_color(2569, 7, 7, 0);
    set_sprite_pixel_color(2570, 7, 7, 0);
    set_sprite_pixel_color(2571, 7, 7, 0);
    set_sprite_pixel_color(2572, 7, 7, 0);
    set_sprite_pixel_color(2573, 0, 0, 0);
    set_sprite_pixel_color(2574, 0, 0, 0);
    set_sprite_pixel_color(2575, 0, 0, 0);
    set_sprite_pixel_color(2576, 0, 0, 0);
    set_sprite_pixel_color(2577, 0, 0, 0);
    set_sprite_pixel_color(2578, 0, 0, 0);
    set_sprite_pixel_color(2579, 0, 0, 0);
    set_sprite_pixel_color(2580, 0, 0, 0);
    set_sprite_pixel_color(2581, 7, 7, 0);
    set_sprite_pixel_color(2582, 7, 7, 0);
    set_sprite_pixel_color(2583, 7, 7, 0);
    set_sprite_pixel_color(2584, 7, 7, 0);
    set_sprite_pixel_color(2585, 7, 7, 0);
    set_sprite_pixel_color(2586, 7, 7, 0);
    set_sprite_pixel_color(2587, 7, 7, 0);
    set_sprite_pixel_color(2588, 7, 7, 0);
    set_sprite_pixel_color(2589, 7, 7, 0);
    set_sprite_pixel_color(2590, 7, 7, 0);
    set_sprite_pixel_color(2591, 7, 7, 0);
    set_sprite_pixel_color(2592, 0, 0, 0);
    set_sprite_pixel_color(2593, 0, 0, 0);
    set_sprite_pixel_color(2594, 0, 0, 0);
    set_sprite_pixel_color(2595, 0, 0, 0);
    set_sprite_pixel_color(2596, 0, 0, 0);
    set_sprite_pixel_color(2597, 0, 0, 0);
    set_sprite_pixel_color(2598, 0, 0, 0);
    set_sprite_pixel_color(2599, 0, 0, 0);
    set_sprite_pixel_color(2600, 0, 0, 0);
    set_sprite_pixel_color(2601, 7, 7, 0);
    set_sprite_pixel_color(2602, 7, 7, 0);
    set_sprite_pixel_color(2603, 7, 7, 0);
    set_sprite_pixel_color(2604, 7, 7, 0);
    set_sprite_pixel_color(2605, 7, 7, 0);
    set_sprite_pixel_color(2606, 7, 7, 0);
    set_sprite_pixel_color(2607, 7, 7, 0);
    set_sprite_pixel_color(2608, 7, 7, 0);
    set_sprite_pixel_color(2609, 7, 7, 0);
    set_sprite_pixel_color(2610, 7, 7, 0);
    set_sprite_pixel_color(2611, 7, 7, 0);
    set_sprite_pixel_color(2612, 0, 0, 0);
    set_sprite_pixel_color(2613, 0, 0, 0);
    set_sprite_pixel_color(2614, 0, 0, 0);
    set_sprite_pixel_color(2615, 0, 0, 0);
    set_sprite_pixel_color(2616, 0, 0, 0);
    set_sprite_pixel_color(2617, 0, 0, 0);
    set_sprite_pixel_color(2618, 0, 0, 0);
    set_sprite_pixel_color(2619, 0, 0, 0);
    set_sprite_pixel_color(2620, 0, 0, 0);
    set_sprite_pixel_color(2621, 7, 7, 0);
    set_sprite_pixel_color(2622, 7, 7, 0);
    set_sprite_pixel_color(2623, 7, 7, 0);
    set_sprite_pixel_color(2624, 7, 7, 0);
    set_sprite_pixel_color(2625, 7, 7, 0);
    set_sprite_pixel_color(2626, 7, 7, 0);
    set_sprite_pixel_color(2627, 7, 7, 0);
    set_sprite_pixel_color(2628, 7, 7, 0);
    set_sprite_pixel_color(2629, 7, 7, 0);
    set_sprite_pixel_color(2630, 7, 7, 0);
    set_sprite_pixel_color(2631, 7, 7, 0);
    set_sprite_pixel_color(2632, 7, 7, 0);
    set_sprite_pixel_color(2633, 0, 0, 0);
    set_sprite_pixel_color(2634, 0, 0, 0);
    set_sprite_pixel_color(2635, 0, 0, 0);
    set_sprite_pixel_color(2636, 0, 0, 0);
    set_sprite_pixel_color(2637, 0, 0, 0);
    set_sprite_pixel_color(2638, 0, 0, 0);
    set_sprite_pixel_color(2639, 0, 0, 0);
    set_sprite_pixel_color(2640, 0, 0, 0);
    set_sprite_pixel_color(2641, 7, 7, 0);
    set_sprite_pixel_color(2642, 7, 7, 0);
    set_sprite_pixel_color(2643, 7, 7, 0);
    set_sprite_pixel_color(2644, 7, 7, 0);
    set_sprite_pixel_color(2645, 7, 7, 0);
    set_sprite_pixel_color(2646, 7, 7, 0);
    set_sprite_pixel_color(2647, 7, 7, 0);
    set_sprite_pixel_color(2648, 7, 7, 0);
    set_sprite_pixel_color(2649, 7, 7, 0);
    set_sprite_pixel_color(2650, 7, 7, 0);
    set_sprite_pixel_color(2651, 7, 7, 0);
    set_sprite_pixel_color(2652, 7, 7, 0);
    set_sprite_pixel_color(2653, 7, 7, 0);
    set_sprite_pixel_color(2654, 0, 0, 0);
    set_sprite_pixel_color(2655, 0, 0, 0);
    set_sprite_pixel_color(2656, 0, 0, 0);
    set_sprite_pixel_color(2657, 0, 0, 0);
    set_sprite_pixel_color(2658, 0, 0, 0);
    set_sprite_pixel_color(2659, 0, 0, 0);
    set_sprite_pixel_color(2660, 0, 0, 0);
    set_sprite_pixel_color(2661, 7, 7, 0);
    set_sprite_pixel_color(2662, 7, 7, 0);
    set_sprite_pixel_color(2663, 7, 7, 0);
    set_sprite_pixel_color(2664, 7, 7, 0);
    set_sprite_pixel_color(2665, 7, 7, 0);
    set_sprite_pixel_color(2666, 7, 7, 0);
    set_sprite_pixel_color(2667, 7, 7, 0);
    set_sprite_pixel_color(2668, 7, 7, 0);
    set_sprite_pixel_color(2669, 7, 7, 0);
    set_sprite_pixel_color(2670, 7, 7, 0);
    set_sprite_pixel_color(2671, 7, 7, 0);
    set_sprite_pixel_color(2672, 7, 7, 0);
    set_sprite_pixel_color(2673, 7, 7, 0);
    set_sprite_pixel_color(2674, 7, 7, 0);
    set_sprite_pixel_color(2675, 0, 0, 0);
    set_sprite_pixel_color(2676, 0, 0, 0);
    set_sprite_pixel_color(2677, 0, 0, 0);
    set_sprite_pixel_color(2678, 0, 0, 0);
    set_sprite_pixel_color(2679, 0, 0, 0);
    set_sprite_pixel_color(2680, 0, 0, 0);
    set_sprite_pixel_color(2681, 0, 0, 0);
    set_sprite_pixel_color(2682, 7, 7, 0);
    set_sprite_pixel_color(2683, 7, 7, 0);
    set_sprite_pixel_color(2684, 7, 7, 0);
    set_sprite_pixel_color(2685, 7, 7, 0);
    set_sprite_pixel_color(2686, 7, 7, 0);
    set_sprite_pixel_color(2687, 7, 7, 0);
    set_sprite_pixel_color(2688, 7, 7, 0);
    set_sprite_pixel_color(2689, 7, 7, 0);
    set_sprite_pixel_color(2690, 7, 7, 0);
    set_sprite_pixel_color(2691, 7, 7, 0);
    set_sprite_pixel_color(2692, 7, 7, 0);
    set_sprite_pixel_color(2693, 7, 7, 0);
    set_sprite_pixel_color(2694, 7, 7, 0);
    set_sprite_pixel_color(2695, 7, 7, 0);
    set_sprite_pixel_color(2696, 0, 0, 0);
    set_sprite_pixel_color(2697, 0, 0, 0);
    set_sprite_pixel_color(2698, 0, 0, 0);
    set_sprite_pixel_color(2699, 0, 0, 0);
    set_sprite_pixel_color(2700, 0, 0, 0);
    set_sprite_pixel_color(2701, 0, 0, 0);
    set_sprite_pixel_color(2702, 0, 0, 0);
    set_sprite_pixel_color(2703, 7, 7, 0);
    set_sprite_pixel_color(2704, 7, 7, 0);
    set_sprite_pixel_color(2705, 7, 7, 0);
    set_sprite_pixel_color(2706, 7, 7, 0);
    set_sprite_pixel_color(2707, 7, 7, 0);
    set_sprite_pixel_color(2708, 7, 7, 0);
    set_sprite_pixel_color(2709, 7, 7, 0);
    set_sprite_pixel_color(2710, 7, 7, 0);
    set_sprite_pixel_color(2711, 7, 7, 0);
    set_sprite_pixel_color(2712, 7, 7, 0);
    set_sprite_pixel_color(2713, 7, 7, 0);
    set_sprite_pixel_color(2714, 7, 7, 0);
    set_sprite_pixel_color(2715, 7, 7, 0);
    set_sprite_pixel_color(2716, 7, 7, 0);
    set_sprite_pixel_color(2717, 0, 0, 0);
    set_sprite_pixel_color(2718, 0, 0, 0);
    set_sprite_pixel_color(2719, 0, 0, 0);
    set_sprite_pixel_color(2720, 0, 0, 0);
    set_sprite_pixel_color(2721, 0, 0, 0);
    set_sprite_pixel_color(2722, 0, 0, 0);
    set_sprite_pixel_color(2723, 0, 0, 0);
    set_sprite_pixel_color(2724, 7, 7, 0);
    set_sprite_pixel_color(2725, 7, 7, 0);
    set_sprite_pixel_color(2726, 7, 7, 0);
    set_sprite_pixel_color(2727, 7, 7, 0);
    set_sprite_pixel_color(2728, 7, 7, 0);
    set_sprite_pixel_color(2729, 7, 7, 0);
    set_sprite_pixel_color(2730, 7, 7, 0);
    set_sprite_pixel_color(2731, 7, 7, 0);
    set_sprite_pixel_color(2732, 7, 7, 0);
    set_sprite_pixel_color(2733, 7, 7, 0);
    set_sprite_pixel_color(2734, 7, 7, 0);
    set_sprite_pixel_color(2735, 7, 7, 0);
    set_sprite_pixel_color(2736, 7, 7, 0);
    set_sprite_pixel_color(2737, 7, 7, 0);
    set_sprite_pixel_color(2738, 0, 0, 0);
    set_sprite_pixel_color(2739, 0, 0, 0);
    set_sprite_pixel_color(2740, 0, 0, 0);
    set_sprite_pixel_color(2741, 0, 0, 0);
    set_sprite_pixel_color(2742, 0, 0, 0);
    set_sprite_pixel_color(2743, 0, 0, 0);
    set_sprite_pixel_color(2744, 0, 0, 0);
    set_sprite_pixel_color(2745, 7, 7, 0);
    set_sprite_pixel_color(2746, 7, 7, 0);
    set_sprite_pixel_color(2747, 7, 7, 0);
    set_sprite_pixel_color(2748, 7, 7, 0);
    set_sprite_pixel_color(2749, 7, 7, 0);
    set_sprite_pixel_color(2750, 7, 7, 0);
    set_sprite_pixel_color(2751, 7, 7, 0);
    set_sprite_pixel_color(2752, 7, 7, 0);
    set_sprite_pixel_color(2753, 7, 7, 0);
    set_sprite_pixel_color(2754, 7, 7, 0);
    set_sprite_pixel_color(2755, 7, 7, 0);
    set_sprite_pixel_color(2756, 7, 7, 0);
    set_sprite_pixel_color(2757, 7, 7, 0);
    set_sprite_pixel_color(2758, 0, 0, 0);
    set_sprite_pixel_color(2759, 0, 0, 0);
    set_sprite_pixel_color(2760, 0, 0, 0);
    set_sprite_pixel_color(2761, 0, 0, 0);
    set_sprite_pixel_color(2762, 0, 0, 0);
    set_sprite_pixel_color(2763, 0, 0, 0);
    set_sprite_pixel_color(2764, 0, 0, 0);
    set_sprite_pixel_color(2765, 0, 0, 0);
    set_sprite_pixel_color(2766, 0, 0, 0);
    set_sprite_pixel_color(2767, 7, 7, 0);
    set_sprite_pixel_color(2768, 7, 7, 0);
    set_sprite_pixel_color(2769, 7, 7, 0);
    set_sprite_pixel_color(2770, 7, 7, 0);
    set_sprite_pixel_color(2771, 7, 7, 0);
    set_sprite_pixel_color(2772, 7, 7, 0);
    set_sprite_pixel_color(2773, 7, 7, 0);
    set_sprite_pixel_color(2774, 7, 7, 0);
    set_sprite_pixel_color(2775, 7, 7, 0);
    set_sprite_pixel_color(2776, 0, 0, 0);
    set_sprite_pixel_color(2777, 0, 0, 0);
    set_sprite_pixel_color(2778, 0, 0, 0);
    set_sprite_pixel_color(2779, 0, 0, 0);
    set_sprite_pixel_color(2780, 0, 0, 0);
    set_sprite_pixel_color(2781, 0, 0, 0);
    set_sprite_pixel_color(2782, 0, 0, 0);
    set_sprite_pixel_color(2783, 0, 0, 0);
    set_sprite_pixel_color(2784, 0, 0, 0);
    set_sprite_pixel_color(2785, 0, 0, 0);
    set_sprite_pixel_color(2786, 0, 0, 0);
    set_sprite_pixel_color(2787, 0, 0, 0);
    set_sprite_pixel_color(2788, 0, 0, 0);
    set_sprite_pixel_color(2789, 0, 0, 0);
    set_sprite_pixel_color(2790, 0, 0, 0);
    set_sprite_pixel_color(2791, 0, 0, 0);
    set_sprite_pixel_color(2792, 0, 0, 0);
    set_sprite_pixel_color(2793, 0, 0, 0);
    set_sprite_pixel_color(2794, 0, 0, 0);
    set_sprite_pixel_color(2795, 0, 0, 0);
    set_sprite_pixel_color(2796, 0, 0, 0);
    set_sprite_pixel_color(2797, 0, 0, 0);
    set_sprite_pixel_color(2798, 0, 0, 0);
    set_sprite_pixel_color(2799, 0, 0, 0);

    set_sprite_pixel_color(2800, 0, 0, 0); // sprite boca para esquerda
    set_sprite_pixel_color(2801, 0, 0, 0);
    set_sprite_pixel_color(2802, 0, 0, 0);
    set_sprite_pixel_color(2803, 0, 0, 0);
    set_sprite_pixel_color(2804, 0, 0, 0);
    set_sprite_pixel_color(2805, 0, 0, 0);
    set_sprite_pixel_color(2806, 0, 0, 0);
    set_sprite_pixel_color(2807, 0, 0, 0);
    set_sprite_pixel_color(2808, 7, 7, 0);
    set_sprite_pixel_color(2809, 7, 7, 0);
    set_sprite_pixel_color(2810, 0, 0, 0);
    set_sprite_pixel_color(2811, 0, 0, 0);
    set_sprite_pixel_color(2812, 0, 0, 0);
    set_sprite_pixel_color(2813, 0, 0, 0);
    set_sprite_pixel_color(2814, 0, 0, 0);
    set_sprite_pixel_color(2815, 0, 0, 0);
    set_sprite_pixel_color(2816, 0, 0, 0);
    set_sprite_pixel_color(2817, 0, 0, 0);
    set_sprite_pixel_color(2818, 0, 0, 0);
    set_sprite_pixel_color(2819, 0, 0, 0);
    set_sprite_pixel_color(2820, 0, 0, 0);
    set_sprite_pixel_color(2821, 0, 0, 0);
    set_sprite_pixel_color(2822, 0, 0, 0);
    set_sprite_pixel_color(2823, 0, 0, 0);
    set_sprite_pixel_color(2824, 7, 7, 0);
    set_sprite_pixel_color(2825, 7, 7, 0);
    set_sprite_pixel_color(2826, 7, 7, 0);
    set_sprite_pixel_color(2827, 7, 7, 0);
    set_sprite_pixel_color(2828, 7, 7, 0);
    set_sprite_pixel_color(2829, 7, 7, 0);
    set_sprite_pixel_color(2830, 7, 7, 0);
    set_sprite_pixel_color(2831, 7, 7, 0);
    set_sprite_pixel_color(2832, 7, 7, 0);
    set_sprite_pixel_color(2833, 7, 7, 0);
    set_sprite_pixel_color(2834, 0, 0, 0);
    set_sprite_pixel_color(2835, 0, 0, 0);
    set_sprite_pixel_color(2836, 0, 0, 0);
    set_sprite_pixel_color(2837, 0, 0, 0);
    set_sprite_pixel_color(2838, 0, 0, 0);
    set_sprite_pixel_color(2839, 0, 0, 0);
    set_sprite_pixel_color(2840, 0, 0, 0);
    set_sprite_pixel_color(2841, 0, 0, 0);
    set_sprite_pixel_color(2842, 7, 7, 0);
    set_sprite_pixel_color(2843, 7, 7, 0);
    set_sprite_pixel_color(2844, 7, 7, 0);
    set_sprite_pixel_color(2845, 7, 7, 0);
    set_sprite_pixel_color(2846, 7, 7, 0);
    set_sprite_pixel_color(2847, 7, 7, 0);
    set_sprite_pixel_color(2848, 7, 7, 0);
    set_sprite_pixel_color(2849, 7, 7, 0);
    set_sprite_pixel_color(2850, 7, 7, 0);
    set_sprite_pixel_color(2851, 7, 7, 0);
    set_sprite_pixel_color(2852, 7, 7, 0);
    set_sprite_pixel_color(2853, 7, 7, 0);
    set_sprite_pixel_color(2854, 7, 7, 0);
    set_sprite_pixel_color(2855, 7, 7, 0);
    set_sprite_pixel_color(2856, 7, 7, 7);
    set_sprite_pixel_color(2857, 0, 0, 0);
    set_sprite_pixel_color(2858, 0, 0, 0);
    set_sprite_pixel_color(2859, 0, 0, 0);
    set_sprite_pixel_color(2860, 0, 0, 0);
    set_sprite_pixel_color(2861, 7, 7, 0);
    set_sprite_pixel_color(2862, 7, 7, 0);
    set_sprite_pixel_color(2863, 7, 7, 0);
    set_sprite_pixel_color(2864, 7, 7, 0);
    set_sprite_pixel_color(2865, 7, 7, 0);
    set_sprite_pixel_color(2866, 7, 7, 0);
    set_sprite_pixel_color(2867, 7, 7, 0);
    set_sprite_pixel_color(2868, 7, 7, 0);
    set_sprite_pixel_color(2869, 7, 7, 0);
    set_sprite_pixel_color(2870, 7, 7, 0);
    set_sprite_pixel_color(2871, 7, 7, 0);
    set_sprite_pixel_color(2872, 7, 7, 0);
    set_sprite_pixel_color(2873, 7, 7, 0);
    set_sprite_pixel_color(2874, 7, 7, 0);
    set_sprite_pixel_color(2875, 7, 7, 0);
    set_sprite_pixel_color(2876, 7, 7, 0);
    set_sprite_pixel_color(2877, 0, 0, 0);
    set_sprite_pixel_color(2878, 0, 0, 0);
    set_sprite_pixel_color(2879, 0, 0, 0);
    set_sprite_pixel_color(2880, 0, 0, 0);
    set_sprite_pixel_color(2881, 0, 0, 0);
    set_sprite_pixel_color(2882, 7, 7, 0);
    set_sprite_pixel_color(2883, 7, 7, 0);
    set_sprite_pixel_color(2884, 7, 7, 0);
    set_sprite_pixel_color(2885, 7, 7, 0);
    set_sprite_pixel_color(2886, 7, 7, 0);
    set_sprite_pixel_color(2887, 7, 7, 0);
    set_sprite_pixel_color(2888, 7, 7, 0);
    set_sprite_pixel_color(2889, 7, 7, 0);
    set_sprite_pixel_color(2890, 7, 7, 0);
    set_sprite_pixel_color(2891, 7, 7, 0);
    set_sprite_pixel_color(2892, 7, 7, 0);
    set_sprite_pixel_color(2893, 7, 7, 0);
    set_sprite_pixel_color(2894, 7, 7, 0);
    set_sprite_pixel_color(2895, 7, 7, 0);
    set_sprite_pixel_color(2896, 7, 7, 0);
    set_sprite_pixel_color(2897, 7, 7, 0);
    set_sprite_pixel_color(2898, 0, 0, 0);
    set_sprite_pixel_color(2899, 0, 0, 0);
    set_sprite_pixel_color(2900, 0, 0, 0);
    set_sprite_pixel_color(2901, 0, 0, 0);
    set_sprite_pixel_color(2902, 0, 0, 0);
    set_sprite_pixel_color(2903, 0, 0, 0);
    set_sprite_pixel_color(2904, 7, 7, 0);
    set_sprite_pixel_color(2905, 7, 7, 0);
    set_sprite_pixel_color(2906, 7, 7, 0);
    set_sprite_pixel_color(2907, 0, 0, 0);
    set_sprite_pixel_color(2908, 0, 0, 0);
    set_sprite_pixel_color(2909, 0, 0, 0);
    set_sprite_pixel_color(2910, 7, 7, 0);
    set_sprite_pixel_color(2911, 7, 7, 0);
    set_sprite_pixel_color(2912, 7, 7, 0);
    set_sprite_pixel_color(2913, 7, 7, 0);
    set_sprite_pixel_color(2914, 7, 7, 0);
    set_sprite_pixel_color(2915, 7, 7, 0);
    set_sprite_pixel_color(2916, 7, 7, 0);
    set_sprite_pixel_color(2917, 7, 7, 0);
    set_sprite_pixel_color(2918, 7, 7, 0);
    set_sprite_pixel_color(2919, 0, 0, 0);
    set_sprite_pixel_color(2920, 0, 0, 0);
    set_sprite_pixel_color(2921, 0, 0, 0);
    set_sprite_pixel_color(2922, 0, 0, 0);
    set_sprite_pixel_color(2923, 0, 0, 0);
    set_sprite_pixel_color(2924, 0, 0, 0);
    set_sprite_pixel_color(2925, 7, 7, 0);
    set_sprite_pixel_color(2926, 7, 7, 0);
    set_sprite_pixel_color(2927, 7, 7, 0);
    set_sprite_pixel_color(2928, 0, 0, 0);
    set_sprite_pixel_color(2929, 0, 0, 0);
    set_sprite_pixel_color(2930, 7, 7, 0);
    set_sprite_pixel_color(2931, 7, 7, 0);
    set_sprite_pixel_color(2932, 7, 7, 0);
    set_sprite_pixel_color(2933, 7, 7, 0);
    set_sprite_pixel_color(2934, 7, 7, 0);
    set_sprite_pixel_color(2935, 7, 7, 0);
    set_sprite_pixel_color(2936, 7, 7, 0);
    set_sprite_pixel_color(2937, 7, 7, 0);
    set_sprite_pixel_color(2938, 7, 7, 0);
    set_sprite_pixel_color(2939, 0, 0, 0);
    set_sprite_pixel_color(2940, 0, 0, 0);
    set_sprite_pixel_color(2941, 0, 0, 0);
    set_sprite_pixel_color(2942, 0, 0, 0);
    set_sprite_pixel_color(2943, 0, 0, 0);
    set_sprite_pixel_color(2944, 0, 0, 0);
    set_sprite_pixel_color(2945, 0, 0, 0);
    set_sprite_pixel_color(2946, 7, 7, 0);
    set_sprite_pixel_color(2947, 7, 7, 0);
    set_sprite_pixel_color(2948, 7, 7, 0);
    set_sprite_pixel_color(2949, 7, 7, 0);
    set_sprite_pixel_color(2950, 7, 7, 0);
    set_sprite_pixel_color(2951, 7, 7, 0);
    set_sprite_pixel_color(2952, 7, 7, 0);
    set_sprite_pixel_color(2953, 7, 7, 0);
    set_sprite_pixel_color(2954, 7, 7, 0);
    set_sprite_pixel_color(2955, 7, 7, 0);
    set_sprite_pixel_color(2956, 7, 7, 0);
    set_sprite_pixel_color(2957, 7, 7, 0);
    set_sprite_pixel_color(2958, 7, 7, 0);
    set_sprite_pixel_color(2959, 7, 7, 0);
    set_sprite_pixel_color(2960, 0, 0, 0);
    set_sprite_pixel_color(2961, 0, 0, 0);
    set_sprite_pixel_color(2962, 0, 0, 0);
    set_sprite_pixel_color(2963, 0, 0, 0);
    set_sprite_pixel_color(2964, 0, 0, 0);
    set_sprite_pixel_color(2965, 0, 0, 0);
    set_sprite_pixel_color(2966, 0, 0, 0);
    set_sprite_pixel_color(2967, 7, 7, 0);
    set_sprite_pixel_color(2968, 7, 7, 0);
    set_sprite_pixel_color(2969, 7, 7, 0);
    set_sprite_pixel_color(2970, 7, 7, 0);
    set_sprite_pixel_color(2971, 7, 7, 0);
    set_sprite_pixel_color(2972, 7, 7, 0);
    set_sprite_pixel_color(2973, 7, 7, 0);
    set_sprite_pixel_color(2974, 7, 7, 0);
    set_sprite_pixel_color(2975, 7, 7, 0);
    set_sprite_pixel_color(2976, 7, 7, 0);
    set_sprite_pixel_color(2977, 7, 7, 0);
    set_sprite_pixel_color(2978, 7, 7, 0);
    set_sprite_pixel_color(2979, 7, 7, 0);
    set_sprite_pixel_color(2980, 0, 0, 0);
    set_sprite_pixel_color(2981, 0, 0, 0);
    set_sprite_pixel_color(2982, 0, 0, 0);
    set_sprite_pixel_color(2983, 0, 0, 0);
    set_sprite_pixel_color(2984, 0, 0, 0);
    set_sprite_pixel_color(2985, 0, 0, 0);
    set_sprite_pixel_color(2986, 0, 0, 0);
    set_sprite_pixel_color(2987, 0, 0, 0);
    set_sprite_pixel_color(2988, 0, 0, 0);
    set_sprite_pixel_color(2989, 7, 7, 0);
    set_sprite_pixel_color(2990, 7, 7, 0);
    set_sprite_pixel_color(2991, 7, 7, 0);
    set_sprite_pixel_color(2992, 7, 7, 0);
    set_sprite_pixel_color(2993, 7, 7, 0);
    set_sprite_pixel_color(2994, 7, 7, 0);
    set_sprite_pixel_color(2995, 7, 7, 0);
    set_sprite_pixel_color(2996, 7, 7, 0);
    set_sprite_pixel_color(2997, 7, 7, 0);
    set_sprite_pixel_color(2998, 7, 7, 0);
    set_sprite_pixel_color(2999, 7, 7, 0);
    set_sprite_pixel_color(3000, 0, 0, 0);
    set_sprite_pixel_color(3001, 0, 0, 0);
    set_sprite_pixel_color(3002, 0, 0, 0);
    set_sprite_pixel_color(3003, 0, 0, 0);
    set_sprite_pixel_color(3004, 0, 0, 0);
    set_sprite_pixel_color(3005, 0, 0, 0);
    set_sprite_pixel_color(3006, 0, 0, 0);
    set_sprite_pixel_color(3007, 0, 0, 0);
    set_sprite_pixel_color(3008, 7, 7, 0);
    set_sprite_pixel_color(3009, 7, 7, 0);
    set_sprite_pixel_color(3010, 7, 7, 0);
    set_sprite_pixel_color(3011, 7, 7, 0);
    set_sprite_pixel_color(3012, 7, 7, 0);
    set_sprite_pixel_color(3013, 7, 7, 0);
    set_sprite_pixel_color(3014, 7, 7, 0);
    set_sprite_pixel_color(3015, 7, 7, 0);
    set_sprite_pixel_color(3016, 7, 7, 0);
    set_sprite_pixel_color(3017, 7, 7, 0);
    set_sprite_pixel_color(3018, 7, 7, 0);
    set_sprite_pixel_color(3019, 7, 7, 0);
    set_sprite_pixel_color(3020, 0, 0, 0);
    set_sprite_pixel_color(3021, 0, 0, 0);
    set_sprite_pixel_color(3022, 0, 0, 0);
    set_sprite_pixel_color(3023, 0, 0, 0);
    set_sprite_pixel_color(3024, 0, 0, 0);
    set_sprite_pixel_color(3025, 0, 0, 0);
    set_sprite_pixel_color(3026, 0, 0, 0);
    set_sprite_pixel_color(3027, 7, 7, 0);
    set_sprite_pixel_color(3028, 7, 7, 0);
    set_sprite_pixel_color(3029, 7, 7, 0);
    set_sprite_pixel_color(3030, 7, 7, 0);
    set_sprite_pixel_color(3031, 7, 7, 0);
    set_sprite_pixel_color(3032, 7, 7, 0);
    set_sprite_pixel_color(3033, 7, 7, 0);
    set_sprite_pixel_color(3034, 7, 7, 0);
    set_sprite_pixel_color(3035, 7, 7, 0);
    set_sprite_pixel_color(3036, 7, 7, 0);
    set_sprite_pixel_color(3037, 7, 7, 0);
    set_sprite_pixel_color(3038, 7, 7, 0);
    set_sprite_pixel_color(3039, 7, 7, 0);
    set_sprite_pixel_color(3040, 0, 0, 0);
    set_sprite_pixel_color(3041, 0, 0, 0);
    set_sprite_pixel_color(3042, 0, 0, 0);
    set_sprite_pixel_color(3043, 0, 0, 0);
    set_sprite_pixel_color(3044, 0, 0, 0);
    set_sprite_pixel_color(3045, 0, 0, 0);
    set_sprite_pixel_color(3046, 7, 7, 0);
    set_sprite_pixel_color(3047, 7, 7, 0);
    set_sprite_pixel_color(3048, 7, 7, 0);
    set_sprite_pixel_color(3049, 7, 7, 0);
    set_sprite_pixel_color(3050, 7, 7, 0);
    set_sprite_pixel_color(3051, 7, 7, 0);
    set_sprite_pixel_color(3052, 7, 7, 0);
    set_sprite_pixel_color(3053, 7, 7, 0);
    set_sprite_pixel_color(3054, 7, 7, 0);
    set_sprite_pixel_color(3055, 7, 7, 0);
    set_sprite_pixel_color(3056, 7, 7, 0);
    set_sprite_pixel_color(3057, 7, 7, 0);
    set_sprite_pixel_color(3058, 7, 7, 0);
    set_sprite_pixel_color(3059, 7, 7, 0);
    set_sprite_pixel_color(3060, 0, 0, 0);
    set_sprite_pixel_color(3061, 0, 0, 0);
    set_sprite_pixel_color(3062, 0, 0, 0);
    set_sprite_pixel_color(3063, 0, 0, 0);
    set_sprite_pixel_color(3064, 0, 0, 0);
    set_sprite_pixel_color(3065, 7, 7, 0);
    set_sprite_pixel_color(3066, 7, 7, 0);
    set_sprite_pixel_color(3067, 7, 7, 0);
    set_sprite_pixel_color(3068, 7, 7, 0);
    set_sprite_pixel_color(3069, 7, 7, 0);
    set_sprite_pixel_color(3070, 7, 7, 0);
    set_sprite_pixel_color(3071, 7, 7, 0);
    set_sprite_pixel_color(3072, 7, 7, 0);
    set_sprite_pixel_color(3073, 7, 7, 0);
    set_sprite_pixel_color(3074, 7, 7, 0);
    set_sprite_pixel_color(3075, 7, 7, 0);
    set_sprite_pixel_color(3076, 7, 7, 0);
    set_sprite_pixel_color(3077, 7, 7, 0);
    set_sprite_pixel_color(3078, 7, 7, 0);
    set_sprite_pixel_color(3079, 0, 0, 0);
    set_sprite_pixel_color(3080, 0, 0, 0);
    set_sprite_pixel_color(3081, 0, 0, 0);
    set_sprite_pixel_color(3082, 0, 0, 0);
    set_sprite_pixel_color(3083, 0, 0, 0);
    set_sprite_pixel_color(3084, 7, 7, 0);
    set_sprite_pixel_color(3085, 7, 7, 0);
    set_sprite_pixel_color(3086, 7, 7, 0);
    set_sprite_pixel_color(3087, 7, 7, 0);
    set_sprite_pixel_color(3088, 7, 7, 0);
    set_sprite_pixel_color(3089, 7, 7, 0);
    set_sprite_pixel_color(3090, 7, 7, 0);
    set_sprite_pixel_color(3091, 7, 7, 0);
    set_sprite_pixel_color(3092, 7, 7, 0);
    set_sprite_pixel_color(3093, 7, 7, 0);
    set_sprite_pixel_color(3094, 7, 7, 0);
    set_sprite_pixel_color(3095, 7, 7, 0);
    set_sprite_pixel_color(3096, 7, 7, 0);
    set_sprite_pixel_color(3097, 7, 7, 0);
    set_sprite_pixel_color(3098, 7, 7, 0);
    set_sprite_pixel_color(3099, 0, 0, 0);
    set_sprite_pixel_color(3100, 0, 0, 0);
    set_sprite_pixel_color(3101, 0, 0, 0);
    set_sprite_pixel_color(3102, 0, 0, 0);
    set_sprite_pixel_color(3103, 7, 7, 0);
    set_sprite_pixel_color(3104, 7, 7, 0);
    set_sprite_pixel_color(3105, 7, 7, 0);
    set_sprite_pixel_color(3106, 7, 7, 0);
    set_sprite_pixel_color(3107, 7, 7, 0);
    set_sprite_pixel_color(3108, 7, 7, 0);
    set_sprite_pixel_color(3109, 7, 7, 0);
    set_sprite_pixel_color(3110, 7, 7, 0);
    set_sprite_pixel_color(3111, 7, 7, 0);
    set_sprite_pixel_color(3112, 7, 7, 0);
    set_sprite_pixel_color(3113, 7, 7, 0);
    set_sprite_pixel_color(3114, 7, 7, 0);
    set_sprite_pixel_color(3115, 7, 7, 0);
    set_sprite_pixel_color(3116, 7, 7, 0);
    set_sprite_pixel_color(3117, 7, 7, 0);
    set_sprite_pixel_color(3118, 0, 0, 0);
    set_sprite_pixel_color(3119, 0, 0, 0);
    set_sprite_pixel_color(3120, 0, 0, 0);
    set_sprite_pixel_color(3121, 0, 0, 0);
    set_sprite_pixel_color(3122, 7, 7, 0);
    set_sprite_pixel_color(3123, 7, 7, 0);
    set_sprite_pixel_color(3124, 7, 7, 0);
    set_sprite_pixel_color(3125, 7, 7, 0);
    set_sprite_pixel_color(3126, 7, 7, 0);
    set_sprite_pixel_color(3127, 7, 7, 0);
    set_sprite_pixel_color(3128, 7, 7, 0);
    set_sprite_pixel_color(3129, 7, 7, 0);
    set_sprite_pixel_color(3130, 7, 7, 0);
    set_sprite_pixel_color(3131, 7, 7, 0);
    set_sprite_pixel_color(3132, 7, 7, 0);
    set_sprite_pixel_color(3133, 7, 7, 0);
    set_sprite_pixel_color(3134, 7, 7, 0);
    set_sprite_pixel_color(3135, 7, 7, 0);
    set_sprite_pixel_color(3136, 7, 7, 0);
    set_sprite_pixel_color(3137, 0, 0, 0);
    set_sprite_pixel_color(3138, 0, 0, 0);
    set_sprite_pixel_color(3139, 0, 0, 0);
    set_sprite_pixel_color(3140, 0, 0, 0);
    set_sprite_pixel_color(3141, 0, 0, 0);
    set_sprite_pixel_color(3142, 7, 7, 0);
    set_sprite_pixel_color(3143, 7, 7, 0);
    set_sprite_pixel_color(3144, 7, 7, 0);
    set_sprite_pixel_color(3145, 7, 7, 0);
    set_sprite_pixel_color(3146, 7, 7, 0);
    set_sprite_pixel_color(3147, 7, 7, 0);
    set_sprite_pixel_color(3148, 7, 7, 0);
    set_sprite_pixel_color(3149, 7, 7, 0);
    set_sprite_pixel_color(3150, 7, 7, 0);
    set_sprite_pixel_color(3151, 7, 7, 0);
    set_sprite_pixel_color(3152, 7, 7, 0);
    set_sprite_pixel_color(3153, 7, 7, 0);
    set_sprite_pixel_color(3154, 7, 7, 0);
    set_sprite_pixel_color(3155, 0, 0, 0);
    set_sprite_pixel_color(3156, 0, 0, 0);
    set_sprite_pixel_color(3157, 0, 0, 0);
    set_sprite_pixel_color(3158, 0, 0, 0);
    set_sprite_pixel_color(3159, 0, 0, 0);
    set_sprite_pixel_color(3160, 0, 0, 0);
    set_sprite_pixel_color(3161, 0, 0, 0);
    set_sprite_pixel_color(3162, 0, 0, 0);
    set_sprite_pixel_color(3163, 0, 0, 0);
    set_sprite_pixel_color(3164, 7, 7, 0);
    set_sprite_pixel_color(3165, 7, 7, 0);
    set_sprite_pixel_color(3166, 7, 7, 0);
    set_sprite_pixel_color(3167, 7, 7, 0);
    set_sprite_pixel_color(3168, 7, 7, 0);
    set_sprite_pixel_color(3169, 7, 7, 0);
    set_sprite_pixel_color(3170, 7, 7, 0);
    set_sprite_pixel_color(3171, 7, 7, 0);
    set_sprite_pixel_color(3172, 7, 7, 0);
    set_sprite_pixel_color(3173, 0, 0, 0);
    set_sprite_pixel_color(3174, 0, 0, 0);
    set_sprite_pixel_color(3175, 0, 0, 0);
    set_sprite_pixel_color(3176, 0, 0, 0);
    set_sprite_pixel_color(3177, 0, 0, 0);
    set_sprite_pixel_color(3178, 0, 0, 0);
    set_sprite_pixel_color(3179, 0, 0, 0);
    set_sprite_pixel_color(3180, 0, 0, 0);
    set_sprite_pixel_color(3181, 0, 0, 0);
    set_sprite_pixel_color(3182, 0, 0, 0);
    set_sprite_pixel_color(3183, 0, 0, 0);
    set_sprite_pixel_color(3184, 0, 0, 0);
    set_sprite_pixel_color(3185, 0, 0, 0);
    set_sprite_pixel_color(3186, 0, 0, 0);
    set_sprite_pixel_color(3187, 0, 0, 0);
    set_sprite_pixel_color(3188, 0, 0, 0);
    set_sprite_pixel_color(3189, 0, 0, 0);
    set_sprite_pixel_color(3190, 0, 0, 0);
    set_sprite_pixel_color(3191, 0, 0, 0);
    set_sprite_pixel_color(3192, 0, 0, 0);
    set_sprite_pixel_color(3193, 0, 0, 0);
    set_sprite_pixel_color(3194, 0, 0, 0);
    set_sprite_pixel_color(3195, 0, 0, 0);
    set_sprite_pixel_color(3196, 0, 0, 0);
    set_sprite_pixel_color(3197, 0, 0, 0);
    set_sprite_pixel_color(3198, 0, 0, 0);
    set_sprite_pixel_color(3199, 0, 0, 0);

    set_sprite_pixel_color(3200, 0, 0, 0); // sprite da bola
    set_sprite_pixel_color(3201, 0, 0, 0);
    set_sprite_pixel_color(3202, 0, 0, 0);
    set_sprite_pixel_color(3203, 0, 0, 0);
    set_sprite_pixel_color(3204, 0, 0, 0);
    set_sprite_pixel_color(3205, 0, 0, 0);
    set_sprite_pixel_color(3206, 0, 0, 0);
    set_sprite_pixel_color(3207, 0, 0, 0);
    set_sprite_pixel_color(3208, 0, 0, 0);
    set_sprite_pixel_color(3209, 7, 4, 0);
    set_sprite_pixel_color(3210, 7, 4, 0);
    set_sprite_pixel_color(3211, 7, 4, 0);
    set_sprite_pixel_color(3212, 7, 4, 0);
    set_sprite_pixel_color(3213, 7, 4, 0);
    set_sprite_pixel_color(3214, 7, 5, 3);
    set_sprite_pixel_color(3215, 0, 0, 0);
    set_sprite_pixel_color(3216, 0, 0, 0);
    set_sprite_pixel_color(3217, 0, 0, 0);
    set_sprite_pixel_color(3218, 0, 0, 0);
    set_sprite_pixel_color(3219, 0, 0, 0);
    set_sprite_pixel_color(3220, 0, 0, 0);
    set_sprite_pixel_color(3221, 0, 0, 0);
    set_sprite_pixel_color(3222, 0, 0, 0);
    set_sprite_pixel_color(3223, 0, 0, 0);
    set_sprite_pixel_color(3224, 0, 0, 0);
    set_sprite_pixel_color(3225, 0, 0, 0);
    set_sprite_pixel_color(3226, 0, 0, 0);
    set_sprite_pixel_color(3227, 7, 4, 0);
    set_sprite_pixel_color(3228, 7, 4, 0);
    set_sprite_pixel_color(3229, 7, 4, 0);
    set_sprite_pixel_color(3230, 7, 4, 0);
    set_sprite_pixel_color(3231, 7, 4, 0);
    set_sprite_pixel_color(3232, 7, 4, 0);
    set_sprite_pixel_color(3233, 7, 4, 0);
    set_sprite_pixel_color(3234, 7, 4, 0);
    set_sprite_pixel_color(3235, 7, 4, 0);
    set_sprite_pixel_color(3236, 7, 4, 0);
    set_sprite_pixel_color(3237, 0, 0, 0);
    set_sprite_pixel_color(3238, 0, 0, 0);
    set_sprite_pixel_color(3239, 0, 0, 0);
    set_sprite_pixel_color(3240, 0, 0, 0);
    set_sprite_pixel_color(3241, 0, 0, 0);
    set_sprite_pixel_color(3242, 0, 0, 0);
    set_sprite_pixel_color(3243, 0, 0, 0);
    set_sprite_pixel_color(3244, 0, 0, 0);
    set_sprite_pixel_color(3245, 7, 4, 0);
    set_sprite_pixel_color(3246, 7, 4, 0);
    set_sprite_pixel_color(3247, 7, 4, 0);
    set_sprite_pixel_color(3248, 7, 4, 0);
    set_sprite_pixel_color(3249, 7, 4, 0);
    set_sprite_pixel_color(3250, 7, 4, 0);
    set_sprite_pixel_color(3251, 7, 4, 0);
    set_sprite_pixel_color(3252, 7, 4, 0);
    set_sprite_pixel_color(3253, 7, 4, 0);
    set_sprite_pixel_color(3254, 7, 4, 0);
    set_sprite_pixel_color(3255, 7, 4, 0);
    set_sprite_pixel_color(3256, 7, 4, 0);
    set_sprite_pixel_color(3257, 7, 4, 0);
    set_sprite_pixel_color(3258, 7, 4, 0);
    set_sprite_pixel_color(3259, 0, 0, 0);
    set_sprite_pixel_color(3260, 0, 0, 0);
    set_sprite_pixel_color(3261, 0, 0, 0);
    set_sprite_pixel_color(3262, 0, 0, 0);
    set_sprite_pixel_color(3263, 0, 0, 0);
    set_sprite_pixel_color(3264, 7, 4, 0);
    set_sprite_pixel_color(3265, 7, 4, 0);
    set_sprite_pixel_color(3266, 7, 4, 0);
    set_sprite_pixel_color(3267, 7, 4, 0);
    set_sprite_pixel_color(3268, 7, 4, 0);
    set_sprite_pixel_color(3269, 7, 6, 0);
    set_sprite_pixel_color(3270, 7, 6, 0);
    set_sprite_pixel_color(3271, 7, 6, 0);
    set_sprite_pixel_color(3272, 7, 6, 0);
    set_sprite_pixel_color(3273, 7, 6, 0);
    set_sprite_pixel_color(3274, 7, 6, 0);
    set_sprite_pixel_color(3275, 7, 4, 0);
    set_sprite_pixel_color(3276, 7, 4, 0);
    set_sprite_pixel_color(3277, 7, 4, 0);
    set_sprite_pixel_color(3278, 7, 4, 0);
    set_sprite_pixel_color(3279, 7, 4, 0);
    set_sprite_pixel_color(3280, 0, 0, 0);
    set_sprite_pixel_color(3281, 0, 0, 0);
    set_sprite_pixel_color(3282, 0, 0, 0);
    set_sprite_pixel_color(3283, 7, 4, 0);
    set_sprite_pixel_color(3284, 7, 4, 0);
    set_sprite_pixel_color(3285, 7, 4, 0);
    set_sprite_pixel_color(3286, 7, 4, 0);
    set_sprite_pixel_color(3287, 7, 7, 3);
    set_sprite_pixel_color(3288, 7, 7, 3);
    set_sprite_pixel_color(3289, 7, 7, 5);
    set_sprite_pixel_color(3290, 7, 7, 3);
    set_sprite_pixel_color(3291, 7, 7, 3);
    set_sprite_pixel_color(3292, 7, 7, 3);
    set_sprite_pixel_color(3293, 7, 6, 0);
    set_sprite_pixel_color(3294, 7, 6, 0);
    set_sprite_pixel_color(3295, 7, 6, 0);
    set_sprite_pixel_color(3296, 7, 6, 0);
    set_sprite_pixel_color(3297, 7, 4, 0);
    set_sprite_pixel_color(3298, 7, 4, 0);
    set_sprite_pixel_color(3299, 7, 4, 0);
    set_sprite_pixel_color(3300, 0, 0, 0);
    set_sprite_pixel_color(3301, 0, 0, 0);
    set_sprite_pixel_color(3302, 7, 5, 3);
    set_sprite_pixel_color(3303, 7, 4, 0);
    set_sprite_pixel_color(3304, 7, 4, 0);
    set_sprite_pixel_color(3305, 7, 4, 0);
    set_sprite_pixel_color(3306, 7, 7, 3);
    set_sprite_pixel_color(3307, 7, 7, 3);
    set_sprite_pixel_color(3308, 7, 7, 5);
    set_sprite_pixel_color(3309, 7, 7, 5);
    set_sprite_pixel_color(3310, 7, 7, 5);
    set_sprite_pixel_color(3311, 7, 7, 5);
    set_sprite_pixel_color(3312, 7, 7, 5);
    set_sprite_pixel_color(3313, 7, 7, 3);
    set_sprite_pixel_color(3314, 7, 6, 0);
    set_sprite_pixel_color(3315, 7, 6, 0);
    set_sprite_pixel_color(3316, 7, 6, 0);
    set_sprite_pixel_color(3317, 7, 6, 0);
    set_sprite_pixel_color(3318, 7, 4, 0);
    set_sprite_pixel_color(3319, 7, 4, 0);
    set_sprite_pixel_color(3320, 0, 0, 0);
    set_sprite_pixel_color(3321, 0, 0, 0);
    set_sprite_pixel_color(3322, 7, 4, 0);
    set_sprite_pixel_color(3323, 7, 4, 0);
    set_sprite_pixel_color(3324, 7, 4, 0);
    set_sprite_pixel_color(3325, 7, 6, 0);
    set_sprite_pixel_color(3326, 7, 7, 3);
    set_sprite_pixel_color(3327, 7, 7, 5);
    set_sprite_pixel_color(3328, 7, 7, 6);
    set_sprite_pixel_color(3329, 7, 7, 6);
    set_sprite_pixel_color(3330, 7, 7, 6);
    set_sprite_pixel_color(3331, 7, 7, 5);
    set_sprite_pixel_color(3332, 7, 7, 5);
    set_sprite_pixel_color(3333, 7, 7, 3);
    set_sprite_pixel_color(3334, 7, 7, 3);
    set_sprite_pixel_color(3335, 7, 6, 0);
    set_sprite_pixel_color(3336, 7, 6, 0);
    set_sprite_pixel_color(3337, 7, 6, 0);
    set_sprite_pixel_color(3338, 7, 6, 0);
    set_sprite_pixel_color(3339, 7, 4, 0);
    set_sprite_pixel_color(3340, 0, 0, 0);
    set_sprite_pixel_color(3341, 7, 5, 3);
    set_sprite_pixel_color(3342, 7, 4, 0);
    set_sprite_pixel_color(3343, 7, 4, 0);
    set_sprite_pixel_color(3344, 7, 6, 0);
    set_sprite_pixel_color(3345, 7, 7, 3);
    set_sprite_pixel_color(3346, 7, 7, 5);
    set_sprite_pixel_color(3347, 7, 7, 5);
    set_sprite_pixel_color(3348, 7, 7, 6);
    set_sprite_pixel_color(3349, 7, 7, 7);
    set_sprite_pixel_color(3350, 7, 7, 7);
    set_sprite_pixel_color(3351, 7, 7, 7);
    set_sprite_pixel_color(3352, 7, 7, 6);
    set_sprite_pixel_color(3353, 7, 7, 5);
    set_sprite_pixel_color(3354, 7, 7, 3);
    set_sprite_pixel_color(3355, 7, 6, 0);
    set_sprite_pixel_color(3356, 7, 6, 0);
    set_sprite_pixel_color(3357, 7, 6, 0);
    set_sprite_pixel_color(3358, 7, 6, 0);
    set_sprite_pixel_color(3359, 7, 6, 0);
    set_sprite_pixel_color(3360, 0, 0, 0);
    set_sprite_pixel_color(3361, 7, 4, 0);
    set_sprite_pixel_color(3362, 7, 4, 0);
    set_sprite_pixel_color(3363, 7, 4, 0);
    set_sprite_pixel_color(3364, 7, 6, 0);
    set_sprite_pixel_color(3365, 7, 6, 2);
    set_sprite_pixel_color(3366, 7, 7, 5);
    set_sprite_pixel_color(3367, 7, 7, 5);
    set_sprite_pixel_color(3368, 7, 7, 6);
    set_sprite_pixel_color(3369, 7, 7, 7);
    set_sprite_pixel_color(3370, 7, 7, 7);
    set_sprite_pixel_color(3371, 7, 7, 7);
    set_sprite_pixel_color(3372, 7, 7, 6);
    set_sprite_pixel_color(3373, 7, 7, 5);
    set_sprite_pixel_color(3374, 7, 7, 5);
    set_sprite_pixel_color(3375, 7, 7, 3);
    set_sprite_pixel_color(3376, 7, 6, 0);
    set_sprite_pixel_color(3377, 7, 6, 0);
    set_sprite_pixel_color(3378, 7, 6, 0);
    set_sprite_pixel_color(3379, 7, 6, 0);
    set_sprite_pixel_color(3380, 0, 0, 0);
    set_sprite_pixel_color(3381, 7, 4, 0);
    set_sprite_pixel_color(3382, 7, 4, 0);
    set_sprite_pixel_color(3383, 7, 4, 0);
    set_sprite_pixel_color(3384, 7, 6, 0);
    set_sprite_pixel_color(3385, 7, 7, 3);
    set_sprite_pixel_color(3386, 7, 7, 5);
    set_sprite_pixel_color(3387, 7, 7, 5);
    set_sprite_pixel_color(3388, 7, 7, 6);
    set_sprite_pixel_color(3389, 7, 7, 6);
    set_sprite_pixel_color(3390, 7, 7, 7);
    set_sprite_pixel_color(3391, 7, 7, 6);
    set_sprite_pixel_color(3392, 7, 7, 6);
    set_sprite_pixel_color(3393, 7, 7, 6);
    set_sprite_pixel_color(3394, 7, 7, 5);
    set_sprite_pixel_color(3395, 7, 7, 3);
    set_sprite_pixel_color(3396, 7, 6, 2);
    set_sprite_pixel_color(3397, 7, 6, 0);
    set_sprite_pixel_color(3398, 7, 6, 0);
    set_sprite_pixel_color(3399, 7, 6, 0);
    set_sprite_pixel_color(3400, 0, 0, 0);
    set_sprite_pixel_color(3401, 7, 4, 0);
    set_sprite_pixel_color(3402, 7, 4, 0);
    set_sprite_pixel_color(3403, 7, 6, 0);
    set_sprite_pixel_color(3404, 7, 6, 0);
    set_sprite_pixel_color(3405, 7, 7, 3);
    set_sprite_pixel_color(3406, 7, 7, 3);
    set_sprite_pixel_color(3407, 7, 7, 5);
    set_sprite_pixel_color(3408, 7, 7, 6);
    set_sprite_pixel_color(3409, 7, 7, 6);
    set_sprite_pixel_color(3410, 7, 7, 6);
    set_sprite_pixel_color(3411, 7, 7, 7);
    set_sprite_pixel_color(3412, 7, 7, 6);
    set_sprite_pixel_color(3413, 7, 7, 6);
    set_sprite_pixel_color(3414, 7, 7, 5);
    set_sprite_pixel_color(3415, 7, 7, 3);
    set_sprite_pixel_color(3416, 7, 6, 2);
    set_sprite_pixel_color(3417, 7, 6, 2);
    set_sprite_pixel_color(3418, 7, 6, 0);
    set_sprite_pixel_color(3419, 7, 6, 0);
    set_sprite_pixel_color(3420, 0, 0, 0);
    set_sprite_pixel_color(3421, 7, 4, 0);
    set_sprite_pixel_color(3422, 7, 4, 0);
    set_sprite_pixel_color(3423, 7, 6, 0);
    set_sprite_pixel_color(3424, 7, 6, 0);
    set_sprite_pixel_color(3425, 7, 6, 0);
    set_sprite_pixel_color(3426, 7, 7, 3);
    set_sprite_pixel_color(3427, 7, 7, 5);
    set_sprite_pixel_color(3428, 7, 7, 5);
    set_sprite_pixel_color(3429, 7, 7, 6);
    set_sprite_pixel_color(3430, 7, 7, 6);
    set_sprite_pixel_color(3431, 7, 7, 6);
    set_sprite_pixel_color(3432, 7, 7, 6);
    set_sprite_pixel_color(3433, 7, 7, 6);
    set_sprite_pixel_color(3434, 7, 7, 6);
    set_sprite_pixel_color(3435, 7, 7, 3);
    set_sprite_pixel_color(3436, 7, 7, 3);
    set_sprite_pixel_color(3437, 7, 7, 3);
    set_sprite_pixel_color(3438, 7, 6, 2);
    set_sprite_pixel_color(3439, 7, 6, 0);
    set_sprite_pixel_color(3440, 0, 0, 0);
    set_sprite_pixel_color(3441, 7, 4, 0);
    set_sprite_pixel_color(3442, 7, 4, 0);
    set_sprite_pixel_color(3443, 7, 4, 0);
    set_sprite_pixel_color(3444, 7, 6, 0);
    set_sprite_pixel_color(3445, 7, 6, 0);
    set_sprite_pixel_color(3446, 7, 6, 2);
    set_sprite_pixel_color(3447, 7, 7, 5);
    set_sprite_pixel_color(3448, 7, 7, 5);
    set_sprite_pixel_color(3449, 7, 7, 5);
    set_sprite_pixel_color(3450, 7, 7, 6);
    set_sprite_pixel_color(3451, 7, 7, 6);
    set_sprite_pixel_color(3452, 7, 7, 7);
    set_sprite_pixel_color(3453, 7, 7, 6);
    set_sprite_pixel_color(3454, 7, 7, 5);
    set_sprite_pixel_color(3455, 7, 7, 5);
    set_sprite_pixel_color(3456, 7, 7, 3);
    set_sprite_pixel_color(3457, 7, 7, 3);
    set_sprite_pixel_color(3458, 7, 6, 2);
    set_sprite_pixel_color(3459, 7, 6, 0);
    set_sprite_pixel_color(3460, 0, 0, 0);
    set_sprite_pixel_color(3461, 7, 4, 0);
    set_sprite_pixel_color(3462, 7, 4, 0);
    set_sprite_pixel_color(3463, 7, 4, 0);
    set_sprite_pixel_color(3464, 7, 6, 0);
    set_sprite_pixel_color(3465, 7, 6, 0);
    set_sprite_pixel_color(3466, 7, 6, 0);
    set_sprite_pixel_color(3467, 7, 6, 2);
    set_sprite_pixel_color(3468, 7, 7, 5);
    set_sprite_pixel_color(3469, 7, 7, 6);
    set_sprite_pixel_color(3470, 7, 7, 6);
    set_sprite_pixel_color(3471, 7, 7, 6);
    set_sprite_pixel_color(3472, 7, 7, 7);
    set_sprite_pixel_color(3473, 7, 7, 7);
    set_sprite_pixel_color(3474, 7, 7, 6);
    set_sprite_pixel_color(3475, 7, 7, 6);
    set_sprite_pixel_color(3476, 7, 7, 5);
    set_sprite_pixel_color(3477, 7, 7, 3);
    set_sprite_pixel_color(3478, 7, 6, 2);
    set_sprite_pixel_color(3479, 7, 6, 2);
    set_sprite_pixel_color(3480, 0, 0, 0);
    set_sprite_pixel_color(3481, 7, 4, 0);
    set_sprite_pixel_color(3482, 7, 4, 0);
    set_sprite_pixel_color(3483, 7, 4, 0);
    set_sprite_pixel_color(3484, 7, 6, 0);
    set_sprite_pixel_color(3485, 7, 6, 0);
    set_sprite_pixel_color(3486, 7, 6, 2);
    set_sprite_pixel_color(3487, 7, 6, 2);
    set_sprite_pixel_color(3488, 7, 7, 3);
    set_sprite_pixel_color(3489, 7, 7, 3);
    set_sprite_pixel_color(3490, 7, 7, 5);
    set_sprite_pixel_color(3491, 7, 7, 6);
    set_sprite_pixel_color(3492, 7, 7, 7);
    set_sprite_pixel_color(3493, 7, 7, 7);
    set_sprite_pixel_color(3494, 7, 7, 6);
    set_sprite_pixel_color(3495, 7, 7, 5);
    set_sprite_pixel_color(3496, 7, 7, 5);
    set_sprite_pixel_color(3497, 7, 7, 3);
    set_sprite_pixel_color(3498, 7, 7, 3);
    set_sprite_pixel_color(3499, 7, 6, 2);
    set_sprite_pixel_color(3500, 0, 0, 0);
    set_sprite_pixel_color(3501, 0, 0, 0);
    set_sprite_pixel_color(3502, 7, 4, 0);
    set_sprite_pixel_color(3503, 7, 4, 0);
    set_sprite_pixel_color(3504, 7, 4, 0);
    set_sprite_pixel_color(3505, 7, 6, 0);
    set_sprite_pixel_color(3506, 7, 6, 0);
    set_sprite_pixel_color(3507, 7, 6, 2);
    set_sprite_pixel_color(3508, 7, 7, 3);
    set_sprite_pixel_color(3509, 7, 7, 5);
    set_sprite_pixel_color(3510, 7, 7, 5);
    set_sprite_pixel_color(3511, 7, 7, 6);
    set_sprite_pixel_color(3512, 7, 7, 6);
    set_sprite_pixel_color(3513, 7, 7, 7);
    set_sprite_pixel_color(3514, 7, 7, 6);
    set_sprite_pixel_color(3515, 7, 7, 6);
    set_sprite_pixel_color(3516, 7, 7, 5);
    set_sprite_pixel_color(3517, 7, 7, 3);
    set_sprite_pixel_color(3518, 7, 6, 2);
    set_sprite_pixel_color(3519, 7, 4, 0);
    set_sprite_pixel_color(3520, 0, 0, 0);
    set_sprite_pixel_color(3521, 0, 0, 0);
    set_sprite_pixel_color(3522, 7, 4, 0);
    set_sprite_pixel_color(3523, 7, 4, 0);
    set_sprite_pixel_color(3524, 7, 4, 0);
    set_sprite_pixel_color(3525, 7, 6, 0);
    set_sprite_pixel_color(3526, 7, 6, 2);
    set_sprite_pixel_color(3527, 7, 6, 2);
    set_sprite_pixel_color(3528, 7, 6, 2);
    set_sprite_pixel_color(3529, 7, 7, 3);
    set_sprite_pixel_color(3530, 7, 7, 5);
    set_sprite_pixel_color(3531, 7, 7, 5);
    set_sprite_pixel_color(3532, 7, 7, 6);
    set_sprite_pixel_color(3533, 7, 7, 6);
    set_sprite_pixel_color(3534, 7, 7, 6);
    set_sprite_pixel_color(3535, 7, 7, 5);
    set_sprite_pixel_color(3536, 7, 7, 3);
    set_sprite_pixel_color(3537, 7, 7, 3);
    set_sprite_pixel_color(3538, 7, 6, 2);
    set_sprite_pixel_color(3539, 7, 4, 0);
    set_sprite_pixel_color(3540, 0, 0, 0);
    set_sprite_pixel_color(3541, 0, 0, 0);
    set_sprite_pixel_color(3542, 0, 0, 0);
    set_sprite_pixel_color(3543, 7, 4, 0);
    set_sprite_pixel_color(3544, 7, 4, 0);
    set_sprite_pixel_color(3545, 7, 4, 0);
    set_sprite_pixel_color(3546, 7, 6, 0);
    set_sprite_pixel_color(3547, 7, 6, 2);
    set_sprite_pixel_color(3548, 7, 6, 2);
    set_sprite_pixel_color(3549, 7, 7, 3);
    set_sprite_pixel_color(3550, 7, 7, 3);
    set_sprite_pixel_color(3551, 7, 7, 5);
    set_sprite_pixel_color(3552, 7, 7, 5);
    set_sprite_pixel_color(3553, 7, 7, 5);
    set_sprite_pixel_color(3554, 7, 7, 5);
    set_sprite_pixel_color(3555, 7, 7, 5);
    set_sprite_pixel_color(3556, 7, 7, 3);
    set_sprite_pixel_color(3557, 7, 7, 3);
    set_sprite_pixel_color(3558, 7, 4, 0);
    set_sprite_pixel_color(3559, 7, 4, 0);
    set_sprite_pixel_color(3560, 0, 0, 0);
    set_sprite_pixel_color(3561, 0, 0, 0);
    set_sprite_pixel_color(3562, 0, 0, 0);
    set_sprite_pixel_color(3563, 7, 4, 0);
    set_sprite_pixel_color(3564, 7, 4, 0);
    set_sprite_pixel_color(3565, 7, 4, 0);
    set_sprite_pixel_color(3566, 7, 4, 0);
    set_sprite_pixel_color(3567, 7, 6, 0);
    set_sprite_pixel_color(3568, 7, 6, 2);
    set_sprite_pixel_color(3569, 7, 6, 2);
    set_sprite_pixel_color(3570, 7, 7, 3);
    set_sprite_pixel_color(3571, 7, 7, 3);
    set_sprite_pixel_color(3572, 7, 7, 3);
    set_sprite_pixel_color(3573, 7, 7, 3);
    set_sprite_pixel_color(3574, 7, 7, 3);
    set_sprite_pixel_color(3575, 7, 7, 3);
    set_sprite_pixel_color(3576, 7, 6, 2);
    set_sprite_pixel_color(3577, 7, 4, 0);
    set_sprite_pixel_color(3578, 7, 4, 0);
    set_sprite_pixel_color(3579, 7, 4, 0);
    set_sprite_pixel_color(3580, 0, 0, 0);
    set_sprite_pixel_color(3581, 0, 0, 0);
    set_sprite_pixel_color(3582, 0, 0, 0);
    set_sprite_pixel_color(3583, 0, 0, 0);
    set_sprite_pixel_color(3584, 7, 4, 0);
    set_sprite_pixel_color(3585, 7, 4, 0);
    set_sprite_pixel_color(3586, 7, 4, 0);
    set_sprite_pixel_color(3587, 7, 4, 0);
    set_sprite_pixel_color(3588, 7, 4, 0);
    set_sprite_pixel_color(3589, 7, 6, 0);
    set_sprite_pixel_color(3590, 7, 7, 3);
    set_sprite_pixel_color(3591, 7, 7, 3);
    set_sprite_pixel_color(3592, 7, 7, 3);
    set_sprite_pixel_color(3593, 7, 7, 3);
    set_sprite_pixel_color(3594, 7, 6, 0);
    set_sprite_pixel_color(3595, 7, 4, 0);
    set_sprite_pixel_color(3596, 7, 4, 0);
    set_sprite_pixel_color(3597, 7, 4, 0);
    set_sprite_pixel_color(3598, 7, 4, 0);
    set_sprite_pixel_color(3599, 7, 4, 0);
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
