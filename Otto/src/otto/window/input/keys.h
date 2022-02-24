#pragma once

#include "otto/base.h"

namespace otto 
{
    using Key = uint8;

    enum class Keys : Key
    {
        // Windows

        BACKSPACE = 0x08,
        TAB = 0x09,
        CLEAR = 0x0c,

        ENTER = 0x0d,
        LEFT_ENTER = ENTER,
        NUMPAD_ENTER = 0x0f,

        SHIFT = 0x10,
        LEFT_SHIFT = SHIFT,

        CONTROL = 0x11,
        LEFT_CONTROL = CONTROL,

        ALT = 0x12,
        LEFT_ALT = ALT,

        PAUSE = 0x13,

        CAPS_LOCK = 0x14,

        IME_KANA = 0x15,
        IME_HANGUL = 0x15,
        IME_ON = 0x16,
        IME_JUNJA = 0x17,
        IME_FINAL = 0x18,
        IME_HANJA = 0x19,
        IME_KANJI = 0x19,
        IME_OFF = 0x1a,

        ESCAPE = 0x1b,

        IME_CONVERT = 0x01c,
        IME_NONCONVERT = 0x1d,
        IME_ACCEPT = 0x1e,
        IME_MODE_CHANGE = 0x1f,

        SPACE = 0x20,

        PAGE_UP = 0x21,
        PAGE_DOWN = 0x22,
        END = 0x23,
        HOME = 0x24,
        POS_1 = HOME,

        LEFT = 0x25,
        ARROW_LEFT = LEFT,
        UP = 0x26,
        ARROW_UP = UP,
        RIGHT = 0x27,
        ARROW_RIGHT = RIGHT,
        DOWN = 0x28,
        ARROW_DOWN = DOWN,
        SELECT = 0x29,
        PRINT = 0x2a,
        EXECUTE = 0x2b,
        PRINT_SCREEN = 0x2c,
        INSERT = 0x2d,
        DELETE = 0x2e,
        HELP = 0x2f,

        _0 = 0x30,
        K_0 = 0x30,
        _1 = 0x31,
        K_1 = 0x31,
        _2 = 0x32,
        K_2 = 0x32,
        _3 = 0x33,
        K_3 = 0x33,
        _4 = 0x34,
        K_4 = 0x34,
        _5 = 0x35,
        K_5 = 0x35,
        _6 = 0x36,
        K_6 = 0x36,
        _7 = 0x37,
        K_7 = 0x37,
        _8 = 0x38,
        K_8 = 0x38,
        _9 = 0x39,
        K_9 = 0x39,

        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4a,
        K = 0x4b,
        L = 0x4c,
        M = 0x4d,
        N = 0x4e,
        O = 0x4f,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5a,

        WINDOWS = 0x5b,
        LEFT_WINDOWS = WINDOWS,
        RIGHT_WINDOWS = 0x5c,

        APPS = 0x5d,
        SLEEP = 0x5f,

        NUMPAD_0 = 0x60,
        NUMPAD_1 = 0x61,
        NUMPAD_2 = 0x62,
        NUMPAD_3 = 0x63,
        NUMPAD_4 = 0x64,
        NUMPAD_5 = 0x65,
        NUMPAD_6 = 0x66,
        NUMPAD_7 = 0x67,
        NUMPAD_8 = 0x68,
        NUMPAD_9 = 0x69,

        NUMPAD_MULTIPLY= 0x6a,
        NUMPAD_ADD = 0x6b,

        SEPERATOR = 0x6c,

        NUMPAD_SUBSTRACT = 0x6d,
        NUMPAD_DECIMAL = 0x6e,
        NUMPAD_DIVIDE = 0x6f,

        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,
        F10 = 0x79,
        F11 = 0x7a,
        F12 = 0x7b,
        F13 = 0x7c,
        F14 = 0x7d,
        F15 = 0x7e,
        F16 = 0x7f,
        F17 = 0x80,
        F18 = 0x81,
        F19 = 0x82,
        F20 = 0x83,
        F21 = 0x84,
        F22 = 0x85,
        F23 = 0x86,
        F24 = 0x87,

        NUM_LOCK = 0x90,
        SCROLL = 0x91,

        RIGHT_SHIFT = SHIFT, // They appear to be the same, at least on windows
        // 0xa2 is another left shift?

        RIGHT_CONTROL = 0xa3,
        // 0xa2 is another left control?

        RIGHT_ALT = 0xa5,
        // 0xa2 is another left alt?

        BROWSER_BACK = 0xa6,
        BROWSER_FORWARD = 0xa7,
        BROWSER_REFRESH = 0xa8,
        BROWSER_STOP = 0xa9,
        BROWSER_SEARCH = 0xaa,
        BROWSER_FAVORITES = 0xab,
        BROWSER_HOME = 0xac,

        VOLUME_MUTE = 0xad,
        VOLUME_DOWN = 0xae,
        VOLUME_UP = 0xaf,

        MEDIA_NEXT_TRACK = 0xb0,
        MEDIA_PREVIOUS_TRACK = 0xb1,
        MEDIA_STOP = 0xb2,
        MEDIA_PLAY = 0xb3,
        MEDIA_PAUSE = 0xb3,

        START_MAIL = 0xb5,
        SELECT_MEDIA = 0xb5,
        START_APP_1 = 0xb6,
        START_APP_2 = 0xb7,
            
        OEM_1 = 0xba,

        PLUS = 0xbb,
        COMMA = 0xbc,
        MINUS = 0xbd,
        PERIOD = 0xbe,

        OEM_2 = 0xbf,
        OEM_3 = 0xc0,
        OEM_4 = 0xdb,
        OEM_5 = 0xdc,
        OEM_6 = 0xdd,
        OEM_7 = 0xde,
        OEM_8 = 0xdf,
        OEM_102 = 0xe2,
        
        IME_PROCESS = 0xe5,
        ATTN = 0xf6,
        CRSEL = 0xf7,
        EX_SEL = 0xf8,
        ERASE_EOF = 0xf9,
        PLAY = 0xfa,
        ZOOM = 0xfb,
        PA_1 = 0xfd,
            
        OEM_CLEAR = 0xfe,

        _HIGHEST_KEY = 0xfe
    };


} // namespace otto
