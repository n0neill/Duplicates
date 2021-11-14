// Header files
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>

// External functions
extern char *strSHA2(char *filename);
extern int options(int argc, char *argv[]);
extern char *get_relative_path(char *fullpath, int argc, char *argv[]);

// Constants
#define OPTLIST "aAfhlq"

// Global variables
extern int g_file_count;
extern int g_total_size;
extern int g_unique_files; 
extern int g_possiblemin;
