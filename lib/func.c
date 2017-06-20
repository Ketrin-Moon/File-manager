#define _SVID_SOURCE
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>
#include <stdio.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>
#include "../include/func.h"
#include <unistd.h>
#define MAX_LENGTH 255

/**
@function draw_menu
Создание верхней и нижней панели.
@param start_col - параметр, отвечающий за цвет панели
*/

void **draw_menu(int start_col)
{
	WINDOW **items;
	int row, col;

	items = (WINDOW **)malloc(sizeof(WINDOW*));
	getmaxyx(stdscr, row, col);
	items[0] = newwin(1, col, 0, start_col);
	wbkgd(items[0], COLOR_PAIR(1));
	wprintw(items[0], "\tLeft panel\t\tFile\t\tCommand\t\tSettings\t    Right panel");
	items[1] = newwin(row / 2 - 5, col, row-1, start_col);
	wbkgd(items[1], COLOR_PAIR(1));
	wprintw(items[1], "   1 Open \t 2 Save \t 3 Exit ");
	wrefresh(items[0]);
	wrefresh(items[1]);

	return EXIT_SUCCESS;
}

/**
@function print
Вывод файлов на экран с подсветкой позиции, на которой находится пользователь.
Используется для активного окна.
@param win - параметр, передающий указатель на окно, в которое необходимо
вывести список файлов
@param filenames - параметр, передающий элементы массива, которые необходимо
вывести на экран
@param count - количество элементов массива
@param user_pos - параметр, отвечающий за позицию, в которой находится
пользователь
@param color_bg - параметр, отвечающий за цвет подсветки позиции, в которой
находится пользователь
*/

void print(WINDOW *win, char **filenames, int count, int user_pos, int color_bg)
{
        int i;
        wclear(win);
        for(i = 0; i < count; i++){
                if(i == user_pos){
                        wattron(win, COLOR_PAIR(color_bg));
                        wprintw(win, "   %s\t\t\n", filenames[i]);
                        wattroff(win, COLOR_PAIR(color_bg));
                }
                else
                        wprintw(win, "   %s\n", filenames[i]);
        }

        wrefresh(win);

}

/**
@function no_print_line
Функция выводит список файлов без подсветки позиции пользователя. Используется
для вывода в пассивном окне.
@param win - параметр, передающий указатель на окно, в которое необходимо
вывести список файлов
@param filenames - параметр, передающий элементы массива, которые необходимо
вывести на экран
@param count - количество элементов массива
*/

void no_print_line(WINDOW *win, char **filenames, int count)
{
        int i;
        wclear(win);
        for(i = 0; i < count; i++)
                wprintw(win, "   %s\n", filenames[i]);
        wrefresh(win);
}

/**
@function direct
Функция считывает содержимое директории и сохраняет в массив(в
лексикографическом порядке).
@param dp - параметр, который хранит путь директории, содержимое которой
необходимо прочитать
@param count - параметр, отвечающий за количество файлов, находящихся
в директории
*/

char** direct(char *dp, int *count)
{
        struct dirent **entry;
        char **filenames;
        int i;

        (*count) = 0;
        (*count) = scandir(dp, &entry, 0, alphasort);
        filenames = (char**)malloc(sizeof(char*)*(*count));
        for(i = 1; i < (*count); ++i){
                filenames[i - 1] = malloc(255);
		strcpy(filenames[i - 1], entry[i]->d_name);
        }
        (*count)--;
        return filenames;
}

/**
@function color_pair
Функция, инициализирующая палитру цветов. Задает шаблон цветов, используемых
для оформления окон.
@param win - параметр, передающий указатель на окно, в которое необходимо
вывести список файлов
@param color_bg - параметр, отвечающий за цвет подсветки позиции, в которой
находится пользователь
*/

void color_pair(WINDOW *win, int color_bg)
{
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_CYAN);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);

        wbkgd(win, COLOR_PAIR(color_bg));
}

/**
@function window
Функция отвечает за создание и прорисовку окон, а также вызывает функцию
чтения директорий, которая отображает все файлы, находящиеся в ней, в окна менеджера.
Здесь же описана навигация по файловому менеджеру(KEY_UP, KEY_DOWN, TAB, ENTER).
*/

