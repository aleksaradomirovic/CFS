#include "cfs/dir.h"

#include <argp.h>
#include <error.h>
#include <stdio.h>

static const struct argp_option args_options[] = {
    { "recursive", 'r', 0, 0, "List subdirectories recursively as well", 0 },
    { 0 }
};

static int recursive = 0;

static int args_parser(int key, char *, struct argp_state *) {
    switch(key) {
        case 'r':
            recursive = 1;
            return 0;
    }

    return ARGP_ERR_UNKNOWN;
}

static const struct argp args_info = {
    .options = args_options,
    .parser = args_parser
};

int main(int argc, char * argv[]) {
    int status;
    int i;
    status = argp_parse(&args_info, argc, argv, 0, &i, NULL);
    if(status != 0) return -1;

    for(; i < argc; i++) {
        struct directory_entry * list;
        if(recursive) status = list_directory_recursive(argv[i], &list);
        else status = list_directory(argv[i], &list);
        if(status != 0) error(status, errno, "%s", argv[i]);

        for(struct directory_entry * ent = list; ent != NULL; ent = ent->next) {
            fprintf(stdout, "%s\n", ent->path);
        }

        free_directory_list(list);
    }
    return 0;
}
