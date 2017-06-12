#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>
#include <stdio.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>

WINDOW *my_win;
WINDOW *my_win_2;

WINDOW *create_newwin(int h, int w, int y, int x)
{
    WINDOW *local_win;

    local_win = newwin(h, w,y,x);
    box(local_win, 0,0);
    wrefresh(local_win);
    return local_win;
}

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char*) &size);
	resizeterm(size.ws_row, size.ws_col);
}

void color_pair()
{
    start_color();
    refresh();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_RED);
}

void print(WINDOW *my_win,int x, int y, char *str)
{
    mvwprintw(my_win, x, y, "%s",str);
    wrefresh(my_win);
}

void direct(char dp[255])
{
    DIR *dir;
    
    struct dirent *entry;
    char **filenames;
    char user_char;
//    char position_symbol[] = "->";
    char path[255];
//    int user_position = 0;
    int i;

    strcpy(path, dp);
    dir = opendir(dp);

    if(!dir){
	printf("No directory\n");
	exit(1);
    }

    filenames = malloc(sizeof(char*)*255);

    while((entry = readdir(dir)) != NULL){
	filenames[i] = entry->d_name;
	print(my_win, i+1, 2, filenames[i]);
	print(my_win_2, i+1, 2, filenames[i]);
    	i++;
    }

//    int count = i;

    closedir(dir);

    user_char = getchar();
    switch(user_char){
        case 'q':
    	    endwin();
	    exit(0);
    }
}


void window(char dp[255])
{
	
	int x,y,w,h;

	initscr();
	signal(SIGWINCH, sig_winch);
	nonl();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	h=getmaxy(stdscr);
	w=getmaxx(stdscr)/2;
	y=0;
	x=0;    
	noecho();
	refresh();
	color_pair();
	my_win=create_newwin(h,w,y,x);
	wbkgd(my_win, COLOR_PAIR(1));
	my_win_2 = create_newwin(h,w,y,x+w);
	wbkgd(my_win_2, COLOR_PAIR(1));
	wrefresh(my_win);	
	wrefresh(my_win_2);

	direct(dp);

/*	switch(getchar()){
	    case 'q':
		endwin();
		exit(0);
	}
*/
}


int main()
{
    char path[256] = "/";

    window(path);
//    direct(path);
    
    return 0;

}
