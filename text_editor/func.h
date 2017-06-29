#ifndef _FUNC_H
#define _FUNC_H

/**
* @function Cycle -
* @detail performs processing of keystrokes.
* @param my_win - The parameter that passes the pointer to the window. Needed
* in order to use the move cursor command
* @param user_pos_X - the parameter responsible for the position of the user in the rows
* @param user_pos_Y - the parameter responsible for the position of the user in the column
* @param row - parameter, which stores the number of rows
* @param col - parameter, which stores the number of columns
* @param filename - the parameter contains the name of the file that you want
* to open
* @param count - the parameter that counts the number of lines in a file
*/

void cycle(WINDOW *my_win, int user_pos_X, int user_pos_Y, int row, int col, char* filename, int count);

/**
* @function Write_file -
* @detail function allows you to enter words from the keyboard that will be
* written to a file
* @param filename - the parameter contains the name of the file that you want
* to open
* @param string - array of new lines to be written to the file
* @param count - the parameter that counts the number of lines in a file
* @param my_win - The parameter that passes the pointer to the window. Needed
* in order to use the move cursor command
*/

void write_file(char* filename, char** string, int count, WINDOW* my_win);

/**
* @function Read_file -
* @detail the function reads each line of the file and writes it to an array
* of strings.
* @param filename - the parameter contains the name of the file that you want
* to open
* @param count - the parameter that counts the number of lines in a file
* @return an array of strings
*/

char** read_file(char* filename, int* count);

/**
* @function Color_pair -
* @detail function that initializes the color palette. Specifies the color
* template used to decorate the windows.
* @param win - the parameter that passes a pointer to the window into which
* it is necessary list files
* @param color_bg - the parameter responsible for the highlight color of
* the position in which the user is located
*/

void color_pair(WINDOW *win, int color_bg);

/**
* @function Window -
* @detail creates a window in which the contents of the file are output
* @param filename - the parameter contains the name of the file that you want
* to open
*/

void window(char* filename);

#endif
