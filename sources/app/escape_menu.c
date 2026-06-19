#include "escape_menu.h"
#include "ak.h"

/* ============================ Global Context ================================= */
escape_game_context_t g_escape_game = {0};

/* ============================ Embedded Software Logo (ASCII Art) ============= */
// Embedded Software ASCII Logo representation
#define EMBEDDED_LOGO_LINE1 "    /--\\  /--\\  /--\\ "
#define EMBEDDED_LOGO_LINE2 "   /(__)\\(  )(  ) "
#define EMBEDDED_LOGO_LINE3 "  (__)(__)(__)(__)    "
#define EMBEDDED_LOGO_LINE4 "Embedded Software"

/* ============================ Forward Declarations ============================ */
static void escape_menu_state_transition(menu_state_t new_state);
static void escape_menu_logo_render(void);
static void escape_menu_main_render(void);
static void escape_menu_settings_render(void);
static void escape_menu_about_render(void);
static void escape_menu_game_running_render(void);
static void escape_menu_game_pause_render(void);
static void escape_menu_game_over_render(void);

/* ============================ Logo Screen ==================================== */
static uint32_t logo_display_time = 0;
#define LOGO_DISPLAY_DURATION_MS 3000  // Show logo for 3 seconds

void escape_menu_logo_enter(void) {
    logo_display_time = ak_sys_get_tick();
    AK_LOG_INFO("Menu", "Logo screen entered - FeTel@HCMUS");
}

static void escape_menu_logo_render(void) {
    // Display FeTel@HCMUS logo with Embedded Software ASCII art
    AK_LOG_INFO("Menu", "╔════════════════════════════════════════════╗");
    AK_LOG_INFO("Menu", "║                                            ║");
    AK_LOG_INFO("Menu", "║          fetel@HCMUS                       ║");
    AK_LOG_INFO("Menu", "║   KHOA ĐIỆN TỬ - VIỄN THÔNG               ║");
    AK_LOG_INFO("Menu", "║                                            ║");
    AK_LOG_INFO("Menu", "║      /--\\  /--\\  /--\\                  ║");
    AK_LOG_INFO("Menu", "║     /(__)\\(  )(  )                      ║");
    AK_LOG_INFO("Menu", "║    (__)(__)(__)(__)                       ║");
    AK_LOG_INFO("Menu", "║  Embedded Software                        ║");
    AK_LOG_INFO("Menu", "║                                            ║");
    AK_LOG_INFO("Menu", "║         Loading...                         ║");
    AK_LOG_INFO("Menu", "╚════════════════════════════════════════════╝");
    
    // Check if logo display time has elapsed
    uint32_t current_time = ak_sys_get_tick();
    if ((current_time - logo_display_time) >= LOGO_DISPLAY_DURATION_MS) {
        escape_menu_state_transition(MENU_STATE_MAIN);
    }
}

/* ============================ Main Menu Screen ================================ */
static menu_item_t main_menu_items[] = {
    {"Start Game", MENU_STATE_GAME_START, escape_menu_start_game},
    {"Settings", MENU_STATE_SETTINGS, NULL},
    {"About", MENU_STATE_ABOUT, NULL},
};

static menu_screen_t main_menu_screen = {
    .title = "C0 LOA ESCAPE",
    .items = main_menu_items,
    .item_count = sizeof(main_menu_items) / sizeof(main_menu_items[0]),
    .selected_index = 0,
    .parent_state = MENU_STATE_LOGO
};

void escape_menu_main_enter(void) {
    g_escape_game.current_screen = &main_menu_screen;
    main_menu_screen.selected_index = 0;
    AK_LOG_INFO("Menu", "Main menu entered");
}

static void escape_menu_main_render(void) {
    AK_LOG_INFO("Menu", "╔════════════════════════════════════╗");
    AK_LOG_INFO("Menu", "║    C0 LOA ESCAPE                   ║");
    AK_LOG_INFO("Menu", "╠════════════════════════════════════╣");
    
    for (uint8_t i = 0; i < main_menu_screen.item_count; i++) {
        char prefix = (i == main_menu_screen.selected_index) ? '►' : ' ';
        AK_LOG_INFO("Menu", "║ %c %-32s ║", prefix, main_menu_screen.items[i].label);
    }
    
    AK_LOG_INFO("Menu", "╚════════════════════════════════════╝");
    AK_LOG_INFO("Menu", "Use UP/DOWN to navigate, SEL to confirm");
}

