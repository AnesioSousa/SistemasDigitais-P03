#include "gpu_lib.h"

extern void mem_map();
extern void set_background_block(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R);
extern void clear_background();
extern void mem_unmap();

void iniciar_tela() {
    mem_map();
    // clear_background();
    // for(i=0; i<3; i++)
    // set_background_block(20, 16, 0, 0, 7);
}

void desmapear_tela() {
    mem_unmap();
}

void desenhar_tela_inicial() {
    limpar_tela();
    desenhar_T_vermelho();
    desenhar_E_laranja();
    desenhar_T();
}

void desenhar_T_vermelho() {
    set_background_block(9, 14, 0, 0, 7);
    set_background_block(9, 15, 0, 0, 7);
    set_background_block(9, 16, 0, 0, 7);
    set_background_block(9, 17, 0, 0, 7);
    set_background_block(9, 18, 0, 0, 7);
    set_background_block(9, 19, 0, 0, 7);
    set_background_block(9, 20, 0, 0, 7);
    set_background_block(9, 21, 0, 0, 7);
    set_background_block(10, 14, 0, 0, 7);
    set_background_block(10, 15, 0, 0, 7);
    set_background_block(10, 16, 0, 0, 7);
    set_background_block(10, 17, 0, 0, 7);
    set_background_block(10, 18, 0, 0, 7);
    set_background_block(10, 19, 0, 0, 7);
    set_background_block(10, 20, 0, 0, 7);
    set_background_block(10, 21, 0, 0, 7);
    set_background_block(11, 17, 0, 0, 7);
    set_background_block(11, 18, 0, 0, 7);
    set_background_block(12, 17, 0, 0, 7);
    set_background_block(12, 18, 0, 0, 7);
    set_background_block(13, 17, 0, 0, 7);
    set_background_block(13, 18, 0, 0, 7);
    set_background_block(14, 17, 0, 0, 7);
    set_background_block(14, 18, 0, 0, 7);
    set_background_block(15, 17, 0, 0, 7);
    set_background_block(15, 18, 0, 0, 7);
    set_background_block(16, 17, 0, 0, 7);
    set_background_block(16, 18, 0, 0, 7);
    set_background_block(17, 17, 0, 0, 7);
    set_background_block(17, 18, 0, 0, 7);
    set_background_block(18, 17, 0, 0, 7);
    set_background_block(18, 18, 0, 0, 7);
}

void desenhar_E_laranja() {
    set_background_block(9, 25, 0, 0, 7);
    set_background_block(9, 26, 0, 0, 7);
    set_background_block(9, 27, 0, 0, 7);
    set_background_block(9, 28, 0, 0, 7);
    set_background_block(9, 29, 0, 0, 7);
    set_background_block(9, 30, 0, 0, 7);
    set_background_block(9, 31, 0, 0, 7);
    set_background_block(10, 25, 0, 0, 7);
    set_background_block(10, 26, 0, 0, 7);
    set_background_block(10, 27, 0, 0, 7);
    set_background_block(10, 28, 0, 0, 7);
    set_background_block(10, 29, 0, 0, 7);
    set_background_block(10, 30, 0, 0, 7);
    set_background_block(10, 31, 0, 0, 7);

    set_background_block(11, 25, 0, 0, 7);
    set_background_block(11, 26, 0, 0, 7);
    set_background_block(12, 25, 0, 0, 7);
    set_background_block(12, 26, 0, 0, 7);

    set_background_block(13, 25, 0, 0, 7);
    set_background_block(13, 26, 0, 0, 7);
    set_background_block(13, 27, 0, 0, 7);
    set_background_block(13, 28, 0, 0, 7);
    set_background_block(13, 29, 0, 0, 7);
    set_background_block(14, 25, 0, 0, 7);
    set_background_block(14, 26, 0, 0, 7);
    set_background_block(14, 27, 0, 0, 7);
    set_background_block(14, 28, 0, 0, 7);
    set_background_block(14, 29, 0, 0, 7);

    set_background_block(15, 25, 0, 0, 7);
    set_background_block(15, 26, 0, 0, 7);
    set_background_block(16, 25, 0, 0, 7);
    set_background_block(16, 26, 0, 0, 7);

    set_background_block(17, 25, 0, 0, 7);
    set_background_block(17, 26, 0, 0, 7);
    set_background_block(17, 27, 0, 0, 7);
    set_background_block(17, 28, 0, 0, 7);
    set_background_block(17, 29, 0, 0, 7);
    set_background_block(17, 30, 0, 0, 7);
    set_background_block(17, 31, 0, 0, 7);
    set_background_block(18, 25, 0, 0, 7);
    set_background_block(18, 26, 0, 0, 7);
    set_background_block(18, 27, 0, 0, 7);
    set_background_block(18, 28, 0, 0, 7);
    set_background_block(18, 29, 0, 0, 7);
    set_background_block(18, 30, 0, 0, 7);
    set_background_block(18, 31, 0, 0, 7);

    set_background_block(11, 17, 0, 0, 7);
    set_background_block(11, 18, 0, 0, 7);
    set_background_block(12, 17, 0, 0, 7);
    set_background_block(12, 18, 0, 0, 7);
    set_background_block(13, 17, 0, 0, 7);
    set_background_block(13, 18, 0, 0, 7);
    set_background_block(14, 17, 0, 0, 7);
    set_background_block(14, 18, 0, 0, 7);
    set_background_block(15, 17, 0, 0, 7);
    set_background_block(15, 18, 0, 0, 7);
    set_background_block(16, 17, 0, 0, 7);
    set_background_block(16, 18, 0, 0, 7);
    set_background_block(17, 17, 0, 0, 7);
    set_background_block(17, 18, 0, 0, 7);
    set_background_block(18, 17, 0, 0, 7);
    set_background_block(18, 18, 0, 0, 7);
}

void desenhar_T_amarelo() {
    set_background_block(9, 14, 0, 7, 7);
    set_background_block(9, 15, 0, 7, 7);
    set_background_block(9, 16, 0, 7, 7);
    set_background_block(9, 17, 0, 7, 7);
    set_background_block(9, 18, 0, 7, 7);
    set_background_block(9, 19, 0, 7, 7);
    set_background_block(9, 20, 0, 7, 7);
    set_background_block(9, 21, 0, 7, 7);
    set_background_block(10, 14, 0, 7, 7);
    set_background_block(10, 15, 0, 7, 7);
    set_background_block(10, 16, 0, 7, 7);
    set_background_block(10, 17, 0, 7, 7);
    set_background_block(10, 18, 0, 7, 7);
    set_background_block(10, 19, 0, 7, 7);
    set_background_block(10, 20, 0, 7, 7);
    set_background_block(10, 21, 0, 7, 7);
    set_background_block(11, 17, 0, 7, 7);
    set_background_block(11, 18, 0, 7, 7);
    set_background_block(12, 17, 0, 7, 7);
    set_background_block(12, 18, 0, 7, 7);
    set_background_block(13, 17, 0, 7, 7);
    set_background_block(13, 18, 0, 7, 7);
    set_background_block(14, 17, 0, 7, 7);
    set_background_block(14, 18, 0, 7, 7);
    set_background_block(15, 17, 0, 7, 7);
    set_background_block(15, 18, 0, 7, 7);
    set_background_block(16, 17, 0, 7, 7);
    set_background_block(16, 18, 0, 7, 7);
    set_background_block(17, 17, 0, 7, 7);
    set_background_block(17, 18, 0, 7, 7);
    set_background_block(18, 17, 0, 7, 7);
    set_background_block(18, 18, 0, 7, 7);
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
