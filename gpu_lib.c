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
    char tetris[5][36] = {{0}};
    char letraT[5][5] = {{0}}, letraE[5][5] = {{0}}, letraR[5][5] = {{0}}, letraI[5][5] = {{0}}, letraS[5][5] = {{0}};
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
    char pts[5][26] = {{0}};
    char letraT[5][5] = {{0}},letraS[5][5] = {{0}},letraP[5][5] = {{0}},doisP[5][5] = {{0}};
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
    char digito1[5][5] = {{0}},digito2[5][5] = {{0}},digito3[5][5] = {{0}},digito4[5][5] = {{0}},digito5[5][5] = {{0}},digito6[5][5] = {{0}};
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
        char temp[5][5] = {{0}};
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
        char temp[5][5] = {{0}};
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

/*return void?*/
void renderizar_por_mais_tempo(){
    return reset_pulsecounter();
}

int botoes(){
	return button();
}

void escrever_GameOver(int corG,int corA,int corM,int corE,int corO,int corV,int corR,int posX, int posY,int tamanho){
    char GameOver[5][60] = {{0}};
    char letraG[5][5] = {{0}},letraA[5][5]= {{0}},letraM[5][5]= {{0}},letraO[5][5]= {{0}},letraV[5][5]= {{0}},letraE[5][5]= {{0}},letraR[5][5]= {{0}};
    int i,j;

    definirLetraG(letraG);
    mudarCor(5,5,letraG,corG);
    
    definirLetraA(letraA);
    mudarCor(5,5,letraA,corA);
    
    definirLetraM(letraM);
    mudarCor(5,5,letraM,corM);
    
    definirLetraE(letraE);
    mudarCor(5,5,letraE,corE);
    
    definirLetraO(letraO);
    mudarCor(5,5,letraO,corO);

    definirLetraV(letraV);
    mudarCor(5,5,letraV,corV);

    definirLetraE(letraE);
    mudarCor(5,5,letraE,corE);

    definirLetraR(letraR);
    mudarCor(5,5,letraR,corR);
    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
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
    ler_matriz(5,60,GameOver,1,posX,posY,tamanho);
}

void escreverPause(int corP,int corA,int corU,int corS,int corE,int posX, int posY,int tamanho){
    char Pause[5][36] = {{0}};
    char letraP[5][5]= {{0}}, letraA[5][5]={{0}}, letraU[5][5]={{0}}, letraS[5][5]={{0}},letraE[5][5]={{0}} ;
    int i,j;
    // Definindo as letras
    definirLetraP(letraP);
    mudarCor(5,5,letraP,corP);

    definirLetraA(letraA);
    mudarCor(5,5,letraA,corA);

    definirLetraU(letraU);
    mudarCor(5,5,letraU,corU);

    definirLetraS(letraS);
    mudarCor(5,5,letraS,corS);

    definirLetraE(letraE);
    mudarCor(5,5,letraE,corE);

    // Concatenando as letras na matriz
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            Pause[i][j] = letraP[i][j]; // T
            Pause[i][6 + j] = letraA[i][j]; // E
            Pause[i][12 + j] = letraU[i][j]; // T
            Pause[i][18 + j] = letraS[i][j]; // R
            Pause[i][24 + j] = letraE[i][j]; // I
        }
    }
    ler_matriz(5,36,Pause,2,posX,posY,tamanho);
}

void escreverPressionePB(int corP,int corR,int corE,int corS,int corI,int corO,int corN,int corB,int corA,int corJ,int corG,int posX, int posY,int tamanho){
    int i,j;
    char Pressione[5][60] = {{0}};
    char PB[5][16] = {{0}};
    char Para[5][30] = {{0}};
    char Jogar[5][31] = {{0}};
    char letraP[5][5]= {{0}}, letraR[5][5] = {{0}},letraE[5][5]= {{0}},letraS[5][5]= {{0}},letraI[5][5]= {{0}} ,letraO[5][5]= {{0}},letraN[5][5]= {{0}},letraB[5][5]= {{0}}, letraA[5][5]= {{0}},letraJ[5][5]= {{0}},letraG[5][5]= {{0}} ;

    definirLetraP(letraP);
    mudarCor(5,5,letraP,corP);

    definirLetraR(letraR);
    mudarCor(5,5,letraR,corR);

    definirLetraE(letraE);
    mudarCor(5,5,letraE,corE);

    definirLetraS(letraS);
    mudarCor(5,5,letraS,corS);
    
    definirLetraI(letraI);
    mudarCor(5,5,letraI,corI);

    definirLetraO(letraO);
    mudarCor(5,5,letraO,corO);
    
    definirLetraN(letraN);
    mudarCor(5,5,letraN,corN);

    definirLetraA(letraA);
    mudarCor(5,5,letraA,corA);

    definirLetraB(letraB);
    mudarCor(5,5,letraB,corB);

    definirLetraJ(letraJ);
    mudarCor(5,5,letraJ,corJ);

    definirLetraG(letraG);
    mudarCor(5,5,letraG,corG);

    //PRESSIONE
    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
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

    //PB

    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
            PB[i][j] = letraP[i][j]; 
            PB[i][6 + j] = letraB[i][j]; 
        }
    }

    //PARA
    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
            Para[i][j] = letraP[i][j]; 
            Para[i][6 + j] = letraA[i][j]; 
            Para[i][12 + j] = letraR[i][j]; 
            Para[i][18 + j] = letraA[i][j]; 
        }
    }

    //JOGAR
    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
            Jogar[i][j] = letraJ[i][j]; 
            Jogar[i][6 + j] = letraO[i][j]; 
            Jogar[i][12 + j] = letraG[i][j]; 
            Jogar[i][18 + j] = letraA[i][j];
            Jogar[i][24 + j] = letraR[i][j]; 
        }
    }
    ler_matriz(5,60,Pressione,1,posX,posY,tamanho);
    ler_matriz(5,16,PB,1,posX+60,posY,tamanho);
    ler_matriz(5,30,Para,1,posX+5,posY+6,tamanho);
    ler_matriz(5,31,Jogar,1,posX+36,posY+6,tamanho);
}