/* ============================ Settings Menu ================================== */
static menu_item_t settings_menu_items[] = {
    {"Difficulty", MENU_STATE_SETTINGS, NULL},
    {"Sound", MENU_STATE_SETTINGS, escape_menu_toggle_sound},
    {"Brightness", MENU_STATE_SETTINGS, NULL},
    {"Contrast", MENU_STATE_SETTINGS, NULL},
    {"Reset", MENU_STATE_SETTINGS, escape_menu_reset_settings},
};

static menu_screen_t settings_menu_screen = {
    .title = "SETTINGS",
    .items = settings_menu_items,
    .item_count = sizeof(settings_menu_items) / sizeof(settings_menu_items[0]),
    .selected_index = 0,
    .parent_state = MENU_STATE_MAIN
};

void escape_menu_settings_enter(void) {
    g_escape_game.current_screen = &settings_menu_screen;
    settings_menu_screen.selected_index = 0;
    AK_LOG_INFO("Menu", "Settings menu entered");
}

static void escape_menu_settings_render(void) {
    AK_LOG_INFO("Menu", "╔════════════════════════════════════╗");
    AK_LOG_INFO("Menu", "║         SETTINGS                   ║");
    AK_LOG_INFO("Menu", "╠════════════════════════════════════╣");
    
    for (uint8_t i = 0; i < settings_menu_screen.item_count; i++) {
        char prefix = (i == settings_menu_screen.selected_index) ? '►' : ' ';
        
        switch (i) {
            case 0:
                AK_LOG_INFO("Menu", "║ %c Difficulty: %-16s ║", prefix, 
                    g_escape_game.settings.difficulty == 0 ? "Easy" :
                    g_escape_game.settings.difficulty == 1 ? "Normal" : "Hard");
                break;
            case 1:
                AK_LOG_INFO("Menu", "║ %c Sound: %-23s ║", prefix,
                    g_escape_game.settings.sound_enabled ? "ON" : "OFF");
                break;
            case 2:
                AK_LOG_INFO("Menu", "║ %c Brightness: %-18u%% ║", prefix,
                    g_escape_game.settings.brightness);
                break;
            case 3:
                AK_LOG_INFO("Menu", "║ %c Contrast: %-20u%% ║", prefix,
                    g_escape_game.settings.contrast);
                break;
            case 4:
                AK_LOG_INFO("Menu", "║ %c %-32s ║", prefix, settings_menu_items[i].label);
                break;
        }
    }
    
    AK_LOG_INFO("Menu", "╚════════════════════════════════════╝");
}

/* ============================ About Menu ===================================== */
void escape_menu_about_enter(void) {
    AK_LOG_INFO("Menu", "About menu entered");
}

static void escape_menu_about_render(void) {
    AK_LOG_INFO("Menu", "╔════════════════════════════════════╗");
    AK_LOG_INFO("Menu", "║         ABOUT THIS GAME            ║");
    AK_LOG_INFO("Menu", "╠════════════════════════════════════╣");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "║  Platform: AK Embedded Base Kit    ║");
    AK_LOG_INFO("Menu", "║  MCU: STM32L151CBT6                ║");
    AK_LOG_INFO("Menu", "║  Display: OLED 1.54\" (SSD1309)    ║");
    AK_LOG_INFO("Menu", "║  Learning: FTel Embedded Bootcamp  ║");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "║  fetel@HCMUS                       ║");
    AK_LOG_INFO("Menu", "║  KHOA ĐIỆN TỬ - VIỄN THÔNG       ║");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "╚════════════════════════════════════╝");
}

