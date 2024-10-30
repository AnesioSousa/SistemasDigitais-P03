#include "gpu_lib.h"

extern void mem_map();
extern void set_background_block(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R);
extern void clear_background();
extern void mem_unmap();
void apagar_Tudo();

void iniciar_tela(){
  mem_map();
  //clear_background();
  //for(i=0; i<3; i++)
  //set_background_block(20, 16, 0, 0, 7);
}

void desmapear_tela(){
  mem_unmap();
}

void desenhar_bloco(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R){
  set_background_block(line, col, B, G, R);
}

void limpar_tela(){
    int i, j;   
    for(i=0; i<=59; i++){
      for (j=0; j <= 79; j++){
        set_background_block(i, j, 0, 0, 0);
      }
    }
}

