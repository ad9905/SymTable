/*--------------------------------------------------------------------*/
/* symtablelist.c (Version 6)                                         */
/* Author: Anh Dao                                                    */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include "symtable.h"
#include <string.h>

/*--------------------------------------------------------------------*/

/* Each item is stored in a SymTableNode.  SymTableNodes are linked to
   form a list.  */

struct SymTableNode
{
   /* The key */
   const char *pcKey;

   /* The value. */
   const void *pvValue;

   /* The address of the next SymTableNode. */
   struct SymTableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTableNode. 
This is the managing structure - needs two fields */

struct SymTable
{
   /* The address of the first SymTableNode. */
   struct SymTableNode *psFirstNode;

   /* Number of nodes in linked list */
   size_t nodeQuantity;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstNode = NULL;
   oSymTable-> nodeQuantity = 0;
   return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   struct SymTableNode *psCurrentNode;
   struct SymTableNode *psNextNode;

   assert(oSymTable != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
   {
      psNextNode = psCurrentNode->psNextNode;
      
      /* free the current node's key,
      const char stars are protected, must cast as plain char star to free. */
      free((char*)psCurrentNode->pcKey);
      free(psCurrentNode);
   }

   free(oSymTable);
}

/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable, 
const char *pcKey, const void *pvValue)
{
   struct SymTableNode *psNewNode;
   /* Travelling, placeholder node */
   struct SymTableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

/* WALK for loop */
   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
      /* pcCurrentNode walks to it's OWN next node */
        psCurrentNode = psCurrentNode->psNextNode)
   {
      if (strcmp(pcKey, psCurrentNode->pcKey) == 0)
      {
         return 0;
      }
   }
   
   /* in the case that there are no duplicates, make space and put in a new node */
   
   psNewNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
   if (psNewNode == NULL)
      return 0;
   
   /* need to make space for defensive copy of the key */
   malloc(strlength(pcKey) + 1));

   psNewNode->pvItem = pvItem;
   psNewNode->psNextNode = oSymTable->psFirstNode;
   oSymTable->psFirstNode = psNewNode;
   return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_pop(SymTable_T oSymTable)
{
   const void *pvItem;
   struct SymTableNode *psNextNode;

   assert(oSymTable != NULL);
   assert(oSymTable->psFirstNode != NULL);

   pvItem = oSymTable->psFirstNode->pvItem;
   psNextNode = oSymTable->psFirstNode->psNextNode;
   free(oSymTable->psFirstNode);
   oSymTable->psFirstNode = psNextNode;
   return (void*)pvItem;
}

/*--------------------------------------------------------------------*/

int SymTable_isEmpty(SymTable_T oSymTable)
{
   assert(oSymTable != NULL);

   return oSymTable->psFirstNode == NULL;
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
               void (*pfApply)(void *pvItem, void *pvExtra),
               const void *pvExtra)
{
   struct SymTableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
      (*pfApply)((void*)psCurrentNode->pvItem, (void*)pvExtra);
}
