#include "create_sprite.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_SPEED_BASE 2
#define OFFSET_PLAYER_1 18
#define OFFSET_PLAYER_2 19
// #define BULLET_SPEED_BASE 15

typedef enum {
    running = 1,
    in_pause,
    in_menu,
    finish,
    win,
    lose,
} states;

typedef enum {
    single_player = 1,
    dual_player,
} game_modes;

sprite_t player_1_sprite = {
    .data_register = 31,
    .coord_x = 192,
    .coord_y = 340,
    .offset = OFFSET_PLAYER_1,
    .speed = PLAYER_SPEED_BASE,
    .visibility = 1,
};

sprite_t player_2_sprite = {
    .data_register = 30,
    .coord_x = 432,
    .coord_y = 340,
    .offset = OFFSET_PLAYER_2,
    .speed = PLAYER_SPEED_BASE,
    .visibility = 1,
};

// Mutexes utilizados
pthread_mutex_t gpu_mutex;
pthread_mutex_t mouse_1_mutex;
pthread_mutex_t mouse_2_mutex;
pthread_mutex_t obstacle_mutex;
pthread_mutex_t bullets_mutex;
pthread_mutex_t player_1_invunerability_mutex;
pthread_mutex_t player_2_invunerability_mutex;
pthread_mutex_t colision_mutex;
// Condicionais utilizadas
pthread_cond_t mouse_1_cond;
pthread_cond_t mouse_2_cond;
pthread_cond_t obstacle_cond;
pthread_cond_t bullets_cond;
pthread_cond_t player_1_invulnerability_cond;
pthread_cond_t player_2_invulnerability_cond;
pthread_cond_t colision_cond;
// threads
pthread_t obstacle_thread, background_thread, mouse_1_thread, mouse_2_thread, player_1_timer_thread, player_2_timer_thread, bullets_thread, colision_thread;

states state;
game_modes current_game_mode;

