#include "duplicates.h"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-option] [directory]\n", argv[0]);
    } else if (argc > 0) {
        options(argc, argv);
    } else {
        fprintf(stderr, "ERROR");
        exit(EXIT_FAILURE);
    }
    return 0;
}
