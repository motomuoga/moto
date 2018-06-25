#include "helix.h"
#include "bootloader.h"
#include "action_layer.h"
#include "eeconfig.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#include "LUFA/Drivers/Peripheral/TWI.h"
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _TENKEY 1
#define _MOUSE 2
#define _LOWER 3
#define _RAISE 4
#define _ADJUST 16

char base_mode = (char)_QWERTY;
bool hoge = false;

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  TENKEY,
  MOUSE,
  LOWER,
  RAISE,
  ADJUST,
  BACKLIT,
  EISU,
  KANA,
  RGBRST,
  HOGE,
  ASHN, // 英数変換
  KNHN, // かな変換
  KKHN, // カナ変換
  ZNHN, // 全角変換
  HNHN  // 半角変換
//  MAC,  // Macモード
//  WIN   // Winモード
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

#define LR_SPC  LT(_RAISE,KC_SPC)
#define LL_EIS  LT(_LOWER,KC_LANG2)
#define LL_KAN  LT(_LOWER,KC_LANG1)
#define LM_ESC  LT(_MOUSE,KC_ESC)
#define HS_GUI  MT(MOD_LSFT,KC_LGUI)
#define HS_RMB  MT(MOD_RSFT,KC_RGUI)
#define HC_TAB  MT(MOD_LCTL,KC_TAB)
#define HC_ENT  MT(MOD_RCTL,KC_ENT)
#define HA_ESC  MT(MOD_LALT,KC_ESC)
#define HA_BSPC MT(MOD_RALT,KC_BSPC)
#define MAC AG_NORM
#define WIN AG_SWAP
#define IS_MAC (keymap_config.swap_lalt_lgui==false)


// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
//Macros
#define M_SAMPLE M(KC_SAMPLEMACRO)

