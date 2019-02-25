/* Copyright 2017 Robin  Hallabro-Kokko
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
#include "keymap_swedish.h"

enum {
  TD_COPY_PASTE = 1,
};

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || state->pressed==0) return SINGLE_TAP;
    return SINGLE_HOLD;
  }

  if (state->count == 2) {
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    if (state->pressed) return DOUBLE_HOLD;
    return DOUBLE_TAP;
  }

  return 6; //magic number. At some point this method will expand to work for more presses
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_COPY_PASTE] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_INS), LSFT(KC_INS))
};

extern keymap_config_t keymap_config;

#define _________ KC_TRNS
#define KC_BRKTL  RALT(KC_8)
#define KC_BRKTR  RALT(KC_9)
#define KC_LCBRC  RALT(KC_7)
#define KC_RCBRC  RALT(KC_0)
#define KC_CPYPST TD(TD_COPY_PASTE)
#define RALT_MOVE LM(_MOVEMENT, MOD_RALT)

#define MOVEMENT MO(_MOVEMENT)

#define _DVORAK 0
#define _QWERTY 1
#define _LOWER 2
#define _NUMPAD 3
#define _MOVEMENT 4

LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY(KC_INS) {
      layer_invert(_NUMPAD);
    }

    SEQ_ONE_KEY(NO_OSLH) {
      layer_invert(_QWERTY);
    }

    SEQ_ONE_KEY(KC_1) {
      SEND_STRING(SS_TAP(X_F1));
    }
    SEQ_ONE_KEY(KC_2) {
      SEND_STRING(SS_TAP(X_F2));
    }
    SEQ_ONE_KEY(KC_3) {
      SEND_STRING(SS_TAP(X_F3));
    }
    SEQ_ONE_KEY(KC_4) {
      SEND_STRING(SS_TAP(X_F4));
    }
    SEQ_ONE_KEY(KC_5) {
      SEND_STRING(SS_TAP(X_F5));
    }
    SEQ_ONE_KEY(KC_6) {
      SEND_STRING(SS_TAP(X_F6));
    }
    SEQ_ONE_KEY(KC_7) {
      SEND_STRING(SS_TAP(X_F7));
    }
    SEQ_ONE_KEY(KC_8) {
      SEND_STRING(SS_TAP(X_F8));
    }
    SEQ_ONE_KEY(KC_9) {
      SEND_STRING(SS_TAP(X_F9));
    }
    SEQ_ONE_KEY(KC_0) {
      SEND_STRING(SS_TAP(X_F10));
    }
    SEQ_ONE_KEY(KC_MINS) {
      SEND_STRING(SS_TAP(X_F11));
    }
    SEQ_ONE_KEY(KC_DOT) {
      SEND_STRING(SS_TAP(X_F12));
    }

    SEQ_ONE_KEY(KC_ESC) {
      reset_keyboard();
    }
  }
}

//enum custom_keycodes
//{
 //KC_BSPCD = SAFE_RANGE
//};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch (keycode) {
      // RALT_MOVE, disables RALT modifier
    case KC_LEFT:
    case KC_RIGHT:
    case KC_UP:
    case KC_DOWN:
      if (record->event.pressed) {
	uint8_t current_mods = get_mods();
	if (current_mods & MOD_RALT) {
	  clear_mods();
	  register_code(keycode);
	  unregister_code(keycode);
	  set_mods(current_mods);
	}
      }
      return false;
    }
  }

  return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT(
    KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_PGUP,                                  KC_PGDN,      KC_6,     KC_7,       KC_8,      KC_9,      KC_0,    KC_INS,
    NO_PLUS,   NO_MINS,   KC_DOT,    KC_COMMA,  KC_P,      KC_Y,      KC_HOME,                                   KC_END,      KC_F,     KC_G,       KC_C,      KC_R,      KC_L, KC_CPYPST,
    KC_LCTRL,  KC_A,      KC_O,      KC_E,      KC_U,      KC_I,      KC_LCBRC,                                KC_RCBRC,      KC_D,     KC_H,       KC_T,      KC_N,      KC_S,  KC_RCTRL,
    KC_LSPO,   KC_NUBS,   KC_Q,      KC_J,      KC_K,      KC_X,      KC_BRKTL,                                KC_BRKTR,      KC_B,     KC_M,       KC_W,      KC_V,      KC_Z,   KC_RSPC,
    KC_LEAD,   NO_QUOT,   KC_NUHS,   KC_EQL,    KC_TAB,    KC_SPACE,  KC_LGUI,     KC_LALT,        RALT_MOVE,  KC_ENTER, KC_BSPACE,   KC_DEL,      NO_AA,     NO_AE,   NO_OSLH,   KC_LEAD
  ),

  [_QWERTY] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________,      KC_Q,      KC_W,      KC_E,      KC_R,      KC_T, _________,                              _________,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P, _________,
    _________,      KC_A,      KC_S,      KC_D,      KC_F,      KC_G, _________,                              _________,      KC_H,      KC_J,      KC_K,      KC_L,   _______, _________,
    _________,      KC_Z,      KC_X,      KC_C,      KC_V,      KC_B, _________,                              _________,      KC_N,      KC_M,  KC_COMMA,    KC_DOT,   KC_NUBS, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),

  [_NUMPAD] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,      KC_7,      KC_8,      KC_9, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,      KC_4,      KC_5,      KC_6, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,      KC_1,      KC_2,      KC_3, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________,      KC_0, _________, _________, _________, _________
  ),

  [_MOVEMENT] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,   KC_LEFT,   KC_DOWN,     KC_UP,   KC_RGHT, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),
};
