#include QMK_KEYBOARD_H
#include "transactions.h"
#include "os_detection.h"

// Animation constants
#define ANIM_FRAME_TIME_SLOW 500
#define ANIM_FRAME_TIME_FAST 100
#define WPM_THRESHOLD_FAST 100
#define WPM_THRESHOLD_OFF 5

// Menu constants
#define MENU_MAX_ITEMS 4
static const char PROGMEM macro_0[] = "#[derive(Clone, Debug)]\n";
static const char PROGMEM macro_1[] = "#[cfg(test)]\nmod tests {\nuse super::*;\nuse rstest::*;\nuse speculoos::prelude::*;\nuse mockall::predicate::*;\n\n";
static const char PROGMEM macro_2[] = "";
static const char PROGMEM macro_3[] = "";
static const char PROGMEM *macros[MENU_MAX_ITEMS] = {
    macro_0,
    macro_1,
    macro_2,
    macro_3
};
static const char PROGMEM menu_line[MENU_MAX_ITEMS][7] = {
    "Derive",
    "Test  ",
    "Opt. 2",
    "Opt. 3"
};
static uint8_t menu_selected = 0;

// Custom keycodes
enum custom_keycodes {
    _MACRO = SAFE_RANGE,
};

enum {
    _TESC,
};

tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Tab, twice for Escape
    [_TESC] = ACTION_TAP_DANCE_DOUBLE(KC_TAB, KC_ESC),
};

// KEYMAPS
enum layers {
    _BASE = 0,
    _MAC  = 1,
    _LO   = 2,
    _HI   = 3,
    _FN   = 4,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        TD(_TESC), KC_Q,  KC_W,    KC_E,    KC_R,    KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,              KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LALT,
                                   KC_LWIN, TL_LOWR, KC_SPC,            KC_ENT,  TL_UPPR, XXXXXXX
    ),
    [_MAC] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______,
        KC_LWIN, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______,
                                   KC_LCTL, _______, _______,           _______, _______, _______
    ),
    [_LO] = LAYOUT_split_3x6_3(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                   _______, _______, _______,           _______, _______, KC_MUTE
    ),
    [_HI] = LAYOUT_split_3x6_3(
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,           KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                   _MACRO,  _______, _______,           _______, _______, _______
    ),
    [_FN] = LAYOUT_split_3x6_3(
        KC_SLEP, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,             KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  TG(_MAC),
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                   _______, _______, _______,           _______, _______, _______
    ),

};

uint8_t get_layer(void) {
    if(layer_state_is(_FN)) {
        return _FN;
    } else if(layer_state_is(_HI)) {
        return _HI;
    } else if(layer_state_is(_LO)) {
        return _LO;
    } else if(layer_state_is(_MAC)) {
        return _MAC;
    } else  {
        return _BASE;
    }
}

// RGB Lights
void set_layer_light(uint8_t layer) {
    switch (layer) {
        case _LO:
            rgblight_sethsv(HSV_YELLOW);
            break;
        case _HI:
            rgblight_sethsv(HSV_AZURE);
            break;
        case _FN:
            rgblight_sethsv(HSV_MAGENTA);
            break;
        case _MAC:
            rgblight_sethsv(HSV_GREEN);
            break;
        default:
            rgblight_sethsv(HSV_WHITE);
            break;
    }
}

// ROTARY ENCODERS
#if defined(ENCODER_ENABLE)
bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t layer = get_layer();

    switch (layer) {
        case _HI:
            if (index == 0) {
                if (clockwise) {
                    menu_selected = (menu_selected + 1) % MENU_MAX_ITEMS;
                } else {
                    menu_selected = (menu_selected + MENU_MAX_ITEMS - 1) % MENU_MAX_ITEMS;
                }
            }
            break;
        case _LO:
            if (index == 0) {
                tap_code_delay(clockwise ? KC_PGUP : KC_PGDN, 10);
            } else {
                tap_code_delay(clockwise ? KC_VOLU : KC_VOLD, 10);
            }
            break;
        default:
            if (index == 0) {
                tap_code_delay(clockwise ? KC_WH_U : KC_WH_D, 10);
            } else {
                tap_code_delay(clockwise ? KC_WH_R : KC_WH_L, 10);
            }
            break;
    }

    return false;
}
#endif

