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

int main()
{
    char *filename = "t.txt";

    window(filename);
    //read_file(filename);

    return 0;
}
