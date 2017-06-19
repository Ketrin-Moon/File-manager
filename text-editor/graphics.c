#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>
#include <stdio.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>
#include "header.h"

void color_pair(WINDOW *win, int color_bg)
{
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_CYAN);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);

        wbkgd(win, COLOR_PAIR(color_bg));
}

void init()
{
	initscr();
        curs_set(0);
        refresh();
	keypad(stdscr, true);
        cbreak();
	noecho();
}

void window(char* filename)
{

        int col, row;

        WINDOW *my_win;
	
	init();
        getmaxyx(stdscr, row, col);
        my_win = newwin(row, col, 0, 0);
//	box(my_win, 0,0);
	color_pair(my_win, 2);
        wrefresh(my_win);
	
	write_file(my_win, filename);
	wrefresh(my_win);

	sleep(5);
	endwin();
	exit(0);
}
