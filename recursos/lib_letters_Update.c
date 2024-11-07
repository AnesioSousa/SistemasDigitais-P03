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
