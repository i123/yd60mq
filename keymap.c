/* Copyright 2020 rbange https://github.com/rbange
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
static uint32_t key_timer = 0;
static bool key_trigger = true; // スクリーンセーバ抑止機能 true(ON)/false(OFF)
enum custom_keycodes {
    HOGE = SAFE_RANGE,
	KEY_TIM,
};

const rgblight_segment_t PROGMEM rgb_L0_layer[] = RGBLIGHT_LAYER_SEGMENTS(
   { 0,11, HSV_BLACK}
 );
 
 const rgblight_segment_t PROGMEM rgb_L1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
   { 0,11, HSV_GREEN}
 );

 const rgblight_segment_t PROGMEM rgb_L2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
   { 0,11, HSV_BLUE}
 );

 const rgblight_segment_t PROGMEM rgb_L3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
   { 0,11, HSV_RED}
 );
   
 const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
   rgb_L0_layer,  // 配列0のためのダミー定義
   rgb_L1_layer,
   rgb_L2_layer,
   rgb_L3_layer
 );

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    rgblight_set_layer_state(1, layer_state_cmp(state, 1));
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));
    rgblight_set_layer_state(3, layer_state_cmp(state, 3));
    return state;
}

// キーボード初期化後に呼ばれる関数
void keyboard_post_init_user(void) {
	// LEDの初期化. 点灯させない
	int i;
	for(i=0;i<=11;i++){
		rgblight_sethsv_at(HSV_BLACK, i); 
	} 
   // レイヤーのLED情報を読み込み
   rgblight_layers = rgb_layers;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
		case HOGE:
			if (record->event.pressed) {
				// when keycode HOGE is pressed
				SEND_STRING("https://qmk.fm/\n");
				SEND_STRING(SS_TAP(X_ENTER));
			} else {
				// when keycode HOGE is released
			}
			break;
        case KEY_TIM:
            if (record->event.pressed) {
                key_trigger ^= true;
				// 機能のON/OFF LEDで表示
				if (key_trigger) {
					rgblight_sethsv_at(HSV_RED, 11);    
				}else{
					rgblight_sethsv_at(HSV_GREEN, 11);  
				}
            }
			break; 	
    }
    return true;
}

void matrix_scan_user(void) { 
	// スクリーンセーバ抑止
    if (timer_elapsed32(key_timer) > 30000) { // 30 seconds
        key_timer = timer_read32();  // resets timer
        if (key_trigger) {
			tap_code(KC_MS_UP);
			tap_code(KC_MS_DOWN);
		}else{
		}
	}
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT_all(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_JYEN, KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NUHS,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFT_T(KC_RO), SFT_T(KC_UP), SFT_T(KC_DEL),
        KC_LCTL, KC_LGUI, KC_LALT,          LT(1, KC_MHEN),   KC_SPC,  LT(1,KC_HENK),             KC_RALT, KC_APP, KC_LEFT, KC_DOWN, CTL_T(KC_RGHT)
    ),

	[1] = LAYOUT_all(
        RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_DEL,
        KC_TRNS, RGB_TOG, KC_UP,   RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, BL_DEC,  BL_TOGG, BL_INC,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [2] = LAYOUT_all(
        KEY_TIM, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [3] = LAYOUT_all(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

};

 