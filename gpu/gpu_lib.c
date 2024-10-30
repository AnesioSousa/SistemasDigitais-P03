#include "gpu_lib.h"
#include <stdint.h>

extern void mem_map();
extern void set_background_block(uint32_t line, uint32_t col, uint32_t B, uint32_t G, uint32_t R);
extern void clear_background();
extern void mem_unmap();
void apagar_Tudo();

void iniciar_tela(){
  int i, j;
  mem_map();
  //clear_background();
  //for(i=0; i<3; i++)
  //set_background_block(1, 0, 0, 0, 7);
  apagar_Tudo();
}

void desmapear_tela(){
  mem_unmap();
}

void apagar_Tudo(){
    int i, j;
  
    for (j=0; j <= 79; j++){
            set_background_block(0, j, 7, 0, 0);
    }

}

