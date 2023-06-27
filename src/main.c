#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>

#include "arg_parser.h"

int main(int argc, char **argv)
{
    args *arg = ap_init(argc, argv);

    if (arg->flags & ARG_AREA) {
        printf("x1: %4d, y1: %4d, x2: %4d, y2: %4d\n", arg->area.x1, arg->area.y1, arg->area.x2, arg->area.y2);
    } else if (arg->flags & ARG_APP_NAME) {
        printf("App name: %s\n", arg->app_name);
    } else if (arg->flags & ARG_APP_CURSOR) {
        printf("App under the cursor.\n");
    } else {
        printf("Cursor position view.\n");
    }

    ap_deinit(arg);
    return 0;
}

