#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "list.h"

//  PROTOTYPING THE strdup() FUNCTION 
#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

//  ---------------------------------------------------------------------

//  'CREATE' A NEW, EMPTY LIST - JUST A NULL POINTER
LIST *list_new(void)
{
    return NULL;
}

// ------------- SEARCHING --------------
//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
bool list_find_string(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->path, wanted) == 0) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}

bool list_find_hash(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->hash, wanted) == 0) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}

//  DETERMINE IF A REQUIRED ITEM (AN INT) IS STORED IN A GIVEN LIST
bool list_find_int(LIST *list, int wanted)
{
    while(list != NULL) {
	if(list->filesize == wanted) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}

// ------------- ALLOCATING SPACE --------------

//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
LIST *list_new_item_s(char *newstring)
{
    LIST *new       = malloc( sizeof(LIST) );
    CHECK_ALLOC(new);
    new->path     =  strdup(newstring);
    CHECK_ALLOC(new->path);
    new->next       =  NULL;
    return new;
}


LIST *list_new_item_hash(char *newstring, char *hash)
{
    LIST *new       = malloc( sizeof(LIST) );
    CHECK_ALLOC(new);
    new->path     =  strdup(newstring);
    CHECK_ALLOC(new->path);
    new->hash     =   strdup(hash);
    CHECK_ALLOC(new->hash);
    new->next       =  NULL;
    return new;
}
LIST *list_new_item_l(char *newstring)
{
    LIST *new       = malloc( sizeof(LIST) );
    CHECK_ALLOC(new);
    new->hash     =  strdup(newstring);
    CHECK_ALLOC(new->hash);
    new->next       =  NULL;
    return new;
}
// ------------- ADDING --------------

//  ADD A NEW ITEM TO AN EXISTING LIST
LIST *list_add_string(LIST *list, char *newstring)
{
    if(list_find_string(list, newstring)) {                       // only add each item once
        return list;
    }
    else {                                                      // add new item to head of list
        LIST *new   = list_new_item_s(newstring);
        new->next   = list;
        return new;
    }
}
LIST *list_add_string_l(LIST *list, char *path, char *hash)
{                    // add new item to head of list
    LIST *new   = list_new_item_hash(path, hash);
    new->next   = list;
    return new;
}

LIST *list_add_all(LIST *list, char *newstring, int newint, char *hash)
{
    if(list_find_string(list, newstring)) {                     // only add each item once
        return list;
    }
    else {                                                       // add new item to head of list
        LIST *new   = list_new_item_hash(newstring, hash);
        new->filesize = newint;
        new->next   = list;
        return new;
    }
}

LIST *list_add_all_l(LIST *list, char *newstring, int newint, char *hash)
{                                                     // add new item to head of list
    LIST *new   = list_new_item_hash(newstring, hash);
    new->filesize = newint;
    new->next   = list;
    return new;

}
LIST *list_add_all_clean(LIST *list, char *path, char *hash, int size)
{                                                     // add new item to head of list
    LIST *new   = list_new_item_hash(path, hash);
    new->filesize = size;
    new->next   = list;
    return new;
}
LIST *list_add_hash_l(LIST *list, char *hash)
{                                      // add new item to head of list
    LIST *new   = list_new_item_l(hash);
    new->next   = list;
    return new;
}

// ------------- PRINTING --------------

//  PRINT EACH ITEM  IN A GIVEN LIST TO stdout
void list_print(LIST *list)
{
    if(list != NULL) {
        while(list != NULL) {
	    printf("%s", list->path);
	    if(list->next != NULL) {
	        printf(" -> ");
            }
	    list	= list->next;
        }
	printf("\n");
    }
}
void list_print_h(LIST *list)
{
    if(list != NULL) {
        while(list != NULL) {
	    printf("%s", list->hash);
	    if(list->next != NULL) {
	        printf(" -> ");
            }
	    list	= list->next;
        }
	printf("\n");
    }
}

void list_print_i(LIST *list)
{
    if(list != NULL) {
        while(list != NULL) {
	    printf("%i", (list->filesize));
	    if(list->next != NULL) {
	        printf(" -> ");
            }
	    list	= list->next;
        }
	printf("\n");
    }
}

void print_path(LIST *list)
{
    if(list != NULL) {
        while(list != NULL) {
	    printf("%s", list->path);
	    if(list->next != NULL) {
	        printf("\t");
        }
	    list	= list->next;
        }
	printf("\n");
    }
}

void print_path_relative(LIST *list,  int argc, char *argv[]) // fullpath cannot be changed, must be just list 
{
    if(list != NULL) {
        while(list != NULL) {
            char *p = get_relative_path_l(list, argc, argv);
	        printf("%s\t", p);
	        list	= list->next;
        }
	printf("\n");
    }
}
