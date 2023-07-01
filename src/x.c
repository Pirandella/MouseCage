#include "x.h"

static Display *display;
static Window root_window;
static Window selected_window;
static uint32_t x_flags = 0;

#define X_AREA          (1U << 0U)
#define X_BY_NAME       (1U << 1U)
#define X_UNDER_CURSOR  (1U << 2U)

static int wx1, wy1, ww, wh, wx2, wy2;

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

static void get_selected_window_size()
{
    Window child;
    XWindowAttributes xwa;
    XTranslateCoordinates(display, selected_window, root_window, 0, 0, &wx1, &wy1, &child);
    XGetWindowAttributes(display, selected_window, &xwa);
    ww = xwa.width;
    wh = xwa.height;

    // For some reason window title bar height is not acounted for windows that was retrived by name.
    if (x_flags & X_BY_NAME) {
        int top = 0;
        Atom prop = XInternAtom(display, "_NET_FRAME_EXTENTS", False);
        Atom type;
        int format;
        unsigned long nitems, bytes_after;
        unsigned char *property;
        if (XGetWindowProperty(display, selected_window, prop, 0, 16, 0, XA_CARDINAL,
                            &type, &format, &nitems, &bytes_after, &property) == Success) {
            if (type != None && nitems == 4) {
                top = (int)((long *)property)[2];
            }
            XFree(property);
        }
        wy1 -= top;
        wh += top;
    }
}

static void print_window_info()
{
    get_selected_window_size();
    printf("X: %4d\tY: %4d\nW: %4d\tH: %4d\n", wx1, wy1, ww, wh);
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

static void list_window_properties()
{
    int prop_num = 0;
    Atom *list = XListProperties(display, selected_window, &prop_num);
    if (prop_num == 0) {
        printf("No properties found for this window 0x%08lX\n", selected_window);
        return;
    }

    puts("Available properties:");
    for (int i = 0; i < prop_num; i++) {
        printf("\t%s\n", XGetAtomName(display, list[i]));
    }
}

/* --------------------------------------------------------------------------------------- */

void x_set_area(int x1, int y1, int x2, int y2)
{
    if (x1 > x2 || y1 > y2) {
        printf("Error: x1 and y1 should be less than x2 and y2!\n");
        exit(-1);
    }

    x_flags = X_AREA;

    wx1 = x1;
    wy1 = y1;
    wx2 = x2;
    wy2 = y2;

    printf("Cage area:\n\tX1: %4d\tY1: %4d\n\tX2: %4d\tY2: %4d\n", wx1, wy1, wx2, wy2);
}

void x_select_window_by_name(char *name)
{
    x_flags = X_BY_NAME;
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
    x_flags = X_UNDER_CURSOR;
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

void x_live_cursor_position()
{
    int win_x, win_y;
    int x, y;
    unsigned int mask;
    Window child;
    while (1) {
        XQueryPointer(display, root_window, &root_window, &child, &x, &y, &win_x, &win_y, &mask);
        printf("\tX: %4d\tY: %4d\r", x, y);
        usleep(1000);
    }
}
