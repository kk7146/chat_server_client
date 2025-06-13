#include <stdio.h>
#include "ui_utils.h"
#include "chat.h"
#include "login.h"

int main() {
    disable_flow_control();
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, screen_height, screen_width);

    while (app_state != STATE_EXIT) {
        if (app_state == STATE_LOGIN) {
            run_login_page();
        } else if (app_state == STATE_CHAT) {
            chat_ui_loop();
        }
    }
    endwin();
    return 0;
}