#if HELIX_ROWS == 5
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | Home |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | End  |
   * |   Q  |   W  |   E  |   R  |   T  |  Tab |             |  BS  |   Y  |   U  |   I  |   O  |   P  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |   A  |   S  |   D  |   F  |   G  |  Ctl |             |Enter |   H  |   J  |   K  |   L  |   ;  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |   Z  |   X  |   C  |   V  |   B  | Shft |  Ins |  Del | RMB  |   N  |   M  |   ,  |   .  |   /  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |  Esc |Adjust| CAPS | ALT  |  GUI | EISU |Space |Space | KANA | GUI  |   ←  |   ↓  |   ↑  |   →  |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_QWERTY] = KEYMAP( \
      KC_HOME, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_END,  \
      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_TAB,                    KC_BSPC, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
      KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LCTL,                   KC_ENT,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, \
      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LSFT, KC_INS,  KC_DEL,  KC_RSFT, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
      LM_ESC , ADJUST,  KC_CAPS, KC_LALT, KC_LGUI, LL_EIS,  LR_SPC,  LR_SPC,  LL_KAN,  KC_RGUI, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT  \
      ),

  /* TenKey
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   7  |   8  |   9  |   /  |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   4  |   5  |   6  |   *  |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |   1  |   2  |   3  |   -  |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |  Ent |   0  |   .  |   +  |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_TENKEY] = KEYMAP( \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P7,   KC_P8,   KC_P9,   KC_PSLS, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P4,   KC_P5,   KC_P6,   KC_PAST, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_PENT, KC_P0,   KC_PDOT, KC_PPLS, XXXXXXX, XXXXXXX  \
      ),

  /* Mouse
  * ,-----------------------------------------.             ,-----------------------------------------.
  * |      |      |      |      |      |      |             |      |      |      |      |      |      |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |      |      |      |      |      |             |  M←  |  M↑  |  M→  |      |      |      |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |      |      |      |      |      |             |  M←  |  M↓  |  M→  |      |      |      |
  * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
  * |      | Acl0 | Acl1 | Acl2 |      |      |      |      |  M←  |  M↓  |  M→  |      |      |      |
  * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
  * |      | Btn3 | Btn2 |      |      | Btn1 |      |      | Btn3 |      |      |      |      |      |
  * `-------------------------------------------------------------------------------------------------'
   */
  [_MOUSE] = KEYMAP( \
    HOGE,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_MS_L, KC_MS_U, KC_MS_R, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, \
    XXXXXXX, KC_ACL0, KC_ACL1, KC_ACL2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, \
    XXXXXXX, KC_BTN3, KC_BTN2, XXXXXXX, _______, KC_BTN1, XXXXXXX, XXXXXXX, KC_BTN3, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
      ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |   !  |   "  |   #  |   $  |   %  |             |   &  |   '  |   (  |   )  |   -  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |   ~  |   ^  |             |   @  |   ¥  |   {  |   }  |   +  |   :  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |   _  |   `  |      |      |   =  |   |  |   [  |   ]  |   *  |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * | ASHN | KNHN | KKHN | ZNHN | HNHN |      |      |      |      |      |   <  |   >  |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = KEYMAP( \
      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
      _______, KC_EXLM, KC_DQT,  KC_HASH, KC_DLR,  KC_PERC,                   KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, _______, \
      _______, _______, _______, _______, KC_TILD, KC_CIRC,                   KC_AT,   KC_BSLS, KC_LCBR, KC_RCBR, KC_PLUS, KC_COLN, \
      _______, _______, _______, _______, KC_UNDS, KC_GRV,  _______, _______, KC_EQL,  KC_PIPE, KC_LBRC, KC_RBRC, KC_ASTR, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LABK, KC_RABK, _______, _______  \
      ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |  F7  |  F8  |  F9  | F10  | F11  | F12  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Home |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | End  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |  F7  |  F8  |  F9  | F10  | F11  | F12  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | Home | PgDn | PgUp | End  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      | Cut  | Copy |Paste |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = KEYMAP( \
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
      KC_HOME, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_END, \
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
      ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |      |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  Win |  Mac |      |Qwerty|TenKey| Mouse|             |      |   ↑  |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   ←  |   ↓  |   →  |      |      |      |
   * |------+------+------+------+------+------|-------------|------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |   ↓  |      |      |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  KEYMAP( \
      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
      WIN,     MAC,     _______, QWERTY,  TENKEY,  MOUSE,                     _______, KC_UP,   _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______,                   KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DOWN, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
      )
};

#elif HELIX_ROWS == 4

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   Q  |   W  |   E  |   R  |   T  |  Tab |             |  BS  |   Y  |   U  |   I  |   O  |   P  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |   A  |   S  |   D  |   F  |   G  |  Ctl |             |Enter |   H  |   J  |   K  |   L  |   ;  |
   * |------+------+------+------+------+------+             +------+------+------+------+------+------|
   * |   Z  |   X  |   C  |   V  |   B  | Shft |             | RMB  |   N  |   M  |   ,  |   .  |   /  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |  Esc |Adjust| CAPS | ALT  |  GUI | EISU |Space |Space | KANA | GUI  |   ←  |   ↓  |   ↑  |   →  |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_QWERTY] = KEYMAP( \
      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_TAB,                    KC_BSPC, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
      KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LCTL,                   KC_ENT,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, \
      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LSFT,                   KC_RSFT, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
      LM_ESC , ADJUST,  KC_CAPS, KC_LALT, KC_LGUI, LL_EIS,  LR_SPC,  LR_SPC,  LL_KAN,  KC_RGUI, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT  \
      ),

  /* TenKey
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |      |      |      |      |             |   7  |   8  |   9  |   /  |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   4  |   5  |   6  |   *  |      |      |
   * |------+------+------+------+------+------+             +------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   1  |   2  |   3  |   -  |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |  Ent |   0  |   .  |   +  |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_TENKEY] = KEYMAP( \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P7,   KC_P8,   KC_P9,   KC_PSLS, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P4,   KC_P5,   KC_P6,   KC_PAST, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P1,   KC_P2,   KC_P3,   KC_PMNS, XXXXXXX, XXXXXXX, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_PENT, KC_P0,   KC_PDOT, KC_PPLS, XXXXXXX, XXXXXXX  \
      ),

  /* Mouse
  * ,-----------------------------------------.             ,-----------------------------------------.
  * |      |      |      |      |      |      |             |  M←  |  M↑  |  M→  |      |      |      |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |      |      |      |      |      |             |  M←  |  M↓  |  M→  |      |      |      |
  * |------+------+------+------+------+------+             +------+------+------+------+------+------|
  * |      | Acl0 | Acl1 | Acl2 |      |      |             |  M←  |  M↓  |  M→  |      |      |      |
  * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
  * |      | Btn3 | Btn2 |      |      | Btn1 |      |      | Btn3 |      |      |      |      |      |
  * `-------------------------------------------------------------------------------------------------'
   */
  [_MOUSE] = KEYMAP( \
    HOGE,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_MS_L, KC_MS_U, KC_MS_R, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, \
    XXXXXXX, KC_ACL0, KC_ACL1, KC_ACL2, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, \
    XXXXXXX, KC_BTN3, KC_BTN2, XXXXXXX, _______, KC_BTN1, XXXXXXX, XXXXXXX, KC_BTN3, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
      ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |   !  |   "  |   #  |   $  |   %  |             |   &  |   '  |   (  |   )  |   -  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |   ~  |   ^  |             |   @  |   ¥  |   {  |   }  |   +  |   :  |
   * |------+------+------+------+------+------+             +------+------+------+------+------+------|
   * |      |      |      |      |   _  |   `  |             |   =  |   |  |   [  |   ]  |   *  |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * | ASHN | KNHN | KKHN | ZNHN | HNHN |      |      |      |      |      |   <  |   >  |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = KEYMAP( \
      _______, KC_EXLM, KC_DQT,  KC_HASH, KC_DLR,  KC_PERC,                   KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, _______, \
      _______, _______, _______, _______, KC_TILD, KC_CIRC,                   KC_AT,   KC_BSLS, KC_LCBR, KC_RCBR, KC_PLUS, KC_COLN, \
      _______, _______, _______, _______, KC_UNDS, KC_GRV,                    KC_EQL,  KC_PIPE, KC_LBRC, KC_RBRC, KC_ASTR, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LABK, KC_RABK, _______, _______  \
      ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | Home |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | End  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |  F7  |  F8  |  F9  | F10  | F11  | F12  |
   * |------+------+------+------+------+------+             +------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |      | Home | PgDn | PgUp | End  |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      | Cut  | Copy |Paste |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = KEYMAP( \
      KC_HOME, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_END, \
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
      _______, _______, _______, _______, _______, _______,                   _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
      ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  Win |  Mac |      |Qwerty|TenKey| Mouse|             |      |   ↑  |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |   ←  |   ↓  |   →  |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |      |      |             |      |   ↓  |      |      |      |      |
   * |------+------+------+------+------+------+-------------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  KEYMAP( \
      WIN,     MAC,     _______, QWERTY,  TENKEY,  MOUSE,                     _______, KC_UP,   _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______,                   KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______,                   _______, KC_DOWN, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
      )
};

#else
#error "undefined keymaps"
#endif


#ifdef AUDIO_ENABLE

float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
float tone_plover[][2]     = SONG(PLOVER_SOUND);
float tone_plover_gb[][2]  = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
#endif

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    #ifdef RGBLIGHT_ENABLE
      //rgblight_mode(RGB_current_mode);
    #endif
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        persistent_default_layer_set(1UL<<_QWERTY);
        base_mode = (char)_QWERTY;
      }
      return false;
      break;
    case TENKEY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_TENKEY);
        base_mode = (char)_TENKEY;
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
          //not sure how to have keyboard check mode and set it to a variable, so my work around
          //uses another variable that would be set to true after the first time a reactive key is pressed.
        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
        } else {
          TOG_STATUS = !TOG_STATUS;
          #ifdef RGBLIGHT_ENABLE
            //rgblight_mode(16);
          #endif
        }
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        #ifdef RGBLIGHT_ENABLE
          //rgblight_mode(RGB_current_mode);   // revert RGB to initial mode prior to RGB mode change
        #endif
        TOG_STATUS = false;
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        //not sure how to have keyboard check mode and set it to a variable, so my work around
        //uses another variable that would be set to true after the first time a reactive key is pressed.
        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
        } else {
          TOG_STATUS = !TOG_STATUS;
          #ifdef RGBLIGHT_ENABLE
            //rgblight_mode(15);
          #endif
        }
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        #ifdef RGBLIGHT_ENABLE
          //rgblight_mode(RGB_current_mode);  // revert RGB to initial mode prior to RGB mode change
        #endif
        layer_off(_RAISE);
        TOG_STATUS = false;
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
      //led operations - RGB mode change now updates the RGB_current_mode to allow the right RGB mode to be set after reactive keys are released
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case EISU:
      if (record->event.pressed) {
        if(keymap_config.swap_lalt_lgui==false){
          register_code(KC_LANG2);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG2);
      }
      return false;
      break;
    case KANA:
      if (record->event.pressed) {
        if(keymap_config.swap_lalt_lgui==false){
          register_code(KC_LANG1);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
    case HOGE:
      if (record->event.pressed) {
        hoge = true;
      }else{
        hoge = false;
      }
      break;
      /*
    case ASHN:
      if (record->event.pressed) {
        if(IS_MAC){
          //SEND_STRING(SS_LCTRL(":"));
          //register_code(LCTL(KC_COLN));
          CTL_T(KC_COLN);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
//      } else {
//        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case KNHN:
        if (record->event.pressed) {
          if(IS_MAC){
            SEND_STRING(SS_LCTRL("ｊ"));
          }else{
            SEND_STRING(SS_LALT("`"));
          }
  //      } else {
  //        unregister_code(KC_LANG1);
        }
        return false;
        break;
      case KKHN:
        if (record->event.pressed) {
          if(IS_MAC){
            SEND_STRING(SS_LCTRL("ｋ"));
          }else{
            SEND_STRING(SS_LALT("`"));
          }
//      } else {
//        unregister_code(KC_LANG1);
        }
        return false;
        break;
      case ZNHN:
        if (record->event.pressed) {
          if(IS_MAC){
            SEND_STRING(SS_LCTRL("ｌ"));
          }else{
            SEND_STRING(SS_LALT("`"));
          }
  //      } else {
  //        unregister_code(KC_LANG1);
        }
        return false;
        break;
      case HNHN:
        if (record->event.pressed) {
          if(IS_MAC){
            SEND_STRING(SS_LCTRL(";"));
          }else{
            SEND_STRING(SS_LALT("`"));
          }
  //      } else {
  //        unregister_code(KC_LANG1);
        }
        return false;
        break;
