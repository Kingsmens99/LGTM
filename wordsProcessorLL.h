#include <stdio.h>

typedef struct Node {
	int  amt;
	char word[45];
	struct Node *next;
	struct Node *prev;
} Node;

typedef struct List {
	char *filename;
	int size_tot;
	int size_uniq;
	Node *head;
} List;

List* countWordsWithLinkedList(FILE* fp);
int printWordsLinkedList(List *list);