int pause_obstacle, pause_mouse_1, pause_mouse_2, pause_bullets, pause_colision;
int player_1_invunerability;
int player_2_invunerability;
int obstacle_on_screen_status[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int score_1;
int score_2;
int bullets[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int key_press_1;
int key_press_2;

void init_game() {
    sprite_t invisible_sprite = {
        .coord_x = 1,
        .coord_y = 1,
        .offset = 0,
        .speed = 0,
        .visibility = 0};

    // clean_all_obstacles(screen_obs, obstacle_on_screen_status);
    // clear();
    score_1 = 0;
    score_2 = 0;
    /*for(int i = 0; i < 10; i++){
        if (bullets[i]) {
            bullets[i] = 0;
        }
    }*/
    reestart_threads();
    player_1_sprite.visibility = 1;
    player_2_sprite.visibility = 1;
    bg_animation_module_init();
    set_sprite(player_1_sprite);
    set_sprite(player_2_sprite);
    return;
}

void menu() {
    state = in_menu;
    char btn_pressed;
    int i;
    while (state != finish) {
        KEYS_read(&btn_pressed);

        // START
        if (btn_pressed == BUTTON0 && state == in_menu) {
            state = running;
            init_game();
            // PAUSE
        } else if (btn_pressed == BUTTON1 && state == running) {
            state = in_pause;
            pause_screen();
            // RETORNO DO PAUSE
        } else if (btn_pressed == BUTTON1 && state == in_pause) {
            state = running;
            return_screen();
            // RESET
        } else if (bt_pressed == BUTTON2) {
            state = in_menu;
            pause_threads();
            clear();
            /*clean_all_obstacles(screen_obs, obstacle_on_screen_status);
              for(i = 0; i < 10; i++){
              if(bullets[i]) {
              bullets[i] = 0;
              }
              }
              score_1 = 0;
              score_2 = 0;
              draw_cover_art();
              set_menu(); */
        } else if (btn_pressed == BUTTON3) {
            state = finish;
        }
    }
    return;
}

int main() {
    // inicializacao dos inteiros
    score_1 = 0;
    score_2 = 0;
    pause_mouse_1 = 1;
    pause_mouse_2 = 1;
    pause_obstacle = 1;
    player_1_invunerability = 0;
    player_2_invunerability = 0;
    pause_bullets = 1;
    pause_colision = 1;
    key_press_1 = 0;
    key_press_2 = 0;
    
    state = in_menu;
    current_game_mode = dual_player;

    module_init_mouse_1();
    module_init_mouse_2();
    GPU_open();
    display_open();
    KEYS_open();
    initialize_obstacle_vector(obs);
    set_new_sprites();
    clear();
    draw_cover_art();
    set_menu();

    // inicialização dos mutex
    pthread_mutex_init(&gpu_mutex, NULL);
    pthread_mutex_init(&mouse_1_mutex, NULL);
    pthread_mutex_init(&mouse_2_mutex, NULL);
    pthread_mutex_init(&obstacle_mutex, NULL);
    pthread_mutex_init(&player_1_invunerability_mutex, NULL);
    pthread_mutex_init(&player_2_invunerability_mutex, NULL);
    pthread_mutex_init(&bullets_mutex, NULL);
    pthread_mutex_init(&colision_mutex, NULL);

    // inicialização das condições
    pthread_cond_init(&mouse_1_cond, NULL);
    pthread_cond_init(&mouse_2_cond, NULL);
    pthread_cond_init(&obstacle_cond, NULL);
    pthread_cond_init(&player_1_invulnerability_cond, NULL);
    pthread_cond_init(&player_2_invulnerability_cond, NULL);
    pthread_cond_init(&bullets_cond, NULL);
    pthread_cond_init(&colision_cond, NULL);

    // inicialização das threads

    pthread_create(&mouse_1_thread, NULL, mouse_1_polling_routine, NULL);
    pthread_create(&mouse_2_thread, NULL, mouse_2_polling_routine, NULL);
    pthread_create(&obstacle_thread, NULL, random_obstacle_generate_routine, NULL);
    pthread_create(&colision_thread, NULL, colision_routine, NULL);
    pthread_create(&player_1_timer_thread, NULL, player_1_invulnerability_timer, NULL);
    pthread_create(&player_2_timer_thread, NULL, player_2_invulnerability_timer, NULL);

    // loop principal do jogo
    menu();

    // TODO: tela de finalizar o jogo

    pthread_cancel(mouse_1_thread);
    pthread_cancel(mouse_2_thread);
    pthread_cancel(player_1_timer_thread);
    pthread_cancel(player_2_timer_thread);
    pthread_cancel(bullets_thread);
    pthread_cancel(obstacle_thread);
    pthread_cancel(bullets_thread);

    pause_colision = 0;
    pause_mouse_1 = 0;
    pause_mouse_2 = 0;
    pause_obstacle = 0;
    pthread_cond_broadcast(&colision_cond);
    pthread_cond_broadcast(&obstacle_cond);
    pthread_cond_broadcast(&mouse_1_cond);
    pthread_cond_broadcast(&mouse_2_cond);

    pthread_join(colision_thread, NULL);
    // encerrando os mutex
    pthread_mutex_destroy(&gpu_mutex);
    pthread_mutex_destroy(&mouse_1_mutex);
    pthread_mutex_destroy(&mouse_2_mutex);
    pthread_mutex_destroy(&obstacle_mutex);
    pthread_mutex_destroy(&player_1_invunerability_mutex);
    pthread_mutex_destroy(&player_2_invunerability_mutex);
    pthread_mutex_destroy(&bullets_mutex);
    pthread_mutex_destroy(&colision_mutex);
    printf("encerrou mutex\n");

    // encerrando as condicionais
    pthread_cond_destroy(&mouse_1_cond);
    pthread_cond_destroy(&mouse_2_cond);
    pthread_cond_destroy(&obstacle_cond);
    pthread_cond_destroy(&player_1_invulnerability_cond);
    pthread_cond_destroy(&player_2_invulnerability_cond);
    pthread_cond_destroy(&bullets_cond);
    pthread_cond_destroy(&colision_cond);
    printf("encerrou tudo e ta no close\n");

    printf("encerrou os mouses\n");
    GPU_close();
    printf("encerrou a gpu\n");
    display_close();
    printf("encerrou o display\n");
    KEYS_close();
    printf("encerrou os botões\n");

    return 0;
}
