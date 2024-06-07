#include "args.h"
#include "maze.h"

#include <stdio.h>
#include <string.h>
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
            "   -S, --solve                   solve the maze.\n"
            "   --strat                       specify the maze solving strategy.\n"
            "                                 possible values:\n"
            "                                     - best (greedy best first search) \n"
            "                                     - breadth (breadth first search) \n"
            "                                     - depth (depth first search) \n"
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
    args->solve = false;
    args->solving_strat = MAZE_SOLVING_STRATEGY_BEST_FIRST_SEARCH;
    args->maze_width = 8;
    args->maze_height = 5;
    args->starting_point = (coordinate_t){ 0, 0 };
    args->ending_point = (coordinate_t){ 0, 0 };
}

maze_solving_strategy_t __parse_maze_strategy(char *strat)
{
    if (strcmp(strat, "best") == 0) {
        return MAZE_SOLVING_STRATEGY_BEST_FIRST_SEARCH;
    }

    if (strcmp(strat, "breadth") == 0) {
        return MAZE_SOLVING_STRATEGY_BREADTH_FIRST_SEARCH;
    }

    if (strcmp(strat, "depth") == 0) {
        return MAZE_SOLVING_STRATEGY_DEPTH_FIRST_SEARCH;
    }

    return -1;
}

#define ARG_WIDTH_ID 69
#define ARG_HEIGHT_ID 420
#define ARG_STRAT_ID 666

void args_parse(args_t *args, int argc, char **argv)
{
    args_set_default_values(args);

    char *short_opts = ":s:e:Sgh";
    struct option long_opts[] = {
        { "starting", required_argument, NULL, 's' },
        { "ending", required_argument, NULL, 'e' },
        { "solve", no_argument, NULL, 'S' },
        { "strat", required_argument, NULL, ARG_STRAT_ID },
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
        case 'S':
            args->solve = true;
            break;
        case ARG_STRAT_ID:
            args->solving_strat = __parse_maze_strategy(optarg);

            if ((int)args->solving_strat == -1) {
                fprintf(stderr, "%s: Unknown solving strategy `%s`\n", argv[0], optarg);
                exit(EXIT_FAILURE);
            }

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
                fprintf(stderr, "%s: '--width' value cannot be negative\n", argv[0]);
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
