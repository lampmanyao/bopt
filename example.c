#include <stdio.h>
#include <stdlib.h>

#include "bopt.h"

int main(int argc, char **argv)
{
	int c;
	char *barvalue = NULL;
	char *foovalue = NULL;

	boptctl = 0;  /* Comment this line to see the difference */

	struct boption opts[] = {
		{"bar", brequired_argument, 'b'},
		{"foo", boptional_argument, 'f'},
		{"help", bno_argument, 'h'},
		{NULL, 0, 0}
	};

	while ((c = bgetopt(argc, argv, opts)) > 0) {
		switch (c) {
		case 'b':
			barvalue = boptarg;
			break;
		case 'f':
			foovalue = boptarg;
			break;
		case 'h':
			printf("--help\n");
			break;
		case ':':
			printf("option `%s' missing argument\n", boptarg);
			break;
		default:
			printf("unknown `%s'\n", boptarg);
			break;
		}
	}

	printf("--bar=%s\n", barvalue);
	printf("--foo=%s\n", foovalue);

	return 0;
}

