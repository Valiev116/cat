#define _GNU_SOURCE

#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <errno.h>
#include <regex.h>
#include <stdlib.h>

typedef struct
{
    int regex_flag;
    bool invert;
    bool countMatchs;
    bool filesMatchs;
    bool numberLine;
} Flags;

Flags GrepReadFlags(int argc, char *argv[])
{
    Flags flags = {0, false, false, false, false};
    int currentFlag = getopt(argc, argv, "eivcln");
    for (; currentFlag != -1; currentFlag = getopt(argc, argv, "eivcln"))
    {
        switch (currentFlag)
        {
        case 'e':
            flags.regex_flag |= REG_EXTENDED;
            break;
        case 'i':
            flags.regex_flag |= REG_ICASE;
            break;
        case 'v':
            flags.invert = true;
            break;
        case 'c':
            flags.countMatchs = true;
            break;
        case 'l':
            flags.filesMatchs = true;
            break;
        case 'n':
            flags.numberLine = true;
            break;
        }
    }
    return flags;
}

void GrepMatchs(FILE *file, char const *filename, Flags flags, regex_t *preg)
{
    (void)flags;
    char *line = 0;
    size_t length = 0;
    regmatch_t match;
    int count = 0;
    while (getline(&line, &length, file) > 0)
    {
        if (!(regexec(preg, line, 1, &match, 0)))
            ++count;
    }
    if (flags.countMatchs)
        printf("%i\n", count);
    else if (flags.filesMatchs && count > 0)
        printf("%s\n", filename);
    free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg)
{
    char *line = 0;
    size_t length = 0;
    regmatch_t match;
    int numLine = 0;
    while (getline(&line, &length, file) > 0)
    {
        ++numLine;
        if (flags.invert)
        {
            if (regexec(preg, line, 1, &match, 0))
            {
                if (flags.numberLine)
                    printf("%i:", numLine);
                printf("%s", line);
            }
        }
        else
        {
            if (!(regexec(preg, line, 1, &match, 0)))
            {
                if (flags.numberLine)
                    printf("%i:", numLine);
                printf("%s", line);
            }
        }
    }
    free(line);
}

void Grep(int argc, char *argv[], Flags flags)
{
    char **pattern = &argv[1];
    char **end = &argv[argc];
    regex_t preg_storage;
    regex_t *preg = &preg_storage;
    for (; pattern != end && pattern[0][0] == '-'; ++pattern)
        ;
    if (pattern == end)
    {
        fprintf(stderr, "no pattern\n");
        exit(1);
    }
    if (regcomp(preg, *pattern, flags.regex_flag))
    {
        fprintf(stderr, "failed to cimpile regex\n");
        exit(1);
    }
    for (char **filename = pattern + 1; filename != end; ++filename)
    {
        if (**filename == '-')
            continue;
        errno = 0;
        FILE *file = fopen(*filename, "rb");
        if (errno)
        {
            fprintf(stderr, "%s", argv[0]);
            perror(*filename);
            continue;
        }
        if (flags.countMatchs || flags.filesMatchs)
            GrepMatchs(file, *filename, flags, preg);
        GrepFile(file, flags, preg);
        fclose(file);
    }
}

int main(int argc, char *argv[])
{
    Flags flags = GrepReadFlags(argc, argv);
    Grep(argc, argv, flags);
}
