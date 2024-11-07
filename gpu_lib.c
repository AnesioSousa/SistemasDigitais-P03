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
/*
void escrever_gameover(int posX, int posY,int tamanho){
    char pts[5][26] = {{0}};
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
*/
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
    
    if(p!=0){
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
    }else{
      for ( i = 0; i < 6; i++)
      {   
        char temp[5][5]= {0};
        definirNumero0(temp);
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


/**
 * \brief           Desenha a tela de FIM DE JOGO do jogo
 */
void draw_game_over_screen() {
    set_background_block(23, 18, 7, 0, 0);
    set_background_block(24, 18, 7, 0, 0);
    set_background_block(25, 18, 7, 0, 0);
    set_background_block(26, 18, 7, 0, 0);
    set_background_block(33, 18, 7, 0, 0);
    set_background_block(34, 18, 7, 0, 0);
    set_background_block(35, 18, 7, 0, 0);
    set_background_block(36, 18, 7, 0, 0);
    set_background_block(41, 18, 7, 0, 0);
    set_background_block(42, 18, 7, 0, 0);
    set_background_block(49, 18, 7, 0, 0);
    set_background_block(50, 18, 7, 0, 0);
    set_background_block(53, 18, 7, 0, 0);
    set_background_block(54, 18, 7, 0, 0);
    set_background_block(55, 18, 7, 0, 0);
    set_background_block(56, 18, 7, 0, 0);
    set_background_block(57, 18, 7, 0, 0);
    set_background_block(58, 18, 7, 0, 0);
    set_background_block(59, 18, 7, 0, 0);
    set_background_block(60, 18, 7, 0, 0);
    set_background_block(23, 19, 7, 0, 0);
    set_background_block(24, 19, 7, 0, 0);
    set_background_block(25, 19, 7, 0, 0);
    set_background_block(26, 19, 7, 0, 0);
    set_background_block(33, 19, 7, 0, 0);
    set_background_block(34, 19, 7, 0, 0);
    set_background_block(35, 19, 7, 0, 0);
    set_background_block(36, 19, 7, 0, 0);
    set_background_block(41, 19, 7, 0, 0);
    set_background_block(42, 19, 7, 0, 0);
    set_background_block(49, 19, 7, 0, 0);
    set_background_block(50, 19, 7, 0, 0);
    set_background_block(53, 19, 7, 0, 0);
    set_background_block(54, 19, 7, 0, 0);
    set_background_block(55, 19, 7, 0, 0);
    set_background_block(56, 19, 7, 0, 0);
    set_background_block(57, 19, 7, 0, 0);
    set_background_block(58, 19, 7, 0, 0);
    set_background_block(59, 19, 7, 0, 0);
    set_background_block(60, 19, 7, 0, 0);
    set_background_block(21, 20, 7, 0, 0);
    set_background_block(22, 20, 7, 0, 0);
    set_background_block(31, 20, 7, 0, 0);
    set_background_block(32, 20, 7, 0, 0);
    set_background_block(37, 20, 7, 0, 0);
    set_background_block(38, 20, 7, 0, 0);
    set_background_block(41, 20, 7, 0, 0);
    set_background_block(42, 20, 7, 0, 0);
    set_background_block(43, 20, 7, 0, 0);
    set_background_block(44, 20, 7, 0, 0);
    set_background_block(47, 20, 7, 0, 0);
    set_background_block(48, 20, 7, 0, 0);
    set_background_block(49, 20, 7, 0, 0);
    set_background_block(50, 20, 7, 0, 0);
    set_background_block(53, 20, 7, 0, 0);
    set_background_block(54, 20, 7, 0, 0);
    set_background_block(21, 21, 7, 0, 0);
    set_background_block(22, 21, 7, 0, 0);
    set_background_block(31, 21, 7, 0, 0);
    set_background_block(32, 21, 7, 0, 0);
    set_background_block(37, 21, 7, 0, 0);
    set_background_block(38, 21, 7, 0, 0);
    set_background_block(41, 21, 7, 0, 0);
    set_background_block(42, 21, 7, 0, 0);
    set_background_block(43, 21, 7, 0, 0);
    set_background_block(44, 21, 7, 0, 0);
    set_background_block(47, 21, 7, 0, 0);
    set_background_block(48, 21, 7, 0, 0);
    set_background_block(49, 21, 7, 0, 0);
    set_background_block(50, 21, 7, 0, 0);
    set_background_block(53, 21, 7, 0, 0);
    set_background_block(54, 21, 7, 0, 0);
    set_background_block(21, 22, 7, 0, 0);
    set_background_block(22, 22, 7, 0, 0);
    set_background_block(25, 22, 7, 0, 0);
    set_background_block(26, 22, 7, 0, 0);
    set_background_block(27, 22, 7, 0, 0);
    set_background_block(28, 22, 7, 0, 0);
    set_background_block(31, 22, 7, 0, 0);
    set_background_block(32, 22, 7, 0, 0);
    set_background_block(33, 22, 7, 0, 0);
    set_background_block(34, 22, 7, 0, 0);
    set_background_block(35, 22, 7, 0, 0);
    set_background_block(36, 22, 7, 0, 0);
    set_background_block(37, 22, 7, 0, 0);
    set_background_block(38, 22, 7, 0, 0);
    set_background_block(41, 22, 7, 0, 0);
    set_background_block(42, 22, 7, 0, 0);
    set_background_block(45, 22, 7, 0, 0);
    set_background_block(46, 22, 7, 0, 0);
    set_background_block(49, 22, 7, 0, 0);
    set_background_block(50, 22, 7, 0, 0);
    set_background_block(53, 22, 7, 0, 0);
    set_background_block(54, 22, 7, 0, 0);
    set_background_block(55, 22, 7, 0, 0);
    set_background_block(56, 22, 7, 0, 0);
    set_background_block(57, 22, 7, 0, 0);
    set_background_block(58, 22, 7, 0, 0);
    set_background_block(21, 23, 7, 0, 0);
    set_background_block(22, 23, 7, 0, 0);
    set_background_block(25, 23, 7, 0, 0);
    set_background_block(26, 23, 7, 0, 0);
    set_background_block(27, 23, 7, 0, 0);
    set_background_block(28, 23, 7, 0, 0);
    set_background_block(31, 23, 7, 0, 0);
    set_background_block(32, 23, 7, 0, 0);
    set_background_block(33, 23, 7, 0, 0);
    set_background_block(34, 23, 7, 0, 0);
    set_background_block(35, 23, 7, 0, 0);
    set_background_block(36, 23, 7, 0, 0);
    set_background_block(37, 23, 7, 0, 0);
    set_background_block(38, 23, 7, 0, 0);
    set_background_block(41, 23, 7, 0, 0);
    set_background_block(42, 23, 7, 0, 0);
    set_background_block(45, 23, 7, 0, 0);
    set_background_block(46, 23, 7, 0, 0);
    set_background_block(49, 23, 7, 0, 0);
    set_background_block(50, 23, 7, 0, 0);
    set_background_block(53, 23, 7, 0, 0);
    set_background_block(54, 23, 7, 0, 0);
    set_background_block(55, 23, 7, 0, 0);
    set_background_block(56, 23, 7, 0, 0);
    set_background_block(57, 23, 7, 0, 0);
    set_background_block(58, 23, 7, 0, 0);
    set_background_block(21, 24, 7, 0, 0);
    set_background_block(22, 24, 7, 0, 0);
    set_background_block(27, 24, 7, 0, 0);
    set_background_block(28, 24, 7, 0, 0);
    set_background_block(31, 24, 7, 0, 0);
    set_background_block(32, 24, 7, 0, 0);
    set_background_block(37, 24, 7, 0, 0);
    set_background_block(38, 24, 7, 0, 0);
    set_background_block(41, 24, 7, 0, 0);
    set_background_block(42, 24, 7, 0, 0);
    set_background_block(49, 24, 7, 0, 0);
    set_background_block(50, 24, 7, 0, 0);
    set_background_block(53, 24, 7, 0, 0);
    set_background_block(54, 24, 7, 0, 0);
    set_background_block(21, 25, 7, 0, 0);
    set_background_block(22, 25, 7, 0, 0);
    set_background_block(27, 25, 7, 0, 0);
    set_background_block(28, 25, 7, 0, 0);
    set_background_block(31, 25, 7, 0, 0);
    set_background_block(32, 25, 7, 0, 0);
    set_background_block(37, 25, 7, 0, 0);
    set_background_block(38, 25, 7, 0, 0);
    set_background_block(41, 25, 7, 0, 0);
    set_background_block(42, 25, 7, 0, 0);
    set_background_block(49, 25, 7, 0, 0);
    set_background_block(50, 25, 7, 0, 0);
    set_background_block(53, 25, 7, 0, 0);
    set_background_block(54, 25, 7, 0, 0);
    set_background_block(23, 26, 7, 0, 0);
    set_background_block(24, 26, 7, 0, 0);
    set_background_block(25, 26, 7, 0, 0);
    set_background_block(26, 26, 7, 0, 0);
    set_background_block(31, 26, 7, 0, 0);
    set_background_block(32, 26, 7, 0, 0);
    set_background_block(37, 26, 7, 0, 0);
    set_background_block(38, 26, 7, 0, 0);
    set_background_block(41, 26, 7, 0, 0);
    set_background_block(42, 26, 7, 0, 0);
    set_background_block(49, 26, 7, 0, 0);
    set_background_block(50, 26, 7, 0, 0);
    set_background_block(53, 26, 7, 0, 0);
    set_background_block(54, 26, 7, 0, 0);
    set_background_block(55, 26, 7, 0, 0);
    set_background_block(56, 26, 7, 0, 0);
    set_background_block(57, 26, 7, 0, 0);
    set_background_block(58, 26, 7, 0, 0);
    set_background_block(59, 26, 7, 0, 0);
    set_background_block(60, 26, 7, 0, 0);
    set_background_block(23, 27, 7, 0, 0);
    set_background_block(24, 27, 7, 0, 0);
    set_background_block(25, 27, 7, 0, 0);
    set_background_block(26, 27, 7, 0, 0);
    set_background_block(31, 27, 7, 0, 0);
    set_background_block(32, 27, 7, 0, 0);
    set_background_block(37, 27, 7, 0, 0);
    set_background_block(38, 27, 7, 0, 0);
    set_background_block(41, 27, 7, 0, 0);
    set_background_block(42, 27, 7, 0, 0);
    set_background_block(49, 27, 7, 0, 0);
    set_background_block(50, 27, 7, 0, 0);
    set_background_block(53, 27, 7, 0, 0);
    set_background_block(54, 27, 7, 0, 0);
    set_background_block(55, 27, 7, 0, 0);
    set_background_block(56, 27, 7, 0, 0);
    set_background_block(57, 27, 7, 0, 0);
    set_background_block(58, 27, 7, 0, 0);
    set_background_block(59, 27, 7, 0, 0);
    set_background_block(60, 27, 7, 0, 0);
    set_background_block(23, 30, 7, 0, 0);
    set_background_block(24, 30, 7, 0, 0);
    set_background_block(25, 30, 7, 0, 0);
    set_background_block(26, 30, 7, 0, 0);
    set_background_block(31, 30, 7, 0, 0);
    set_background_block(32, 30, 7, 0, 0);
    set_background_block(39, 30, 7, 0, 0);
    set_background_block(40, 30, 7, 0, 0);
    set_background_block(43, 30, 7, 0, 0);
    set_background_block(44, 30, 7, 0, 0);
    set_background_block(45, 30, 7, 0, 0);
    set_background_block(46, 30, 7, 0, 0);
    set_background_block(47, 30, 7, 0, 0);
    set_background_block(48, 30, 7, 0, 0);
    set_background_block(49, 30, 7, 0, 0);
    set_background_block(50, 30, 7, 0, 0);
    set_background_block(53, 30, 7, 0, 0);
    set_background_block(54, 30, 7, 0, 0);
    set_background_block(55, 30, 7, 0, 0);
    set_background_block(56, 30, 7, 0, 0);
    set_background_block(57, 30, 7, 0, 0);
    set_background_block(58, 30, 7, 0, 0);
    set_background_block(23, 31, 7, 0, 0);
    set_background_block(24, 31, 7, 0, 0);
    set_background_block(25, 31, 7, 0, 0);
    set_background_block(26, 31, 7, 0, 0);
    set_background_block(31, 31, 7, 0, 0);
    set_background_block(32, 31, 7, 0, 0);
    set_background_block(39, 31, 7, 0, 0);
    set_background_block(40, 31, 7, 0, 0);
    set_background_block(43, 31, 7, 0, 0);
    set_background_block(44, 31, 7, 0, 0);
    set_background_block(45, 31, 7, 0, 0);
    set_background_block(46, 31, 7, 0, 0);
    set_background_block(47, 31, 7, 0, 0);
    set_background_block(48, 31, 7, 0, 0);
    set_background_block(49, 31, 7, 0, 0);
    set_background_block(50, 31, 7, 0, 0);
    set_background_block(53, 31, 7, 0, 0);
    set_background_block(54, 31, 7, 0, 0);
    set_background_block(55, 31, 7, 0, 0);
    set_background_block(56, 31, 7, 0, 0);
    set_background_block(57, 31, 7, 0, 0);
    set_background_block(58, 31, 7, 0, 0);
    set_background_block(21, 32, 7, 0, 0);
    set_background_block(22, 32, 7, 0, 0);
    set_background_block(27, 32, 7, 0, 0);
    set_background_block(28, 32, 7, 0, 0);
    set_background_block(31, 32, 7, 0, 0);
    set_background_block(32, 32, 7, 0, 0);
    set_background_block(39, 32, 7, 0, 0);
    set_background_block(40, 32, 7, 0, 0);
    set_background_block(43, 32, 7, 0, 0);
    set_background_block(44, 32, 7, 0, 0);
    set_background_block(53, 32, 7, 0, 0);
    set_background_block(54, 32, 7, 0, 0);
    set_background_block(59, 32, 7, 0, 0);
    set_background_block(60, 32, 7, 0, 0);
    set_background_block(21, 33, 7, 0, 0);
    set_background_block(22, 33, 7, 0, 0);
    set_background_block(27, 33, 7, 0, 0);
    set_background_block(28, 33, 7, 0, 0);
    set_background_block(31, 33, 7, 0, 0);
    set_background_block(32, 33, 7, 0, 0);
    set_background_block(39, 33, 7, 0, 0);
    set_background_block(40, 33, 7, 0, 0);
    set_background_block(43, 33, 7, 0, 0);
    set_background_block(44, 33, 7, 0, 0);
    set_background_block(53, 33, 7, 0, 0);
    set_background_block(54, 33, 7, 0, 0);
    set_background_block(59, 33, 7, 0, 0);
    set_background_block(60, 33, 7, 0, 0);
    set_background_block(21, 34, 7, 0, 0);
    set_background_block(22, 34, 7, 0, 0);
    set_background_block(27, 34, 7, 0, 0);
    set_background_block(28, 34, 7, 0, 0);
    set_background_block(33, 34, 7, 0, 0);
    set_background_block(34, 34, 7, 0, 0);
    set_background_block(37, 34, 7, 0, 0);
    set_background_block(38, 34, 7, 0, 0);
    set_background_block(43, 34, 7, 0, 0);
    set_background_block(44, 34, 7, 0, 0);
    set_background_block(45, 34, 7, 0, 0);
    set_background_block(46, 34, 7, 0, 0);
    set_background_block(47, 34, 7, 0, 0);
    set_background_block(48, 34, 7, 0, 0);
    set_background_block(53, 34, 7, 0, 0);
    set_background_block(54, 34, 7, 0, 0);
    set_background_block(55, 34, 7, 0, 0);
    set_background_block(56, 34, 7, 0, 0);
    set_background_block(57, 34, 7, 0, 0);
    set_background_block(58, 34, 7, 0, 0);
    set_background_block(21, 35, 7, 0, 0);
    set_background_block(22, 35, 7, 0, 0);
    set_background_block(27, 35, 7, 0, 0);
    set_background_block(28, 35, 7, 0, 0);
    set_background_block(33, 35, 7, 0, 0);
    set_background_block(34, 35, 7, 0, 0);
    set_background_block(37, 35, 7, 0, 0);
    set_background_block(38, 35, 7, 0, 0);
    set_background_block(43, 35, 7, 0, 0);
    set_background_block(44, 35, 7, 0, 0);
    set_background_block(45, 35, 7, 0, 0);
    set_background_block(46, 35, 7, 0, 0);
    set_background_block(47, 35, 7, 0, 0);
    set_background_block(48, 35, 7, 0, 0);
    set_background_block(53, 35, 7, 0, 0);
    set_background_block(54, 35, 7, 0, 0);
    set_background_block(55, 35, 7, 0, 0);
    set_background_block(56, 35, 7, 0, 0);
    set_background_block(57, 35, 7, 0, 0);
    set_background_block(58, 35, 7, 0, 0);
    set_background_block(21, 36, 7, 0, 0);
    set_background_block(22, 36, 7, 0, 0);
    set_background_block(27, 36, 7, 0, 0);
    set_background_block(28, 36, 7, 0, 0);
    set_background_block(33, 36, 7, 0, 0);
    set_background_block(34, 36, 7, 0, 0);
    set_background_block(37, 36, 7, 0, 0);
    set_background_block(38, 36, 7, 0, 0);
    set_background_block(43, 36, 7, 0, 0);
    set_background_block(44, 36, 7, 0, 0);
    set_background_block(53, 36, 7, 0, 0);
    set_background_block(54, 36, 7, 0, 0);
    set_background_block(57, 36, 7, 0, 0);
    set_background_block(58, 36, 7, 0, 0);
    set_background_block(21, 37, 7, 0, 0);
    set_background_block(22, 37, 7, 0, 0);
    set_background_block(27, 37, 7, 0, 0);
    set_background_block(28, 37, 7, 0, 0);
    set_background_block(33, 37, 7, 0, 0);
    set_background_block(34, 37, 7, 0, 0);
    set_background_block(37, 37, 7, 0, 0);
    set_background_block(38, 37, 7, 0, 0);
    set_background_block(43, 37, 7, 0, 0);
    set_background_block(44, 37, 7, 0, 0);
    set_background_block(53, 37, 7, 0, 0);
    set_background_block(54, 37, 7, 0, 0);
    set_background_block(57, 37, 7, 0, 0);
    set_background_block(58, 37, 7, 0, 0);
    set_background_block(23, 38, 7, 0, 0);
    set_background_block(24, 38, 7, 0, 0);
    set_background_block(25, 38, 7, 0, 0);
    set_background_block(26, 38, 7, 0, 0);
    set_background_block(35, 38, 7, 0, 0);
    set_background_block(36, 38, 7, 0, 0);
    set_background_block(43, 38, 7, 0, 0);
    set_background_block(44, 38, 7, 0, 0);
    set_background_block(45, 38, 7, 0, 0);
    set_background_block(46, 38, 7, 0, 0);
    set_background_block(47, 38, 7, 0, 0);
    set_background_block(48, 38, 7, 0, 0);
    set_background_block(49, 38, 7, 0, 0);
    set_background_block(50, 38, 7, 0, 0);
    set_background_block(53, 38, 7, 0, 0);
    set_background_block(54, 38, 7, 0, 0);
    set_background_block(59, 38, 7, 0, 0);
    set_background_block(60, 38, 7, 0, 0);
    set_background_block(23, 39, 7, 0, 0);
    set_background_block(24, 39, 7, 0, 0);
    set_background_block(25, 39, 7, 0, 0);
    set_background_block(26, 39, 7, 0, 0);
    set_background_block(35, 39, 7, 0, 0);
    set_background_block(36, 39, 7, 0, 0);
    set_background_block(43, 39, 7, 0, 0);
    set_background_block(44, 39, 7, 0, 0);
    set_background_block(45, 39, 7, 0, 0);
    set_background_block(46, 39, 7, 0, 0);
    set_background_block(47, 39, 7, 0, 0);
    set_background_block(48, 39, 7, 0, 0);
    set_background_block(49, 39, 7, 0, 0);
    set_background_block(50, 39, 7, 0, 0);
    set_background_block(53, 39, 7, 0, 0);
    set_background_block(54, 39, 7, 0, 0);
    set_background_block(59, 39, 7, 0, 0);
    set_background_block(60, 39, 7, 0, 0);
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

int renderizou(){
    return time_is_up();
} 

void renderizar_por_mais_tempo(){
    return reset_pulsecounter();
}