// OLED SCREEN
#ifdef OLED_ENABLE
static void render_background(void) {
    static const char PROGMEM ferris_logo_bg[][8*16] = {
        [_BASE] = {
            0,  0,  0,240,216,252,124,252,204,216,240,  0,  0,  4, 10,  4,  0,  0, 64,  0,  0,  0,  0,  0,  0, 32, 80, 32,  0,  0,  0,  0, 64,128,160,248,145, 99,227,211, 35,249, 80,160,192,176,172,254,172, 80,192,224,240, 96, 80,252, 88, 96,160,240,144,252, 72,240,181, 95,250,207,250,171,255,174,181, 95,255,213,255,182,202,255,174,255,214,234,127,239,213,255,101,251,214,255,213,186,165,255, 13, 71, 55, 99, 18, 99, 35,  3, 13,131,129,225,227,231,193,193,193,227,225,225,133,153, 13,  1,  3, 67, 19, 99, 18,103,  7, 31,
        },
        [_MAC] = {
            0,  0,  0,240,216,252,124,252,204,216,240,  0,  0,  4, 10,  4,  0,  0, 64,  0,  0,  0,  0,  0,  0, 32, 80, 32,  0,  0,  0,  0, 64,128,160,248,145, 99,227,211, 35,249, 80,160,192,176,172,254,172, 80,192,224,240, 96, 80,252, 88, 96,160,240,144,252, 72,240,181, 95,250,207,250,171,255,174,181, 95,255,213,255,182,202,255,174,255,214,234,127,239,213,255,101,251,214,255,213,186,165,255, 13, 71, 55, 99, 18, 99, 35,  3, 13,131,129,225,227,231,193,193,193,227,225,225,133,153, 13,  1,  3, 67, 19, 99, 18,103,  7, 31,
        },
        [_LO] = {
            251,245,251,253,251,221,187,213,187,221,255,255,255,255,127,127,127,127,251,247,251,247,251,255,127,255,127,191,125,187,125,187,119,187,119,187,127,191,127,191,127,191,  7,121,254,254,255,255,255,255,254,254,121,  7,191,127,191,126,191,126,191,126,191,127,  0, 64, 64, 64, 64,  2,  2,  8, 10,  2, 42, 14, 42, 46,190,254,254,190, 46, 42, 14, 42, 10,  2, 10,  0,  2,  0, 32, 32,  0,  0,128,128,128,132, 68, 68, 64, 64, 65,129,128,224,224,232,200,201,193,224,224,224,128,128,144,145,129, 65, 65, 64,128,132,132,132,
        },
        [_HI] = {
            255,127,255,255,251,247,251,255,255,255,255,255,255,191,127,191,255,255,255,255,255,255, 15,247,251,251, 59,123,247, 15,255,255, 64,171,212,235,119, 55, 47, 31,135, 91,167,223, 63,143, 39, 65,170, 85, 59,119,247,111,199,130,  1, 64,169, 85,170,125,219,247,129, 70,189,243,239,126, 40, 21,  2,145,109,208,224,212,170,213,122,173, 86,163,128, 67,174, 92,252,184, 26, 13,  2,165, 82,189, 10, 13,  6,  7,  3, 67, 18, 97,  1,128,141,224,225,227,199,193,201,229,225,227,130,131, 22,103,  7, 15, 29, 14,  5,  6, 53, 30,
        },
        [_FN] = {
            42,117,106,117, 42, 93, 26,213, 26,157, 42,117,106,117, 42, 21, 90, 13, 82,255, 82,  5, 14,173,138,157,186,189, 42,117,106,117,128,149,128,249, 84, 62, 21, 63, 16, 34,124,215,255, 84, 57, 32,101,224,253, 87,127,120,255, 22,188, 52,188, 30,191, 32,245,216, 80,250,210,255,220,244,192,213, 64,121,104,123,104,125, 96,125, 84,124, 84,124, 96,101,127, 40,122,168,250,  0,234,160,255,162, 21, 55,  9, 27,  5, 13,  2,  6,  1,131,129,227,225,227,193,195,193,227,225,227,129,131,  1,  3,  1,  6,  2, 13,  5, 26, 11, 54,
        },
    };

    uint8_t layer = get_layer();

    oled_set_cursor(0, 0);
    oled_write_raw_P(ferris_logo_bg[layer], sizeof(ferris_logo_bg[layer]));
}

