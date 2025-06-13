#include <ncurses.h>

#include "login.h"
#include "ui_utils.h"

void run_login_page() {
    clear();
    echo();

    WINDOW *login_win = newwin(10, 40, (screen_height - 10) / 2, (screen_width - 40) / 2);
    box(login_win, 0, 0);
    mvwprintw(login_win, 1, 2, "Enter your username:");
    wrefresh(login_win);

    mvwgetnstr(login_win, 3, 2, username, sizeof(username) - 1);

    // 서버에 로그인 요청 보낼 수 있음
    // send_login_request(username);

    noecho();
    delwin(login_win);
    clear();
    refresh();

    app_state = STATE_CHAT;
}
