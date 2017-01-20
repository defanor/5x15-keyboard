#include "WProgram.h"
#include "keys.h"

// [esc] [ ~ ] [ 1 ] [ 2 ] [ 3 ] [ 4 ] [ 5 ]             [ 6 ] [ 7 ] [ 8 ] [ 9 ] [ 0 ] [ - ] [ + ]
// [ \ ] [ / ] [ q ] [ w ] [ f ] [ p ] [ g ]             [ j ] [ l ] [ u ] [ y ] [ ; ] [ [ ] [ ] ]
//   [ shift ] [ a ] [ r ] [ s ] [ t ] [ d ]  [  tab  ]  [ h ] [ n ] [ e ] [ i ] [ o ] [ shift ]
//   [control] [ z ] [ x ] [ c ] [ v ] [ b ]  [ bkspc ]  [ k ] [ m ] [ , ] [ . ] [ ' ] [control]
//   [ meta  ] [       ] [ layout] [  alt  ]  [  ret  ]  [ space ] [layout ] [ altgr ] [ meta  ]

uint8_t keymap[3][7][15] = {
  { // qwerty
    {K_ESC,    K_TILDE, K_1, K_2, K_3, K_4, K_5, 0,      0,     0,   0,       0,     0,       0,        0},
    {0,        0,       0,   0,   0,   0,   0,   0,      K_6,   K_7, K_8,     K_9,   K_0,     K_MINUS,  K_EQUAL},
    {K_BSLASH, K_SLASH, K_Q, K_W, K_E, K_R, K_T, 0,      0,     0,   0,       0,     0,       0,        0},
    {0,        0,       0,   0,   0,   0,   0,   0,      K_Y,   K_U, K_I,     K_O,   K_P,     K_LBRACE, K_RBRACE},
    {0,        0,       K_A, K_S, K_D, K_F, K_G, K_TAB,  K_H,   K_J, K_K,     K_L,   K_SCOL,  0,        0},
    {0,        0,       K_Z, K_X, K_C, K_V, K_B, K_BSPC, K_N,   K_M, K_COMMA, K_DOT, K_QUOTE, 0,        0},
    {0,        0,       0,   0,   0,   0,   0,   K_RET,  K_SPC, 0,   0,       0,     0,       0,        0}
  },
  { // mostly functional keys
    // ESC, DEL, and F*
    {K_ESC, K_DEL, K_F1,    K_F2,   K_F3,   K_F4,   K_F5,  0,     0,     0,       0,    0,       0,       0,     0},
    {0,     0,     0,       0,      0,      0,      0,     0,     K_F6,  K_F7,    K_F8, K_F9,    K_F10,   K_F11, K_F12},
    // nothing for now
    {0,     0,     0,       0,      0,      0,      0,     0,     0,      0,      0,    0,       0,       0,     0},
    {0,     0,     0,       0,      0,      0,      0,     0,     0,      0,      0,    0,       0,       0,     0},
    // hjkl-style navigation keys
    {0,     0,     0,       K_HOME, K_PGUP, K_PGDN, K_END, K_TAB, K_LEFT, K_DOWN, K_UP, K_RIGHT, 0,       0,     0},
    // nothing for now
    {0,     0,     0,       0,      0,      0,      0,     K_BSPC,0,      0,      0,    0,       0,       0,     0},
    {0,     0,     0,       0,      0,      0,      0,     K_RET, K_SPC,  0,      0,    0,       0,       0,     0}
  },
  { // colemak
    {K_ESC,    K_TILDE, K_1, K_2, K_3, K_4, K_5, 0,      0,     0,   0,       0,     0,       0,        0},
    {0,        0,       0,   0,   0,   0,   0,   0,      K_6,   K_7, K_8,     K_9,   K_0,     K_MINUS,  K_EQUAL},
    {K_BSLASH, K_SLASH, K_Q, K_W, K_F, K_P, K_G, 0,      0,     0,   0,       0,     0,       0,        0},
    {0,        0,       0,   0,   0,   0,   0,   0,      K_J,   K_L, K_U,     K_Y,   K_SCOL,  K_LBRACE, K_RBRACE},
    {0,        0,       K_A, K_R, K_S, K_T, K_D, K_TAB,  K_H,   K_N, K_E,     K_I,   K_O,     0,        0},
    {0,        0,       K_Z, K_X, K_C, K_V, K_B, K_BSPC, K_K,   K_M, K_COMMA, K_DOT, K_QUOTE, 0,        0},
    {0,        0,       0,   0,   0,   0,   0,   K_RET,  K_SPC, 0,   0,       0,     0,       0,        0}
  }
};

