#include <stdbool.h>
#include "duplicates.h"


// CHECK IF ALLOCATIONS WERE SUCCESSFUL
#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }


//   LIST DATATYPE
typedef struct _list {
    char           *path;
    char           *hash;
    int             filesize;
    struct _list   *next;
} LIST;

//  THESE FUNCTIONS ARE DECLARED HERE, AND DEFINED IN list.c :

//  CREATING AN EMPTY LIST
extern	LIST	*list_new(void);

// ------------- SEARCHING --------------
//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
extern	bool	 list_find_string(LIST *list, char *wanted);
extern	bool	 list_find_hash(LIST *list, char *wanted);
extern	bool	 list_find_int(LIST *list, int wanted);

// ------------- PRINTING --------------
//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
extern	void	 list_print(LIST *list);
extern	void	 list_print_i(LIST *list);
extern  void     list_print_h(LIST *list);

extern void print_path_relative(LIST *list, int argc, char *argv[]);

// ------------- ADDING --------------
//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
extern	LIST	*list_add_string(  LIST *list, char *newstring);

extern LIST *list_add_both(LIST *list, char *newstring, int newint);
extern LIST *list_add_all(LIST *list, char *newstring, int newint, char *hash);
extern LIST *list_add_string_l(LIST *list, char *path, char *hash);
extern LIST *list_new_item_l(char *newstring);
extern LIST *list_add_hash_l(LIST *list, char *hash);
extern char *get_relative_path_l(LIST *list, int argc, char *argv[]);



// ------------- ALLOCATING --------------

extern LIST *list_new_item_s(char *newstring);
extern LIST *list_new_item_i(int newint);
extern LIST *list_new_item_both(char *newstring, int newint); // string & int
extern LIST *list_new_item_hash(char *newstring, char *hash);

void list_fs(LIST *list, char *wanted);
LIST *list_add_all_l(LIST *list, char *newstring, int newint, char *hash);
extern void print_path(LIST *list);

extern LIST *list_add_all_clean(LIST *list, char *path, char *hash, int size);

