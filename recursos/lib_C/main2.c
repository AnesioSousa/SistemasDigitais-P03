#include "gpu_lib.c"
#include <stdio.h>

int main() {
     iniciar_tela();
    limpar_tela();
    char pontuacao[5][36] = {0};
    /*
    posX = 5 e posY = 25 com tamanho 2 fica centralizado
    espacamento definido na lib

    void escreverTetris(int corT,int corE,int corR,int corI,int corS,int posX, int posY,int tamanho)
    */
    //escreverTetris(1,2,3,4,5,5,25,2);
    
    /*
            por algum motivo exibir TETRIS  e PTS: juntos buga o pts, funcionamento aparenta estar ok
    void escrever_Pts(int corT,int corS,int corP,int corDoisP,int posX, int posY,int tamanho)
    */
    escrever_Pts(1,2,3,4,0,54,1);
    
    /*
    escolher numero qualquer de 6 digitos para pontuação
    tive problemas na hora de compilar entao mantive alguns parametros redundantes
    testar tamanho da matriz

    void exibirPontuacao(int pontuacao,int linhas,int colunas,char matriz[linhas][colunas])

	
    */

    exibirPontuacao(793056,5,36,pontuacao);
    desmapear_tela();
    return 0;
}
