#include "gpu_letters.h"
// Função para definir cada letra em uma submatriz 5x5

void definirLetraP(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1;matriz[0][3] = 1;       // Parte superior do "P"
    matriz[1][0] = 1;                 matriz[1][3] = 1;         // Lado direito do topo
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1;matriz[2][3] = 1;       // Centro do "P"
    matriz[3][0] = 1;                                         // Tronco
    matriz[4][0] = 1;                                         // Tronco
};

void definirLetraT(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // T superior
    matriz[1][0] = 0;matriz[1][1] = 0;matriz[1][2] = 1; // Tronco da letra T
    matriz[2][1] = 0;matriz[2][2] = 1; // Tronco da letra T
    matriz[3][0] = 0;matriz[3][1] = 0;matriz[3][2] = 1;matriz[3][4] = 0; // Tronco da letra T
    matriz[4][2] = 1;matriz[4][3] = 0; // Tronco da letra T
};

void definirLetraE(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // E superior
    matriz[1][0] = 1; matriz[1][4] = 0; // E lados
    matriz[2][0] = 1; matriz[2][1] = 1;matriz[2][2] = 1;matriz[2][3] = 1; // E meio
    matriz[3][0] = 1; matriz[3][4] = 0; // E lados
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // E inferior
};

void definirLetraR(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // R superior
    matriz[1][0] = 1; matriz[1][1] = 0;
    matriz[1][2] = 0; // R corpo
    matriz[1][3] = 0; matriz[1][4] = 1; // Vazio
    matriz[2][0] = 1; // R perna diagonal
    matriz[2][1] = 1; 
    matriz[2][2] = 1; 
    matriz[2][3] = 1; 
    matriz[2][4] = 0; 
    matriz[3][0] = 1; // R base
    matriz[3][1] = 0; 
    matriz[3][2] = 1; 
    matriz[3][3] = 1; 
    matriz[3][4] = 0; 
    matriz[4][0] = 1;
    matriz[4][3] = 1;
    matriz[4][4] = 1;
};

void definirLetraI(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // I superior
    matriz[1][2] = 1; // I tronco
    matriz[2][2] = 1; // Vazio
    matriz[3][2] = 1; // I tronco
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // I inferior
};

void definirLetraS(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // S superior
    matriz[1][0] = 1; matriz[1][1] = 0; matriz[1][2] = 0; matriz[1][3] = 0; matriz[1][4] = 0; // S meio
    matriz[2][0] = 0; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 0; matriz[2][4] = 0; // S meio
    matriz[3][0] = 0; matriz[3][1] = 0; matriz[3][2] = 0; matriz[3][3] = 1; matriz[3][4] = 1; // S inferior
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // S inferior
};

void definirDoisPontos(char matriz[5][5]) {
    // Ponto superior dos dois pontos
    matriz[1][2] = 1; // Linha 2, coluna 3 (índice começa em 0)

    // Ponto inferior dos dois pontos
    matriz[3][2] = 0; // Linha 4, coluna 3 (índice começa em 0)
    matriz[3][3] = 0; // Linha 4, coluna 3 (índice começa em 0)
    matriz[4][2] = 1; // Linha 4, coluna 3 (índice começa em 0)
    matriz[4][3] = 0; // Linha 4, coluna 3 (índice começa em 0)
};

//numeros
void definirNumero0(char matriz[5][5]) {
    // Define o número 0
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][0] = 1; matriz[1][1] = 0; matriz[1][4] = 1;
    matriz[2][0] = 1; matriz[2][4] = 1;
    matriz[3][0] = 1; matriz[3][2] = 0 ;matriz[3][3] = 0; matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
};

void definirNumero1(char matriz[5][5]) {
    // Define o número 1
    matriz[0][0] = 0;matriz[0][1] = 1;matriz[0][2] = 1;
    matriz[1][2] = 1;
    matriz[2][2] = 1;
    matriz[3][2] = 1;
    matriz[4][2] = 1;
};

void definirNumero2(char matriz[5][5]) {
    // Define o número 2
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][4] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][0] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
};

void definirNumero3(char matriz[5][5]) {
    // Define o número 3
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][4] = 1;
    matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
};

void definirNumero4(char matriz[5][5]) {
    // Define o número 4
    matriz[0][0] = 1; matriz[0][4] = 1;
    matriz[1][0] = 1; matriz[1][4] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][4] = 1;
};

void definirNumero5(char matriz[5][5]) {
    // Define o número 5
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][0] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
};

void definirNumero6(char matriz[5][5]) {
    // Define o número 6
    matriz[0][0] = 1;matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1;
    matriz[1][0] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1;
    matriz[3][0] = 1; matriz[3][3] = 1;
    matriz[4][0] = 1;matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;
};

void definirNumero7(char matriz[5][5]) {
    // Define o número 7
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][4] = 1;
    matriz[2][3] = 1;
    matriz[3][1] = 0;matriz[3][2] = 1;
    matriz[4][1] = 1;
};

void definirNumero8(char matriz[5][5]) {
    // Define o número 8
    matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1;
    matriz[1][0] = 1; matriz[1][4] = 1;
    matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1;
    matriz[3][0] = 1; matriz[3][4] = 1;
    matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;
};

void definirNumero9(char matriz[5][5]) {
    // Define o número 9
    matriz[0][0] = 1;matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1;matriz[0][4] = 1;
    matriz[1][0] = 1; matriz[1][4] = 1;
    matriz[2][0] = 1;matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][0] = 1;matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;matriz[4][4] = 1;
};
