#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

/* 
	-a : Specific area of the screen x1, y1, x2, y2
	-n : Area determend by the app dimentions (selecteion by app name)
	-p : Same as -n but app selected by mouse click
	-c : Show cursor location
	-h : Help
 */

#define ARG_AREA			(1U << 0U)
#define ARG_APP_NAME		(1U << 1U)
#define ARG_APP_CURSOR		(1U << 2U)
#define ARG_CURSOR_POS		(1U << 3U)

typedef struct {
	uint32_t x1;
	uint32_t y1;
	uint32_t x2;
	uint32_t y2;
} screen_area;

typedef struct _args {
	char *app_name;
	uint32_t flags;
	screen_area area;
} args;

args *ap_init(int argc, char **argv);

void ap_deinit(args *arg);

#endif /* _ARG_PARSER_H_ */