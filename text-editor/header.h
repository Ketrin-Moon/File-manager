#ifndef _HEADER_H_
#define _HEADER_H

void color_pair(WINDOW *win, int color_bg);
void window();
void init();
void read_file(WINDOW *win, char* filename);
void write_file(WINDOW *win, char* filename);

#endif