*/
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}


#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
}

void shutdown_user()
{
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif


//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
     iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

//assign the right code to your layers for OLED display
#define L_BASE 0
#define L_TENKEY 1
#define L_MOUSE 4
#define L_LOWER 8
#define L_RAISE 16
#define L_FNLAYER 64        // 未使用
#define L_NUMLAY 128        // 未使用
#define L_NLOWER 136        // 未使用
#define L_NFNLAYER 192      // 未使用
#define L_MOUSECURSOR 256   // 未使用
#define L_ADJUST 65536
#define L_ADJUST_TRI 65560

static void render_logo(struct CharacterMatrix *matrix) {

  static char logo[]={
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
    0};
  matrix_write(matrix, logo);
  //matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}



void render_status(struct CharacterMatrix *matrix) {

  // Render to mode icon
  // {}内にhelixfont.hに配置してあるフォントのアドレスを記載する。連続して描画する文字数分書く。
  // １行のサイズは 0x20 。　0x00と書いた場合には、最初の文字が出力される。
  static char logo[][2][3]={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
  if(keymap_config.swap_lalt_lgui==false){
    matrix_write(matrix, logo[0][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[0][1]);
  }else{
    matrix_write(matrix, logo[1][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[1][1]);
  }

  // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
  char buf[40];
  snprintf(buf,sizeof(buf), "Undef-%ld", layer_state);
  matrix_write_P(matrix, PSTR("\nLayer: "));
    switch (layer_state) {
      case L_BASE:
        switch ((int)base_mode) {
          case _QWERTY:
            matrix_write_P(matrix, PSTR("Default"));
            break;
          case _TENKEY:
            matrix_write_P(matrix, PSTR("TenKey"));
            break;
        }
         break;
      case L_TENKEY:
        matrix_write_P(matrix, PSTR("TenKey"));
        break;
      case L_MOUSE:
//      matrix_write_P(matrix, PSTR("Mouse"));
      if(hoge){
         matrix_write_P(matrix, PSTR("Mouse ON"));
       }else{
         matrix_write_P(matrix, PSTR("Mouse OFF"));
       }
         break;
      case L_RAISE:
         matrix_write_P(matrix, PSTR("Raise"));
         break;
      case L_LOWER:
         matrix_write_P(matrix, PSTR("Lower"));
         break;
      case L_ADJUST:
      case L_ADJUST_TRI:
         matrix_write_P(matrix, PSTR("Adjust"));
         break;
      default:
         matrix_write(matrix, buf);
    }

  // Host Keyboard LED Status
  char led[40];
    snprintf(led, sizeof(led), "\n%s  %s  %s",
            (host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
            (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
            (host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
  matrix_write(matrix, led);
}


void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable) {
    return;
  }
#endif

  matrix_clear(&matrix);
  if(is_master){
    render_status(&matrix);
  }else{
    render_logo(&matrix);
  }
  matrix_update(&display, &matrix);
}

#endif
