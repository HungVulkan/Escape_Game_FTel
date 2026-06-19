#ifndef __ESCAPE_MENU_H__
#define __ESCAPE_MENU_H__

#include <stdint.h>
#include <stdbool.h>

/* ============================ Menu State Definitions ======================== */
typedef enum {
    MENU_STATE_LOGO,
    MENU_STATE_MAIN,
    MENU_STATE_SETTINGS,
    MENU_STATE_ABOUT,
    MENU_STATE_GAME_START,
    MENU_STATE_GAME_RUNNING,
    MENU_STATE_GAME_PAUSE,
    MENU_STATE_GAME_OVER
} menu_state_t;

typedef enum {
    MENU_ACTION_NONE,
    MENU_ACTION_UP,
    MENU_ACTION_DOWN,
    MENU_ACTION_SELECT,
    MENU_ACTION_BACK,
    MENU_ACTION_HOME
} menu_action_t;

/* ============================ Menu Item Definitions =========================== */
typedef struct {
    const char* label;
    menu_state_t next_state;
    void (*callback)(void);
} menu_item_t;

typedef struct {
    const char* title;
    menu_item_t* items;
    uint8_t item_count;
    uint8_t selected_index;
    menu_state_t parent_state;
} menu_screen_t;

/* ============================ Game Settings ================================== */
typedef struct {
    uint8_t difficulty;      // 0: Easy, 1: Normal, 2: Hard
    uint8_t sound_enabled;   // 0: OFF, 1: ON
    uint8_t brightness;      // 0-100
    uint8_t contrast;        // 0-100
} game_settings_t;

/* ============================ Game State ===================================== */
typedef struct {
    menu_state_t current_state;
    menu_state_t previous_state;
    menu_screen_t* current_screen;
    game_settings_t settings;
    uint32_t game_start_time;
    uint16_t timer_remaining;
    uint8_t player_lives;
    uint16_t player_score;
} escape_game_context_t;

/* ============================ Function Prototypes ============================= */
extern escape_game_context_t g_escape_game;

/* Initialize the menu system */
void escape_menu_init(void);

/* Handle menu navigation */
void escape_menu_handle_action(menu_action_t action);

/* Render current menu screen */
void escape_menu_render(void);

/* Menu state functions */
void escape_menu_logo_enter(void);
void escape_menu_main_enter(void);
void escape_menu_settings_enter(void);
void escape_menu_about_enter(void);
void escape_menu_game_start_enter(void);
void escape_menu_game_running_enter(void);
void escape_menu_game_pause_enter(void);
void escape_menu_game_over_enter(void);

/* Menu action callbacks */
void escape_menu_start_game(void);
void escape_menu_resume_game(void);
void escape_menu_exit_game(void);
void escape_menu_adjust_difficulty(uint8_t level);
void escape_menu_toggle_sound(void);
void escape_menu_adjust_brightness(uint8_t value);
void escape_menu_adjust_contrast(uint8_t value);
void escape_menu_reset_settings(void);

#endif // __ESCAPE_MENU_H__
