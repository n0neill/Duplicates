#include "hashtable.h"
#include "duplicates.h"

#define CYAN "\033[1;36m"
#define PURPLE "\033[1;35m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"

#define RESET "\033[0m"
#define HASHTABLE_SIZE 1000003

    HASHTABLE *hashtable;
    LIST *list = NULL;
    int size = 0;

void someStats(struct dirent *entry) {
    printf("Entry name:    %s%s\n%s", PURPLE, entry->d_name, RESET);
    printf("Entry path:    %s%s\n%s", RED, list->path, RESET);
    printf("Entry size(B): %s%d\n%s", GREEN, size, RESET);
    printf("Entry hash:    %s\n\n", list->hash);        
}

// Fills linked list and gathers some statistics
int fillList(char arg[]) {
    DIR *dir;
    FILE *fp;
    struct stat stats;
    struct dirent *entry;

    if ((dir = opendir(arg)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (arg[strlen(arg)-1] != '/')
                    strcat(arg, "/");
            char fullname[5000];
            sprintf(fullname, "%s%s", arg, entry->d_name);
            if (stat(fullname, &stats) != 0) {
                continue;
            } else if (S_ISREG(stats.st_mode) && (entry->d_name[0] != '.')) {   // Checking if the directory entry is a regular file, and ignoring hidden files
                if (!strcmp (entry->d_name, ".") || !strcmp (entry->d_name, "..")) continue;    // Skipping parent directory

                // Incrementing global variable
                ++g_file_count;
            
                // Gathering entry size in bytes
                fp = fopen(fullname, "r");
                if (fp == NULL) {
                    fprintf(stderr, "Error: failed to open entry.\n");
                    fclose(fp);
                }
                stat(fullname, &stats);
                size = stats.st_size;
                fclose(fp);
                // Incrementing total size of all files
                g_total_size += size;

                list = list_add_all(list, fullname, size, strSHA2(fullname));
                // ------ TESTING STATISTICS
                someStats(entry);
    
            } else if(S_ISDIR(stats.st_mode)) {
                // Clearing arrays
                if (!strcmp (entry->d_name, ".")) continue;
                if (!strcmp (entry->d_name, "..")) continue;
             
                // Recursively searching subdirectory
                fillList(fullname);
            }
        }
        closedir(dir);
    } else {
        fprintf(stderr,"Error opening dir\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

// Considers hidden files
int option_a(char arg[]) {
    DIR *dir;
    FILE *fp;
    struct stat stats;
    struct dirent *entry;

    if ((dir = opendir(arg)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (arg[strlen(arg)-1] != '/')
                    strcat(arg, "/");
            char fullname[5000];
            sprintf(fullname, "%s%s", arg, entry->d_name);
            if (stat(fullname, &stats) != 0) {
                continue;
            } else if (entry->d_type == DT_REG) {
                if (!strcmp (entry->d_name, ".") || !strcmp (entry->d_name, "..")) continue;

                // incrementing g_file_count
                g_file_count++;

                // getting entry size in bytes
                fp = fopen(fullname, "r");
                if (fp == NULL) {
                    fprintf(stderr, "Error: failed to open entry.\n");
                    fclose(fp);
                }
                stat(fullname, &stats);
                size = stats.st_size;
                fclose(fp);
            
                // Incrementing total size of all files
                g_total_size += size;

                // Filling list
                list = list_add_all(list, fullname, size, strSHA2(fullname));
                // ------ TESTING STATISTICS
                someStats(entry);
                
            } else if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, ".") != 0) {
                // Skipping parent directory
                if (!strcmp (entry->d_name, ".") || !strcmp (entry->d_name, "..")) continue;

                // Recursively searching subdirectory
                option_a(fullname);
            }
        }
        closedir(dir);
    } else {
        fprintf(stderr,"Error opening dir\n");
        exit(EXIT_FAILURE);
    }
    return 0;    
}

// Gathers unique file count & minimum possible file size
int morestats(){ 
    hashtable = calloc(HASHTABLE_SIZE, sizeof(LIST *)); 
    // -----------------------------------------
    if (list != NULL) {
        while(list != NULL) {
           hashtable_add_all(hashtable, list->hash, list->filesize, list->path); 
           list=list->next;
        }
    }
    int count = 0;       
    for (int i=0;i<HASHTABLE_SIZE;++i){
        if(hashtable[i] != NULL) {
            while(hashtable[i] != NULL) {
                if(list_find_string(hashtable[i], hashtable[i]->path)){
                    ++count; // number of unique files
                    g_possiblemin += hashtable[i]->filesize; // possible min file size
                    hashtable[i] = hashtable[i]->next;
                } else {
                    hashtable[i] = hashtable[i]->next;
                }
            }
        }
    }
    g_unique_files = count;
    return 0;
}

// Gathers relative filepath from full filepath
char *get_relative_path(char *fullpath, int argc, char *argv[])
{
    char *result = malloc(1000);
    char *source = strdup(argv[argc-1]);
    result = strdup(fullpath) + strlen(source);

    // Adding '/' to start of relative pathname
    if (result[0] != '/') {
        memmove(result+1, result, strlen(result));
        result[0] = '/';
    }

    return result;
}
// Modified for option -l
char *get_relative_path_l(LIST *list, int argc, char *argv[])
{
    char *fullpath = strdup(list->path);
    char *source = strdup(argv[argc-1]);

    char *result =  strdup(fullpath) + strlen(source);
    
    if (result[0] != '/') {
        memmove(result+1, result, strlen(result));
        result[0] = '/';
    }

    return result;
}

// Lists the relative pathname of files whose SHA2 hatch matches the indicated file
int option_f(int argc, char *argv[]) {
    hashtable = calloc(HASHTABLE_SIZE, sizeof(LIST *)); 
    // -----------------------------------------
    if (list != NULL) {
        while(list != NULL) {
           hashtable_add(hashtable, list->path, list->hash, list->filesize); 
           list=list->next;
        }
    }
    // -----------------------------------------
    bool b = false;

    char *filepath = strdup(argv[argc-2]);
    if (filepath[strlen(filepath)-1] == '/')  // If filepath ends in '/', remove
        filepath[strlen(filepath)-1] = '\0';

    char *tmp = strSHA2(filepath);
    int hindex = hash_string(tmp) % HASHTABLE_SIZE; // index of duplicates

    while(hashtable[hindex] != NULL) {
        if(strcmp(tmp, hashtable[hindex]->hash) == 0 && strcmp(filepath, hashtable[hindex]->path) != 0){
            char *p = get_relative_path(hashtable[hindex]->path, argc, argv);
            printf("%s\n", p);
           
            b = true;
        }
        hashtable[hindex] = hashtable[hindex]->next;
    }

    if (b) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

// Lists the relative pathname of all files with the inputted SHA2 hash
int option_h(int argc, char *argv[]) {
    hashtable = calloc(HASHTABLE_SIZE, sizeof(LIST *)); 

    // -----------------------------------------
    if (list != NULL) {
        while(list != NULL) {
           hashtable_add(hashtable, list->path, list->hash, list->filesize); 
           list=list->next;
        }
    }
    // -----------------------------------------

    bool b = false;
    char *hash = argv[argc-2];
    int hindex = hash_string(hash) % HASHTABLE_SIZE; // index of duplicates
    // REMEMBER TO REMOVE ----

    while(hashtable[hindex] != NULL) {
        if(strcmp(hash, hashtable[hindex]->hash) == 0){
            char *ptr = get_relative_path(hashtable[hindex]->path, argc, argv);
            printf("%s\n", ptr);
            b = true;
        }
        hashtable[hindex] = hashtable[hindex]->next;
    }
    if (b) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
    return 0;
}

// Lists the relative pathnames of all duplicate files
int option_l(int argc, char *argv[]) {
     hashtable = calloc(HASHTABLE_SIZE, sizeof(LIST *)); 
    // -----------------------------------------
    if (list != NULL) {
        while(list != NULL) { 
           hashtable_add(hashtable, list->path, list->hash, list->filesize); 
           list=list->next;
        }
    }
    // ----------------------------------------- 
    LIST *tmp = NULL;
    for (int i=0; i< HASHTABLE_SIZE;++i){
        if(hashtable[i] != NULL) { 
            LIST *tmp2 = NULL; // stores duplicate hashs -- > ie stores all duplicates in a particular hashtable index. Empties after each duplicate is found.
            while(hashtable[i] != NULL) {
                tmp = hashtable[i];
                int c = 0;
                while(tmp != NULL) {
                    if(strcmp(hashtable[i]->hash, tmp->hash) == 0){
                        ++c;
                    }
                    tmp = tmp->next;
                }
                if (c > 1 && tmp2 == NULL) {
                    print_path_relative(hashtable[i], argc, argv);
                    tmp2 =list_add_hash_l(tmp2, hashtable[i]->hash);
                }

                hashtable[i] = hashtable[i]->next;
            }
        }
    }
    return 0;
}

// Test to see if the named directory contains any duplicate files
void option_q() {
    if (g_unique_files == g_file_count) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

void printstats() {
    printf("Total file count:    \t%s%i%s\n", CYAN, g_file_count, RESET);
    printf("Total size(B): \t\t%s%i%s\n", CYAN, g_total_size, RESET);
    printf("Unique files:  \t\t%s%i%s\n", CYAN, g_unique_files, RESET);
    printf("Min total size(B): \t%s%i%s\n", CYAN, g_possiblemin, RESET);
}   

int options(int argc, char *argv[]) {
    
    int opt = 0;
    if (argc > 2) {
    while ((opt = getopt(argc, argv, OPTLIST)) != -1) {
        switch(opt) {
            case 'a':{
                option_a(argv[argc-1]);
                morestats();
                printstats();
                break;
            }
            case 'A':  {
                exit(EXIT_FAILURE);
                break;
            }
            case 'f': {
                fillList(argv[argc-1]);
                option_f(argc, argv);
                break;
            }
            case 'h': {
                fillList(argv[argc-1]);
                option_h(argc, argv);
                break;
            }
            case 'l': {
                fillList(argv[argc-1]);
                option_l(argc, argv); 
                break;
            }
            case 'q': {
                fillList(argv[argc-1]);
                morestats();
                option_q();
                break;
            }
            default: {
                fprintf(stderr, "INCORRECT OPTION\nUsage: %s [-a] [-A] [-f filename] [-h hash] [-l] [-q] [directory]\n", argv[0]);
                break;
            }
        }
    }
    } else {
        fillList(argv[argc-1]);
        morestats();
        printstats();
    }
    return 0;
}
