#include "acelerometro.c"
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
void *accel_working(void *args);
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

int main(){
	mapear_gpu();
	limpar_tela();

	clear_poligonos();
	clear_sprites();
	Sprite sprt_1;
	sprt_1.data_register  = 1;  sprt_1.coord_x = 300;  sprt_1.coord_y = 200;  sprt_1.offset = 0; sprt_1.active = 1; 

	//while(1){ 
	//	if(time_is_up() == 0) {
	//		set_sprite(sprt_1.data_register, sprt_1.coord_x, sprt_1.coord_y, sprt_1.offset, sprt_1.active); 
	//		break; 
	//	}
	//}


	//desenhar_sprite(1, 300, 200, 1, 1);
	//desenhar_sprite(2, 300, 400, 2, 1);
	//desenhar_poligono(endereco, forma, B, G,  R,  tamanho,  ponto_ref_y, ponto_ref_x);
	int i = 1;	
	while(1){
		if(i==3)
			i=1;
		desenhar_poligono(1,1,7,0,0,6,i*100,200);
		sleep(1);
		i++;
	}
	/*	
		pthread_t thread_accel;
		inicializacao_accel();

	//desenhar_poligono(1,1,1,1,1,1,1,1);
	pthread_create(&thread_accel, NULL, accel_working, NULL);
	limpar_tela();

	desenhar_sprite(1, 20, 20, 1, 1);
	while(1){
	if (X[0] > 20) {
	desenhar_sprite(1, X[0], 20, 1, 1);
	usleep(3000);
	} else if (X[0] < -20) {
	desenhar_sprite(1, X[0], 20, 1, 1);
	usleep(3000);
	}
	}

	pthread_join(thread_accel, NULL);*/
	desmapear_gpu();
	return 0;
}

void inicializacao_accel() {
	fd = open_fd();
	regs = map_i2c(fd);
	I2C0_Init(&regs);         // Estabelece a comunicação com o acelerômetro
	accelerometer_init(regs); // Configura o acelerômetro
}

void *accel_working(void *args) {
	while (ACCEL)
		if (accelereometer_isDataReady(regs))
			accelerometer_x_read(X, regs); // lê os dados do eixo x
	return NULL;
}



