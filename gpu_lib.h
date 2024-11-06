#ifndef GPU_LIB_H
#define GPU_LIB_H
#include <stdint.h>
#include "gpu_letters.h"


void mapear_gpu();
void desmapear_gpu();
void limpar_tela();
void desenhar_bloco(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R);
void desenhar_sprite(uint32_t reg, uint32_t x, uint32_t y, uint32_t offset, uint32_t sp);
void desenhar_tela_inicial();
void desenhar_T_vermelho();
void desenhar_E_laranja();
void desenhar_T_amarelo();
int time_is_up();
void reset_pulsecounter();
#endif
