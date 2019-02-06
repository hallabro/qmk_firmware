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
  X_SPACE_ENTER = 0,
  X_DOT_COMMA = 1,
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

//**************** Definitions needed for quad function to work *********************//
//instanalize an instance of 'tap' for the 'x' tap dance.
static tap se_tap_state = {
  .is_press_action = true,
  .state = 0
};

void se_finished (qk_tap_dance_state_t *state, void *user_data) {
  se_tap_state.state = cur_dance(state);
  switch (se_tap_state.state) {
    case SINGLE_TAP: register_code(KC_SPC); break;
    case SINGLE_HOLD: register_code(KC_ENT); break;
    default: register_code(KC_SPC); unregister_code(KC_SPC); register_code(KC_SPC);
  }
}

void se_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (se_tap_state.state) {
    case SINGLE_TAP: unregister_code(KC_SPC); break;
    case SINGLE_HOLD: unregister_code(KC_ENT); break;
    default: unregister_code(KC_SPC);
  }
  se_tap_state.state = 0;
}

void dot_finished (qk_tap_dance_state_t *state, void *user_data) {
  se_tap_state.state = cur_dance(state);
  switch (se_tap_state.state) {
  case SINGLE_TAP: register_code(KC_DOT); break;
  case DOUBLE_TAP: register_code(KC_COMMA); break;
  default: register_code(KC_DOT); unregister_code(KC_COMMA); register_code(KC_DOT);
  }
}

void dot_reset (qk_tap_dance_state_t *state, void *er_data) {
  switch (se_tap_state.state) {
  case SINGLE_TAP: unregister_code(KC_DOT); break;
  case DOUBLE_TAP: unregister_code(KC_COMMA); break;
  default: unregister_code(KC_DOT);
  }
  se_tap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [X_SPACE_ENTER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, se_finished, se_reset),
  [X_DOT_COMMA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dot_finished, dot_reset)
};

extern keymap_config_t keymap_config;

#define _________ KC_TRNS
#define KC_BRKTL  RALT(KC_8)
#define KC_BRKTR  RALT(KC_9)
#define KC_LCBRC  RALT(KC_7)
#define KC_RCBRC  RALT(KC_0)
#define SHIFT_MOD MOD_BIT(KC_LSFT)
#define TM_NUMPAD MO(_NUMPAD)
#define KC_XCPY   LCTL(KC_INS)
#define KC_XINS   LSFT(KC_INS)

#define KC_SPCENT TD(X_SPACE_ENTER)
#define KC_DOTCMA TD(X_DOT_COMMA)
#define KC_BSPDEL TD(X_BACKSPACE_DEL)

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

    SEQ_ONE_KEY(KC_F) {
      // Anything you can do in a macro.
      SEND_STRING("QMK is awesome.");
    }
    SEQ_TWO_KEYS(KC_D, KC_D) {
      SEND_STRING(SS_LCTRL("a")SS_LCTRL("c"));
    }
    SEQ_THREE_KEYS(KC_D, KC_D, KC_S) {
      SEND_STRING("https://start.duckduckgo.com"SS_TAP(X_ENTER));
    }
    SEQ_TWO_KEYS(KC_A, KC_S) {
      register_code(KC_LGUI);
      register_code(KC_S);
      unregister_code(KC_S);
      unregister_code(KC_LGUI);
    }
  }
}

enum custom_keycodes
{
  KC_BSPCD = SAFE_RANGE
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_BSPCD:
              if (record->event.pressed) {
                uint8_t current_mods = get_mods();
                if (current_mods & SHIFT_MOD) {
                  clear_mods();
                  SEND_STRING(SS_TAP(X_DELETE));
                  set_mods(current_mods);
                } else {
                  SEND_STRING(SS_TAP(X_BSPACE));
                }
              }
              return false;
        }
    }

    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DVORAK] = LAYOUT(
    KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      _________,                              _________,      KC_6,     KC_7,       KC_8,      KC_9,      KC_0,     RESET,
    _________, _________, _________, KC_DOTCMA, KC_P,      KC_Y,      KC_HOME,                                   KC_END,      KC_F,     KC_G,       KC_C,      KC_R,      KC_L,     NO_AA,
    KC_LCTRL,  KC_A,      KC_O,      KC_E,      KC_U,      KC_I,      KC_LCBRC,                                KC_RCBRC,      KC_D,     KC_H,       KC_T,      KC_N,      KC_S,     NO_AE,
    KC_LSFT,   KC_NUBS,   KC_Q,      KC_J,      KC_K,      KC_X,      KC_BRKTL,                                KC_BRKTR,      KC_B,     KC_M,       KC_W,      KC_V,      KC_Z,   NO_OSLH,
    _________, NO_MINS,   NO_PLUS,   _________, _________, KC_SPCENT, KC_LALT,     KC_LGUI,        KC_LEAD,     KC_RALT,  KC_BSPCD, _________, _________, _________, _________, _________
  ),

  [_QWERTY] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________,      KC_Q,      KC_W,      KC_E,      KC_R,      KC_T, _________,                              _________,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,     NO_AA,
    _________,      KC_A,      KC_S,      KC_D,      KC_F,      KC_G, _________,                              _________,      KC_H,      KC_J,      KC_K,      KC_L,   NO_OSLH,     NO_AE,
    _________,      KC_Z,      KC_X,      KC_C,      KC_V,      KC_B, _________,                              _________,      KC_N,      KC_M, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),

  [_LOWER] = LAYOUT(
    _________,      KC_1,      KC_2,      KC_3,      KC_4,      KC_5, _________,                              _________,      KC_6,     KC_7,       KC_8,      KC_9,      KC_0, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),

  [_NUMPAD] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,   KC_KP_7,   KC_KP_8,   KC_KP_9, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,   KC_KP_4,   KC_KP_5,   KC_KP_6, _________, KC_BSPACE,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,   KC_KP_1,   KC_KP_2,   KC_KP_3, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________,   KC_KP_0, _________, _________, _________, _________
  ),

  [_MOVEMENT] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,   KC_LEFT,   KC_DOWN,     KC_UP,   KC_RGHT, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),
};

