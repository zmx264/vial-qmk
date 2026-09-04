/* Copyright 2023 splitkb.com <support@splitkb.com>
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

#include "quantum.h"
#include "host.h"
#if defined(SPLIT_KEYBOARD) && defined(SPLIT_TRANSACTION_IDS_KB)
#    include "transactions.h"
#endif

// OLED animation
// #include "lib/bongocat.h"

// The first four layers gets a name for readability, which is then used in the OLED below.
// clang-format off
enum layers {
  _DEFAULT,
  _LOWER,
  _RAISE,
  _ADJUST
};
// clang-format on

#ifdef OLED_ENABLE
// NOTE: Most of the OLED code was originally written by Soundmonster for the Corne,
// and has been copied directly from `crkbd/soundmonster/keymap.c`

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (!is_keyboard_left()) {
        oled_set_brightness(OLED_BRIGHTNESS - 20);
        return OLED_ROTATION_180; // flips the display 180 degrees for right side
    }
    oled_set_brightness(OLED_BRIGHTNESS);
    return OLED_ROTATION_270;
}

void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[]  = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[]  = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[]  = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[]  = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0};
    static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0};
    static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0};
    static const char PROGMEM on_on_2[]   = {0xcc, 0};

    if (modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if (modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if (modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if (modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if (modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_2, false);
    } else if (modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if (modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if (modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[]  = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[]  = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[]  = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[]  = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0};
    static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0};
    static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0};
    static const char PROGMEM on_on_2[]   = {0xcc, 0};

    if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_2, false);
    } else if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}

void render_logo(void) {
    // clang-format off
    static const char PROGMEM aurora_logo[] = {
        0x00, 0x00, 0x00, 0xe0, 0x00, 0xf8, 0xc0, 0xf8, 0xe0, 0xc0, 0xfc, 0x00, 0x7e, 0x18, 0x00, 0x80,
        0x00, 0x02, 0x80, 0xf0, 0x00, 0xc0, 0x80, 0xf8, 0xc0, 0xe0, 0x70, 0x60, 0x3c, 0x38, 0x3c, 0x1c,
        0x00, 0x3f, 0x0c, 0x0f, 0x1f, 0x03, 0x07, 0x01, 0xc3, 0x00, 0xe0, 0x80, 0x00, 0xe0, 0x80, 0xf8,
        0x80, 0xc0, 0xf7, 0xc7, 0x6f, 0x7b, 0x39, 0x30, 0x00, 0x80, 0x00, 0xc0, 0x00, 0xc0, 0xc2, 0xe0,
        0x00, 0x40, 0x38, 0x30, 0x38, 0x1e, 0x18, 0x1e, 0x0f, 0x0c, 0x07, 0x07, 0x07, 0x03, 0x03, 0x21,
        0x21, 0x31, 0x30, 0x18, 0x18, 0x1c, 0x08, 0x0c, 0x0e, 0x07, 0x06, 0x07, 0x03, 0xc3, 0x03, 0x01,
        0x4c, 0xcc, 0xc2, 0xc2, 0x41, 0x49, 0x09, 0x2b, 0x2a, 0x6a, 0x6e, 0x24, 0x24, 0x04, 0x92, 0x92,
        0xb1, 0xf1, 0xf1, 0xf2, 0xe6, 0xa4, 0xa4, 0x04, 0x04, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28
    };
    // clang-format on
    oled_write_raw_P(aurora_logo, sizeof(aurora_logo));
    oled_set_cursor(0, 4);
}

void render_logo_text(void) {
    oled_write_P(PSTR("sofle"), false);
}

void render_kb_LED_state(void) {
    // Host Keyboard LED Status
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(led_usb_state.num_lock ? PSTR("N ") : PSTR("  "), false);
    oled_write_P(led_usb_state.caps_lock ? PSTR("C ") : PSTR("  "), false);
    oled_write_P(led_usb_state.scroll_lock ? PSTR("S ") : PSTR("  "), false);
}

void render_layer_state(void) {
    // clang-format off
    static const char PROGMEM default_layer[] = {
        0x20, 0x94, 0x95, 0x96, 0x20,
        0x20, 0xb4, 0xb5, 0xb6, 0x20,
        0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static const char PROGMEM raise_layer[] = {
        0x20, 0x97, 0x98, 0x99, 0x20,
        0x20, 0xb7, 0xb8, 0xb9, 0x20,
        0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static const char PROGMEM lower_layer[] = {
        0x20, 0x9a, 0x9b, 0x9c, 0x20,
        0x20, 0xba, 0xbb, 0xbc, 0x20,
        0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    static const char PROGMEM adjust_layer[] = {
        0x20, 0x9d, 0x9e, 0x9f, 0x20,
        0x20, 0xbd, 0xbe, 0xbf, 0x20,
        0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
    // clang-format on

    switch (get_highest_layer(layer_state | default_layer_state)) {
        case _LOWER:
            oled_write_P(lower_layer, false);
            break;
        case _RAISE:
            oled_write_P(raise_layer, false);
            break;
        case _ADJUST:
            oled_write_P(adjust_layer, false);
            break;
        default:
            oled_write_P(default_layer, false);
    }
}

static void render_zmx_logo(void) {
    static const unsigned char PROGMEM raw_logo[] = {
        0, 0, 0, 0, 0, 0, 0, 0,   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 96, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   192, 56, 8, 8, 24, 48, 96, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 96, 48, 24, 28, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 32, 32, 32, 64,  64,  128, 128, 0,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 64, 0,  0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 64, 96, 48, 24, 12, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 63, 0, 0, 0, 0, 0, 0, 3, 14, 56, 224, 0, 0, 192, 96, 48, 24, 6, 3, 0, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 4, 4, 24, 160, 96, 96, 80, 140, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 224, 16, 8,  12, 6,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0,   0,  0, 0, 0,  0,  0,  0,   0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,   0,  0,  0,  0,  255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  128, 192, 96,  16,  24, 8, 12, 6, 3, 1, 0, 0, 0, 0, 0, 1, 1, 2,   6,   24, 48, 192, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,   1,   1,  1,  3,  2,  2,  2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,   0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,   0, 0, 0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,   0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    oled_write_raw_P((const char *)raw_logo, sizeof(raw_logo));
}

static bool mouse_jiggler_enabled = false;
static uint32_t mouse_jiggler_timer = 0;
static uint8_t mouse_jiggler_phase = 0;
static uint32_t mouse_jiggler_step_timer = 0;
static uint32_t mouse_jiggler_start_time = 0;
static bool host_is_suspended = false;
static uint8_t mouse_jiggler_axis = 0; // Alternates between 0 (X) and 1 (Y)
static uint16_t mouse_jiggler_current_interval = 30000;

#define MOUSE_JIGGLER_STEP_DELAY 20                             // 20 ms return pulse
#define MOUSE_JIGGLER_AUTO_OFF_MS (8UL * 60UL * 60UL * 1000UL) // 8 hours auto-off safety timeout

static uint16_t get_next_jiggle_interval(void) {
    // Randomized interval between 25s and 35s to appear natural and evade detection
    return 25000 + (timer_read() % 10000);
}

void mouse_jiggler_toggle(void) {
    mouse_jiggler_enabled = !mouse_jiggler_enabled;
    mouse_jiggler_phase = 0;
    mouse_jiggler_timer = timer_read32();
    mouse_jiggler_start_time = timer_read32();
    mouse_jiggler_current_interval = get_next_jiggle_interval();
}

bool mouse_jiggler_is_enabled(void) {
    return mouse_jiggler_enabled;
}

static void mouse_jiggler_task(void) {
    if (!mouse_jiggler_enabled || !is_keyboard_master() || host_is_suspended) {
        return;
    }

    // Auto-off safety timeout: auto disable after 8 hours of continuous operation
    if (timer_elapsed32(mouse_jiggler_start_time) >= MOUSE_JIGGLER_AUTO_OFF_MS) {
        mouse_jiggler_enabled = false;
        mouse_jiggler_phase = 0;
        return;
    }

    // Smart Inactivity Detection: do not jiggle while the user is actively typing or turning knobs
    if (last_input_activity_elapsed() < mouse_jiggler_current_interval) {
        mouse_jiggler_timer = timer_read32();
        return;
    }

    if (mouse_jiggler_phase == 0) {
        if (timer_elapsed32(mouse_jiggler_timer) >= mouse_jiggler_current_interval) {
            report_mouse_t report = {0};
            mouse_jiggler_axis = !mouse_jiggler_axis;
            if (mouse_jiggler_axis) {
                report.x = 1;
            } else {
                report.y = 1;
            }
            host_mouse_send(&report);
            mouse_jiggler_phase = 1;
            mouse_jiggler_step_timer = timer_read32();
        }
    } else if (mouse_jiggler_phase == 1) {
        if (timer_elapsed32(mouse_jiggler_step_timer) >= MOUSE_JIGGLER_STEP_DELAY) {
            report_mouse_t report = {0};
            if (mouse_jiggler_axis) {
                report.x = -1;
            } else {
                report.y = -1;
            }
            host_mouse_send(&report);
            mouse_jiggler_phase = 0;
            mouse_jiggler_timer = timer_read32();
            mouse_jiggler_current_interval = get_next_jiggle_interval();
        }
    }
}

#if defined(SPLIT_KEYBOARD) && defined(SPLIT_TRANSACTION_IDS_KB)
static void mouse_jiggler_rpc_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen >= sizeof(bool)) {
        mouse_jiggler_enabled = *(const bool *)in_data;
    }
}
#endif

#ifdef OLED_ENABLE
static void render_mouse_jiggler_status(void) {
    if (oled_max_chars() > 5) {
        // Horizontal / Landscape orientation (e.g. OLED_ROTATION_0 or OLED_ROTATION_180: 21 cols x 4 lines)
        // Positioned as an inverted badge in the top-right corner
        if (mouse_jiggler_enabled) {
            uint8_t col = (oled_max_chars() >= 6) ? (oled_max_chars() - 6) : 0;
            oled_set_cursor(col, 0);
            oled_write_P(PSTR(" JGL "), true);
        }
    } else {
        // Vertical / Portrait orientation (e.g. OLED_ROTATION_90 or OLED_ROTATION_270: 5 cols x 16 lines)
        // Positioned on the bottom line of the 5-character wide display
        oled_set_cursor(0, oled_max_lines() - 1);
        if (mouse_jiggler_enabled) {
            oled_write_P(PSTR(" JGL "), true);
        } else {
            oled_write_P(PSTR("     "), false);
        }
    }
}
#endif

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_left()) {
        // Renders the current keyboard state (layers and mods)
        render_logo();
        render_logo_text();
        render_space();
        render_layer_state();
        render_space();
        render_mod_status_gui_alt(get_mods() | get_oneshot_mods());
        render_mod_status_ctrl_shift(get_mods() | get_oneshot_mods());
        render_kb_LED_state();
    } else {
        render_zmx_logo();
        render_mouse_jiggler_status();
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    // 0 is left-half encoder,
    // 1 is right-half encoder
    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return true;
}
#endif

void keyboard_post_init_kb(void) {
#if defined(SPLIT_KEYBOARD) && defined(SPLIT_TRANSACTION_IDS_KB)
    transaction_register_rpc(RPC_ID_MOUSE_JIGGLER, mouse_jiggler_rpc_slave_handler);
#endif
    keyboard_post_init_user();
}

void suspend_power_down_kb(void) {
    host_is_suspended = true;
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    host_is_suspended = false;
    mouse_jiggler_timer = timer_read32();
    suspend_wakeup_init_user();
}

void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
#if defined(SPLIT_KEYBOARD) && defined(SPLIT_TRANSACTION_IDS_KB)
        static bool last_synced_state = false;
        static uint32_t last_sync_time = 0;
        bool state_changed = (mouse_jiggler_enabled != last_synced_state);
        if ((state_changed && timer_elapsed32(last_sync_time) > 50) || timer_elapsed32(last_sync_time) > 500) {
            last_sync_time = timer_read32();
            if (transaction_rpc_send(RPC_ID_MOUSE_JIGGLER, sizeof(mouse_jiggler_enabled), &mouse_jiggler_enabled)) {
                last_synced_state = mouse_jiggler_enabled;
            }
        }
#endif
        mouse_jiggler_task();
    }
    housekeeping_task_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case QK_KB_0:
            if (record->event.pressed) {
                mouse_jiggler_toggle();
            }
            return false;
    }
    return true;
}
