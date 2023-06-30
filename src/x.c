#include "x.h"

static Display *display;
static Window root_window;
static Window selected_window;

void x_init()
{
    setlocale(LC_ALL, "");

    printf("Conncting to X server... ");
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        puts("Fail [Cannot connect to X server]");
        exit (-1);
    }
    root_window = DefaultRootWindow(display);
    puts("Succsess!");
}

static void print_window_info()
{
    int x, y;
    Window child;
    XWindowAttributes xwa;
    XTranslateCoordinates(display, selected_window, root_window, 0, 0, &x, &y, &child);
    XGetWindowAttributes(display, selected_window, &xwa);
    printf("X: %4d\tY: %4d\nW: %4d\tH: %4d\n", x, y, xwa.width, xwa.height);
}

static bool match_window_name(Display *_display, Window _window, char *app_name)
{
    XTextProperty text_data;
    Atom atom = XInternAtom(_display, "WM_NAME", True);
    XGetTextProperty(_display, _window, &text_data, atom);
    char *window_name = (char *)text_data.value;
    if (text_data.nitems != 0)
        if (!strcmp(app_name, window_name)) return true;

    return false;
}

static Window get_window_id_by_name(Window root, char *app_name, uint32_t depth)
{
    Window parent;
    Window *children;
    unsigned int count;
    static Window ret = 0;

    Status status = XQueryTree(display, root, &root, &parent, &children, &count);
    if (!status) {
        puts("Failed to retrive windows tree\n");
        return ret;
    }

    for (size_t i = 0; i < count; i++) {
        if (match_window_name(display, children[i], app_name)) ret = children[i];
        get_window_id_by_name(children[i], app_name, depth + 1);
        if (ret != 0) break;
    }

    return ret;   
}

void x_select_window_by_name(char *name)
{
    selected_window = get_window_id_by_name(root_window, name, 0);
    if (selected_window == 0) {
        printf("No window with this name \"%s\" exists!\n", name);
        exit(-1);
    }
    printf("Window:\n\tID:   0x%08lX\n\tName: %s\n", selected_window, name);

    print_window_info();
}

void x_select_window_under_cursor()
{
    Cursor cursor = XCreateFontCursor(display, XC_crosshair);

    XGrabPointer(display, root_window, 0, (ButtonMotionMask | ButtonPressMask | ButtonReleaseMask),
                 GrabModeAsync, GrabModeAsync, root_window, cursor, CurrentTime);
    XEvent e;
    while (1) {
        XNextEvent(display, &e);
        if (e.type == ButtonRelease) {
            selected_window = e.xbutton.subwindow;
            break;
        }
    }

    XUngrabPointer(display, CurrentTime);
    XFreeCursor(display, cursor);

    puts("Window selected!");
    print_window_info();
}
