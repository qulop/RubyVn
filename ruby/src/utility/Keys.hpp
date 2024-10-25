#pragma once

#include <types/TypeTraits.hpp>


namespace Ruby::Keys {
    constexpr i32 KEY_NONE = -1;


    constexpr i32 KEY_SPACE = 32;
    constexpr i32 KEY_APOSTROPHE = 39;   //  '
    constexpr i32 KEY_COMMA = 44;        //  ;
    constexpr i32 KEY_MINUS = 45;        //  -
    constexpr i32 KEY_DOT = 46;          //  .
    constexpr i32 KEY_SLASH = 47;        //  /


    constexpr i32 KEY_0 = 48;
    constexpr i32 KEY_1 = 49;
    constexpr i32 KEY_2 = 50;
    constexpr i32 KEY_3 = 51;
    constexpr i32 KEY_4 = 52;
    constexpr i32 KEY_5 = 53;
    constexpr i32 KEY_6 = 54;
    constexpr i32 KEY_7 = 55;
    constexpr i32 KEY_8 = 56;
    constexpr i32 KEY_9 = 57;


    constexpr i32 KEY_SEMICOLON = 59;    //  ;
    constexpr i32 KEY_EQUAL = 61;        //  =


    constexpr i32  KEY_A = 65;
    constexpr i32  KEY_B = 66;
    constexpr i32  KEY_C = 67;
    constexpr i32  KEY_D = 68;
    constexpr i32  KEY_E = 69;
    constexpr i32  KEY_F = 70;
    constexpr i32  KEY_G = 71;
    constexpr i32  KEY_H = 72;
    constexpr i32  KEY_I = 73;
    constexpr i32  KEY_J = 74;
    constexpr i32  KEY_K = 75;
    constexpr i32  KEY_L = 76;
    constexpr i32  KEY_M = 77;
    constexpr i32  KEY_N = 78;
    constexpr i32  KEY_O = 79;
    constexpr i32  KEY_P = 80;
    constexpr i32  KEY_Q = 81;
    constexpr i32  KEY_R = 82;
    constexpr i32  KEY_S = 83;
    constexpr i32  KEY_T = 84;
    constexpr i32  KEY_U = 85;
    constexpr i32  KEY_V = 86;
    constexpr i32  KEY_W = 87;
    constexpr i32  KEY_X = 88;
    constexpr i32  KEY_Y = 89;
    constexpr i32  KEY_Z = 90;


    constexpr i32 KEY_LEFT_BRACKET = 91;     //  [
    constexpr i32 KEY_BACKSLASH = 92;
    constexpr i32 KEY_RIGHT_BRACKET = 93;    //  ]
    constexpr i32 KEY_GRAVE_ACCENT = 96;     //  `


    constexpr i32  KEY_ESCAPE = 256;
    constexpr i32  KEY_ENTER = 257;
    constexpr i32  KEY_TAB = 258;
    constexpr i32  KEY_BACKSPACE = 259;
    constexpr i32  KEY_INSERT = 260;
    constexpr i32  KEY_DELETE = 261;
    constexpr i32  KEY_RIGHT = 262;
    constexpr i32  KEY_LEFT = 263;
    constexpr i32  KEY_DOWN = 264;
    constexpr i32  KEY_UP = 265;
    constexpr i32  KEY_HOME = 268;
    constexpr i32  KEY_END = 269;
    constexpr i32  KEY_CAPS_LOCK = 280;
    constexpr i32  KEY_PRi32_SCREEN = 283;
    constexpr i32  KEY_PAUSE = 284;


    constexpr i32 KEY_F1 = 290;
    constexpr i32 KEY_F2 = 291;
    constexpr i32 KEY_F3 = 292;
    constexpr i32 KEY_F4 = 293;
    constexpr i32 KEY_F5 = 294;
    constexpr i32 KEY_F6 = 295;
    constexpr i32 KEY_F7 = 296;
    constexpr i32 KEY_F8 = 297;
    constexpr i32 KEY_F9 = 298;
    constexpr i32 KEY_F10 = 299;
    constexpr i32 KEY_F11 = 300;
    constexpr i32 KEY_F12 = 301;
    constexpr i32 KEY_F13 = 302;
    constexpr i32 KEY_F14 = 303;
    constexpr i32 KEY_F15 = 304;
    constexpr i32 KEY_F16 = 305;
    constexpr i32 KEY_F17 = 306;
    constexpr i32 KEY_F18 = 307;
    constexpr i32 KEY_F19 = 308;
    constexpr i32 KEY_F20 = 309;
    constexpr i32 KEY_F21 = 310;
    constexpr i32 KEY_F22 = 311;
    constexpr i32 KEY_F23 = 312;
    constexpr i32 KEY_F24 = 313;
    constexpr i32 KEY_F25 = 314;


    // keypad keys
    constexpr i32 KEY_KP_0 = 320;
    constexpr i32 KEY_KP_1 = 321;
    constexpr i32 KEY_KP_2 = 322;
    constexpr i32 KEY_KP_3 = 323;
    constexpr i32 KEY_KP_4 = 324;
    constexpr i32 KEY_KP_5 = 325;
    constexpr i32 KEY_KP_6 = 326;
    constexpr i32 KEY_KP_7 = 327;
    constexpr i32 KEY_KP_8 = 328;
    constexpr i32 KEY_KP_9 = 329;
    constexpr i32 KEY_KP_DECIMAL = 330;
    constexpr i32 KEY_KP_DIVIDE = 331;
    constexpr i32 KEY_KP_MULTIPLY = 332;
    constexpr i32 KEY_KP_SUBTRACT = 333;
    constexpr i32 KEY_KP_ADD = 334;
    constexpr i32 KEY_KP_ENTER = 335;
    constexpr i32 KEY_KP_EQUAL = 336;


    constexpr i32  KEY_LEFT_SHIFT = 340;
    constexpr i32  KEY_LEFT_CONTROL = 341;
    constexpr i32  KEY_LEFT_ALT = 342;
    constexpr i32  KEY_LEFT_SUPER = 343;
    constexpr i32  KEY_RIGHT_SHIFT = 344;
    constexpr i32  KEY_RIGHT_CONTROL = 345;
    constexpr i32  KEY_RIGHT_ALT = 346;
    constexpr i32  KEY_RIGHT_SUPER = 347;
    constexpr i32  KEY_MENU = 348;
    constexpr i32  KEY_LAST = KEY_MENU;


    constexpr i32 KEY_MOUSE_BUTTON_1  = 0;
    constexpr i32 KEY_MOUSE_BUTTON_2  = 1;
    constexpr i32 KEY_MOUSE_BUTTON_3  = 2;
    constexpr i32 KEY_MOUSE_BUTTON_4  = 3;
    constexpr i32 KEY_MOUSE_BUTTON_5  = 4;
    constexpr i32 KEY_MOUSE_BUTTON_6  = 5;
    constexpr i32 KEY_MOUSE_BUTTON_7  = 6;
    constexpr i32 KEY_MOUSE_BUTTON_8  = 7;
    constexpr i32 KEY_MOUSE_BUTTON_LAST = KEY_MOUSE_BUTTON_8;
    constexpr i32 KEY_MOUSE_BUTTON_LEFT = KEY_MOUSE_BUTTON_1;
    constexpr i32 KEY_MOUSE_BUTTON_RIGHT = KEY_MOUSE_BUTTON_1;
    constexpr i32 KEY_MOUSE_BUTTON_MIDDLE = KEY_MOUSE_BUTTON_3;
}