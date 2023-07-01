#include <unistd.h>

#include "arg_parser.h"
#include "x.h"

int main(int argc, char **argv)
{
    args *arg = ap_init(argc, argv);

    x_init(arg->flags);

    if (arg->flags & ARG_AREA) {
        printf("x1: %4d, y1: %4d, x2: %4d, y2: %4d\n", arg->area.x1, arg->area.y1, arg->area.x2, arg->area.y2);
    } else if (arg->flags & ARG_APP_NAME) {
        x_select_window_by_name(arg->app_name);
    } else if (arg->flags & ARG_APP_CURSOR) {
        printf("App under the cursor.\n");
        x_select_window_under_cursor();
    } else {
        printf("Cursor position:\n");
        x_live_cursor_position();
    }
    
    ap_deinit(arg);
    return 0;
}