void window()
{

        int col, row, count_f_1 = 0, count_f_2 = 0, user_char, active_count;
        int user_pos = 0, user_pos_right = 0, user_pos_left = 0;
        char **filenames1 = direct(getenv("HOME"), &count_f_1);
        char **filenames2 = direct(getenv("HOME"), &count_f_2);
        char **active_filenames;
        char dir_f_1[255], dir_f_2[255];

        WINDOW *my_win, *my_win_2;
        WINDOW *my_sub_win, *my_sub_win_2, *active_win;

        initscr();
        curs_set(0);
        refresh();
        getmaxyx(stdscr, row, col);
        my_win = newwin(row, col / 2 , 0, 0);
	box(my_win, 0, 0);
        my_sub_win = derwin(my_win, row - 2, col / 2 - 2, 1, 1);
        my_win_2 = newwin(row, col/2, 0, col/2);
        box(my_win_2, 0, 0);
        my_sub_win_2 = derwin(my_win_2, row - 2, col / 2 - 2 , 1, 1);
      	keypad(stdscr, true);
        cbreak();

        user_pos = count_f_1;

        color_pair(my_win, 2);
        color_pair(my_win_2, 2);
        color_pair(my_sub_win, 2);
        color_pair(my_sub_win_2, 2);

        print(my_sub_win, filenames1, count_f_1, user_pos, 1);
        print(my_sub_win_2, filenames2, count_f_2, user_pos, 1);
        wrefresh(my_win);
        wrefresh(my_win_2);

	draw_menu(0);

        active_win = my_sub_win;
        active_count = count_f_1;
        active_filenames = filenames1;

        strcpy(dir_f_1, getenv("HOME"));
        strcpy(dir_f_2, getenv("HOME"));

        noecho();

	user_pos = 0;

        while((user_char = getch()) != 'q' ){
                switch(user_char){
                        case KEY_UP:
                                if(user_pos > 0)
                                        user_pos--;
				print(active_win, active_filenames, active_count, user_pos, 1);
                                break;
                        case KEY_DOWN:
				if(user_pos < active_count - 1)
                                        user_pos++;
                                print(active_win, active_filenames, active_count, user_pos, 1);
                                break;
                        case 0x9:
                                if(active_win == my_sub_win){
                                        no_print_line(active_win, active_filenames, active_count);
                                        active_win = my_sub_win_2;
                                        active_filenames = filenames2;
                                        active_count = count_f_2;
                                        user_pos_left = user_pos;
                                        user_pos = user_pos_right;
                                        print(active_win, active_filenames, active_count, user_pos, 1);
                                }
                                else
                                        if(active_win == my_sub_win_2){
                                                no_print_line(active_win, active_filenames, active_count);
                                                active_win = my_sub_win;
                                                active_filenames = filenames1;
                                                active_count = count_f_1;
                                                user_pos_right = user_pos;
                                                user_pos = user_pos_left;
                                                print(active_win, active_filenames, active_count, user_pos, 1);
                                        }
                                break;
                        case 10:
                                if(active_win == my_sub_win){
                                        if(!chdir(dir_f_1)){
                                                if(!chdir(active_filenames[user_pos])){
                                                        free(filenames1);
							filenames1 = NULL;
                                                        filenames1 = direct(".", &count_f_1);
                                                        user_pos = 0;
                                                        active_filenames = filenames1;
                                                        active_count = count_f_1;
                                                        getcwd(dir_f_1, MAX_LENGTH);
                                                        print(active_win, active_filenames, active_count, user_pos, 1);
                                                }
                                        }
                                }
                                else
                                        if(!chdir(dir_f_2)){
                                                if(!chdir(active_filenames[user_pos])){
                                                        free(filenames2);
                                                        filenames2 = NULL;
                                                        filenames2 = direct(".", &count_f_2);
                                                        user_pos = 0;
                                                        active_filenames = filenames2;
                                                        active_count = count_f_2;
                                                        getcwd(dir_f_2, MAX_LENGTH);
                                                        print(active_win, active_filenames, active_count, user_pos, 1);
                                                }
                                        }
                                break;
                }
                wrefresh(my_sub_win);
                wrefresh(active_win);
        }
        endwin();
        exit(0);
}



