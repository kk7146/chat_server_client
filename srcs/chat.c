#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "chat.h"
#include "ui_utils.h"

int selected_menu = 0;
int selected_user = 0;
int selected_room = 0;

WINDOW *help_win, *chat_win, *input_win;
WINDOW *userlist_win, *roomlist_win;
FocusState focus_state = FOCUS_INPUT;

int input_height = 3;
int help_height = 3;
int chat_line = 2;

static void init_chat_win();
static void init_menu_win();
static void init_userlist_win();
static void init_roomlist_win();
static void init_input_win();
static void destroy_ui();
static void update_chat_header();
static void update_menu_win();
static void update_userlist_win();
static void update_roomlist_win();

static void destroy_ui() {
    delwin(help_win);
    delwin(chat_win);
    delwin(input_win);
    delwin(userlist_win);
    delwin(roomlist_win);
}

static void init_ui() {
    clear();
    refresh();
    init_menu_win();
    init_chat_win();
    init_userlist_win();
    init_roomlist_win();
    init_input_win();
}

static void update_chat_header() {
    mvwprintw(chat_win, 0, 1, "[Current Room: %s]", room_name);
    wclrtoeol(chat_win);
    box(chat_win, 0, 0);
    wrefresh(chat_win);
}

static void init_chat_win() {
    int userlist_width = 20;
    int chat_height = screen_height - input_height - help_height;
    chat_win = newwin(chat_height, screen_width - userlist_width, help_height, 0);
    scrollok(chat_win, TRUE);
    box(chat_win, 0, 0);
    update_chat_header();
}

static void update_menu_win() {
    const char* labels[] = {"Exit", "Select Room", "Leave Room"};
    int label_count = 3;
    int x = 2;

    werase(help_win);
    box(help_win, 0, 0);

    for (int i = 0; i < label_count; ++i) {
        if (i == selected_menu)
            wattron(help_win, A_REVERSE);
        
        mvwprintw(help_win, 1, x, "[%s]", labels[i]);

        if (i == selected_menu)
            wattroff(help_win, A_REVERSE);

        x += strlen(labels[i]) + 4;
    }

    wrefresh(help_win);
}

static void init_menu_win() {
    help_win = newwin(help_height, screen_width, 0, 0);
    box(help_win, 0, 0);
    update_menu_win();
}

static void update_userlist_win() {
    const char *users[] = {"- Alice", "- Bob"};
    for (int i = 0; i < 2; i++) {
        if (i == selected_user) wattron(userlist_win, A_REVERSE);
        mvwprintw(userlist_win, i + 2, 1, users[i]);
        if (i == selected_user) wattroff(userlist_win, A_REVERSE);
    }
    wrefresh(userlist_win);
}

static void init_userlist_win() {
    int userlist_width = 20;
    int chat_height = screen_height - input_height - help_height;
    int side_height = chat_height + input_height;
    int userlist_height = side_height / 2;
    userlist_win = newwin(userlist_height, userlist_width, help_height, screen_width - userlist_width);
    box(userlist_win, 0, 0);
    update_userlist_win();
}

static void update_roomlist_win() {
    const char *rooms[] = {"- Lobby", "- Dungeon", "- Arena"};
    for (int i = 0; i < 3; i++) {
        if (i == selected_room) wattron(roomlist_win, A_REVERSE);
        mvwprintw(roomlist_win, i + 2, 1, rooms[i]);
        if (i == selected_room) wattroff(roomlist_win, A_REVERSE);
    }
    wrefresh(roomlist_win);
}

static void init_roomlist_win() {
    int userlist_width = 20;
    int chat_height = screen_height - input_height - help_height;
    int side_height = chat_height + input_height;
    int userlist_height = side_height / 2;
    int roomlist_height = side_height - userlist_height;
    roomlist_win = newwin(roomlist_height, userlist_width, help_height + userlist_height, screen_width - userlist_width);
    box(roomlist_win, 0, 0);
    update_roomlist_win();
}

static void init_input_win() {
    int userlist_width = 20;
    input_win = newwin(input_height, screen_width - userlist_width, screen_height - input_height, 0);
    keypad(input_win, TRUE);
    box(input_win, 0, 0);
    wrefresh(input_win);
}

void chat_ui_loop() {
    char input[MAX_INPUT];
    int max_chat_lines = screen_height - input_height - help_height - 2;

    init_ui();

    while (1) {
        int len = 0;
        memset(input, 0, sizeof(input));

        werase(input_win);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 1, "> ");
        wmove(input_win, 1, 3);
        wrefresh(input_win);

        int ch;
        while ((ch = wgetch(input_win)) != '\n') {
            if (ch == KEY_RIGHT) {
                if (focus_state == FOCUS_INPUT) focus_state = FOCUS_MENU;
                else if (focus_state == FOCUS_MENU) focus_state = FOCUS_USERS;
                else if (focus_state == FOCUS_USERS) focus_state = FOCUS_ROOMS;
            } else if (ch == KEY_LEFT) {
                if (focus_state == FOCUS_ROOMS) focus_state = FOCUS_USERS;
                else if (focus_state == FOCUS_USERS) focus_state = FOCUS_MENU;
                else if (focus_state == FOCUS_MENU) focus_state = FOCUS_INPUT;
            } else if (ch == KEY_UP || ch == KEY_DOWN) {
                if (focus_state == FOCUS_MENU) {
                    selected_menu += (ch == KEY_DOWN) - (ch == KEY_UP);
                    if (selected_menu < 0) selected_menu = 0;
                    if (selected_menu > 2) selected_menu = 2; // TODO : 이 부분 개수를 뜻해서 구조체 만들면 바꿔야 함
                    update_menu_win();
                } else if (focus_state == FOCUS_USERS) {
                    selected_user += (ch == KEY_DOWN) - (ch == KEY_UP);
                    if (selected_user < 0) selected_user = 0;
                    if (selected_user > 1) selected_user = 1;
                    update_userlist_win();
                } else if (focus_state == FOCUS_ROOMS) {
                    selected_room += (ch == KEY_DOWN) - (ch == KEY_UP);
                    if (selected_room < 0) selected_room = 0;
                    if (selected_room > 2) selected_room = 2;
                    update_roomlist_win();
                }
            }
            if (focus_state == FOCUS_INPUT) {
                if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                    if (len > 0) {
                        len--;
                        input[len] = '\0';
                        mvwprintw(input_win, 1, 3 + len, " ");
                        wmove(input_win, 1, 3 + len);
                    }
                } else if (len < MAX_INPUT - 1 && ch >= 32 && ch <= 126) {
                    input[len++] = ch;
                    mvwaddch(input_win, 1, 3 + len - 1, ch);
                }
            }
            wrefresh(input_win);
        }

        input[len] = '\0';

        // TODO: 서버로 메시지 전송
        mvwprintw(chat_win, chat_line++, 1, "%s: %s", username, input);

        if (chat_line >= max_chat_lines) {
            wscrl(chat_win, 1);
            chat_line = max_chat_lines;
            box(chat_win, 0, 0);
            update_chat_header();
        }

        wrefresh(chat_win);
    }
    destroy_ui();
}
