typedef struct node
{
	void* item;
	struct node* next;
	struct node* prev;
} node_t;


typedef struct list
{
	node_t* head;
	node_t* tail;
	node_t* currentItem;
	struct list* nextHead;
	int size;
} list_t;

list_t *ListCreate();
int ListCount(list_t *list);
void *ListFirst(list_t *list);
void *ListLast(list_t *list);
void *ListNext(list_t *list);
void *ListPrev(list_t *list);
void *ListCurr(list_t *list);
int ListAdd(list_t* list, void* item);
int ListInsert(list_t* list, void* item);
int ListAppend(list_t* list, void* item);
int ListPrepend(list_t* list, void* item);
void *ListRemove(list_t* list);
void ListConcat(list_t* list1, list_t* list2);
void ListFree(list_t* list, void (*itemFree) (void* itemToBeFreed));
void *ListTrim(list_t* list);
void *ListSearch(list_t* list, int (*comparator) (void*, void*),void* comparisonArg);