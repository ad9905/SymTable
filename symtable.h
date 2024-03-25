/*--------------------------------------------------------------------*/
/* SymTable.h (Version 6)                                             */
/* Author: Anh Dao                                                    */
/*--------------------------------------------------------------------*/

#ifndef SymTable_INCLUDED
#define SymTable_INCLUDED

/* A SymTable_T is a last-in-first-out collection of items. */

typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* Return a new SymTable_T object, or NULL if insufficient memory is
   available. */

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Free oSymTable. */

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Put pvItem onto oSymTable.  Return 1 (TRUE) if successful, or 0
   (FALSE) if insufficient memory is available. */

int SymTable_put(SymTable_T oSymTable, const void *pvItem);

/*--------------------------------------------------------------------*/

/* Pop and return the top item of oSymTable. */

void *SymTable_pop(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Return 1 (TRUE) if oSymTable is empty, or 0 (FALSE) otherwise. */

int SymTable_isEmpty(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each element of oSymTable, passing
   pvExtra as an extra argument.  That is, for each element pvItem
   of oSymTable, call (*pfApply)(pvItem, pvExtra). */

void SymTable_map(SymTable_T oSymTable,
               void (*pfApply)(void *pvItem, void *pvExtra),
               const void *pvExtra);

#endif
