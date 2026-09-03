#pragma once

#include <string>

#include "raylib.h"

inline auto getKeyName(const int keyCode) -> std::string {
    switch (keyCode) {
        // Alphanumeric
        case KEY_SPACE:
            return "Space";
        case KEY_APOSTROPHE:
            return "'";
        case KEY_COMMA:
            return ",";
        case KEY_MINUS:
            return "-";
        case KEY_PERIOD:
            return ".";
        case KEY_SLASH:
            return "/";
        case KEY_ZERO:
            return "0";
        case KEY_ONE:
            return "1";
        case KEY_TWO:
            return "2";
        case KEY_THREE:
            return "3";
        case KEY_FOUR:
            return "4";
        case KEY_FIVE:
            return "5";
        case KEY_SIX:
            return "6";
        case KEY_SEVEN:
            return "7";
        case KEY_EIGHT:
            return "8";
        case KEY_NINE:
            return "9";
        case KEY_SEMICOLON:
            return ";";
        case KEY_EQUAL:
            return "=";
        case KEY_A:
            return "A";
        case KEY_B:
            return "B";
        case KEY_C:
            return "C";
        case KEY_D:
            return "D";
        case KEY_E:
            return "E";
        case KEY_F:
            return "F";
        case KEY_G:
            return "G";
        case KEY_H:
            return "H";
        case KEY_I:
            return "I";
        case KEY_J:
            return "J";
        case KEY_K:
            return "K";
        case KEY_L:
            return "L";
        case KEY_M:
            return "M";
        case KEY_N:
            return "N";
        case KEY_O:
            return "O";
        case KEY_P:
            return "P";
        case KEY_Q:
            return "Q";
        case KEY_R:
            return "R";
        case KEY_S:
            return "S";
        case KEY_T:
            return "T";
        case KEY_U:
            return "U";
        case KEY_V:
            return "V";
        case KEY_W:
            return "W";
        case KEY_X:
            return "X";
        case KEY_Y:
            return "Y";
        case KEY_Z:
            return "Z";

        // Function keys
        case KEY_F1:
            return "F1";
        case KEY_F2:
            return "F2";
        case KEY_F3:
            return "F3";
        case KEY_F4:
            return "F4";
        case KEY_F5:
            return "F5";
        case KEY_F6:
            return "F6";
        case KEY_F7:
            return "F7";
        case KEY_F8:
            return "F8";
        case KEY_F9:
            return "F9";
        case KEY_F10:
            return "F10";
        case KEY_F11:
            return "F11";
        case KEY_F12:
            return "F12";

        // Control keys
        case KEY_ESCAPE:
            return "Escape";
        case KEY_ENTER:
            return "Enter";
        case KEY_TAB:
            return "Tab";
        case KEY_BACKSPACE:
            return "Backspace";
        case KEY_INSERT:
            return "Insert";
        case KEY_DELETE:
            return "Delete";
        case KEY_RIGHT:
            return "Right";
        case KEY_LEFT:
            return "Left";
        case KEY_DOWN:
            return "Down";
        case KEY_UP:
            return "Up";
        case KEY_PAGE_UP:
            return "Page Up";
        case KEY_PAGE_DOWN:
            return "Page Down";
        case KEY_HOME:
            return "Home";
        case KEY_END:
            return "End";
        case KEY_CAPS_LOCK:
            return "Caps Lock";
        case KEY_SCROLL_LOCK:
            return "Scroll Lock";
        case KEY_NUM_LOCK:
            return "Num Lock";
        case KEY_PRINT_SCREEN:
            return "Print Screen";
        case KEY_PAUSE:
            return "Pause";
        case KEY_GRAVE:
            return "Grave";

        // Modifiers
        case KEY_LEFT_SHIFT:
            return "Left Shift";
        case KEY_LEFT_CONTROL:
            return "Left Ctrl";
        case KEY_LEFT_ALT:
            return "Left Alt";
        case KEY_LEFT_SUPER:
            return "Left Super";
        case KEY_RIGHT_SHIFT:
            return "Right Shift";
        case KEY_RIGHT_CONTROL:
            return "Right Ctrl";
        case KEY_RIGHT_ALT:
            return "Right Alt";
        case KEY_RIGHT_SUPER:
            return "Right Super";
        case KEY_KB_MENU:
            return "Menu";

        // Numpad
        case KEY_KP_0:
            return "Numpad 0";
        case KEY_KP_1:
            return "Numpad 1";
        case KEY_KP_2:
            return "Numpad 2";
        case KEY_KP_3:
            return "Numpad 3";
        case KEY_KP_4:
            return "Numpad 4";
        case KEY_KP_5:
            return "Numpad 5";
        case KEY_KP_6:
            return "Numpad 6";
        case KEY_KP_7:
            return "Numpad 7";
        case KEY_KP_8:
            return "Numpad 8";
        case KEY_KP_9:
            return "Numpad 9";
        case KEY_KP_DECIMAL:
            return "Numpad .";
        case KEY_KP_DIVIDE:
            return "Numpad /";
        case KEY_KP_MULTIPLY:
            return "Numpad *";
        case KEY_KP_SUBTRACT:
            return "Numpad -";
        case KEY_KP_ADD:
            return "Numpad +";
        case KEY_KP_ENTER:
            return "Numpad Enter";
        case KEY_KP_EQUAL:
            return "Numpad =";

        default:
            return "Unknown (" + std::to_string(keyCode) + ")";
    }
}

inline std::string getMouseButtonName(const int button) {
    switch (button) {
        case MOUSE_BUTTON_LEFT:
            return "Mouse Left";
        case MOUSE_BUTTON_RIGHT:
            return "Mouse Right";
        case MOUSE_BUTTON_MIDDLE:
            return "Mouse Middle";
        case MOUSE_BUTTON_SIDE:
            return "Mouse Side";
        case MOUSE_BUTTON_EXTRA:
            return "Mouse Extra";
        case MOUSE_BUTTON_FORWARD:
            return "Mouse Forward";
        case MOUSE_BUTTON_BACK:
            return "Mouse Back";
        default:
            return "Unknown Mouse Button (" + std::to_string(button) + ")";
    }
}
