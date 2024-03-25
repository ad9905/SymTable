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
   psNewNode->pcKey = malloc(strlen(pcKey) + 1);

   /* check for sufficient memory */
   if (psNewNode->pcKey == NULL)
   {
      free(psNewNode);
      return 0;
   }
   oSymTable->nodeQuantity++;
   psNewNode->pvValue = pvValue;
   psNewNode->psNextNode = oSymTable->psFirstNode;
   oSymTable->psFirstNode = psNewNode;
   return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
{
   struct SymTableNode* psCurrentNode;
   void *oldValue;

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
         oldValue = (void*)psCurrentNode->pvValue;
         psCurrentNode->pvValue = pvValue;
         return oldValue;
      }
   }
return NULL;
}

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableNode* psCurrentNode;
   
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
         return 1;
      }
   }
return 0;
}

/*--------------------------------------------------------------------*/

  void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableNode* psCurrentNode;
   
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
        return (void*)psCurrentNode->pvValue;
      }
   }
return NULL;
}


/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableNode* psCurrentNode;
   void *oldValue;
   struct SymTableNode* psPrevNode = NULL;
   
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
         /* save the value */
         oldValue = (void*)psCurrentNode->pvValue;
         /* if else statement to change the links to remove the node */
         if (psPrevNode == NULL)
         {
            /* sym tables first node skip current and go to current's next */
            oSymTable->psFirstNode = psCurrentNode->psNextNode;
         }
         else 
         {
           /* previous node's next has to point to current node's next */
            psPrevNode->psNextNode = psCurrentNode->psNextNode;
         }
         /* free the key and free the node */
         free ((void*)psCurrentNode->pcKey);
         free (psCurrentNode);
         /* decrement count */
         oSymTable->nodeQuantity--;
         /* return value */
         return oldValue;
      }
     /* move previous to become the current one */
      psPrevNode = psCurrentNode;
   }
return NULL;
}


/*--------------------------------------------------------------------*/


 void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra)
{
   struct SymTableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
      /* pfApply has three arguments, key, value, extra */ 
      (*pfApply)(psCurrentNode->pcKey, (void*)psCurrentNode->pvValue, (void*)pvExtra);
}
