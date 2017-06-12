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
    char path[255];
    int user_position = 0;
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
    	i++;
		
    }

    int count = i;
    while(true){
        clear();
	for(i = 0; i < count; i++){
	    if(i == user_position)
    		addch('>');
	    else
		addch(' ');
		printw("%s\n",filenames[i]);
    

	}
    
	i = 0;
        switch(getch()){
	    case KEY_UP:
		if(user_position > 0)
		    user_position--;
		break;
	    case KEY_DOWN:
		if(user_position < count)
		    user_position++;
		break;
	    case 'm':
		strcat(path,"/");
		strcat(path, filenames[user_position]);
		if(strcmp(filenames[user_position], "..") == 0)
		    chdir("..");
		direct(path);
		free(filenames);
		closedir(dir);
    	    case 'q':
    		endwin();
		exit(0);
	}
      }
//    closedir(dir);
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
	wbkgd(my_win, COLOR_PAIR(2));
	my_win_2 = create_newwin(h,w,y,x+w);
	wbkgd(my_win_2, COLOR_PAIR(2));
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
    
    return 0;

}
