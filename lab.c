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


int main(){
	pthread_t thread_accel;
	mapear_gpu();
	inicializacao_accel();

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

	pthread_join(thread_accel, NULL);
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



