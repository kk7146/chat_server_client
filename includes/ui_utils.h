#ifndef UI_UTILS_H
#define UI_UTILS_H

#define MAX_INPUT 256

// 화면의 상태
typedef enum {
    STATE_LOGIN,
    STATE_CHAT,
    STATE_EXIT
} AppState;

extern AppState app_state;

extern int screen_height, screen_width;

extern char room_name[64];
extern char username[64];

void disable_flow_control();

#endif