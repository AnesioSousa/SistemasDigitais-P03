// FUNCOES AUXILIARES
void invert_map(int size1, int size2, char map[size1][size2]) {
    int i, j;
    for (i = 0; i < size1; i++) {
        for (j = 0; j < size2; j++) {
            map[i][j] = 1 - map[i][j];
        }
    }
};
void mudarCorFundo(int linhas, int colunas, char matriz[linhas][colunas], int cor) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] == 0) {
                matriz[i][j] = cor;
            }
        }
    }
}
void mudarCorGenerico(int linhas, int colunas, char matriz[linhas][colunas], int cor) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] == 1) {
                matriz[i][j] = cor;
            }
        }
    }
}
