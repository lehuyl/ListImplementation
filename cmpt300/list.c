#include "list.h"
#include <stdlib.h>
#include <stdio.h>

#define LIST_POOL 10
#define NODE_POOL 100
#define TRUE 1
#define FALSE 0

static list_t list_mem[LIST_POOL];
static node_t node_mem[NODE_POOL];
static int isArrInit = FALSE;
static node_t* freePointer = NULL;
static list_t* freeHeads = NULL;
static int isPastPrev = FALSE;
static int isPastNext = FALSE;
static int usedArray = 0;
static int usedHeads = 0;

void initNodes()
{
	int i = 0;
	while(i < NODE_POOL)
	{
		if(i == NODE_POOL -1)
		{
			node_mem[i].item = NULL;
			node_mem[i].next = NULL;
			break;
		}
		else
		{
			node_mem[i].item = NULL;
			node_mem[i].next = &node_mem[i+1];
		}
		
		i++;
	}
	i = NODE_POOL - 1;
	while(i >= 0)
	{
		if(i == 0)
		{
			node_mem[i].prev = NULL;
			break;
		}
		node_mem[i].prev = &node_mem[i-1];
		i--;
	}
}

void initLists()
{
	int i = 0;
	while(i < LIST_POOL)
	{
		list_mem[i].head = NULL;
		list_mem[i].tail = NULL;
		list_mem[i].currentItem = NULL;
		list_mem[i].size = 0;
		if(i == LIST_POOL -1)
		{
			list_mem[i].nextHead = NULL;
			break;
		}
		else
		{
			list_mem[i].nextHead = &list_mem[i+1];
		}
		
		i++;
	}
}

list_t* ListCreate()
{
	list_t* newList;
	if(usedHeads < LIST_POOL)
	{
		if(isArrInit == FALSE)
		{
			initNodes();
			initLists();
			isArrInit = TRUE;

			// list_t* newList = (list_t*) malloc(sizeof(list_t));
			if(newList == NULL)
			{
				return NULL;
			}

			freePointer = &node_mem[0];
			freeHeads = &list_mem[0];
			usedHeads++;
		}

		if(freeHeads != NULL)
		{
			newList = freeHeads;
			newList->head = NULL;
			newList->tail = NULL;
			newList->currentItem = NULL;
			newList->size = 0;
			freeHeads = newList->nextHead;

			return newList;
		}
		
	}	
	return NULL;
}

int ListCount(list_t *list)
{
	return list->size;
}


//looks to be pass by value
//test out passing double pointers
void *ListFirst(list_t *list)
{
	if(list->head == NULL)
	{
		return NULL;
	}
	node_t* front = list->head;
	list->currentItem = front;

	return front->item;
}

void *ListLast(list_t *list)
{
	if(list->tail == NULL)
	{
		return NULL;
	}
	node_t* end = list->tail;
	list->currentItem = end;

	return end->item;
}

void *ListNext(list_t *list)
{
	if(list->currentItem != NULL)
	{
		if(list->currentItem == list->tail)
		{
			list->currentItem = NULL;
			isPastNext = TRUE;
			return list->currentItem->item;
		}

		node_t* current = list->currentItem;
		current = current->next;
		list->currentItem = current;
	}

	if(isPastPrev == TRUE)
	{
		list->currentItem = list->head;
		isPastNext = FALSE;
		isPastPrev = FALSE;
	}
	if(isPastNext)
	{
		return NULL;
	}

	return list->currentItem->item;
}

void *ListPrev(list_t *list)
{
	if(list->currentItem != NULL)
	{
		if(list->currentItem == list->head)
		{
			list->currentItem = NULL;
			isPastPrev = TRUE;
			return list->currentItem->item;
		}

		node_t* current = list->currentItem;
		current = current->prev;
		list->currentItem = current;
	}

	if(isPastNext == TRUE)
	{
		list->currentItem = list->tail;
		isPastNext = FALSE;
		isPastPrev = FALSE;
	}
	if(isPastPrev)
	{
		return NULL;
	}
	
	return list->currentItem->item;
}

void *ListCurr(list_t *list)
{
	if(list->currentItem == NULL)
	{
		return NULL;
	}
	return list->currentItem->item;
}

node_t* nodeCreate(void* item, node_t* prev, node_t* next)
{
	if(freePointer == NULL)
	{
		return NULL;
	}
	node_t* nodeToAdd;
	nodeToAdd = freePointer;
	freePointer = freePointer->next;
	nodeToAdd->item = item;
	nodeToAdd->prev = prev;
	nodeToAdd->next = next;

	return nodeToAdd;
}

