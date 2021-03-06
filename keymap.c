#include QMK_KEYBOARD_H

enum crkbd_layers {
    _BASE,
    _COLEMAK_DH,
    _RIGHT_1,
    _RIGHT_2,
    _RIGHT_3,
    _GAMING,
    _GAMING_NUMPAD,
    _CODING,
    _ALT,
    _FUNCTIONS,
    _RESET
};

// layer keys
#define GAMING      DF(_GAMING)
#define CODING      MO(_CODING)
#define ALT         LM(_ALT, MOD_LALT)
#define FUNCTIONS   LT(_FUNCTIONS, KC_RALT)

// aliases for clarity
#define ________ KC_TRNS
#define XXXXXXXX KC_NO

enum custom_kcs {
    BASE = SAFE_RANGE,
};

int isColemak = false;

// custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BASE:
            if (record->event.pressed) {
                layer_off(_GAMING);
                layer_on(_BASE);
                return false;
            }
            break;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
            KC_BSPC,      KC_P,         KC_O,         KC_I,         KC_U,         KC_Y,               KC_T,         LT(_RESET, KC_R), KC_E,     KC_W,         KC_Q,         KC_TAB,
            KC_QUOT,      KC_SCLN,      KC_L,         KC_K,         KC_J,         KC_H,               KC_G,         LT(_RIGHT_1,KC_F),LT(_RIGHT_2,KC_D),LT(_RIGHT_3,KC_S),KC_A,KC_ESC,
            KC_BSLASH,    KC_SLSH,      KC_DOT,       KC_COMM,      KC_M,         KC_N,               KC_B,         KC_V,         KC_C,         KC_X,         KC_Z,         DF(_COLEMAK_DH),
                                                      FUNCTIONS,    SFT_T(KC_SPC),LGUI_T(KC_ENT),     ALT,          CODING,       KC_LCTL
            ),

    [_COLEMAK_DH] = LAYOUT(
            KC_BSPC,      KC_SCLN,      KC_Y,         KC_U,         KC_L,         KC_J,               KC_B,         LT(_RESET, KC_P), KC_F,         KC_W,         KC_Q,         KC_TAB,
            KC_QUOT,      KC_O,         KC_I,         KC_E,         KC_N,         KC_K,               KC_G,         LT(_RIGHT_1,KC_T),LT(_RIGHT_2,KC_S),LT(_RIGHT_3,KC_R),KC_A,KC_ESC,
            KC_BSLASH,    KC_SLSH,      KC_DOT,       KC_COMM,      KC_H,         KC_M,               KC_V,         KC_D,         KC_C,         KC_X,         KC_Z,         DF(_BASE),
                                                      FUNCTIONS,    SFT_T(KC_SPC),LGUI_T(KC_ENT),     ALT,          CODING,       KC_LCTL
            ),

    [_RIGHT_1] = LAYOUT(
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     KC_RIGHT,     KC_UP,        KC_DOWN,      KC_LEFT,            ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
                                                      ________,     ________,     ________,           ________,     ________,     ________
            ),

    [_RIGHT_2] = LAYOUT(
            DM_REC2,      DM_REC1,      KC_9,         KC_8,         KC_7,         ________,           ________,     ________,     ________,     ________,     ________,     ________,
            DM_PLY2,      DM_PLY1,      KC_6,         KC_5,         KC_4,         ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     KC_3,         KC_2,         KC_1,         ________,           ________,     ________,     ________,     ________,     ________,     ________,
                                                      ________,     KC_0,         ________,           ________,     ________,     ________
            ),

    [_RIGHT_3] = LAYOUT(
            ________,     ________,     ________,     S(KC_RBRC),   S(KC_LBRC),   ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     KC_RBRC,      KC_LBRC,      ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     KC_RPRN,      KC_LPRN,      ________,           ________,     ________,     ________,     ________,     ________,     ________,
                                                      ________,     ________,     ________,           ________,     ________,     ________
            ),

    [_GAMING] = LAYOUT(
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           KC_R,         KC_E,         KC_W,         KC_Q,         KC_TAB,       XXXXXXXX,
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           KC_F,         KC_D,         KC_S,         KC_A,         KC_ESC,       XXXXXXXX,
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           KC_V,         KC_C,         KC_X,         KC_Z,         KC_LSFT,      XXXXXXXX,
                                                      DF(_BASE),    XXXXXXXX,     XXXXXXXX,           XXXXXXXX,     KC_SPC,       MO(_GAMING_NUMPAD)
            ),

    [_GAMING_NUMPAD] = LAYOUT(
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           KC_5,         KC_4,         KC_3,         KC_2,         KC_1,       XXXXXXXX,
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           KC_0,         KC_9,         KC_8,         KC_7,         KC_6,       XXXXXXXX,
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,   XXXXXXXX,
                                                      ________,     ________,     ________,           ________,     KC_SPC,       ________
            ),

    [_CODING] = LAYOUT(
            KC_DEL,       KC_0,         KC_9,         KC_8,         KC_7,         KC_6,               KC_5,         KC_4,         KC_3,         KC_2,         KC_1,         KC_GRV,
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     KC_RBRC,      KC_LBRC,      XXXXXXXX,           KC_EQL,       KC_PLUS,      KC_MINS,      XXXXXXXX,     XXXXXXXX,     ________,
            ________,     XXXXXXXX,     XXXXXXXX,     KC_RPRN,      KC_LPRN,      XXXXXXXX,           XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     ________,
                                                      ________,     ________,     ________,           ________,     ________,     ________
            ),

    [_ALT] = LAYOUT(
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
                                                      CODING,       ________,     ________,           ________,     ________,     ________
            ),

    [_FUNCTIONS] = LAYOUT(
            KC_F11,       KC_F10,       KC_F9,        KC_F8,        KC_F7,        KC_F6,              KC_F5,        KC_F4,        KC_F3,        KC_F2,        KC_F1,        ________,
            ________,     KC_LGUI,      KC_MS_R,      KC_MS_U,      KC_MS_D,      KC_MS_L,            GAMING,       ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     KC_BTN2,      KC_BTN1,      ________,           ________,     ________,     ________,     ________,     ________,     ________,
                                                      ________,     ________,     ________,           ________,     ________,     ________
            ),

    [_RESET] = LAYOUT(
            UC_MOD,       ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
                                                      ________,     ________,     ________,           RESET,        ________,     ________
            )
};
