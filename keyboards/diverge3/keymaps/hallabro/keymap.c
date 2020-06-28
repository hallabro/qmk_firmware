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
#include "keymap_nordic.h"

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

enum custom_keycodes {
  TILDE = SAFE_RANGE,
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
    [TD_COPY_PASTE] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_INS), LSFT(KC_INS)),
};

extern keymap_config_t keymap_config;

#define NO_AA     KC_LBRC

#define _________ KC_TRNS
#define KC_BRKTL  RALT(KC_8)
#define KC_BRKTR  RALT(KC_9)
#define KC_LCBRC  RALT(KC_7)
#define KC_RCBRC  RALT(KC_0)
#define KC_FSLASH LSFT(KC_7)
#define KC_BSLASH RALT(KC_MINUS)
#define NO_DOLLAR RALT(KC_4)
#define NO_ASTRSK LSFT(KC_NUHS)
#define TG_QWERTY TG(_QWERTY)
#define TG_GAMING TG(_GAMING)
#define COPYPASTE TD(TD_COPY_PASTE)
#define KC_LPARAN LSFT(KC_8)
#define KC_RPARAN LSFT(KC_9)

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

#define _DVORAK 0
#define _QWERTY 1
#define _GAMING 2
#define _LOWER 3
#define _RAISE 4

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case (TILDE):
    if (record->event.pressed) {
      SEND_STRING(SS_RALT(SS_TAP(X_RBRACKET)SS_TAP(X_RBRACKET)));
    }
    break;
  }

  return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DVORAK] = LAYOUT(
    KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_BRKTL,                                KC_BRKTR,      KC_6,     KC_7,       KC_8,      KC_9,      KC_0,   KC_NUHS,
    KC_NUBS,   NO_MINS,   KC_COMMA,  KC_DOT,    KC_P,      KC_Y,      KC_FSLASH,                              KC_BSLASH,      KC_F,     KC_G,       KC_C,      KC_R,      KC_L, COPYPASTE,
    KC_LCTRL,  KC_A,      KC_O,      KC_E,      KC_U,      KC_I,      KC_LPARAN,                              KC_RPARAN,      KC_D,     KC_H,       KC_T,      KC_N,      KC_S,  KC_RCTRL,
    KC_LSFT,   NO_PLUS,   KC_Q,      KC_J,      KC_K,      KC_X,      KC_LCBRC,                                KC_RCBRC,      KC_B,     KC_M,       KC_W,      KC_V,      KC_Z,   KC_RSFT,
    RAISE,     NO_AA,     NO_OSLH,   NO_AE,   KC_TAB,      KC_SPACE,  KC_LGUI,   KC_LALT,              LOWER,  KC_ENTER, KC_BSPACE,   KC_DEL,    NO_QUOT, NO_ASTRSK,    KC_EQL,   NO_PIPE
  ),
  [_QWERTY] = LAYOUT(
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      _________,                              _________,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P, _________,
    _________, KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      _________,                              _________,      KC_H,      KC_J,      KC_K,      KC_L,  KC_COMMA, _________,
    _________, KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      _________,                              _________,      KC_N,      KC_M,   NO_PLUS,   NO_MINS,    KC_DOT, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),
  [_GAMING] = LAYOUT(
    _________, _________, _________, _________, _________, _________, KC_NUHS,                                _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, KC_6,                                   _________, _________, _________, _________, _________, _________, _________,
    KC_TAB,    _________, _________, _________, _________, _________, KC_7,                                   _________, _________, _________, _________, _________, _________, _________,
    KC_LSHIFT, _________, _________, _________, _________, _________, KC_8,                                   _________, _________, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, LOWER,     _________, KC_LCTRL,  _________,        _________, _________, _________, _________, _________, _________, _________, _________
  ),
  [_LOWER] = LAYOUT(
    _________, KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     _________,                              _________, _________, _________, _________, _________, _________, _________,
    _________, KC_F6,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     _________,                              _________, _________,      KC_7,      KC_8,      KC_9, _________, _________,
    _________, KC_F10,    KC_F11,    KC_F12,    _________, _________, _________,                              _________, _________,      KC_4,      KC_5,      KC_6, _________, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,      KC_1,      KC_2,      KC_3, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,        _________, _________, _________,      KC_0, _________, _________, _________, _________
  ),
  [_RAISE] = LAYOUT(
    RESET,     TG_QWERTY, TG_GAMING, _________, _________, _________, _________,                              _________, _________, _________, _________, _________, _________,     TILDE,
    _________, _________, _________, _________, _________, _________, _________,                              _________, _________,   KC_HOME,   KC_PGDN,   KC_PGUP,    KC_END, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________, NO_DOLLAR,   KC_LEFT,   KC_DOWN,     KC_UP,   KC_RGHT, _________,
    _________, _________, _________, _________, _________, _________, _________,                              _________,     NO_AT, _________, _________, _________, _________, _________,
    _________, _________, _________, _________, _________, _________, _________, _________,          KC_RALT, _________, _________,    KC_INS, _________, _________, _________, _________
  )
};