uint8_t p[7][15]; // pressed keys
uint8_t prev_keys[6] = {0,0,0,0,0,0}, prev_modifiers = 0;
uint8_t layout = 0;
uint8_t layout_state = 0;

int type () {
  uint8_t i, j, ret = 0, k = 0, send = 0, cur_layout = layout;

  // Set modifiers
  keyboard_modifier_keys =
    p[5][1]           // left ctrl
    | (p[4][1] << 1)  // left shift
    | (p[6][6] << 2)  // left alt
    | (p[6][1] << 3)  // left "gui"
    | (p[5][14] << 4) // right ctrl
    | (p[4][14] << 5) // right shift
    | (p[6][12] << 6) // right alt
    | (p[6][14] << 7) // right "gui"
    ;

  // Handle layout keys
  if (p[6][5] && p[6][10]) {
    if (layout_state == 0) {
      // Cycle through layouts. Perhaps should track the order in
      // which Fn keys are pressed, to determine the direction.
      layout = (layout + 1) % 3;
      cur_layout = layout;
      layout_state = 1;
    }
  } else if (p[6][5] || p[6][10]) {
    // Set the functional layout temporary
    cur_layout = 1;
  } else {
    layout_state = 0;
  }

  // Fill keyboard_keys
  for (i = 0; i < 7; i++){
    for (j = 0; j < 15; j++){
      if (p[i][j] && k < 6) {
        if (keymap[layout][i][j]) {
          keyboard_keys[k] = keymap[cur_layout][i][j];
          k++;
        }
      }
    }
  }
  while (k < 6) {
    keyboard_keys[k] = 0;
    k++;
  }

  // Compare it to the previously sent keys, send if they differ
  for (k = 0; k < 6; k++)
    if (prev_keys[k] != keyboard_keys[k])
      send = 1;
  if (prev_modifiers != keyboard_modifier_keys)
    send = 1;

  if (send)
    ret = usb_keyboard_send();

  prev_modifiers = keyboard_modifier_keys;
  for (k = 0; k < 6; k++)
    prev_keys[k] = keyboard_keys[k];

  return ret;
}


extern "C" int main(void) {
  uint8_t i = 0, j = 0, s = 0;
  uint8_t row_pins[7] = {2, 23, 9, 17, 13, 12, 11};
  uint8_t col_pins[15] = {10, 8, 7, 3, 4, 5, 6, 14, 20, 21, 22, 19, 18, 16, 15};

  for (i=0; i < 7; i ++) {
    pinMode(row_pins[i], OUTPUT);
    digitalWriteFast(row_pins[i], LOW);
  }
  for (i=0; i < 15; i ++) {
    pinMode(col_pins[i], INPUT_PULLDOWN);
  }

  while (1) {
    for (i = 0; i < 7; i++) {
      digitalWriteFast(row_pins[i], HIGH);
      for (j = ((i == 1 || i == 3) ? 8 : 0);
           j < ((i == 0 || i == 2) ? 7 : 15);
           j++)
        {
          s = digitalReadFast(col_pins[j]);
          p[i][j] = s;
        }
      digitalWriteFast(row_pins[i], LOW);
    }
    type();
    delay(5);
  }
}
