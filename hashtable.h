#include "list.h"
#include <stdint.h>

//  DEFINING A HASHTABLE
typedef	LIST * HASHTABLE;

//  Functions are declared here, and defined in hashtable.c

//  ALLOCATE SPACE FOR A NEW HASHTABLE (AND ARRAY OF LISTS)
extern	HASHTABLE	*hashtable_new(void);

//  ADD A NEW STRING TO A GIVEN HASHTABLE
extern	void		 hashtable_add( HASHTABLE *, char *path, char *hash, int size);

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
extern	bool		 hashtable_find(HASHTABLE *, char *string);

// Getting hash of string
extern uint32_t hash_string(char *string);

// Adding list do table
extern void hashtable_add_all(HASHTABLE *hashtable, char *string, int newint, char *hash);
extern void hashtable_add_all_l(HASHTABLE *hashtable, char *string, int newint, char *hash);


