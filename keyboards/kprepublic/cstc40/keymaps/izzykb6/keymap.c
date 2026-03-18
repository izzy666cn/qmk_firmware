/* Copyright 2023
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum {
    TD_SCLN_COLN = 0,
    TD_QUOT_DQUO,
};

enum custom_keycodes {
    // TEMP: rctrl / quote(if alone)
    MOD_QUOT = SAFE_RANGE,
    // ~/
    TILD_SLSH,
    // =>
    FAT_ARROW,
    // ->
    THIN_ARROW,
    // ; + Enter
    SCLN_ENT,
};

static bool mod_quot_active  = false;
static bool mod_quot_as_mods = false;

// jk->esc
const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
// io->backspace
const uint16_t PROGMEM io_combo[] = {KC_I, KC_O, COMBO_END};
// kl->enter
const uint16_t PROGMEM kl_combo[] = {KC_K, KC_L, COMBO_END};
// m,->rshift
const uint16_t PROGMEM mcomm_combo[] = {KC_M, KC_COMM, COMBO_END};
// TEMP: ,.->_
const uint16_t PROGMEM commdot_combo[] = {KC_COMM, KC_DOT, COMBO_END};


combo_t key_combos[] = {
    COMBO(jk_combo, KC_ESC),
    COMBO(io_combo, KC_BSPC),
    COMBO(kl_combo, KC_ENT),
    COMBO(mcomm_combo, OS_LSFT),
    COMBO(commdot_combo, KC_UNDS),
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_SCLN_COLN] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_COLN),
    [TD_QUOT_DQUO] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (mod_quot_active && keycode != MOD_QUOT && record->event.pressed && !mod_quot_as_mods) {
        register_mods(MOD_BIT(KC_RCTL));
        mod_quot_as_mods = true;
    }

    switch (keycode) {
        case MOD_QUOT:
            if (record->event.pressed) {
                mod_quot_active  = true;
                mod_quot_as_mods = false;
            } else {
                if (mod_quot_as_mods) {
                    unregister_mods(MOD_BIT(KC_RCTL));
                } else {
                    tap_code(KC_QUOT);
                }
                mod_quot_active  = false;
                mod_quot_as_mods = false;
            }
            return false;
        case TILD_SLSH:
            if (record->event.pressed) {
                SEND_STRING("~/");
            }
            return false;
        case FAT_ARROW:
            if (record->event.pressed) {
                SEND_STRING("=>");
            }
            return false;
        case THIN_ARROW:
            if (record->event.pressed) {
                SEND_STRING("->");
            }
            return false;
        case SCLN_ENT:
            if (record->event.pressed) {
                tap_code(KC_SCLN);
                tap_code(KC_ENT);
            }
            return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Base
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |   =  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |LCtrl |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |   '  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | SHIFT|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | SHIFT |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | MO(4)| MO(5)| LAlt | LGUI |Lower |    Space    |Raise |LCAG  |LCtrl |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[0] = LAYOUT_planck_mit(
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,     KC_U,     KC_I,     KC_O,    KC_P,     KC_EQL,
    KC_LCTL,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,     KC_J,     KC_K,     KC_L,   KC_SCLN,  TD(TD_QUOT_DQUO),
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,    KC_N,     KC_M,     KC_COMM,  KC_DOT,  KC_SLSH,  KC_RSFT,
    MO(4),    MO(5),    KC_LALT,  KC_LGUI,  TL_LOWR,  KC_SPC,  TL_UPPR,  LCAG(KC_NO),  KC_LCTL,  KC_UP,   KC_RGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |  ~/  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   -  |   +  |   ~  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Caps |      |      |      |      |      |   {  |   (  |   )  |   }  |   _  | pipe |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | GUI+X|      |      |      |      |   [  |   ]  |      |  ->  |  =>  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |  Backspace  |      | Left | Down |  Up  | Right|
 * `-----------------------------------------------------------------------------------'
 */
