//HEADER:
void escreverPressionePB(int corP,int corR,int corE,int corS,int corI,int corO,int corN,int corB,int corA,int corJ,int corG,int posX, int posY,int tamanho);
void definirLetraJ(char matriz[5][5]);
void definirLetraB(char matriz[5][5]); 
void definirLetraN(char matriz[5][5]);
//GPU_LETTERS:
void definirLetraG(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; matriz[0][4] = 1; // Parte superior
    matriz[1][0] = 1;                                                   matriz[1][4] = 0;   // Lados do G
    matriz[2][0] = 1;                   matriz[2][2] = 1; matriz[2][3] = 1;matriz[2][4] = 1;   // Lados do G
    matriz[3][0] = 1; matriz[3][1] = 0; matriz[3][2] = 0; matriz[3][3] = 0; matriz[3][4] = 1;
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; matriz[4][4] = 1; // Parte inferior do "G"
};

void definirLetraA(char matriz[5][5]) {
    matriz[0][2] = 1; // Parte superior do "A"
    matriz[1][1] = 1; matriz[1][3] = 1; // Meia altura
    matriz[2][0] = 1; matriz[2][2] = 0; matriz[2][4] = 1; // Meio do "A"
    matriz[3][0] = 1; matriz[3][1] = 1;matriz[3][2] = 1;matriz[3][3] = 1;matriz[3][4] = 1; // Parte inferior
    matriz[4][0] = 1; matriz[4][4] = 1; // Parte inferior
};

void definirLetraM(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][4] = 1; // Parte superior
    matriz[1][0] = 1; matriz[1][1] = 1;matriz[1][3] = 1;matriz[1][4] = 1; // Ponto do "M"
    matriz[2][0] = 1; matriz[2][2] = 1; matriz[2][4] = 1; // Ponto do "M"
    matriz[3][0] = 1; matriz[3][1] = 0; matriz[3][4] = 1; // Inferior
    matriz[4][0] = 1; matriz[4][4] = 1; // Inferior
};

void definirLetraO(char matriz[5][5]) {
    matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; // Parte superior
    matriz[1][0] = 1;                                  matriz[1][4] = 1; // Lados
    matriz[2][0] = 1;                                  matriz[2][4] = 1; // Lados
    matriz[3][0] = 1; matriz[3][2] = 0; matriz[3][4] = 1; // Parte inferior
    matriz[4][0] = 0;matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1;matriz[4][4] = 0; // Parte inferior
};

void definirLetraV(char matriz[5][5]) {
    matriz[1][4] = 1;                       
    matriz[1][0] = 1; 
    matriz[2][0] = 1; matriz[2][4] = 1;     // Meio do "V"
    matriz[3][1] = 1; matriz[3][3] = 1;     // Meio do "V"
    matriz[4][2] = 1;                       // Parte inferior
};

void definirLetraU(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][4] = 1; // Parte superior
    matriz[1][0] = 1; matriz[1][4] = 1; // Lados
    matriz[2][0] = 1; matriz[2][4] = 1; // Lados
    matriz[3][0] = 1; matriz[3][4] = 1; // Lados
    matriz[4][0] = 1; matriz[4][1] = 1;matriz[4][2] = 1; matriz[4][3] = 1;matriz[4][4] = 1; // Parte inferior
};

void definirLetraN(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][4] = 1;matriz[0][4] = 1; // Parte superior
    matriz[1][0] = 1; matriz[3][3] = 1;matriz[1][4] = 1; // Lado esquerdo e meio
    matriz[2][0] = 1; matriz[2][2] = 1;matriz[2][4] = 1; // Meio
    matriz[3][0] = 1; matriz[1][1] = 1;matriz[3][4] = 1; // Lado esquerdo e meio
    matriz[4][0] = 1; matriz[4][4] = 1;matriz[4][4] = 1; // Parte inferior
};

void definirLetraB(char matriz[5][5]) {
    matriz[0][0] = 1; matriz[0][1] = 1; matriz[0][2] = 1; matriz[0][3] = 1; // Parte superior
    matriz[1][0] = 1;                        matriz[1][3] = 1; // Lado esquerdo e lado direito superior
    matriz[2][0] = 1; matriz[2][1] = 1; matriz[2][2] = 1; matriz[2][3] = 0; // Meio
    matriz[3][0] = 1;                        matriz[3][3] = 1; // Lado esquerdo e lado direito inferior
    matriz[4][0] = 1; matriz[4][1] = 1; matriz[4][2] = 1; matriz[4][3] = 1; // Parte inferior
};

void definirLetraJ(char matriz[5][5]) {
    matriz[0][2] = 1; // Parte superior
    matriz[1][2] = 1; // Lado superior
    matriz[2][2] = 1; // Lado superior
    matriz[3][0] = 1;matriz[3][2] = 1; // Lado esquerdo
    matriz[4][0] = 1;matriz[4][1] = 1; matriz[4][2] = 1; // Curva para baixo
};



//GPU_LIB:
void escrever_GameOver(int corG,int corA,int corM,int corE,int corO,int corV,int corR,int posX, int posY,int tamanho){
    char GameOver[5][60] = {{0}};
    char letraG[5][5] = {{0}},letraA[5][5]= {{0}},letraM[5][5]= {{0}},letraE[5][5]= {{0}},letraO[5][5]= {{0}},letraV[5][5]= {{0}},letraE[5][5]= {{0}},letraR[5][5]= {{0}};
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
            GameOver[i][32 + j] = letraO[i][j]; 
            GameOver[i][38 + j] = letraV[i][j]; 
            GameOver[i][42 + j] = letraE[i][j]; 
            GameOver[i][48 + j] = letraR[i][j];  
        }
    }
    ler_matriz(5,60,GameOver,1,posX,posY,tamanho);
}

void escreverPause(int corP,int corA,int corU,int corS,int corE,int posX, int posY,int tamanho){
    char Pause[5][36] = {0};
    char letraP[5][5], letraA[5][5], letraU[5][5], letraS[5][5],letraE[5][5] ;
    int i,j;
    // Definindo as letras
    definirLetraP(letraP);
    mudarCor(5,5,letraP,corP);

    definirLetraA(letraA);
    mudarCor(5,5,letraA,corA);

    definirLetraR(letraU);
    mudarCor(5,5,letraU,corU);

    definirLetraI(letraS);
    mudarCor(5,5,letraS,corS);

    definirLetraS(letraE);
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
    char Para[5][16] = {{0}};
    char Jogar[5][31] = {{0}};
    char letraP[5][5]= {0}, letraR[5][5]= {0},letraE[5][5]= {0},letraS[5][5]= {0},letraI[5][5]= {0} ,letraO[5][5]= {0},letraN[5][5]= {0},letraB[5][5]= {0}, letraA[5][5]= {0},letraJ[5][5]= {0},letraG[5][5]= {0} ;

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
    ler_matriz(5,36,Pressione,2,posX,posY,tamanho);
    ler_matriz(5,36,PB,2,posX+60,posY,tamanho);
    ler_matriz(5,36,Para,2,posX,posY+6,tamanho);
    ler_matriz(5,36,Jogar,2,posX+16,posY+6,tamanho);
}
