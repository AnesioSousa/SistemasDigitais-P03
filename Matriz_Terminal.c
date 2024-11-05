#include <stdio.h>
#include "gpu_letters.c"
void imprimirMatriz(int linhas, int colunas, char matriz[linhas][colunas]);
/*
void definirLetraP(char matriz[5][5]);
void definirLetraT(char matriz[5][5]);
void definirLetraE(char matriz[5][5]);
void definirLetraR(char matriz[5][5]);
void definirLetraI(char matriz[5][5]);
void definirLetraS(char matriz[5][5]);
void definirDoisPontos(char matriz[5][5]);
void definirNumero0(char matriz[5][5]);
void definirNumero1(char matriz[5][5]);
void definirNumero2(char matriz[5][5]);
void definirNumero3(char matriz[5][5]);
void definirNumero4(char matriz[5][5]);
void definirNumero5(char matriz[5][5]);
void definirNumero6(char matriz[5][5]);
void definirNumero7(char matriz[5][5]);
void definirNumero8(char matriz[5][5]);
void definirNumero9(char matriz[5][5]);
*/
void mudarCor(int linhas, int colunas,char matriz[5][5],int cor);
void exibirPontuacao(int pontuacao,int linhas,int colunas,char matriz[linhas][colunas]);
void escrever_Borda(int linhas,int colunas,char matriz[linhas][colunas],int cor);
int main(){
    char matriz[5][36] = {0};
    char matriz_borda[6][37]= {0};
    char pts[5][26] = {0};
    char pontuacao[5][36] = {0};
    char letraT[5][5], letraE[5][5], letraR[5][5], letraI[5][5], letraS[5][5],letraP[5][5],doisP[5][5] = {0};
    char numero0[5][5],numero1[5][5],numero2[5][5],numero3[5][5],numero4[5][5],numero5[5][5],numero6[5][5],numero7[5][5],numero8[5][5],numero9[5][5] = {0};
    // Definindo as letras
    definirLetraT(letraT);
    //imprimirMatriz(5,5,letraT);
    //mudarCor(5,5,letraT,2);
    //imprimirMatriz(5,5,letraT);
    definirLetraE(letraE);
    definirLetraR(letraR);
    definirLetraI(letraI);
    definirLetraS(letraS);
    definirLetraP(letraP);
    definirDoisPontos(doisP);
    //Definido os numeros
    definirNumero0(numero0);
    definirNumero1(numero1);
    definirNumero2(numero2);
    definirNumero3(numero3);
    definirNumero4(numero4);
    definirNumero5(numero5);
    definirNumero6(numero6);
    definirNumero7(numero7);
    definirNumero8(numero8);
    definirNumero9(numero9);
    // Concatenando as letras na matriz
    /*Concatena TETRIS*/
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matriz[i][j] = letraT[i][j]; // T
            matriz[i][6 + j] = letraE[i][j]; // E
            matriz[i][12 + j] = letraT[i][j]; // T
            matriz[i][18 + j] = letraR[i][j]; // R
            matriz[i][24 + j] = letraI[i][j]; // I
            matriz[i][30 + j] = letraS[i][j]; // S
        }
    }
    
   /*Concatena PTS:*/
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            pts[i][j] = letraP[i][j]; 
            pts[i][6 + j] = letraT[i][j]; 
            pts[i][12 + j] = letraS[i][j]; 
            pts[i][18 + j] = doisP[i][j]; 
        }
    }
    
    // Imprime a matriz com "TETRIS"
    imprimirMatriz(5, 36, matriz);
    // Imprime a matriz com "PTS"
    //imprimirMatriz(5,26,pts);
    //Imprime algum numero:
    //imprimirMatriz(5,5,numero9);
    //Imprime pontuacao:
    //exibirPontuacao(456789,5,36,pontuacao);
    //imprimirMatriz(5,36,pontuacao);
    //Imprime "Borda de 'TETRIS':
    escrever_Borda(6,36,matriz_borda,2);
    imprimirMatriz(6,36,matriz_borda);
    return 0;
}
void imprimirMatriz(int linhas, int colunas, char matriz[linhas][colunas]) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] == 1) {
                printf("#");
            } else if (matriz[i][j]==2)
            {
                printf("2");
            }
             
             else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void escrever_Borda(int linhas,int colunas,char matriz[linhas][colunas],int cor){
    int i,j;
    for ( i = 0; i < linhas; i++)
    {
        for ( j = 0; j < colunas; j++)
        {
            if (i==(linhas-1) || j == 0 || j==(colunas-1))
            {
                matriz[i][j] = cor;
            }   
        }
    }
}

void mudarCor(int linhas, int colunas,char matriz[5][5],int cor){
    int i,j;
    for ( i = 0; i < linhas; i++)
    {
        for ( j = 0; j < colunas; j++)
        {
           if (matriz[i][j]==1)
           {
            matriz[i][j]=cor;
           }
           
        }
        
    }
    
}
void copiarMatriz(int linhas,int colunas,char matriz[linhas][colunas],char temp[linhas][colunas]){
    int i,j;
    for ( i = 0; i < linhas; i++)
    {
        for (j = 0;j < colunas; j++)
        {
            matriz[i][j] = temp[i][j];
        }
        
    }
    
}