// GUI+X for Emacs(M-x)
[1] = LAYOUT_planck_mit(
    TILD_SLSH,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_AMPR,     KC_ASTR,     KC_MINS,  KC_PLUS,  KC_TILD,
    KC_CAPS,  _______,  _______,   _______,  _______, _______,  KC_LCBR,  KC_LPRN,     KC_RPRN,     KC_RCBR,   KC_UNDS,  KC_PIPE,
    _______,  _______,  LGUI(KC_X),  _______,  _______,  _______,  _______,  KC_LBRC,  KC_RBRC,  _______,  THIN_ARROW, FAT_ARROW,
    _______,  _______,  _______,  _______,  _______,  KC_BSPC,  _______,  KC_LEFT,     KC_DOWN,     KC_UP,  KC_RIGHT
),
/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | ` |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |      |      |      |      |      |      |      |      |      |      |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |Ctrl+X|Ctrl+C|      |      |      |      |      |Pg Up |Pg Dn |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |     Enter   |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
// Ctrl+X Ctrl+C for Emacs
[2] = LAYOUT_planck_mit(
    _______,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_GRV,
    KC_DEL,   _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_BSLS,
    _______,  _______,  LCTL(KC_X),LCTL(KC_C),  _______,  _______,  _______,  _______,  _______,  KC_PGUP,  KC_PGDN,  _______,
    _______,  _______,  _______,  _______,  _______,  KC_ENTER,  _______,  _______,  _______,  _______,  _______
),

/* Layer4:Numpad + F Keys
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |   7  |   8  |   9  |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   4  |   5  |   6  |   .  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   1  |   2  |   3  |   0  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |   LLCK      |      |   0  |   .  |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[4] = LAYOUT_planck_mit(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_7,  KC_8,  KC_9,  _______,     _______,
    _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_4,  KC_5,  KC_6,  KC_DOT,  _______,
    _______,  KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_1,  KC_2,  KC_3,  KC_0,  _______,
    _______,  _______,  _______,  _______,  _______,  QK_LLCK,  _______,  KC_0,  KC_DOT,  _______,  _______
),

/* Layer5:Jetbrains IDE
 * ,-----------------------------------------------------------------------------------.
 * |      |  F7  |  F8  | A+G+R|      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |S(F8) |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |   LLCK      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[5] = LAYOUT_planck_mit(
    _______,  KC_F7,    KC_F8,    LAG(KC_R), _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  S(KC_F8), _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  QK_LLCK,  _______,  _______,  _______,  _______,  _______
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  | Mode | HUE+ | HUE- | SAT+ | SAT- | VAL+ | VAL- |  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Vol- | Mute | Vol+ | Next | Play |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[3] = LAYOUT_planck_mit(
    _______,  QK_BOOT,  DB_TOGG,  RM_TOGG,  RM_NEXT,  RM_HUEU,  RM_HUED,  RM_SATU,  RM_SATD,  RM_VALU,  RM_VALD,  KC_DEL,
    _______,  KC_VOLD,  KC_MUTE,  KC_VOLU,  KC_MNXT,  KC_MPLY,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______
)

};

#ifdef RGB_MATRIX_ENABLE
void keyboard_post_init_user(void) {
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);

    return state;
}

bool rgb_matrix_indicators_user(void) {
    // Bottom row: left-most is [3][0], TL_LOWR is [3][4], TL_UPPR is [3][7].
    uint8_t led_bottom_left    = g_led_config.matrix_co[3][0];
    uint8_t led_bottom_mo5     = g_led_config.matrix_co[3][1];
    uint8_t led_lower_caps     = g_led_config.matrix_co[1][0];
    uint8_t led_left_of_space  = g_led_config.matrix_co[3][4];
    uint8_t led_right_of_space = g_led_config.matrix_co[3][7];
    uint8_t led_lshift         = g_led_config.matrix_co[2][0];
    uint8_t led_rshift         = g_led_config.matrix_co[2][11];

    rgb_matrix_set_color_all(0, 0, 0);

    switch (get_highest_layer(layer_state)) {
        case 1:
            if (led_left_of_space != NO_LED) {
                rgb_matrix_set_color(led_left_of_space, 0, 255, 0);
            }
            break;
        case 2:
            if (led_right_of_space != NO_LED) {
                rgb_matrix_set_color(led_right_of_space, 255, 255, 0);
            }
            break;
        case 3:
            if (led_left_of_space != NO_LED) {
                rgb_matrix_set_color(led_left_of_space, 255, 0, 0);
            }
            if (led_right_of_space != NO_LED) {
                rgb_matrix_set_color(led_right_of_space, 255, 0, 0);
            }
            break;
        case 4: {
            if (led_bottom_left != NO_LED) {
                rgb_matrix_set_color(led_bottom_left, 0, 0, 255);
            }
            // Layer4: highlight F-zone in cyan and numpad-zone in blue.
            const uint8_t l4_f_keys[][2] = {
                // F-zone
                {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
                {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
            };
            const uint8_t l4_numpad_keys[][2] = {
                {0, 7}, {0, 8}, {0, 9},
                {1, 7}, {1, 8}, {1, 9},
                {2, 7}, {2, 8}, {2, 9}, {2, 10},
            };
            for (uint8_t i = 0; i < ARRAY_SIZE(l4_f_keys); i++) {
                uint8_t led = g_led_config.matrix_co[l4_f_keys[i][0]][l4_f_keys[i][1]];
                if (led != NO_LED) {
                    rgb_matrix_set_color(led, 0, 180, 255);
                }
            }
            for (uint8_t i = 0; i < ARRAY_SIZE(l4_numpad_keys); i++) {
                uint8_t led = g_led_config.matrix_co[l4_numpad_keys[i][0]][l4_numpad_keys[i][1]];
                if (led != NO_LED) {
                    rgb_matrix_set_color(led, 0, 0, 255);
                }
            }
            break;
        }
        case 5: {
            // Layer5
            const uint8_t l5_magenta_keys[][2] = {
                {3, 1}, // MO(5) indicator key
                {0, 1}, // Q
                {0, 2}, // W
                {0, 3}, // E
                {1, 2}, // S
                {3, 5}, // QK_LLCK
            };
            for (uint8_t i = 0; i < ARRAY_SIZE(l5_magenta_keys); i++) {
                uint8_t led = g_led_config.matrix_co[l5_magenta_keys[i][0]][l5_magenta_keys[i][1]];
                if (led != NO_LED) {
                    rgb_matrix_set_color(led, 255, 0, 180);
                }
            }
            // Keep a strong marker on MO(5) key.
            if (led_bottom_mo5 != NO_LED) {
                rgb_matrix_set_color(led_bottom_mo5, 255, 0, 255);
            }
            break;
        }
        default:
            break;
    }

    if (host_keyboard_led_state().caps_lock && led_lower_caps != NO_LED) {
        rgb_matrix_set_color(led_lower_caps, 255, 0, 0);
    }

    // One-shot Shift indicator (OS_LSFT): light while active, off when consumed/cleared.
    if ((get_oneshot_mods() | get_oneshot_locked_mods()) & MOD_MASK_SHIFT) {
        if (led_lshift != NO_LED) {
            rgb_matrix_set_color(led_lshift, 255, 140, 0);
        }
        if (led_rshift != NO_LED) {
            rgb_matrix_set_color(led_rshift, 255, 140, 0);
        }
    }

    return false;
}
#endif
