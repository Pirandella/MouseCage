#include <stdio.h>

#include "arg_parser.h"
#include "x.h"

int main(int argc, char **argv)
{
    args *arg = ap_init(argc, argv);

    x_init();

    if (arg->flags & ARG_AREA) {
        printf("x1: %4d, y1: %4d, x2: %4d, y2: %4d\n", arg->area.x1, arg->area.y1, arg->area.x2, arg->area.y2);
    } else if (arg->flags & ARG_APP_NAME) {
        x_window_name(arg->app_name);
    } else if (arg->flags & ARG_APP_CURSOR) {
        printf("App under the cursor.\n");
    } else {
        printf("Cursor position view.\n");
    }

    ap_deinit(arg);
    return 0;
}