void exibirPontuacao(int pontuacao,int linhas,int colunas,char matriz[linhas][colunas]){
    /* count number of digits */
    int i,j;
    int c = 0; /* digit position */
    int p = pontuacao;
    char digito1[5][5],digito2[5][5],digito3[5][5],digito4[5][5],digito5[5][5],digito6[5][5]= {0};
    while (p != 0)
    {
        p /= 10;
        c++;
    }

    int numberArray[c];

    c = 0;    
    p = pontuacao;

    /* extract each digit */
    while (p != 0)
    {
        numberArray[c] = p % 10;
        p /= 10;
        c++;
    }
    for ( i = 0; i < c; i++)
    {   
        char temp[5][5]= {0};
        printf("%d \n",numberArray[i]);
        switch (numberArray[i])
        {
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
        switch (i+1)
        {
        case 1:
            copiarMatriz(5,5,digito1,temp);
            imprimirMatriz(5,5,digito1);
            break;
        case 2:
            copiarMatriz(5,5,digito2,temp);
            imprimirMatriz(5,5,digito2);
            break;
        case 3:
            copiarMatriz(5,5,digito3,temp);
            imprimirMatriz(5,5,digito3);
            break;
        case 4:
            copiarMatriz(5,5,digito4,temp);
            imprimirMatriz(5,5,digito4);
            break;
        case 5:
            copiarMatriz(5,5,digito5,temp);
            imprimirMatriz(5,5,digito5);
            break;
        case 6:
            copiarMatriz(5,5,digito6,temp);
            imprimirMatriz(5,5,digito6);
            break;
        }

    }
    for ( i = 0; i < 5; i++)
    {
       for (j = 0; j < 5; j++)
       {
        matriz[i][j] = digito6[i][j];
        matriz[i][6 + j] = digito5[i][j];
        matriz[i][12 + j] = digito4[i][j];
        matriz[i][18 + j] = digito3[i][j];
        matriz[i][24 + j] = digito2[i][j];
        matriz[i][30 + j] = digito1[i][j];
       }
       
    }


}


/*
// Função para definir cada letra em uma submatriz 5x5

void definirLetraP(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1;matriz[0][3] = 1;       // Parte superior do "P"
    matriz[1][0] = 1;                 matriz[1][3] = 1;         // Lado direito do topo
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1;matriz[2][3] = 1;       // Centro do "P"
    matriz[3][0] = 1;                                         // Tronco
    matriz[4][0] = 1;                                         // Tronco
}

void definirLetraT(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // T superior
    matriz[1][0] = 0;matriz[1][1] = 0;matriz[1][2] = 1; // Tronco da letra T
    matriz[2][2] = 1; // Tronco da letra T
    matriz[3][1] = 0;matriz[3][2] = 1; // Tronco da letra T
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

void definirDoisPontos(char matriz[5][5]) {
    // Ponto superior dos dois pontos
    matriz[1][2] = 1; // Linha 2, coluna 3 (índice começa em 0)

    // Ponto inferior dos dois pontos
    matriz[3][2] = 0; // Linha 4, coluna 3 (índice começa em 0)
    matriz[3][3] = 0; // Linha 4, coluna 3 (índice começa em 0)
    matriz[4][2] = 1; // Linha 4, coluna 3 (índice começa em 0)
    matriz[4][3] = 0; // Linha 4, coluna 3 (índice começa em 0)
}
*/

/*
//numeros
void definirNumero0(char matriz[5][5]) {
    // Define o número 0
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][0] = 1; matriz[1][1] = 0; matriz[1][4] = 1;
    matriz[2][0] = 1; matriz[2][4] = 1;
    matriz[3][0] = 1; matriz[3][2] = 0 ;matriz[3][3] = 0; matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
}

void definirNumero1(char matriz[5][5]) {
    // Define o número 1
    matriz[0][0] = 0;matriz[0][1] = 1;matriz[0][2] = 1;
    matriz[1][2] = 1;
    matriz[2][2] = 1;
    matriz[3][2] = 1;
    matriz[4][2] = 1;
}

void definirNumero2(char matriz[5][5]) {
    // Define o número 2
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][4] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][0] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
}

void definirNumero3(char matriz[5][5]) {
    // Define o número 3
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][4] = 1;
    matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
}

void definirNumero4(char matriz[5][5]) {
    // Define o número 4
    matriz[0][0] = 1; matriz[0][4] = 1;
    matriz[1][0] = 1; matriz[1][4] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][4] = 1;
}

void definirNumero5(char matriz[5][5]) {
    // Define o número 5
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][0] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1;
}

void definirNumero6(char matriz[5][5]) {
    // Define o número 6
    matriz[0][0] = 1;matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1;
    matriz[1][0] = 1;
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1;
    matriz[3][0] = 1; matriz[3][3] = 1;
    matriz[4][0] = 1;matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;
}

void definirNumero7(char matriz[5][5]) {
    // Define o número 7
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1;
    matriz[1][4] = 1;
    matriz[2][3] = 1;
    matriz[3][1] = 0;matriz[3][2] = 1;
    matriz[4][1] = 1;
}

void definirNumero8(char matriz[5][5]) {
    // Define o número 8
    matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1;
    matriz[1][0] = 1; matriz[1][4] = 1;
    matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1;
    matriz[3][0] = 1; matriz[3][4] = 1;
    matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;
}

void definirNumero9(char matriz[5][5]) {
    // Define o número 9
    matriz[0][0] = 1;matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1;matriz[0][4] = 1;
    matriz[1][0] = 1; matriz[1][4] = 1;
    matriz[2][0] = 1;matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 1; matriz[2][4] = 1;
    matriz[3][4] = 1;
    matriz[4][0] = 1;matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;matriz[4][4] = 1;
}
*/