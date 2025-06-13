#ifndef CHAT_H
#define CHAT_H

#include <ncurses.h>

// chat 윈도우 사이의 포커스 상태
typedef enum {
    FOCUS_INPUT,
    FOCUS_MENU,
    FOCUS_USERS,
    FOCUS_ROOMS
} FocusState;

void chat_ui_loop();

#endif