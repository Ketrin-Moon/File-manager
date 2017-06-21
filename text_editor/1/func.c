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
#include "func.h"
#include <unistd.h>

void cycle(WINDOW *my_win, int user_pos_X, int user_pos_Y, int row, int col, char* filename, int count)
{
	int user_char = 0, i = 0, size = 0, n = 10;
	char **buf, mas[255];

	move(0,0);
	keypad(my_win, true);
	while((user_char = wgetch(my_win)) != 'q' ){
	        switch(user_char){
                       case KEY_UP:
                               if(user_pos_Y > 0){
                                       wmove(my_win, --user_pos_Y, user_pos_X);
                                       move(user_pos_Y, user_pos_X);
                               }
                               break;
                       case KEY_DOWN:
                               if(user_pos_Y < row){
                                      wmove(my_win, ++user_pos_Y, user_pos_X);
                                      move(user_pos_Y, user_pos_X);
                              }
                            break;
			case KEY_LEFT:
                                if(user_pos_X > 0){
                                        wmove(my_win, user_pos_Y, --user_pos_X);
                                        move(user_pos_Y, user_pos_X);
                                }
                                break;
                        case KEY_RIGHT:
                                if(user_pos_X < col){
                                        wmove(my_win, user_pos_Y, ++user_pos_X);
                                        move(user_pos_Y, user_pos_X);
                                }
                                break;
			case 265:
				wrefresh(my_win);
				buf = malloc(sizeof(char*) * (row - 1));
				i=0;
				while(size = mvwinnstr(my_win, i, 0, mas, n) != ERR){
					buf[i] = malloc(n);
					strcpy(buf[i], mas);
					buf[i][n-1] = '\n';
					i++;
					if(i > (row-1)) break;
				}
				write_file(filename, buf, i-1, my_win);
				break;
			default:
				refresh();
				if(user_pos_X > (col - 5)){
					user_pos_X = 0;
					user_pos_Y++;
					wmove(my_win, user_pos_Y, user_pos_X);
					move(user_pos_Y, user_pos_X);
				}
				wprintw(my_win, "%c", user_char);
				user_pos_X++;
		}
		wrefresh(my_win);
      }


}

void write_file(char* filename, char** string, int count, WINDOW* my_win)
{
	FILE *f;
	int i;

	f = fopen(filename, "w");

	if(!f){
		printf("err\n");
		exit(1);
	}
	for(i = 0; i < count; ++i){
		fwrite(string[i], 10, 1, f);
	}
	wmove(my_win, 0, 0);
	fclose(f);
}

char** read_file(char *filename, int* count)
{
        char **strings, mas_str[255];
        int i, size = 0;
	FILE *f = fopen(filename, "r");
	if(!f){
		printf("err\n");
		exit(1);
	}


	strings = malloc(sizeof(char*));
	strings[0] = malloc(255);
        (*count) = 0;
	while(size = fread(mas_str, 1, 255, f)){
		if(size != 255){
			mas_str[size] = '\0';
			strings = realloc(strings, sizeof(char) * ((*count) + 1));
			strings[*count] = realloc(strings[*count], 256);
			strcpy(strings[*count], mas_str);
			(*count)++;
			break;
		}
		strings = realloc(strings, sizeof(char) * ((*count) + 1));
                strings[*count] = realloc(strings[*count], 256);
                strcpy(strings[*count], mas_str);
                (*count)++;
	}

	fclose(f);

        return strings;
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

void window(char* filename)
{

        int col, row, count, i;
	char** string;
	int user_char;

        WINDOW *my_win, *work_win;
        WINDOW *my_sub_win, *my_sub_win_2, *active_win;

        initscr();
        curs_set(2);
        refresh();
        getmaxyx(stdscr, row, col);
        my_win = newwin(row, col, 0, 0);
      	keypad(stdscr, true);
        cbreak();
	noecho();
	color_pair(my_win, 2);

	string = read_file(filename, &count);
	for(i = 0; i < count; i++)
		wprintw(my_win, "%s", string[i]);

        wrefresh(my_win);
	wmove(my_win, 0,0);
	cycle(my_win, 0, 0, row, col, filename, count);

      	endwin();
}



