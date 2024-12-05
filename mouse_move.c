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
int initial_x = -1, initial_y = -1; // Track initial mouse position

// Define previous mouse positions for drag detection
int16_t prev_pos_x = 0, prev_pos_y = 0; // Store previous positions for comparison
int direction_changed = 0;
/**
 * \brief           Usada para abrir o arquivo de drive do mouse
 * \return         Retorna 1 caso o arquivo foi aberto ou retorna 0 caso não seja possivel abrir o arquivo
 */
int open_mouse_device() {
    fd_mouse = open(MICE_PATH, O_RDWR);

    if (fd_mouse < 0) {
        perror("Failed to open the device");
        return 0;
    }
    return 1;
}

/**
 * \brief           Usada para fechar o arquivo de drive do mouse
 */
void close_mouse_device() {
    close(fd_mouse);
}

/**
 * \brief           Usada para ler os inputs do mouse
 * 
 * \param[in]       action: Ponteiro para o inteiro que indica se a habilidade esta ativada ou não
 * \param[in]       power_amount: Ponteiro para o inteiro que indica a quantidade de vezes que a habilidade ainda pode ser ativada
 */
void mouse_movement(int *action, int *power_amount) {
    int bytes = read(fd_mouse, data, sizeof(data)); // Read mouse events

    left = data[0] & 0x1;   // Left button
    right = data[0] & 0x2;  // Right button
    middle = data[0] & 0x4; // Middle button

    // If the left button is pressed and the power amount is greater than 0
    if (left == 1 && *power_amount > 0) {
        *action = 1;
        *power_amount -= 1;

        // Set initial position when the left button is first pressed
        if (initial_x == -1 && initial_y == -1) {
            initial_x = pos_x;
            initial_y = pos_y;
        }
    }
    else if (left == 0) {
        // Reset initial position when the left button is released
        initial_x = -1;
        initial_y = -1;
    }

    // Save the current position as previous position for drag detection
    prev_pos_x = pos_x;
    prev_pos_y = pos_y;

    // Update the position based on the movement from the mouse device
    pos_x += (data[1] / 2); // Horizontal movement
    pos_y -= (data[2] / 2); // Vertical movement

    // Boundaries to keep mouse within screen limits
    if (pos_x < 0) {
        pos_x = 0;
    }
    if (pos_x > 640) {
        pos_x = 640;
    }
    if (pos_y < 0) {
        pos_y = 0;
    }
    if (pos_y > 480) {
        pos_y = 480;
    }
}
