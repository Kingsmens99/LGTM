#include "wordsProcessorLL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quickSort(Node *head);
void _quickSort(Node *head, Node *last);
Node *lastNode(Node *root);
Node *partition(Node *l, Node *h);
void swap_amt(int* a, int* b);
void swap_wrd(char *a, char *b);
/*			*/
void cleanWord(char *word, int i);
void print(Node *head);
void dupRemoverLinkedList(Node *head);
void addNode(List *list, char *word);

/*
	Fuction countWordsWithLinkedList:
		This function creates a new LL, fills it up with words from file, and then alphabetically re-arranges them.
		It returns a pointer to the LL.
*/
List* countWordsWithLinkedList(FILE* fp){ //First required function
	char buff[45];
	List *list = (List*) malloc(sizeof(List));
	list->size_tot = 0;
	list->size_uniq = 0;
	list->head = NULL;

	while( fscanf(fp, "%s", buff) != EOF){ //This list->head.word/-> is a flaw. The head link list "node" only supposed to point to the
					      //true first node in the list. We may have to change this out to a char buff array and pass it
					      //as a parameter in addNode. This should fix alot of things, I hope...
		list->size_tot++;
		list->size_uniq++;
		cleanWord(buff, 0);
		//printf("-> %s\n", buff);
		addNode(list, buff);
	}

	dupRemoverLinkedList(list->head);
	quickSort(list->head);
	
	return list;
}
/* This function takes the unsanitized string and sanitizes it */
void cleanWord(char *word, int i){
/*	char *token, *svptr1, *str1;
	int i;
	
	printf("Word: %s -> ", word);
	for(str1 = word; ; str1 = NULL){
		token = strtok_r(str1, "\"(,.!?)\"", &svptr1);// old del "\"(-,.!?)\"" gotta ask about the -
		if (token == NULL ){ break; }
		printf("%s ", token);
	}
*/
	char *token, *svptr1, *str1;
	

	
        //printf("Word: %s -> ", word);
        for(str1 = word; ; str1 = NULL){
            token = strtok_r(str1, "\"(,.!?)\"", &svptr1);// old del "\"(-,.!?)\"" gotta ask about the -
            if (token == NULL ){ break; }
            //printf("%s ", token);
		    if ( i > 0 ){ strcpy(word, token); return; }
        }

	if(strncmp(word, "\"", 1) == 0 || strncmp(word, "(", 1) == 0){
		i++;
		cleanWord(word, i);
		return;
	}
	else{ return; }
}

/* This functions takes a link list ptr and then traverses the entire link list and prints it out */
int printWordsLinkedList(List *list){ //Second required function
	Node *ptr = list->head;
	
	printf("The file %s has:\n", list->filename);
	while(ptr != NULL){
		printf("%s appears %d times\n", ptr->word, ptr->amt);
		if(ptr->amt > 1){
			list->size_uniq = list->size_uniq - (ptr->amt - 1);
		}
		ptr = ptr->next;
	}
	printf("The total number of words is %d; number of different words is %d.\n\n", list->size_tot, list->size_uniq);
	
	return list->size_uniq;
}

/* This function removes duplicate words from the link list */
void dupRemoverLinkedList(Node *head){
	if(head == NULL || head->next == NULL){ return;}
	Node *x = head;
	Node *y;

	while(x != NULL){
		y = x->next;
		while(y != NULL){
			if (strcasecmp(y->word, x->word) == 0){
				if(y->next != NULL){
                    y->next->prev = y->prev;
		            y->prev->next = y->next;
             	    x->amt = x->amt + y->amt;
				    Node *temp = y;
				    y = y->next;
                    temp->next = NULL;
		            temp->prev = NULL;
           		    free(temp);
				}
                else{
                    //y->next->prev = y->prev; // Cant do this because we are at the end of LL
                    y->prev->next = y->next; // This is fine
                    x->amt = x->amt + y->amt;
                    Node *temp = y;
                    y = y->next;
                    temp->next = NULL;
                    temp->prev = NULL;
                    free(temp);
                }
			}
			else{
                y = y->next;
            }
		}
		x = x->next;
	}
	return;
}
/* This function adds nodes to the link list */
void addNode(List *list, char *word){
	Node* temp;
	Node* search = list->head;
	Node* newNode = (Node*) malloc(sizeof(Node));
    	newNode->amt = 0;
	newNode->prev = NULL;
	newNode->next = NULL;

	if (search == NULL){
		//printf("Adding: %s\n", word);
		strcpy(newNode->word, word);
		newNode->amt++;
		list->head = newNode;		
		return;
    	}

	else{
		while(search != NULL){
			temp = search;
    			search = search->next;
		}

		search = temp;
		newNode->prev = search;
		newNode->next = NULL;
		//printf("Adding: %s\n", word);
		strcpy(newNode->word, word);
		newNode->amt++;
		search->next = newNode;
		return;
	}
}

/*  A function to swap two intergers */
void swap_amt ( int* a, int* b ){ 
	int t = *a; 
	*a = *b; 
	*b = t;
}

/* A function to swap two words array */
void swap_wrd ( char *a, char *b ){
        char t[45];
	memmove(t, a, 45);
	memmove(a, b, 45);
	memmove(b, t, 45);
}


/* A function to find last node of linked list */
Node *lastNode(Node *root){
    while (root && root->next){ root = root->next; }
    return root;
}
 
// Considers last element as pivot, places the
//pivot element at its correct position in sorted array,
//and places all smaller (smaller than pivot) to left
//of pivot and all greater elements to right of pivot 

Node* partition(Node *l, Node *h){
    // set pivot as h element
    char *x = h->word;
 
    // similar to i = l-1 for array implementation
    Node *i = l->prev;
    Node *j;

    // Similar to "for (int j = l; j <= h- 1; j++)"
    for (j = l; j != h; j = j->next){
        if (strcasecmp(j->word, x) < 0){
            // Similar to i++ for array
            i = (i == NULL) ? l : i->next;
 
	    swap_amt(&(i->amt), &(j->amt));
            swap_wrd(i->word, j->word);
        }
    }

    i = (i == NULL) ? l : i->next; // Similar to i++
    swap_amt(&(i->amt), &(h->amt));
    swap_wrd(i->word, h->word);

    return i;
}
 
// A recursive implementation of quicksort for linked list 
void _quickSort(Node* head, Node *last)
{
    if (last != NULL && head != last && head != last->next)
    {
        Node *p = partition(head, last);
        _quickSort(head, p->prev);
        _quickSort(p->next, last);
    }
}
 
// The main function to sort a linked list.
// It mainly calls _quickSort()
void quickSort(Node *head)
{
    // Find last node
    Node *h = lastNode(head);
 
    // Call the recursive QuickSort
    _quickSort(head, h);
}
