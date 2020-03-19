WHAT
====
bgetopt -- a better way to get long options from command line argument list

WHY
===
The getopt_long(3) is hard to use:
  a) The getopt_long(3)'s argument may be specified as a single argument with
     an equal sign, e.g.,

     myprogram --myoption=somevalue

  b) You have to write more codes to detect the non-option arugments and
     unrecognized arguments.

HOW TO USE
==========
The bgetopt() call requires a structure to be initialized describing the long
options. The structure is:

    struct boption {
        char *name;
        int has_arg;
        int val;
    };

    The name field should contain the option name without the leading double dash.

    The has_arg field should be one of:
        bno_argument        no argument to the option is expected
        brequired_argument  an argument to the option is required
        boptional_argument  an argument to the option may be presented

The equal sign is no need any more, because white space is enter easier. e.g.,

    myprogram --myoption somevalue

Return value
The `val' field will be returned if no error occurs. The function returns `:' if
there was a missing option argument, `?' if the user specified an unknown or 
ambiguous option and -1 when the argument list has been exhausted.
