#include "mouse_move.h"
#include <unistd.h>
#include <stdlib.h>


int fd_mouse = 0;
int16_t pos_x = 0;
int16_t pos_y = 0;
uint8_t left = 0;
uint8_t middle = 0;
uint8_t right = 0;
int8_t data[3];

/**
 * \brief           Usada para abrir o arquivo de drive do mouse
 *  \return         Retorna 1 caso o arquivo foi aberto ou retorna 0 caso não seja possivel abrir o arquivo
 */
int open_mouse_device()
{
  fd_mouse = open(MICE_PATH, O_RDWR);

  if (fd_mouse < 0)
  {
    perror("Failed to open the device");
    return 0;
  }
  return 1;
}

/**
 * \brief           Usada para fechar o arquivo de drive do mouse
 */
void close_mouse_device()
{
  close(fd_mouse);
}

/**
 * \brief           Usada ler os inputs do mouse
 * 
 * \param[in]       action: Ponteiro para o inteiro que indica se a habilidade esta ativada ou não
 * \param[in]       power_amount: Ponteiro para o inteiro que indica a quantidade de vezes que ahabilidade ainda pode ser ativada
 * \return         Retorna 1 caso o arquivo foi aberto ou retorna 0 caso não seja possivel abrir o arquivo
 */
void mouse_movement(int *action, int *power_amount)
{
  int bytes = read(fd_mouse, data, sizeof(data)); /* Abre o arquivo MICE para leitura dos eventos de input que o mouse esta enviando */

  left = data[0] & 0x1;   /* Lê o 1º LSB e se for igual a 1 significa que o botão esquerdo foi pressionado então left = 1 tambem */
  right = data[0] & 0x2;  /* Lê o 2º LSB e se for igual a 1 significa que o botão direito foi pressionado então right = 1 tambem */
  middle = data[0] & 0x4; /* Lê o 4º LSB e se for igual a 1 significa que o botão do meio foi pressionado então middle = 1 tambem */
  if (left == 1 && *power_amount > 0) {
    *action = 1;
    *power_amount -= 1;
  }

  pos_x += (data[1]/2);
  pos_y -= (data[2]/2);
  if (pos_x < 0)
  {
    pos_x = 0;
  }
  if (pos_x > 640)
  {

    pos_x = 640;
  }

  if (pos_y < 0)
  {
    pos_y = 0;
  }
  if (pos_y > 480)
  {
    pos_y = 480;
  }
}