static void render_ferris(void) {
    static uint32_t anim_timer = 0;
    static uint8_t frame_idx = 0;


    uint8_t wpm = get_current_wpm();
    uint32_t anim_frame_time = ANIM_FRAME_TIME_SLOW;
    if(wpm > WPM_THRESHOLD_FAST) {
        anim_frame_time = ANIM_FRAME_TIME_FAST;
    }
    uint8_t cur_frame = 0;
    if(timer_elapsed32(anim_timer) > anim_frame_time) {
        anim_timer = timer_read32();
        frame_idx = (frame_idx + 1) % 2;
    }
    if(wpm > WPM_THRESHOLD_OFF) {
        cur_frame += frame_idx + 1;
    }

    uint8_t layer = get_layer();

    oled_set_cursor(0, 4);
    // Upper line
    if(cur_frame == 1) {
        oled_write_P(PSTR("\xd2\xd3"), false);
    } else {
        oled_write_P(PSTR("\xce\xcf"), false);
    }
    switch (layer) {
        case _LO:
            oled_write_P(PSTR("\xb8\xb9\xba\xbb"), false);
            break;
        case _HI:
            oled_write_P(PSTR("\xbc\xbd\xbe\xbf"), false);
            break;
        case _FN:
            oled_write_P(PSTR("\xd8\xd9\xda\xdb"), false);
            break;
        default:
            oled_write_P(PSTR("\xb1\xb2\xb3\xb4"), false);
            break;
    }
    if(cur_frame == 2) {
        oled_write_P(PSTR("\xf2\xf3"), false);
    } else {
        oled_write_P(PSTR("\xee\xef"), false);
    }
    // Lower line
    if(cur_frame == 1) {
        oled_write_P(PSTR("\xd4\xd5"), false);
    } else {
        oled_write_P(PSTR("\xd0\xd1"), false);
    }
    oled_write_P(PSTR("\xb5\xb6\xb7"), false);
    if(cur_frame == 2) {
        oled_write_P(PSTR("\xf4\xf5 "), false);
    } else {
        oled_write_P(PSTR("\xf0\xf1 "), false);
    }
}

void render_os(void) {
    oled_set_cursor(0, 8);
    oled_write_P(PSTR(" \x98\x99\x9a\x9b\x9c\x9d "), false);

    if(layer_state_is(_MAC)) {
        oled_write_P(PSTR("   \x9e\x9f   "), false);
    } else {
        oled_write_P(PSTR(" \x9e\x9f     "), false);
    }
}

