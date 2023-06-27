#include "arg_parser.h"

static void print_help(void);

args *ap_init(int argc, char **argv)
{
	args *arg = (args *)malloc(sizeof(args));
	arg->app_name = NULL;
	arg->flags = 0;
	int opt;

	while((opt = getopt(argc, argv, "a:n:cp")) != -1) {
		switch(opt) {
		case 'a':
			if (argc < 6) print_help();
			arg->flags |= ARG_AREA;
			arg->area.x1 = atoi(argv[2]);
			arg->area.y1 = atoi(argv[3]);
			arg->area.x2 = atoi(argv[4]);
			arg->area.y2 = atoi(argv[5]);
			goto ret;
		case 'n':
			arg->flags |= ARG_APP_NAME;
			arg->app_name = (char *)malloc(sizeof(char) * strlen(optarg) + 1);
			strcpy(arg->app_name, optarg);
			goto ret;
		case 'c':
			arg->flags |= ARG_CURSOR_POS;
			goto ret;
		case 'p':
			arg->flags |= ARG_APP_CURSOR;
			goto ret;
		case 'h':
		default:
			print_help();
		}
	}

ret:
	return arg;
}

void ap_deinit(args *arg)
{
	if (arg == NULL) return;
	if (arg->app_name != NULL) free(arg->app_name);
	free(arg);
}

static void print_help(void)
{
	puts("Bound mouse cursor to specific area of the screen or application:\n\
	-a : Specific area of the screen x1, y1, x2, y2\n\
	-n : Area determend by the app dimentions (selecteion by app name)\n\
	-p : Same as -n but app selected by mouse click\n\
	-c : Show cursor location\n\
	-h : Help");
	exit(EXIT_FAILURE);
}