#include "acelerometro.c"
#include "gpu_lib.h"
#include "mouse_move.c"
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int main() {
    mapear_gpu();

    desmapear_gpu();
    return 0;
}
