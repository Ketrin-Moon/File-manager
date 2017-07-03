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
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_LENGTH 255
#define COPY_BLOCK_SIZE 10

struct Params{
    FILE *in;
    FILE *out;
    char dir1[255];
    char dir2[255];
};

typedef struct Params params;

void* copy(void *param)
{
    unsigned char buf[COPY_BLOCK_SIZE];
    size_t bytes;
    params p = *((params*)param);

    while((bytes = fread(buf, sizeof(char), COPY_BLOCK_SIZE, p.in)) != (size_t)EOF && bytes > 0){
	fwrite(buf, sizeof(char), bytes, p.out);
    }

}

void print_win(void* param)
{
	WINDOW *win, *subwin, *subwin2;
	int row, col;
	params p = *((params*)param);

	getmaxyx(stdscr, row, col);
	win = newwin(row/2 - 10, col/2, 15, 30);
	box(win, 0,0);
	subwin = newwin(row/2 - 20, col/2 - 10, 22, 35);
	subwin2 = newwin(row/2 - 20, col/2 - 10, 18, 35);
	color_pair(win, 3);
	color_pair(subwin, 1);
	color_pair(subwin2, 1);
	wprintw(subwin, "%s", p.dir1);
	wprintw(subwin2, "%s", p.dir2);
	wrefresh(win);
	wrefresh(subwin);
	wrefresh(subwin2);
//	delwin(win);
//	delwin(subwin);
//	delwin(subwin2);
}


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
	wprintw(items[1], "     1 Open(read) \t\t\t 2 Open(read+write) \t\t\t 3 Exit  ");
	wrefresh(items[0]);
	wrefresh(items[1]);

	return EXIT_SUCCESS;
}


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

void no_print_line(WINDOW *win, char **filenames, int count)
{
        int i;
        wclear(win);
        for(i = 0; i < count; i++)
                wprintw(win, "   %s\n", filenames[i]);
        wrefresh(win);
}


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

void color_pair(WINDOW *win, int color_bg)
{
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_CYAN);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);

        wbkgd(win, COLOR_PAIR(color_bg));
}

void init_win()
{
	system("clear");
	initscr();
	curs_set(0);
	refresh();
	keypad(stdscr, true);
	cbreak();
	noecho();
}

void window()
{

        int col, row, count_f_1 = 0, count_f_2 = 0, user_char, active_count;
        int user_pos = 0, user_pos_right = 0, user_pos_left = 0;
        char **filenames1 = direct(getenv("PWD"), &count_f_1);
        char **filenames2 = direct(getenv("PWD"), &count_f_2);
        char **active_filenames;
        char dir_f_1[255], dir_f_2[255];
	struct stat sb;
	int status;
	pid_t pid;
	char buffer[256];


        WINDOW *my_win, *my_win_2;
        WINDOW *my_sub_win, *my_sub_win_2, *active_win;

	init_win();

        getmaxyx(stdscr, row, col);
        my_win = newwin(row, col / 2 , 0, 0);
	box(my_win, 0, 0);
        my_sub_win = derwin(my_win, row - 2, col / 2 - 2, 1, 1);
        my_win_2 = newwin(row, col/2, 0, col/2);
        box(my_win_2, 0, 0);
        my_sub_win_2 = derwin(my_win_2, row - 2, col / 2 - 2 , 1, 1);

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

        strcpy(dir_f_1, getenv("PWD"));
        strcpy(dir_f_2, getenv("PWD"));

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
				if(lstat(active_filenames[user_pos], &sb) < 0)
					printf("Lstat error\n");
				if(S_ISREG(sb.st_mode)){
					pid = fork();
					if(pid == 0){
						execlp("text_editor", "text_editor", active_filenames[user_pos], NULL);
						exit(0);
					}
					wait(&status);
					init_win();
					print(active_win, active_filenames, active_count, user_pos, 1);
					refresh();
				}
				else{

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
				}
				break;
			case '1':
				memset(buffer, 0, 256);
				params *p;
				pthread_t tid[2];

				p = (params*)malloc(sizeof(params));
				strcpy(p->dir1, dir_f_1);
				strcpy(p->dir2, dir_f_2);

				FILE *in, *out;

				pthread_create(&(tid[0]), NULL, print_win, (void *)p);

				if(active_win == my_sub_win){
				    in = fopen(active_filenames[user_pos], "r");

	    			    strcat(buffer, dir_f_2);
				    strcat(buffer, "/");
				    strcat(buffer, active_filenames[user_pos]);

				    out = fopen(buffer, "a+");

				    p = (params*)malloc(sizeof(params));
				    p->in=in;
				    p->out=out;
				    status = pthread_create(&(tid[1]), NULL, copy, (void *)p);
				    filenames2 = direct(dir_f_2, &count_f_2);
				    print(active_win, active_filenames, active_count, user_pos, 1);
				    print(my_sub_win_2, filenames2, count_f_2, 0, 1);
				    color_pair(my_win, 2);
				    color_pair(my_win_2, 2);
				    wrefresh(my_win);
				    wrefresh(my_win_2);
				    draw_menu(0);
 				    wrefresh(active_win);
				    wrefresh(my_sub_win_2);
				} else {
				    in = fopen(active_filenames[user_pos], "r");

	    			    strcat(buffer, dir_f_1);
				    strcat(buffer, "/");
				    strcat(buffer, active_filenames[user_pos]);

				    out = fopen(buffer, "w");

				    p = (params*)malloc(sizeof(params));
				    p->in=in;
				    p->out=out;
				    pthread_create(&(tid[1]), NULL, copy, (void *)p);
				    filenames1 = direct(dir_f_1, &count_f_1);
				    print(active_win, active_filenames, active_count, user_pos, 1);
				    print(my_sub_win, filenames1, count_f_1, 0, 1);
				    color_pair(my_win, 2);
				    color_pair(my_win_2, 2);
				    wrefresh(my_win);
				    wrefresh(my_win_2);
				    draw_menu(0);
 				    wrefresh(active_win);
				    wrefresh(my_sub_win);
				}
				status = pthread_join(tid[1], &status);
//				status = pthread_join(tid[0], &status);
				fclose(in);
				fclose(out);

				break;
		}
                wrefresh(my_sub_win);
                wrefresh(active_win);
        }
        endwin();
	system("clear");
        exit(0);
}



