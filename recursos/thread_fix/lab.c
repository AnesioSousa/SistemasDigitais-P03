#include "acelerometro.c"
#include "mouse_move.c"
#include "gpu_lib.h"
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>


void inicializacao_accel();
int ACCEL = 1, fd;
I2C_Registers regs;
static int16_t X[1];

typedef struct {
	int coord_x;
	int coord_y;
	int direction;
	int offset;
	int data_register;
	int step_x;
	int step_y;
	int active;
	int collision;
} Sprite;

typedef struct{
	int coord_x;
	int coord_y;
	int offset;
	int data_register;
	int active;
} Sprite_Fixed;

void *player_1(void *args);
void *player_2(void *args);

int action = 0;
int power_amount = 1;

int main(){
	mapear_gpu();
	limpar_tela();

	if(!open_mouse_device()){
		return 1;
	}

  inicializacao_accel();
	clear_poligonos();
	clear_sprites();

	pthread_t thread_player_1, thread_player_2;
	pthread_create(&thread_player_1, NULL, player_1, NULL);
	pthread_create(&thread_player_2, NULL, player_2, NULL);
	
  pthread_join(thread_player_1, NULL);
	pthread_join(thread_player_2, NULL);
  
  close_mouse_device();
	desmapear_gpu();
	return 0;
}

void *player_1(void *args) {
  Sprite sprt_1;
  sprt_1.data_register  = 2;  sprt_1.coord_x = 300;  sprt_1.coord_y = 200;  sprt_1.offset = 2; sprt_1.active = 1; 
  
  while (ACCEL){
    if (accelereometer_isDataReady(regs)){
      accelerometer_x_read(X, regs); // lê os dados do eixo x
      set_sprite(sprt_1.data_register, X[0], X[1], sprt_1.offset, sprt_1.active); 
    }
  }
}

void *player_2(void *args) {
  Sprite sprt_1;
  sprt_1.data_register  = 1;  sprt_1.coord_x = 300;  sprt_1.coord_y = 200;  sprt_1.offset = 1; sprt_1.active = 1; 

  while (1){
    mouse_movement(&action, &power_amount);
    //printf("X: %d, Y: %d\n", pos_x, pos_y);
    set_sprite(sprt_1.data_register, pos_x, pos_y, sprt_1.offset, sprt_1.active); 
   }
}



void inicializacao_accel() {
	fd = open_fd();
	regs = map_i2c(fd);
	I2C0_Init(&regs);         // Estabelece a comunicação com o acelerômetro
	accelerometer_init(regs); // Configura o acelerômetro
}

void *accel_working(void *args) {
	return NULL;
}
