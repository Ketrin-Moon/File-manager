#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>
#include <stdio.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>

void print(WINDOW *win, char **filenames, int count, int user_pos, int color_bg)
{
	int i;
	wclear(win);
	for(i = 0; i < count; i++){
		if(i == user_pos){
			wattron(win, COLOR_PAIR(color_bg));
			wprintw(win, "   %s\t\t\t\t\t\t\t\t\t\t\t\n", filenames[i]);
			wattroff(win, COLOR_PAIR(color_bg));
		}
		else
			wprintw(win, "   %s\n", filenames[i]);
	}

	wrefresh(win);

}

void print_tab(WINDOW *win, char **filenames, int count, int user_pos, int color_bg)
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
	init_pair(1, COLOR_WHITE, COLOR_CYAN);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);

	wbkgd(win, COLOR_PAIR(color_bg));
}


/*

char** direct(char *dp, int *count)
{
    DIR *dir;
 
    struct dirent *entry;
    char **filenames;

    dir = opendir(dp);

    if(!dir){
	printf("No directory\n");
	exit(1);
    }

    filenames =(char **)malloc(sizeof(char*));

    while((entry = readdir(dir)) != NULL){
	filenames = (char**)realloc(filenames, sizeof(char*) * (*count+1));
	filenames[*count] = (char*)malloc(255);
	strcpy(filenames[*count], entry->d_name);
//	filenames[*count] = entry->d_name;
  	(*count)++;
    }

    closedir(dir);

    return filenames;
}
*/

void window()
{

	int col, row, count_f_1 = 0, count_f_2 = 0, user_char, active_count;
	int user_pos = 0, user_pos_right = 0, user_pos_left = 0;
	char **filenames1 = direct(getenv("HOME"), &count_f_1);
	char **filenames2 = direct(getenv("HOME"), &count_f_2);
	char **active_filenames;

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

	active_win = my_sub_win;
	active_count = count_f_1;
	active_filenames = filenames1;

	noecho();

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
					print_tab(active_win, active_filenames, active_count, user_pos, 1);
					active_win = my_sub_win_2;
					active_filenames = filenames2;
					active_count = count_f_2;
					user_pos_left = user_pos;
					user_pos = user_pos_right;
					print(active_win, active_filenames, active_count, user_pos, 1);
				}
				else 
					if(active_win == my_sub_win_2){
						print_tab(active_win, active_filenames, active_count, user_pos, 1);
						active_win = my_sub_win;
						active_filenames = filenames1;
						active_count = count_f_1;
						user_pos_right = user_pos;
						user_pos = user_pos_left;
						print(active_win, active_filenames, active_count, user_pos, 1);
					}
				break;
		}
		wrefresh(my_sub_win);
	}
	endwin();
	exit(0);


}


int main()
{

    window();

    return 0;

}
