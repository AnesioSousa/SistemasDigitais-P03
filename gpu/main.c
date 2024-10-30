#include <stdio.h>
#include "gpu_lib.h"

int main() {
  iniciar_tela();

  desenhar_bloco(30, 30, 0, 7, 0);
  limpar_tela();
  desmapear_tela();
  return 0;
}