/* ============================ Game Start Screen ============================== */
void escape_menu_game_start_enter(void) {
    g_escape_game.current_state = MENU_STATE_GAME_START;
    g_escape_game.player_lives = 3;
    g_escape_game.player_score = 0;
    g_escape_game.timer_remaining = 300;  // 5 minutes default
    AK_LOG_INFO("Menu", "Game start screen entered");
}

/* ============================ Game Running Screen ============================ */
void escape_menu_game_running_enter(void) {
    g_escape_game.current_state = MENU_STATE_GAME_RUNNING;
    g_escape_game.game_start_time = ak_sys_get_tick();
    AK_LOG_INFO("Menu", "Game running");
}

static void escape_menu_game_running_render(void) {
    uint32_t elapsed = ak_sys_get_tick() - g_escape_game.game_start_time;
    uint16_t remaining = (elapsed / 1000 < 300) ? (300 - elapsed / 1000) : 0;
    
    AK_LOG_INFO("Game", "╔════════════════════════════════════╗");
    AK_LOG_INFO("Game", "║     ESCAPE GAME - IN PROGRESS      ║");
    AK_LOG_INFO("Game", "╠════════════════════════════════════╣");
    AK_LOG_INFO("Game", "║ Time:  %3u sec                     ║", remaining);
    AK_LOG_INFO("Game", "║ Lives: %u                          ║", g_escape_game.player_lives);
    AK_LOG_INFO("Game", "║ Score: %u                          ║", g_escape_game.player_score);
    AK_LOG_INFO("Game", "╚════════════════════════════════════╝");
}

/* ============================ Game Pause Screen ============================== */
void escape_menu_game_pause_enter(void) {
    g_escape_game.current_state = MENU_STATE_GAME_PAUSE;
    AK_LOG_INFO("Menu", "Game paused");
}

static void escape_menu_game_pause_render(void) {
    AK_LOG_INFO("Menu", "╔════════════════════════════════════╗");
    AK_LOG_INFO("Menu", "║           GAME PAUSED              ║");
    AK_LOG_INFO("Menu", "╠════════════════════════════════════╣");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "║   Press SEL to Resume              ║");
    AK_LOG_INFO("Menu", "║   Press HOME to Return to Menu     ║");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "╚════════════════════════════════════╝");
}

/* ============================ Game Over Screen ================================ */
void escape_menu_game_over_enter(void) {
    g_escape_game.current_state = MENU_STATE_GAME_OVER;
    AK_LOG_INFO("Menu", "Game over");
}

static void escape_menu_game_over_render(void) {
    AK_LOG_INFO("Menu", "╔════════════════════════════════════╗");
    AK_LOG_INFO("Menu", "║         GAME OVER                  ║");
    AK_LOG_INFO("Menu", "╠════════════════════════════════════╣");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "║  Final Score: %u                   ║", g_escape_game.player_score);
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "║  Press SEL to Continue             ║");
    AK_LOG_INFO("Menu", "║                                    ║");
    AK_LOG_INFO("Menu", "╚════════════════════════════════════╝");
}

/* ============================ Menu Callbacks ================================== */
void escape_menu_start_game(void) {
    AK_LOG_INFO("Menu", "Starting game...");
    escape_menu_game_start_enter();
}

void escape_menu_resume_game(void) {
    AK_LOG_INFO("Menu", "Resuming game...");
    escape_menu_game_running_enter();
}

void escape_menu_exit_game(void) {
    AK_LOG_INFO("Menu", "Exiting game...");
    escape_menu_main_enter();
}

void escape_menu_toggle_sound(void) {
    g_escape_game.settings.sound_enabled = !g_escape_game.settings.sound_enabled;
    AK_LOG_INFO("Menu", "Sound toggled: %s", 
        g_escape_game.settings.sound_enabled ? "ON" : "OFF");
}

void escape_menu_adjust_difficulty(uint8_t level) {
    if (level <= 2) {
        g_escape_game.settings.difficulty = level;
        AK_LOG_INFO("Menu", "Difficulty set to: %s",
            level == 0 ? "Easy" : level == 1 ? "Normal" : "Hard");
    }
}

void escape_menu_adjust_brightness(uint8_t value) {
    if (value <= 100) {
        g_escape_game.settings.brightness = value;
        AK_LOG_INFO("Menu", "Brightness set to: %u%%", value);
    }
}

