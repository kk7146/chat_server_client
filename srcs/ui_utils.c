#include "ui_utils.h"

#include <termios.h>

int screen_height, screen_width;

AppState app_state = STATE_LOGIN;

char room_name[64] = "Lobby";
char username[64] = "";

void disable_flow_control() {
    struct termios tio;
    tcgetattr(0, &tio);
    tio.c_iflag &= ~(IXON);
    tcsetattr(0, TCSANOW, &tio);
}
