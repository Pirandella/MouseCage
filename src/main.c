#include <signal.h>

#include "arg_parser.h"
#include "x.h"

args *arg = NULL;

void signal_handler(int sig)
{
    ap_deinit(arg);
    x_deinit();

    puts("\nMouse is free");

    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, signal_handler);

    arg = ap_init(argc, argv);

    x_init();

    if (arg->flags & ARG_AREA) {
        x_set_area(arg->area.x1, arg->area.y1, arg->area.x2, arg->area.y2);
    } else if (arg->flags & ARG_APP_NAME) {
        x_select_window_by_name(arg->app_name);
    } else if (arg->flags & ARG_APP_CURSOR) {
        x_select_window_under_cursor();
    } else {
        x_live_cursor_position();
    }
    
    x_track_cursor();

    return 0;
}