int ListAdd(list_t *list, void* item)
{
	if(usedArray < NODE_POOL)
	{
		node_t* node;
		if(list->head == NULL)
		{
			node = nodeCreate(item, NULL, NULL);
			// node->next = NULL;
			// node->prev = NULL;
			// freePointer = freePointer->next;
			if(node == NULL)
			{
				return -1;
			}
			list->head = node;
			list->currentItem = node;
			list->tail = node;
			list->size = list->size +1;
			usedArray++;

			return 0;
		}
		if(isPastPrev)
		{
			node = nodeCreate(item, NULL, list->head);
			if(node == NULL)
			{
				return -1;
			}
			list->head->prev = node;
			list->currentItem = node;
			list->head = node;
			isPastPrev = FALSE;

			// freePointer = freePointer->next;
			list->size = list->size+1;
			usedArray++;

			return 0;
		}
		else if(isPastNext)
		{
			node = nodeCreate(item, list->currentItem, NULL);
			list->tail->next = node;
			node->prev = list->tail;
			list->tail = node;
			list->currentItem = list->tail;
			isPastNext = FALSE;

			// freePointer = freePointer->next;
			list->size = list->size+1;
			usedArray++;

			return 0;
		}
		else if (list->currentItem->next == NULL)
		{
			node = nodeCreate(item, list->currentItem, list->currentItem->next);
			if(node == NULL)
			{
				return -1;
			}
			list->currentItem->next = node;
			list->currentItem = node;
			list->tail = node;

			// freePointer = freePointer->next;
			list->size = list->size+1;
			usedArray++;

			return 0;
		}
		else
		{
			node = nodeCreate(item, list->currentItem, list->currentItem->next);
			if(node == NULL)
			{
				return -1;
			}
			node_t* temp = list->currentItem->next;
			temp->prev = node;
			list->currentItem->next = node;
			list->currentItem = node;

			// freePointer = freePointer->next;
			list->size = list->size+1;
			usedArray++;

			return 0;
		}
	}
	
	return -1;
}

int ListInsert(list_t* list, void* item)
{
	if(usedArray < NODE_POOL)
	{
		node_t* node;
		if(list->head == NULL)
		{
			node = nodeCreate(item, NULL, NULL);
			if(node == NULL)
			{
				return -1;
			}
			list->head = node;
			list->currentItem = node;
			list->tail = node;
			list->size = list->size +1;
			usedArray++;

			return 0;
		}
		if(isPastPrev)
		{
			node = nodeCreate(item, NULL, list->head);
			if(node == NULL)
			{
				return -1;
			}
			list->head->prev = node;
			list->head = node;
			list->currentItem = node;
			isPastPrev = FALSE;

			// freePointer = freePointer->next;
			list->size = list->size +1;
			usedArray++;

			return 0;
		}
		else if(isPastNext)
		{
			node = nodeCreate(item, list->tail, NULL);
			if(node == NULL)
			{
				return -1;
			}
			list->tail->next = node;
			list->tail = node;
			isPastNext = FALSE;

			freePointer = freePointer->next;
			list->size = list->size +1;
			usedArray++;

			return 0;
		}
		else if(list->currentItem->prev == NULL)
		{
			node = nodeCreate(item, list->currentItem->prev, list->currentItem);
			if(node == NULL)
			{
				return -1;
			}
			list->currentItem->prev = node;
			list->currentItem = node;
			list->head = node;
			
			list->size = list->size+1;
			usedArray++;

			return 0;
		}
		else
		{
			node = nodeCreate(item, list->currentItem->prev, list->currentItem);
			node_t* temp = list->currentItem->prev;
			temp->next = node;
			list->currentItem->prev = node;
			list->currentItem = node;

			list->size = list->size +1;
			usedArray++;

			return 0;
		}

	}	

	return -1;
}

int ListAppend(list_t* list, void* item)
{
	if(usedArray < NODE_POOL)
	{
		node_t* node;
		if(list->head == NULL)
		{
			node = nodeCreate(item, NULL, NULL);
			if(node == NULL)
			{
				return -1;
			}

			list->head = node;
			list->currentItem = node;
			list->tail = node;
			list->size = list->size +1;
			usedArray++;

			return 0;
		}

		node = nodeCreate(item, list->tail, NULL);
		if(node == NULL)
		{
			return -1;
		}
		list->tail->next = node;
		list->tail = node;
		list->currentItem = node;

		list->size = list->size +1;
		usedArray++;

		return 0;
	}
	return -1;
}

