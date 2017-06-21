#ifndef _FUNC_H
#define _FUNC_H

void **draw_menu(int start_col);
void print(WINDOW *win, char **filenames, int count, int user_pos, int color_bg);
void no_print_line(WINDOW *win, char **filenames, int count);
char** read_file(char* filename, int* count);
void color_pair(WINDOW *win, int color_bg);
void window();
void write_file(char* filename, char** string, int count, WINDOW* my_win);

#endif
