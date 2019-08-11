#include QMK_KEYBOARD_H

enum crkbd_layers {
	_BASE,
	_GAMING,
	_VIM,
	_CODING,
	_ALT,
	_FUNCTIONS, 
	_RESET
};

// layer keys
#define GAMING 		DF(_GAMING)
#define VIM 		TG(_VIM)
#define CODING 		MO(_CODING)
#define ALT 		LM(_ALT, MOD_LALT)
#define FUNCTIONS 	MO(_FUNCTIONS)

// aliases for clarity
#define ________ KC_TRNS
#define XXXXXXXX KC_NO

// move workspaces down / up; assuming alt is already pressed
#define WORKDOWN LCTL( KC_DOWN )
#define WORKUP   LCTL( KC_UP )

// vim next tab
enum custom_kcs {
	BASE = SAFE_RANGE,
	VIM_INSERT,
	ESC,
	CC_VNT
};

// startup values
bool isInVimInsert = false;

// custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case BASE:
			if (record->event.pressed) {
				// switching off the vim layer and exiting vim insert mode
				// (which is the same layer as the base layer,
				// but the bool is true, as to change ESC's behavior)
				layer_off(_VIM);
				isInVimInsert = false;
			}
			break;
		case ESC:
			if (isInVimInsert) {
				if (record->event.pressed) {
					layer_on(_VIM);
				}
			} else {
				// to behave exactly the same as a native ESC,
				// a press event is sent when the key is pressed
				// and a release event is sent when the key is
				// released, instead of just sending a tap
				if (record->event.pressed) {
					register_code(KC_ESC);
				} else {
					unregister_code(KC_ESC);
				}
			}
			break;
		case VIM_INSERT:
			// vim insert mode is nothing else than the base layer,
			// but the bool isInVimInsert is set to true, as to indicate
			// that ESC should switch back to the vim layer instead of
			// sending a native ESC key code
			if (record->event.pressed) {
				layer_off(_VIM);
				isInVimInsert = true;
				return false;
			}
			break;
		case CC_VNT:
			if (record->event.pressed) {
				// when CC_VNT has been pressed
				// types :tabnext<ENTER>
				unregister_code( KC_LALT );
				SEND_STRING( SS_LSFT(";") "tabnext\n");
				register_code( KC_LALT );
			}
			break;
	}
	return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT(
			KC_TAB,    KC_Q,     KC_W,     KC_E,     LT(_RESET, KC_R), KC_T,             KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
			ESC,       KC_A,     KC_S,     KC_D,     KC_F,             KC_G,             KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
			XXXXXXXX,  KC_Z,     KC_X,     KC_C,     KC_V,             KC_B,             KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  XXXXXXXX,
			                               KC_LCTL,  CODING,           ALT,              KC_ENT,   SFT_T(KC_SPC),   FUNCTIONS
			),

	[_GAMING] = LAYOUT(
			XXXXXXXX, KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,             XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
			XXXXXXXX, KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,             XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
			XXXXXXXX, KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,             XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
			                              XXXXXXXX, KC_SPC,   XXXXXXXX,         XXXXXXXX, XXXXXXXX, BASE
			),

	[_VIM] = LAYOUT(
			XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, 		XXXXXXXX, XXXXXXXX, VIM_INSERT, XXXXXXXX, XXXXXXXX, XXXXXXXX,
			KC_ESC,   XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, 		KC_LEFT,  KC_DOWN,  KC_UP,      KC_RIGHT, XXXXXXXX, XXXXXXXX,
			BASE,     XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, 		XXXXXXXX, XXXXXXXX, XXXXXXXX,   XXXXXXXX, XXXXXXXX, XXXXXXXX,
										  ________, ________, ________, 		XXXXXXXX, XXXXXXXX, ________
			),

	[_CODING] = LAYOUT(
			KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,             KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_DEL,
			________, XXXXXXXX, XXXXXXXX, KC_MINS,  KC_PLUS,  KC_EQL,           XXXXXXXX, KC_LBRC,  KC_RBRC,  XXXXXXXX, XXXXXXXX, XXXXXXXX,
			________, XXXXXXXX, XXXXXXXX, VIM,      XXXXXXXX, XXXXXXXX,         XXXXXXXX, KC_LPRN,  KC_RPRN,  XXXXXXXX, XXXXXXXX, ________,
			                              ________, ________, ________,         ________, ________, ________
			),

	[_ALT] = LAYOUT( 
			________, ________, ________, ________, KC_F4,    ________,         ________, ________, ________, ________, ________, ________, 
			________, CC_VNT,   ________, ________, ________, ________,         ________, ________, ________, ________, ________, ________, 
			________, ________, ________, ________, ________, ________,         ________, WORKDOWN,	WORKUP,   ________, ________, ________,
			                              ________, ________, ________,         ________, ________, ________
			),

	[_FUNCTIONS] = LAYOUT( 
			________, KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,            KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
			________, ________, ________, ________, ________, GAMING,           KC_MS_L,  KC_MS_D,  KC_MS_U,  KC_MS_R,  KC_LGUI,  ________,
			________, ________, ________, ________, ________, ________,         ________, KC_BTN1,  KC_BTN2,  ________, ________, ________,
			                              ________, ________, ________,         ________, ________, ________
			),

	[_RESET] = LAYOUT(
			________, ________, ________, ________, ________, ________,         ________, ________, ________, ________, ________, ________, 
			________, ________, ________, ________, ________, ________,         ________, ________, ________, ________, ________, ________,
			________, ________, ________, ________, ________, ________,         ________, ________, ________, ________, ________, ________,
			                              ________, ________, RESET,            ________, ________, ________
			)
};
