/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright 2024 splitkb.com <support@splitkb.com> */

#include QMK_KEYBOARD_H

// Disable magic keycodes to save space for AVR as they are not available to set in vial anyways as we disable `QMK_SETTINGS`.

#ifndef MAGIC_ENABLE
uint16_t keycode_config(uint16_t keycode) {
    return keycode;
}
#endif

#ifndef MAGIC_ENABLE
uint8_t mod_config(uint8_t mod) {
    return mod;
}
#endif