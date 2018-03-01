#include <stdio.h>
#include "list.h"

// #define LIST_POOL 1024
// #define ITEM_POOL 1024

// static list_t list_mem[LIST_POOL];
// static item_t item_mem[ITEM_POOL];
// void initNodes();
// void initLists();
// void printNodes();

// int main(int argc, const char * argv[])
// {
// 	int i = 5;
// 	list_t *list = ListCreate();
// 	ListAdd(list, &i);


// 	return 0;
// }

// A1TestDriver.c

// void addRemoveTest();
// void listConcatTrimTest();
// void listSearchTest();
// void listFreeTest();

int compare(void* a, void* b);
void free(void* item);

int main(void) {
	printf("%s\n", "Run multiple times pls, theres variable initialization problems that occurs sometimes");
	testListGetters();
	listFreeTest();
	concatTrimTest();
	searchTest();
}

void testListGetters()
{
	int a = 5;
	int b = 1;
	int c = 3;
	list_t* list = ListCreate();
	ListAdd(list, &a);
	ListAdd(list, &b);
	ListAdd(list, &c);

	printf("First %d\n", *(int*) ListFirst(list));
	printf("Next %d\n", *(int*) ListNext(list));
	printf("Next %d\n", *(int*) ListNext(list));
	printf("Prev %d\n", *(int*) ListPrev(list));
	printf("Next %d\n", *(int*) ListNext(list));
	printf("Last %d\n", *(int*) ListLast(list));
	printf("Prev %d\n", *(int*) ListPrev(list));
	printf("Curr %d\n", *(int*) ListCurr(list));

	printf("%d\n", ListCount(list));

	printf("%s\n", "Current at second index, value 1:");
	int d = 4;
	ListInsert(list, &d);
	printf("Curr %d\n", *(int*) ListCurr(list));
	printf("Prev, Expect 5 %d\n", *(int*) ListPrev(list));

	printf("%s\n", "Append/Prepend");
	int e = 7;
	ListPrepend(list, &e);
	printf("Expect 7 at head: %d\n", *(int*) ListFirst(list));
	int f = 9;
	printf("Expect 3 at tail: %d\n", *(int*) ListLast(list));
	ListAppend(list, &f);
	printf("Expect 9 at tail: %d\n", *(int*) ListLast(list));

	printf("Expect 9: %d\n", *(int*) ListRemove(list));
	printf("New tail 3: %d\n", *(int*) ListLast(list));
	ListPrev(list);
	ListPrev(list);
	printf("Current item: %d\n", *(int*) ListCurr(list));
	printf("Current Tail: %d\n", *(int*) list->tail->item);
	printf("Potential New Tail: %d\n", *(int*) list->tail->prev->item);
	printf("Trim: %d\n", *(int*) ListTrim(list));
	printf("Current Tail: %d\n", *(int*) list->tail->item);


}

void concatTrimTest() {
	int a = 1;
	int b = 2;
	int c = 4;
	int d = 6;
	list_t* list1 = ListCreate();
	list_t* list2 = ListCreate();
	ListAdd(list1, &a);
	ListAdd(list1, &b);
	ListAdd(list2, &c);
	ListAdd(list2, &d);
	ListConcat(list1, list2);

	printf("Tail of Concat expect 6 %d\n", *(int*) ListLast(list1));
}

int compare(void *a, void *b) {
	if (a == b) {
		return 1;
	} else {
		return 0;
	}
}

void searchTest() {
	int a = 3;
	int b = 8;
	int c = 1;
	int d = 2;
	list_t* list = ListCreate();
	ListInsert(list, &a);
	ListInsert(list, &b);
	ListInsert(list, &c);
	ListInsert(list, &d);
	if (*((int *)ListSearch(list, compare, &c)) == c) {
		printf("%s \n","Successfully searched.");
	} else {
		printf("%s \n","Failed searched.");
	}
	if (*((int *)ListSearch(list, compare, &c)) == 4) {
		printf("%s \n","Successfully searched.");
	} else {
		printf("%s \n","Failed searched.");
	}
}

void free(void* item) {
	printf("%s \n","Successfully freed.");
}

void listFreeTest() {
	int a = 5;
	int b = 1;
	int c = 3;
	list_t* list = ListCreate();
	ListAdd(list, &a);
	ListAdd(list, &b);
	ListAdd(list, &c);
	ListFree(list, free);
	printf("%s\n", "Looks good");
}