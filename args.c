#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>

void args_print_usage_and_exit(char *progname, int exit_status)
{
    fprintf(stdout,
            "Usage: %s [options ...] \n"
            "Maze generator.\n"
            "\n"
            "Options:\n"
            "   -s point, --starting point    set the starting point, in comma seperated.\n"
            "   -e point, --ending point      set the ending point, in comma seperated.\n"
            "   --width n                     specify the maze width.\n"
            "   --height n                    specify the maze height.\n"
            "   -g, --gui                     enables grapichal user interface.\n"
            "   -h, --help                    prints this message and exit.\n",
            progname);
    exit(exit_status);
}

void args_set_default_values(args_t *args)
{
    args->use_gui = false;
    args->maze_width = 8;
    args->maze_height = 5;
    args->starting_point = (coordinate_t){ 0, 0 };
    args->ending_point = (coordinate_t){ 0, 0 };
}

#define ARG_WIDTH_ID 69
#define ARG_HEIGHT_ID 420

void args_parse(args_t *args, int argc, char **argv)
{
    args_set_default_values(args);

    char *short_opts = ":s:e:gh";
    struct option long_opts[] = {
        { "starting", required_argument, NULL, 's' },
        { "ending", required_argument, NULL, 'e' },
        { "width", required_argument, NULL, ARG_WIDTH_ID },
        { "height", required_argument, NULL, ARG_HEIGHT_ID },
        { "gui", no_argument, NULL, 'g' },
        { "help", no_argument, NULL, 'h' },
        { 0 }
    };

    for (int opt = 0, long_opts_idx = 0; opt != -1; opt = getopt_long(argc, argv, short_opts, long_opts, &long_opts_idx)) {
        switch (opt) {
        /* fallthrough for the first iteration
         */
        case 0:
            break;
        case 's':
            args->starting_point = coordinate_parse_from_string(optarg);
            break;
        case 'e':
            args->ending_point = coordinate_parse_from_string(optarg);
            break;
        case 'g':
            args->use_gui = true;
            break;
        case ARG_WIDTH_ID: {
            char *end;
            errno = 0;

            args->maze_width = strtol(optarg, &end, 10);

            if (errno || end == optarg) {
                fprintf(stderr, "%s: Could not parse number passed by '--width'\n", argv[0]);
                exit(EXIT_FAILURE);
            }

            if (args->maze_width < 0) {
                fprintf(stderr, "%s: '--width' value cannot be negative \n", argv[0]);
                exit(EXIT_FAILURE);
            }

            break;
        }
        case ARG_HEIGHT_ID: {
            char *end;
            errno = 0;

            args->maze_height = strtol(optarg, &end, 10);

            if (errno || end == optarg) {
                fprintf(stderr, "%s: Could not parse number passed by '--height'\n", argv[0]);
                exit(EXIT_FAILURE);
            }

            if (args->maze_height < 0) {
                fprintf(stderr, "%s: '--height' value cannot be negative \n", argv[0]);
                exit(EXIT_FAILURE);
            }

            break;
        }
        /* When the arg is valid but missing something
         */
        case ':':
            fprintf(stderr, 
                    "%s: Insufficient argument '%c'\n"
                    "Try `%s --help` for more information.\n",
                    argv[0], optopt, argv[0]);
            exit(EXIT_FAILURE);
            break;
        /* Unknown flag
         */
        case '?':
            /* optopt:
             * short opts char if error
             *
             * this is the double dash version
             * the other is single dash error
             */
            if (optopt == 0) {
                fprintf(stderr, 
                        "%s: Invalid option '%s'\n"
                        "Try `%s --help` for more information.\n",
                        argv[0], argv[optind - 1], argv[0]);
            } else {
                fprintf(stderr, 
                        "%s: Invalid option '%c'\n"
                        "Try `%s --help` for more information.\n",
                        argv[0], optopt, argv[0]);
            }
            exit(EXIT_FAILURE);
        case 'h':
        default:
            args_print_usage_and_exit(argv[0], EXIT_SUCCESS);
            break;
        }
    }

    if (args->starting_point.x < 0 || args->starting_point.y < 0) {
        fprintf(stderr, "%s: starting point cannot be negative\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (args->ending_point.x < 0 || args->ending_point.y < 0) {
        fprintf(stderr, "%s: ending point cannot be negative\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