void escape_menu_adjust_contrast(uint8_t value) {
    if (value <= 100) {
        g_escape_game.settings.contrast = value;
        AK_LOG_INFO("Menu", "Contrast set to: %u%%", value);
    }
}

void escape_menu_reset_settings(void) {
    g_escape_game.settings.difficulty = 1;      // Normal
    g_escape_game.settings.sound_enabled = 1;
    g_escape_game.settings.brightness = 100;
    g_escape_game.settings.contrast = 100;
    AK_LOG_INFO("Menu", "Settings reset to defaults");
}

/* ============================ Menu Navigation ================================ */
static void escape_menu_state_transition(menu_state_t new_state) {
    g_escape_game.previous_state = g_escape_game.current_state;
    g_escape_game.current_state = new_state;
    
    switch (new_state) {
        case MENU_STATE_LOGO:
            escape_menu_logo_enter();
            break;
        case MENU_STATE_MAIN:
            escape_menu_main_enter();
            break;
        case MENU_STATE_SETTINGS:
            escape_menu_settings_enter();
            break;
        case MENU_STATE_ABOUT:
            escape_menu_about_enter();
            break;
        case MENU_STATE_GAME_START:
            escape_menu_game_start_enter();
            break;
        case MENU_STATE_GAME_RUNNING:
            escape_menu_game_running_enter();
            break;
        case MENU_STATE_GAME_PAUSE:
            escape_menu_game_pause_enter();
            break;
        case MENU_STATE_GAME_OVER:
            escape_menu_game_over_enter();
            break;
    }
}

void escape_menu_handle_action(menu_action_t action) {
    menu_screen_t* screen = g_escape_game.current_screen;
    
    if (!screen) {
        return;
    }
    
    switch (action) {
        case MENU_ACTION_UP:
            if (screen->selected_index > 0) {
                screen->selected_index--;
            }
            break;
            
        case MENU_ACTION_DOWN:
            if (screen->selected_index < screen->item_count - 1) {
                screen->selected_index++;
            }
            break;
            
        case MENU_ACTION_SELECT:
            {
                menu_item_t* item = &screen->items[screen->selected_index];
                if (item->callback) {
                    item->callback();
                }
                escape_menu_state_transition(item->next_state);
            }
            break;
            
        case MENU_ACTION_BACK:
            if (screen->parent_state != MENU_STATE_MAIN) {
                escape_menu_state_transition(screen->parent_state);
            }
            break;
            
        case MENU_ACTION_HOME:
            escape_menu_state_transition(MENU_STATE_MAIN);
            break;
            
        default:
            break;
    }
}

void escape_menu_render(void) {
    switch (g_escape_game.current_state) {
        case MENU_STATE_LOGO:
            escape_menu_logo_render();
            break;
        case MENU_STATE_MAIN:
            escape_menu_main_render();
            break;
        case MENU_STATE_SETTINGS:
            escape_menu_settings_render();
            break;
        case MENU_STATE_ABOUT:
            escape_menu_about_render();
            break;
        case MENU_STATE_GAME_RUNNING:
            escape_menu_game_running_render();
            break;
        case MENU_STATE_GAME_PAUSE:
            escape_menu_game_pause_render();
            break;
        case MENU_STATE_GAME_OVER:
            escape_menu_game_over_render();
            break;
        default:
            break;
    }
}

void escape_menu_init(void) {
    // Initialize default settings
    g_escape_game.settings.difficulty = 1;       // Normal
    g_escape_game.settings.sound_enabled = 1;    // ON
    g_escape_game.settings.brightness = 100;     // 100%
    g_escape_game.settings.contrast = 100;       // 100%
    
    // Set initial state to LOGO
    g_escape_game.current_state = MENU_STATE_LOGO;
    g_escape_game.current_screen = NULL;
    
    AK_LOG_INFO("Menu", "Menu system initialized - Starting with FeTel@HCMUS logo");
    
    // Transition to logo screen first
    escape_menu_state_transition(MENU_STATE_LOGO);
}
