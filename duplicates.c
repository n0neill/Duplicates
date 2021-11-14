//  CITS2002 Project 2 2021
//  Name(s):             Nathan O'Neill
//  Student number(s):   23206294
#include "duplicates.h"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-option] [directory]\n", argv[0]);
    } else if (argc > 0) {
        options(argc, argv);
    } else {
        fprintf(stderr, "ERROR");
    }
    return 0;
}