int ListPrepend(list_t* list, void* item)
{
	if(usedArray < NODE_POOL)
	{
		node_t* node;
		if(list->head == NULL)
		{
			node = nodeCreate(item, NULL, NULL);
			if(node == NULL)
			{
				return -1;
			}
			list->head = node;
			list->currentItem = node;
			list->tail = node;
			list->size = list->size +1;
			usedArray++;

			return 0;
		}

		node = nodeCreate(item, NULL, list->head);
		if(node == NULL)
		{
			return -1;
		}
		list->head->prev = node;
		list->head = node;
		list->currentItem = node;

		list->size = list->size +1;
		usedArray++;

		return 0;
	}
	return -1;
}

void* ListRemove(list_t* list)
{
	if(list->currentItem == NULL)
	{
		return NULL;
	}
	else if(list->head == list->tail)
	{
		node_t* temp = list->head;
		list->head = NULL;
		list->tail = NULL;

		list->size = list->size - 1;
		usedArray--;
		void* item = temp->item;
		temp->prev = NULL;
		temp->next = freePointer;
		freePointer = temp;

		return item;
	}
	if(list->currentItem == list->tail)
	{
		node_t* temp = list->currentItem;
		node_t* newEnd = temp->prev;
		newEnd->next = NULL;
		list->currentItem = newEnd;
		list->tail = newEnd;

		list->size = list->size -1;
		usedArray--;
		
		void* item = temp->item;
		temp->prev = NULL;
		//make free pointer the previous node and link to next free node
		temp->next = freePointer;
		freePointer = temp;
		
		return item;
	}
	if(list->currentItem == list->head)
	{
		node_t* temp = list->currentItem;
		node_t* newHead = temp->next;
		newHead->prev = NULL;
		list->currentItem = newHead;
		list->head = newHead;

		list->size = list->size -1;
		usedArray--;
		void* item = temp->item;
		temp->prev = NULL;
		temp->next = freePointer;
		freePointer = temp;

		return item;
	}

	node_t* temp = list->currentItem;
	node_t* prev = temp->prev;
	node_t* next = temp->next;
	prev->next = next;
	next->prev = prev;
	list->currentItem = next;


	list->size = list->size -1;
	usedArray--;
	void* item = temp->item;
	temp->prev = NULL;
	temp->next = freePointer;
	freePointer = temp;

	return item;
}

void ListConcat(list_t* list1, list_t* list2)
{
	if(list1 == NULL || list2 == NULL)
	{
		return;
	}
	
	list1->tail->next = list2->head;
	list2->head->prev = list1->tail;
	list1->tail = list2->tail;
	list1->size = (list1->size + list2->size);

	list2->head = NULL;
	list2->currentItem = NULL;
	list2->tail = NULL;
	list2->size = 0;		

	usedHeads--;
	list2->nextHead = freeHeads;
	freeHeads = list2;
}

void ListFree(list_t* list, void (*itemFree) (void* itemToBeFreed))
{
	node_t* temp;
	while(list->head != NULL)
	{
		temp = list->head;
		list->head = list->head->next;
		(*itemFree)(temp->item);
		temp->prev = NULL;
		temp->next = freePointer;
		freePointer = temp;
		list->size = list->size -1;
	}

	usedHeads--;
	list->currentItem = NULL;
	list->tail = NULL;
	list->size = 0;

	list->nextHead = freeHeads;
	freeHeads = list;
}

void *ListTrim(list_t* list)
{
	if(list != NULL)
	{
		node_t* temp = list->tail;
		void* item = temp->item;
		node_t* newEnd = temp->prev;
		list->tail = newEnd;
		list->currentItem = newEnd;
		newEnd->next = NULL;
		temp->next = freePointer;
		freePointer = temp;

		usedArray--;

		return item;
	}
}

void *ListSearch(list_t* list, int (*comparator) (void*, void*), void* comparisonArg)
{
	if((list->head == NULL || list->currentItem == NULL || list->currentItem->item == NULL))
	{
		return NULL;
	}
	if((*comparator)(list->currentItem->item, comparisonArg) == 1)
	{
		return list->currentItem->item;
	}
	while(list->currentItem->next != NULL)
	{
		list->currentItem = list->currentItem->next;
		if((*comparator)(list->currentItem->item, comparisonArg) == 1)
		{
			return list->currentItem->item;
		}
	}

	return NULL;
}