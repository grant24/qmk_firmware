#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define LA_GME TG(GME)
#define SC_SS G(S(KC_S))
#define SC_FIND C(KC_F)

enum layers {
    DEF, // Default
    SYM, // Symbols
    NAV, // Navigation
    NUM, // Numbers & Function
    GME, // Game
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,

    SW_WIN,  // Switch to next window  (alt-tab)
    SW_DESK, // Switch to next desktop (gui-tab)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SCLN,
                                   LA_NAV,  KC_LSFT, KC_SPC,  LA_SYM
    ),

    [SYM] = LAYOUT(
        KC_ESC,  KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD, KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV,
        KC_MINS, KC_ASTR, KC_EQL,  KC_UNDS, KC_DLR,  KC_HASH, OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_PLUS, KC_PIPE, KC_AT,   KC_BSLS, KC_PERC, LA_GME,  KC_SLSH, KC_AMPR, KC_QUES, KC_EXLM,
                                   _______, _______, _______, _______
    ),

    [NAV] = LAYOUT(
        SW_DESK, SW_WIN,  KC_MPRV, KC_MNXT, KC_MPLY, QK_BOOT, KC_HOME, KC_UP,   KC_END,  KC_DEL,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_BTN1, KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC,
        SC_SS,   SC_FIND, KC_VOLD, KC_VOLU, KC_MUTE, QK_REP,  KC_PGUP, KC_TAB,  KC_PGDN, KC_ENT,
                                   _______, _______, _______, _______
    ),

    [NUM] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_F11,  KC_F12,  OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
                                   _______, _______, _______, _______
    ),

    [GME] = LAYOUT(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,
                                   KC_LALT, KC_SPC,  KC_ESC,  LA_GME
    )
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case LA_GME:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case LA_GME:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_desk_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LALT, KC_TAB, SW_WIN,
        keycode, record
    );
    update_swapper(
        &sw_desk_active, KC_LGUI, KC_TAB, SW_DESK,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
