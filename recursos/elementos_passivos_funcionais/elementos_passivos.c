#include "acelerometro.c"
#include "gpu_lib.h"
#include "mouse_move.c"
#include "pthread.h"
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
void *pac_text_thread(void *args);
void *ph_text_thread(void *args);

int main(){
    mapear_gpu();
    limpar_tela();
    pthread_t text_pac,text_ph;
    
    pthread_create(&text_pac, NULL, pac_text_thread, NULL); 
    pthread_create(&text_ph, NULL, ph_text_thread, NULL); 

    pthread_join(text_pac, NULL);
    pthread_join(text_ph, NULL);

    desmapear_gpu();

}


void *pac_text_thread(void *args){
    int i;
    char vazia[5][41] = {{0}};
    while(1){
    for (i = 0; i < 40; i++)
    {
        escrever_PacMan(3,3,3,3,3,3,i,49,1,1);
        usleep(80000);
        ler_matriz(5,41,vazia,1,i,49,1);
    }
    for (i = 39; i >-1; i--)
    {
        escrever_PacMan(3,3,3,3,3,3,i,49,1,1);
        usleep(80000);
        ler_matriz(5,41,vazia,1,i,49,1);
    }
 }
}


void *ph_text_thread(void *args){
    int i;
    char vazia[5][41] = {{0}};
    while(1){
    for (i = 39; i >-1; i--)
    {
        escrever_Phantom(1,1,1,1,1,1,1,i,54,1,1);
        usleep(80000);
        ler_matriz(5,41,vazia,1,i,54,1);
    }
    for (i = 0; i < 40; i++)
    {
        escrever_Phantom(1,1,1,1,1,1,1,i,54,1,1);
        usleep(80000);
        ler_matriz(5,41,vazia,1,i,54,1);
    }

 }
}
