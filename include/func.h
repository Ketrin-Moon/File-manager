/**
* @function Draw_panel -
* @detail creation of upper and lower panels
* @param start_col - color panel parameter
* @return EXIT_SUCCESS in case of success
*/
void **draw_menu(int start_col);

/**
* @function Print -
* @detail output files to the screen with the highlight of the position
* on which the user is located. Used for the active window.
* @param win - the parameter that passes a pointer to the window into which
* it is necessary list files
* @param filenames - tarameter that passes the elements of the array that
* are needed display
* @param count - number of array elements
* @param user_pos - the parameter responsible for the position in which
* the user is located
* @param color_bg - the parameter responsible for the highlight color of
* the position in which the user is located
*/

void print(WINDOW *win, char **filenames, int count, int user_pos, int color_bg);

/**
* @function No_print_line -
* @detail the function displays a list of files without highlighting the
* user's position. Used for output in a passive window.
* @param win - the parameter that passes a pointer to the window into which
* it is necessary list files
* @param filenames - parameter that passes the elements of the array that
* are needed display
* @param count - number of array elements
*/

void no_print_line(WINDOW *win, char **filenames, int count);

/**
* @function Direct -
* @detail the function reads the contents of the directory and stores it
* in an array (in lexicographical order).
* @param dp - the parameter that stores the path of a directory whose
* contents need to be read
* @param count - parameter that is responsible for the number of files
* in the directory
* @return an array of file names(filenames)
*/

char** direct(char *dp, int *count);

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
* @function Init_win -
* @detail initializing library ncurses
*/

void init_win();

/**
* @function Window -
* @detail the function is responsible for creating and drawing windows,
* and also calls the function to read directories, which displays all
* the files in it in the manager windows. Here you can also navigate the
* file manager (KEY_UP, KEY_DOWN, TAB, ENTER).
*/

void window();

