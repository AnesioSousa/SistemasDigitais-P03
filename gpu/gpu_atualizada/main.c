#include "gpu_lib.h"
#include <stdio.h>

int main() {
    iniciar_tela();
    limpar_tela();

    desenhar_sprite(0, 20, 20, 1, 1);
    //desenhar_tela_inicial();
    //desenhar_quadrado(0,0,0,7,0);    
    char shape[3][3] = {
        {0, 1, 1},
        {1, 1, 0},
        {0, 0, 0}
    };
    
 char matriz[60][80] = {0};

    char letraT[5][5], letraE[5][5], letraR[5][5], letraI[5][5], letraS[5][5];
    
    // Definindo as letras
    definirLetraT(letraT);
    definirLetraE(letraE);
    definirLetraR(letraR);
    definirLetraI(letraI);
    definirLetraS(letraS);
    
    // Concatenando as letras na matriz
    int i,j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            matriz[10 + i][30 + j] = letraT[i][j]; // T
            matriz[10 + i][36 + j] = letraE[i][j]; // E
            matriz[10 + i][42 + j] = letraT[i][j]; // T
            matriz[10 + i][48 + j] = letraR[i][j]; // R
            matriz[10 + i][54 + j] = letraI[i][j]; // I
            matriz[10 + i][60 + j] = letraS[i][j]; // S
        }
    }

    //ler_matriz(5,5,letraT,2);        
    //ler_matriz(5,5,letraE,2);
    //ler_matriz(5,5,letraI,2);
        for ( i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            matriz[10 + i][30 + j] = letraT[i][j]; // T
            matriz[10 + i][36 + j] = letraE[i][j]; // E
            matriz[10 + i][42 + j] = letraT[i][j]; // T
            matriz[10 + i][48 + j] = letraR[i][j]; // R
            matriz[10 + i][54 + j] = letraI[i][j]; // I
            matriz[10 + i][60 + j] = letraS[i][j]; // S
        }
    }
    ler_matriz(60,80,matriz,1);
    desmapear_tela();
    return 0;
}
    // Função para definir cada letra em uma submatriz 5x5
void definirLetraT(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // T superior
    matriz[1][2] = 1; // Tronco da letra T
    matriz[2][2] = 1; // Tronco da letra T
    matriz[3][2] = 1; // Tronco da letra T
    matriz[4][2] = 1; // Tronco da letra T
}

void definirLetraE(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // E superior
    matriz[1][0] = 1; matriz[1][4] = 0; // E lados
    matriz[2][0] = 1; matriz[2][1] = 1;matriz[2][2] = 1;matriz[2][3] = 1; // E meio
    matriz[3][0] = 1; matriz[3][4] = 0; // E lados
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // E inferior
}

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
}

void definirLetraI(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // I superior
    matriz[1][2] = 1; // I tronco
    matriz[2][2] = 1; // Vazio
    matriz[3][2] = 1; // I tronco
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // I inferior
}

void definirLetraS(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // S superior
    matriz[1][0] = 1; matriz[1][1] = 0; matriz[1][2] = 0; matriz[1][3] = 0; matriz[1][4] = 0; // S meio
    matriz[2][0] = 0; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 0; matriz[2][4] = 0; // S meio
    matriz[3][0] = 0; matriz[3][1] = 0; matriz[3][2] = 0; matriz[3][3] = 1; matriz[3][4] = 1; // S inferior
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // S inferior
}
