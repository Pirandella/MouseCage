#ifndef _X_H_
#define _X_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XTest.h>

void x_init();

void x_set_area(int x1, int y1, int x2, int y2);
void x_select_window_by_name(char *name);
void x_select_window_under_cursor();
void x_live_cursor_position();
void x_track_cursor();

void x_deinit();

#endif  /* _X_H_ */