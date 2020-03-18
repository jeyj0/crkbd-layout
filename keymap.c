#include QMK_KEYBOARD_H

enum crkbd_layers {
    _BASE,
    _QUICK_ACCESS_RIGHT,
    _GAMING,
    _GAMING_NUMPAD,
    _VIM,
    _CODING,
    _ALT,
    _FUNCTIONS,
    _RESET
};

enum unicode_names {
    A_L_UML, // a-lower-umlaut
    A_U_UML, // a-upper-umlaut
    O_L_UML,
    O_U_UML,
    U_L_UML,
    U_U_UML,
    S_ZET_L, // sz-lower
    S_ZET_U  // sz-upper
};

const uint32_t PROGMEM unicode_map[] = {
    [A_L_UML] = 0x00E4,
    [A_U_UML] = 0x00C4,
    [O_L_UML] = 0x00F6,
    [O_U_UML] = 0x00D6,
    [U_L_UML] = 0x00FC,
    [U_U_UML] = 0x00DC,
    [S_ZET_L] = 0x00DF,
    [S_ZET_U] = 0x1E9E,
};

// layer keys
#define GAMING      DF(_GAMING)
#define VIM         TG(_VIM)
#define CODING      MO(_CODING)
#define ALT         LM(_ALT, MOD_LALT)
#define FUNCTIONS   MO(_FUNCTIONS)

// aliases for clarity
#define ________ KC_TRNS
#define XXXXXXXX KC_NO

// move workspaces down / up; assuming alt is already pressed
#define WORKDOWN LCTL( KC_DOWN )
#define WORKUP   LCTL( KC_UP )

// vim next tab
enum custom_kcs {
    BASE = SAFE_RANGE,
    ESC,
    TT_VIM,
    VIM_ENT,
    VIM_W,
    VIM_B,
    VIM_D,
    VIM_C,
    VIM_U,
    VIM_I,
    VIM_H,
    VIM_L
};

enum vim_actions {
    VA_NONE,
    VA_DELETE,
    VA_CHANGE
};

enum vim_motion_prefixes {
    VMP_NONE,
    VMP_I
};

enum vim_modes {
    VM_OFF,
    VM_NORMAL,
    VM_INSERT
};

// startup values
int vim_mode = VM_OFF;
int vim_action = VA_NONE;
int vim_motion_prefix = VMP_NONE;
int vim_toggle_key_presses = 0;

void switch_to_vim_insert(void) {
    // tap_code(KC_INSERT);
    layer_off(_VIM);
    vim_mode = VM_INSERT;
}

void reset_vim_commands(void) {
    vim_action = VA_NONE;
    vim_motion_prefix = VMP_NONE;
}

void switch_to_vim_normal(void) {
    // tap_code(KC_INSERT);
    layer_on(_VIM);
    vim_mode = VM_NORMAL;
}

void switch_on_vim_mode(void) {
    reset_vim_commands();
    switch_to_vim_normal();
    vim_mode = VM_NORMAL;
}

void switch_off_vim_mode(void) {
    layer_off(_VIM);
    // if (vim_mode == VM_NORMAL) tap_code(KC_INSERT);
    vim_mode = VM_OFF;
}

bool perform_action_on_motion(void (*motion)(bool) ) {
    switch (vim_action) {
        case VA_NONE:
            (*motion)(false);
            break;
        case VA_DELETE:
            (*motion)(true);
            tap_code(KC_BSPC);
            vim_action = VA_NONE;
            break;
        case VA_CHANGE:
            (*motion)(true);
            tap_code(KC_BSPC);
            switch_to_vim_insert();
            vim_action = VA_NONE;
            return false; break;
    }
    reset_vim_commands();
    return true;
}

// custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (keycode == TT_VIM) {
            vim_toggle_key_presses++;
        } else {
            vim_toggle_key_presses = 0;
        }
    }

    switch (keycode) {
        case BASE:
            if (record->event.pressed) {
                switch_off_vim_mode();
                layer_off(_GAMING);
                layer_on(_BASE);
                return false;
            }
            break;
        case TT_VIM:
            if (record->event.pressed && vim_mode == VM_OFF) {
                switch_on_vim_mode();
            } else if (vim_toggle_key_presses < 3) {
                switch_off_vim_mode();
            } else {
                vim_toggle_key_presses = 0;
            }
            return false; break;
        case ESC:
            if (vim_mode == VM_INSERT && record->event.pressed) {
                switch_to_vim_normal();
            } else {
                // to behave exactly the same as a native ESC,
                // a press event is sent when the key is pressed
                // and a release event is sent when the key is
                // released, instead of just sending a tap
                if (record->event.pressed) {
                    if (vim_mode == VM_NORMAL) reset_vim_commands();
                    register_code(KC_ESC);
                } else {
                    unregister_code(KC_ESC);
                }
            }
            return false; break;
        case VIM_I:
            if (record->event.pressed) {
                if (vim_action == VA_NONE) {
                    switch_to_vim_insert();
                } else {
                    vim_motion_prefix = VMP_I;
                }
                return false;
            }
            break;
        case VIM_ENT:
            if (record->event.pressed) {
                tap_code(KC_DOWN);
                tap_code(KC_HOME);
            }
            break;
        case VIM_D:
            if (record->event.pressed) {
                vim_action = VA_DELETE;
            }
            break;
        case VIM_C:
            if (record->event.pressed) {
                vim_action = VA_CHANGE;
            }
            break;
        case VIM_H:
            if (record->event.pressed) {
                void do_motion(bool with_selection) {
                    if (with_selection) register_code(KC_LSFT);
                    tap_code(KC_LEFT);
                    if (with_selection) unregister_code(KC_LSFT);
                }
                return perform_action_on_motion(do_motion);
            }
            break;
        case VIM_L:
            if (record->event.pressed) {
                void do_motion(bool with_selection) {
                    if (with_selection) register_code(KC_LSFT);
                    tap_code(KC_RIGHT);
                    if (with_selection) unregister_code(KC_LSFT);
                }
                return perform_action_on_motion(do_motion);
            }
            break;
        case VIM_W:
            if (record->event.pressed) {
                void do_motion(bool with_selection) {
                    if (with_selection) {
                        switch (vim_motion_prefix) {
                            case VMP_I:
                                // going right once makes it work more like actual vim
                                tap_code(KC_RIGHT);
                                register_code(KC_LCTL);
                                tap_code(KC_LEFT);
                                unregister_code(KC_LCTL);
                                break;
                        }
                        register_code(KC_LSFT);
                    }
                    register_code(KC_LCTL);
                    tap_code(KC_RIGHT);
                    unregister_code(KC_LCTL);
                    if (with_selection) unregister_code(KC_LSFT);
                }
                return perform_action_on_motion(do_motion);
            }
            break;
        case VIM_B:
            if (record->event.pressed) {
                if (vim_motion_prefix != VMP_NONE) {
                    // no motion prefixes supported
                    reset_vim_commands();
                    break;
                }

                void do_motion(bool with_selection) {
                    if (with_selection) register_code(KC_LSFT);
                    register_code(KC_LCTL);
                    tap_code(KC_LEFT);
                    unregister_code(KC_LCTL);
                    if (with_selection) unregister_code(KC_LSFT);
                }
                return perform_action_on_motion(do_motion);
            }
            break;
        case VIM_U:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_Z);
                unregister_code(KC_LCTL);
            }
            break;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
            KC_BSPC,      KC_P,         KC_O,         KC_I,         KC_U,         KC_Y,               KC_T,         LT(_RESET, KC_R), KC_E,     KC_W,         KC_Q,         KC_TAB,
            KC_QUOT,      KC_SCLN,      KC_L,         KC_K,         KC_J,         KC_H,               KC_G,         LT(_QUICK_ACCESS_RIGHT, KC_F), KC_D, KC_S,KC_A,         ESC,
            KC_BSLASH,    KC_SLSH,      KC_DOT,       KC_COMM,      KC_M,         KC_N,               KC_B,         KC_V,         KC_C,         KC_X,         KC_Z,         TT_VIM,
                                                      FUNCTIONS,    SFT_T(KC_SPC),LGUI_T(KC_ENT),     ALT,          CODING,       KC_LCTL
            ),

    [_QUICK_ACCESS_RIGHT] = LAYOUT(
            ________,     ________,     XP(O_L_UML, O_U_UML), ________, XP(U_L_UML, U_U_UML), ________, ________,   ________,     ________,     ________,     ________,     ________,
            ________,     ________,     KC_RIGHT,     KC_UP,        KC_DOWN,      KC_LEFT,            ________,     ________,     ________,     XP(S_ZET_L, S_ZET_U), XP(A_L_UML, A_U_UML), ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
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

    [_VIM] = LAYOUT(
            KC_LEFT,      XXXXXXXX,     XXXXXXXX,     VIM_I,        VIM_U,        XXXXXXXX,           XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     VIM_W,        XXXXXXXX,     XXXXXXXX,
            XXXXXXXX,     XXXXXXXX,     VIM_L,        KC_UP,        KC_DOWN,      VIM_H,              XXXXXXXX,     XXXXXXXX,     VIM_D,        XXXXXXXX,     XXXXXXXX,     ESC,
            VIM,          XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,           VIM_B,        XXXXXXXX,     VIM_C,        XXXXXXXX,     XXXXXXXX,     BASE,
                                                      ________,     XXXXXXXX,     VIM_ENT,            ________,     ________,     ________
            ),

    [_CODING] = LAYOUT(
            KC_DEL,       KC_0,         KC_9,         KC_8,         KC_7,         KC_6,               KC_5,         KC_4,         KC_3,         KC_2,         KC_1,         KC_GRV,
            XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     KC_RBRC,      KC_LBRC,      XXXXXXXX,           KC_EQL,       KC_PLUS,      KC_MINS,      XXXXXXXX,     XXXXXXXX,     ________,
            ________,     XXXXXXXX,     XXXXXXXX,     KC_RPRN,      KC_LPRN,      XXXXXXXX,           XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     XXXXXXXX,     ________,
                                                      ________,     ________,     ________,           ________,     ________,     ________
            ),

    [_ALT] = LAYOUT(
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     KC_F4,        ________,     ________,     ________,     ________,
            ________,     ________,     ________,     ________,     ________,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
            ________,     ________,     ________,      WORKUP,       WORKDOWN,     ________,           ________,     ________,     ________,     ________,     ________,     ________,
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
