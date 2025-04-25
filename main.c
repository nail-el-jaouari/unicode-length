#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vec-str.h"
#include "unicode-length-utf8.h"

#define MAX (2 + 1)

void usage (int err);

int main (int argc, char *argv[])
{
    int ret           = EXIT_SUCCESS;
    int hflag         = 0;
    int nflag         = 0;
    int mflag         = 0;
    char *min_arg     = NULL;
    char *max_arg     = NULL;
    size_t min        = 0ul;
    size_t max        = (size_t)-1;
    struct VecStr line;
    char buffer[MAX];

    for (;;)
    {
        int c = getopt (argc, argv, "hn:m:");
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 'h':
                hflag = 1;
                break;
            case 'n':
                nflag = 1;
                min_arg = optarg;
                break;
            case 'm':
                mflag = 1;
                max_arg = optarg;
                break;
            default:
                usage (1);
                return EXIT_FAILURE;
        }
    }

    if (hflag)
    {
        usage (0);
        ret = EXIT_SUCCESS;
        goto exit_program;
    }

    if (nflag)
    {
        char *endptr = NULL;

        min = strtoul(min_arg, &endptr, 10);

        if (endptr != NULL && *endptr != '\0')
        {
            usage (1);
            ret = EXIT_FAILURE;
            goto exit_program;
        }
    }
    if (mflag)
    {
        char *endptr = NULL;

        max = strtoul(max_arg, &endptr, 10);

        if (endptr != NULL && *endptr != '\0')
        {
            usage (1);
            ret = EXIT_FAILURE;
            goto exit_program;
        }
    }

    if (min > max)
    {
        fprintf (stderr, 
                 "%s must be less than or equals %s\n",
                 min_arg, max_arg);
        ret = EXIT_FAILURE;
        goto exit_program;
    }

    vec_str_init (&line, MIN_CAP);

    for (;;)
    {
        char *p = fgets (buffer, MAX, stdin);

        if (p == NULL)
        {
            vec_str_free(&line);
            break;
        }

        size_t len = strlen (buffer);

        char *s = strdup (buffer);
        vec_str_push (&line, s, len);

        if (len < (MAX - 1) ||
            buffer[len - 1] == '\n')
        {
            char *p = vec_str_line (&line);
            vec_str_free (&line);
            size_t ucp_len = unicode_length_u8string (p);

            if ((ucp_len - 1) >= min &&
                (ucp_len - 1) <= max) // - 1 for '\n'
            {
                printf ("%s", p);
            }

            free (p);
            vec_str_init (&line, MIN_CAP);
        }
    }

    vec_str_free (&line);
exit_program:
    return ret;
}

void usage (int err)
{
    FILE *console = (err == 0) ? stdout : stderr;

    fprintf (console,
        "Usage: uc-length [-h] [-n number] [-m number]\n"
         " Display lines in valide utf8 strings from stdin\n"
         " with number of unicode codepoints at lease n and at\n"
         " most m.\n"
         " If n is not set, the default value is 0.\n"
         " If m is not set, the default value is (size_t)-1.\n"
         " The options are:\n"
         "  -h\t\tDispaly this information\n"
         "  -n <number>\t\tPrint any line of at least <number> codepoints.\n"
         "  -m <number>\t\tPrint any line of at most <number> codepoints.\n"
        );
}