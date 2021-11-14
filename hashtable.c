#include "hashtable.h"
#include <string.h>
//#include <stdint.h>
#define	HASHTABLE_SIZE		1000003

//  --------------------------------------------------------------------
// Returns unsigned 32 bit integer 
uint32_t hash_string(char *string)
{
    uint32_t hash = 0;
    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

//  ALLOCATE SPACE FOR A NEW HASHTABLE (AND ARRAY OF LISTS)
HASHTABLE *hashtable_new(void)
{
    HASHTABLE   *new = malloc(HASHTABLE_SIZE * sizeof(LIST *));
    CHECK_ALLOC(new);
    return new;
}

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
bool hashtable_find(HASHTABLE *hashtable, char *hash)
{
    uint32_t h	= hash_string(hash) % HASHTABLE_SIZE;     // choose list
    return list_find_string(hashtable[h], hash);
}
// Finding a SHA2 hash in an indicated hashtable
bool hashtable_find_hash(HASHTABLE *hashtable, char *hash)
{
    uint32_t h	= hash_string(hash) % HASHTABLE_SIZE;     // choose list
    return list_find_hash(hashtable[h], hash);
}

// Adding file details to indicated hashtable
void hashtable_add_all(HASHTABLE *hashtable, char *path, int newint, char *hash)
{
    uint32_t h   = hash_string(path) % HASHTABLE_SIZE;    // choose list
    hashtable[h] = list_add_all(hashtable[h], path, newint, hash);
}
// Modified for option l
void hashtable_add_all_l(HASHTABLE *hashtable, char *path, int newint, char *hash)
{
    uint32_t h   = hash_string(path) % HASHTABLE_SIZE;    // choose list
    hashtable[h] = list_add_all_l(hashtable[h], path, newint, hash);
}

//  ADD A NEW STRING TO A GIVEN HASHTABLE
void hashtable_add(HASHTABLE *hashtable, char *path, char *hash, int size)
{
    uint32_t h   = hash_string(hash) % HASHTABLE_SIZE;    // choose list
    hashtable[h] = list_add_all_clean(hashtable[h], path, hash, size);
}

