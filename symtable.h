/*--------------------------------------------------------------------*/
/* SymTable.h (Version 6)                                             */
/* Author: Anh Dao                                                    */
/*--------------------------------------------------------------------*/

#ifndef SymTable_INCLUDED
#define SymTable_INCLUDED
#include <stddef.h>

/* A SymTable_T is a last-in-first-out collection of items. */

typedef struct SymTable *SymTable_T;

/* Return a new SymTable_T object, or NULL if insufficient memory is
   available. */

SymTable_T SymTable_new(void);

/*  */

size_t SymTable_getLength(SymTable_T oSymTable);

/* Free oSymTable. */

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Put pvItem onto oSymTable.  Return 1 (TRUE) if successful, or 0
   (FALSE) if insufficient memory is available. */

int SymTable_put(SymTable_T oSymTable,
   const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
   const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each element of oSymTable, passing
   pvExtra as an extra argument.  That is, for each element pvItem
   of oSymTable, call (*pfApply)(pvItem, pvExtra). */

void SymTable_map(SymTable_T oSymTable,
   void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
   const void *pvExtra);

/*--------------------------------------------------------------------*/


#endif



