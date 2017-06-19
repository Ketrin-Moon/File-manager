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

void read_file(WINDOW *win, char* filename)
{
    FILE *fp = fopen(filename, "r");

    char *string;
    int size = 0;

    string = (char*)calloc(256, sizeof(char*));
    size = fread(string, 1, 1024, fp);
    wprintw(win, "%s", string);


    free(string);
    fclose(fp);
}

void write_file(WINDOW *win, char *filename)
{

    FILE *fp = fopen(filename, "a+");

    char *string;
    int size = 0, w_size = 0, i;

    string = (char*)calloc(256, sizeof(char*));
    size = fread(string, 1, 1024, fp);
    wprintw(win, "%s", string);
    free(string);

    string = (char*)calloc(256, sizeof(char*));
    w_size = fwrite(string, 1, 255, fp);
    
    
    fclose(fp);
}