void render_mods(uint8_t modifiers) {
    static const char PROGMEM shift_off_1[] = {0x20, 0x80, 0x81, 0x82, 0};
    static const char PROGMEM shift_off_2[] = {0x20, 0xa0, 0xa1, 0xa2, 0};
    static const char PROGMEM shift_on_1[] = {0x20, 0xc0, 0xc1, 0xc2, 0};
    static const char PROGMEM shift_on_2[] = {0x20, 0xe0, 0xe1, 0xe2, 0};

    static const char PROGMEM ctrl_off_1[] = {0x20, 0x86, 0x87, 0x88, 0};
    static const char PROGMEM ctrl_off_2[] = {0x20, 0xa6, 0xa7, 0xa8, 0};
    static const char PROGMEM ctrl_on_1[] = {0x20, 0xc6, 0xc7, 0xc8, 0};
    static const char PROGMEM ctrl_on_2[] = {0x20, 0xe6, 0xe7, 0xe8, 0};

    static const char PROGMEM gui_off_1[] = {0x83, 0x84, 0x85, 0x20, 0};
    static const char PROGMEM gui_off_2[] = {0xa3, 0xa4, 0xa5, 0x20, 0};
    static const char PROGMEM gui_on_1[] = {0xc3, 0xc4, 0xc5, 0x20, 0};
    static const char PROGMEM gui_on_2[] = {0xe3, 0xe4, 0xe5, 0x20, 0};

    static const char PROGMEM alt_off_1[] = {0x89, 0x8a, 0x8b, 0x20, 0};
    static const char PROGMEM alt_off_2[] = {0xa9, 0xaa, 0xab, 0x20, 0};
    static const char PROGMEM alt_on_1[] = {0xc9, 0xca, 0xcb, 0x20, 0};
    static const char PROGMEM alt_on_2[] = {0xe9, 0xea, 0xeb, 0x20, 0};

    oled_set_cursor(0, 11);
    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }
    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }
    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }
    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

// Render Menu
static void render_menu_line(const char *data, uint8_t pos, bool on) {
    static const char PROGMEM menu_top_off[] = "\x8c\x8d\x8d\x8d\x8d\x8d\x8d\x8e";
    static const char PROGMEM menu_top_on[] = "\xac\xad\xad\xad\xad\xad\xad\xae";
    static const char PROGMEM menu_left_off[] = "\x8f";
    static const char PROGMEM menu_left_on[] = "\xaf";
    static const char PROGMEM menu_right_off[] = "\x90";
    static const char PROGMEM menu_right_on[] = "\xb0";
    static const char PROGMEM menu_mid_off[] = "\xcc\xcd\xcd\xcd\xcd\xcd\xcd\x8e";
    static const char PROGMEM menu_mid_on[] = "\xec\xed\xed\xed\xed\xed\xed\xae";
    static const char PROGMEM menu_bottom[] = "\x91\x92\x92\x92\x92\x92\x92\x20";

    if(on) {
        if(pos == 0) {
            oled_write_P(menu_top_on, false);
        } else {
            oled_write_P(menu_mid_on, false);
        }
        oled_write_P(menu_left_on, false);
        oled_write_P(data, true);
        oled_write_P(menu_right_on, false);
    } else {
        if(pos == 0) {
            oled_write_P(menu_top_off, false);
        } else {
            oled_write_P(menu_mid_off, false);
        }
        oled_write_P(menu_left_off, false);
        oled_write_P(data, false);
        oled_write_P(menu_right_off, false);
    }

    if(pos == MENU_MAX_ITEMS - 1) {
        oled_write_P(menu_bottom, false);
    }
}

static void render_menu(void) {
    for(uint8_t i = 0; i < MENU_MAX_ITEMS; i++) {
        render_menu_line(menu_line[i], i, menu_selected == i);
    }
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_background();
        render_ferris();
        render_os();
        render_mods(get_mods()|get_oneshot_mods());
    } else {
        render_menu();
    }

    return false;
}
#endif

// Synchronisation
typedef struct _master_to_slave_t {
    uint8_t menu_selected;
} master_to_slave_t;

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        // sync every 20ms
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 20) {
            master_to_slave_t m2s = {menu_selected};
            if(transaction_rpc_send(USER_SYNC_MENU, sizeof(m2s), &m2s)) {
                last_sync = timer_read32();
            }
        }
    }
}

void user_sync_menu_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    menu_selected = m2s->menu_selected;
}

// General functions
void keyboard_post_init_user(void) {
    debug_enable = true;
    transaction_register_rpc(USER_SYNC_MENU, user_sync_menu_slave_handler);
    rgblight_sethsv(HSV_WHITE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case _MACRO:
            if (record->event.pressed) {
                SEND_STRING(macros[menu_selected]);
            }
            break;
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    set_layer_light(get_highest_layer(state));
    return state;
}
