#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

int main(int argc, char *argv[]) {
    if (!strcmp(argv[0], "lazybox")) return 1;
    char *file = basename(argv[0]);
    char arg_str[8192];
    int n_chr = 8000;
    argv[0] = file;
    for (int i = 0; i < argc; i++) {
        strncat(arg_str, argv[i], n_chr);
        n_chr -= strlen(argv[i]) + 3;
        if (n_chr < 0) {
            fprintf(stderr, "args too long.");
            return 1;    
        }
        strncat(arg_str, " ", n_chr);
    }
    system(arg_str);
    
    return 0;
}
