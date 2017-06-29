/**
* @author E.Maklashkina
* @version 1.0.0
* @date June,2017
* @warning For the text editor to work, you must specify the environment
* variable "export PATH=$PATH:$PWD"
*/

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

/**
* @function Main -
* @detail the function of the main lines makes a call to the main part of the
* program - initializing and drawing windows, reading directories and files,
* displaying them on the screen, etc.
* @return 0 if the program finishes successfully
*/

int main()
{

    window();

    return 0;

}
