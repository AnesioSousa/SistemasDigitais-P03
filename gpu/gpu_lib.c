#include "gpu_lib.h"
#include <stdint.h>

extern void mem_map();
extern void set_background_block(uint32_t line);
extern void clear_background();
extern void mem_unmap();

void iniciar_tela(){
  int i, j;
mem_map();
  clear_background();

  for(i=40; i<50; i++)
      set_background_block(i);
}

void desmapear_tela(){
  mem_unmap();
}


