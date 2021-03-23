/* bopt.h
 * - public domain getopt library
 * - no warranty implied; use at your own risk
 *
 * LICENSE
 *
 * This software is in the public domain. Where that dedication is not
 * recognized, you are granted a perpetual, irrevocable license to copy,
 * distribute, and modify this file as you see fit.
 */

#ifndef bopt_h
#define bopt_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IS_OPTION_ARG(arg) (arg[0] == '-' && arg[1] == '-')

#define err_nonoption_argument(s)  \
	fprintf(stderr, "ERROR: non-option argument `%s'\n", s)

#define err_unknown_option(s)      \
	fprintf(stderr, "ERROR: unknown option `%s'\n", s)

#define err_missing_argument(s)    \
	fprintf(stderr, "ERROR: option `--%s' is missing argument\n", s)

#define err_no_argument(s)         \
	fprintf(stderr, "ERROR: option `--%s' is no argument\n", s)

#define err_incorrect_has_arg(s) \
	fprintf(stderr, "ERROR: the `has_arg' field of `%s' should be one of:\n" \
		"    brequired_argument  an argument to the option is required\n" \
		"    boptional_argument  an argument to the option may be presented\n" \
		"    bno_argument        no argument to the option is expected\n", s)

#define brequired_argument 0  /* an argument to the option is required */
#define boptional_argument 1  /* an argument to the option may be presented */
#define bno_argument       2  /* no argument to the option is expected */

#define boptctl_exit       1

static const int boptopt = '?';

static int boptind = 1;

/* Control the behavior of bgetopt().
 * The default behavior is print the error message to the STDERR and call
 * exit() if bgetopt() meet an error. If the caller change it to the other
 * else value, bgetopt() will parse all the elements of the argument list
 * and print the error message to the STDERR. */
static int boptctl = boptctl_exit;

/* The caller can use this pointer to get the required argument or
 * optional argument. If bgetopt() meet an error, the caller also can use
 * this pointer to get the non-option argument or unrecognized option.
 */
static char *boptarg = NULL;

struct boption {
	char *name;
	int has_arg;
	int val;
	char **value;
};

static int bgetopt(int argc, char * const *argv, const struct boption *opts)
{
	char *arg;
	char *next_arg;

	if (boptind >= argc) {
		struct boption *p = (struct boption *)opts;
		while (p->name) {
			if (boptctl == boptctl_exit) {
				if (p->has_arg == brequired_argument) {
					err_missing_argument(p->name);
					exit(-1);
				} else if (p->has_arg == bno_argument) {
					err_no_argument(p->name);
					exit(-1);
				} else {
				}
			}
			p++;
		}
		return -1;  /* Reach the end of ARGV */
	}

 	arg = argv[boptind];
	next_arg = argv[boptind + 1];

	if (!(IS_OPTION_ARG(arg))) {
		if (boptctl == boptctl_exit) {
			err_nonoption_argument(arg);
			exit(-1);
		} else {
			boptarg = argv[boptind];
			boptind++;
			return boptopt;
		}
	}

	arg += 2; /* Skip the `--' */

	while (opts->name) {
		if (strlen(opts->name) == strlen(arg) &&
		    strcmp(opts->name, arg) == 0)
			break;
		opts++;
	}

	if (!opts->name) {
		if (boptctl == boptctl_exit) {
			err_unknown_option(argv[boptind]);
			exit(-1);
		} else {
			boptarg = argv[boptind];
			boptind++;
			return boptopt; 
		}
	}

	switch (opts->has_arg) {
	case brequired_argument:
		if (!next_arg || IS_OPTION_ARG(next_arg)) {
			if (boptctl == boptctl_exit) {
				err_missing_argument(opts->name);
				exit(-1);
			} else {
				boptarg = argv[boptind];
				boptind++;
				return ':';
			}
		}

		boptarg = next_arg;
		boptind += 2;
		return opts->val;

	case boptional_argument:
		if (!next_arg || IS_OPTION_ARG(next_arg)) {
			boptarg = NULL;
			boptind++;
			return opts->val;
		}

		boptarg = next_arg;
		boptind += 2;
		return opts->val;

	case bno_argument:
		if (!next_arg || IS_OPTION_ARG(next_arg)) {
			boptarg = NULL;
			boptind++;
			return opts->val;
		}

		if (boptctl == boptctl_exit) {
			err_no_argument(opts->name);
			exit(-1);
		} else {
			boptarg = argv[boptind];
			boptind++;
			return opts->val;
		}

	default:
		if (boptctl == boptctl_exit) {
			err_incorrect_has_arg(opts->name);
			exit(-1);
		} else {
			boptarg = argv[boptind];
			boptind++;
			return boptopt;
		}
	}
}

#ifdef __cplusplus
}
#endif

#endif  /* bopt_h */

