#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <math.h>

#define MICE_PATH "/dev/input/mice"

int fd_mouse;
int16_t pos_x;
int16_t pos_y;
uint8_t left;
uint8_t middle;
uint8_t right;
int8_t data[3];

int open_mouse_device();

int power (int base);

void close_mouse_device();

void mouse_movement(int *action, int *power_amount);

int left_click();
