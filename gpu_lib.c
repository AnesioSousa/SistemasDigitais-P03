#include "gpu_lib.h"
#define LADO_QUADRADO 3
#define offsett_X 0
#define offsett_Y 0

/* Chamadas ao assembly */
extern void mem_map();
extern void set_background_block(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R);
extern void set_sprite(uint32_t reg, uint32_t x, uint32_t y, uint32_t offset, uint32_t sp);
extern void clear_background();
extern void mem_unmap();


/*             API                */
void desenhar_quadrado(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R, int tamanho_quadrado);
void escreverTetris(int corT,int corE,int corR,int corI,int corS,int posX, int posY,int tamanho);
void escrever_Pts(int corT,int corS,int corP,int corDoisP,int posX, int posY,int tamanho);
void exibirPontuacao(int pontuacao,int linhas,int colunas,char matriz[linhas][colunas]);
void escrever_Borda(int linhas,int colunas,char matriz[linhas][colunas],int cor);
void copiarMatriz(int linhas,int colunas,char matriz[linhas][colunas],char temp[linhas][colunas]);
void mudarCor(int linhas, int colunas,char matriz[5][5],int cor);
void ler_matriz(int Max_linhas, int Max_colunas,char Matriz[Max_linhas][Max_colunas],int espacamento,int off_X,int off_Y, int tamanho_quadrado);


void mapear_gpu() {
    mem_map();
}

void desmapear_gpu() {
    mem_unmap();
}

void desenhar_sprite(uint32_t reg, uint32_t x, uint32_t y, uint32_t offset, uint32_t sp){
  set_sprite(reg, x, y, offset, sp);
}

void desenhar_quadrado(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R, int tamanho_quadrado){
int i,j;    
for ( i = 0; i < tamanho_quadrado; i++)
    {
        for (j = 0; j < tamanho_quadrado; j++)
        {
            set_background_block(line+i, col+j, B, G, R);
        }
        
    }
    
}

//espaçamento deve ser no minimo do tamanho do quadrado para evitar conflitos
//planejo usar o lado do quadrado como parametro apos testes
//os defines: lado quadrado e offsett X e Y podem ser parametros dessa função(obs:espacamento deve ser >= tamando do bloco)
//caso a matriz ultrapasse o tamanho da placa podem ser usados offsetts ou mudar a relação lado_quadrado x espaçamento para uma proporção de 1x1 (de preferencia menor que uma matriz 80x60)
void ler_matriz(int Max_linhas, int Max_colunas,char Matriz[Max_linhas][Max_colunas],int espacamento,int off_X,int off_Y, int tamanho_quadrado){
int i,j;   
 for (i = 0; i < Max_linhas; i++)
        for (j = 0; j < Max_colunas; j++)
        {
            switch (Matriz[i][j])
            {
            case 1:
            //case "#":
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,0,0,7,tamanho_quadrado);
                break;
            case 0:
            //case " ":
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,0,0,0,tamanho_quadrado);
                break;
            case 2:
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,0,7,0,tamanho_quadrado);
                break;
            case 3:
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,7,0,0,tamanho_quadrado);
                break;
            case 4:
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,7,7,0,tamanho_quadrado);
                break;
            case 5:
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,0,7,7,tamanho_quadrado);
                break;
            case 6:
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,7,0,7,tamanho_quadrado);
                break;
            case 7:
                desenhar_quadrado((i*espacamento)+off_Y,(j*espacamento)+off_X,7,7,7,tamanho_quadrado);
                break;
            
            }
    }
}

void escreverTetris(int corT,int corE,int corR,int corI,int corS,int posX, int posY,int tamanho){
    char tetris[5][36] = {0};
    char letraT[5][5], letraE[5][5], letraR[5][5], letraI[5][5], letraS[5][5];
    int i,j;
    // Definindo as letras
    definirLetraT(letraT);
    mudarCor(5,5,letraT,corT);

    definirLetraE(letraE);
    mudarCor(5,5,letraE,corE);

    definirLetraR(letraR);
    mudarCor(5,5,letraR,corR);

    definirLetraI(letraI);
    mudarCor(5,5,letraI,corI);

    definirLetraS(letraS);
    mudarCor(5,5,letraS,corS);

    // Concatenando as letras na matriz
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            tetris[i][j] = letraT[i][j]; // T
            tetris[i][6 + j] = letraE[i][j]; // E
            tetris[i][12 + j] = letraT[i][j]; // T
            tetris[i][18 + j] = letraR[i][j]; // R
            tetris[i][24 + j] = letraI[i][j]; // I
            tetris[i][30 + j] = letraS[i][j]; // S
        }
    }
    //testar com off_X = 35 e off_Y = 25
    //e com tamanho = 1 ou 2
    //espacamento pode variar entre 1 e 3
    ler_matriz(5,36,tetris,2,posX,posY,tamanho);
}

void escrever_Pts(int corT,int corS,int corP,int corDoisP,int posX, int posY,int tamanho){
    char pts[5][26] = {0};
    char letraT[5][5],letraS[5][5],letraP[5][5],doisP[5][5];
    int i,j;

    definirLetraT(letraT);
    mudarCor(5,5,letraT,corT);
    
    definirLetraS(letraS);
    mudarCor(5,5,letraS,corS);
    
    definirLetraP(letraP);
    mudarCor(5,5,letraP,corP);
    
    definirDoisPontos(doisP);
    mudarCor(5,5,doisP,corDoisP);
    
    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
            pts[i][j] = letraP[i][j]; 
            pts[i][6 + j] = letraT[i][j]; 
            pts[i][12 + j] = letraS[i][j]; 
            pts[i][18 + j] = doisP[i][j]; 
        }
    }
    //testar com off_X = 35 e off_Y = 25
    //e com tamanho = 1
    ler_matriz(5,26,pts,1,posX,posY,tamanho);
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
            break;
        case 2:
            copiarMatriz(5,5,digito2,temp);
            break;
        case 3:
            copiarMatriz(5,5,digito3,temp);
            break;
        case 4:
            copiarMatriz(5,5,digito4,temp);
            break;
        case 5:
            copiarMatriz(5,5,digito5,temp);
            break;
        case 6:
            copiarMatriz(5,5,digito6,temp);
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
    ler_matriz(linhas,colunas,matriz,1,30,54,1);
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
