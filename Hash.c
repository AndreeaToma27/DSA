#define _CRT_SECURE_NO_WARNINGS
#define LSIZE 256
#define HSIZE 26
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct Task {
	unsigned int id;
	char* date;
	char* owner;
	float completion;
}Task;

typedef struct Node {
	struct Node* next;
	Task* info;
}Node;

typedef struct HashTable {
	int dim;
	Node** vector;
}HT;

Task* createTask(unsigned int id, char* date, char* owner, float completion) {
	Task* t = (Task*)malloc(sizeof(Task));
	if (t) {
		t->id = id;
		t->date = (char*)malloc(strlen(date) + 1);
		strcpy(t->date, date);
		t->owner = (char*)malloc(strlen(owner) + 1);
		strcpy(t->owner, owner);
		t->completion = completion;
	}
	return t;
}

void printTask(Task* t) {
	printf("\n-------%hu  Date:%s, Owner: %s, Completion: %.2f", t->id, t->date, t->owner, t->completion);
}

Node* createNode(Task* t) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = t;
	n->next = NULL;
	return n;
}

void addNode(Node** list, Task* t) {
	Node* n = createNode(t);
	if (*list == NULL) {
		*list = n;
	}
	else {
		n->next = *list;
		*list = n;
	}
}

void printList(Node* list) {
	while (list) {
		printTask(list->info);
		list = list->next;
	}
}

int fhash(Task* t) {
	int hash = 0;
	char* tsk = t->owner;
	while (*tsk) {
		hash += *tsk++;
	}
	return hash % HSIZE;
}

void addHT(HT** ht, Task* t) {
	if (*ht == NULL) {
		*ht = (HT*)malloc(sizeof(HT));

		if (*ht) {
			(*ht)->dim = HSIZE;
			(*ht)->vector = (Node**)malloc(sizeof(Node*) * (*ht)->dim);
			if ((*ht)->vector) {
				memset((*ht)->vector, 0, sizeof(Node*) * (*ht)->dim);
			}
		}
	}
	if (*ht) {
		int index = fhash(t);
		addNode(&(*ht)->vector[index], t);
	}
}

void printHT(HT* ht) {
	for (int i = 0; i < ht->dim; i++) {
		printList(ht->vector[i]);
	}
}

int noTasks(HT* ht, char* own) {
	int counter = 0;
	for (int i = 0; i < ht->dim; i++) {
		Node* current = ht->vector[i];
		while (current) {
			if (strcmp(current->info->owner, own) == 0) {
				counter++;
			}
			current = current->next;
		}
	}
	return counter;
}
int noComplLevel(HT* ht, float compl) {
	int counter = 0; 
	for (int i = 0; i < ht->dim; i++) {
		Node* current = ht->vector[i];
		while (current) {
			if (current->info->completion > compl) {
				counter++;
			}
			current = current->next;
		}
	}
	return counter;
}

void freeTask(Task** t)
{
	free((*t)->date);
	free((*t)->owner);
	*t = NULL;
}

void freeList(Node** list)
{
	// traverse entire list and free Tasks + the node itself
	while (*list)
	{
		Node* freeMe = *list;
		*list = (*list)->next;
		freeTask(&freeMe->info);
		free(freeMe);
	}
	*list = NULL;
}

void freeHT(HT** ht)
{
	// free each bucket individually, then free the Hash Table and set it to NULL
	for (int i = 0; i < (*ht)->dim; i++)
	{
		freeList(&(*ht)->vector[i]);
	}

	//free the buckets array
	free((*ht)->vector);

	//free the HashTable
	free(*ht);
	*ht = NULL;
}


int main() {
	unsigned int id;
	char date[LSIZE];
	char owner[LSIZE];
	float completion;

	char buffer[LSIZE];
	char del[] = ",\n";
	char* tok;
	HT* ht = NULL;

	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;
	while (fgets(buffer, LSIZE, fp)) {
		tok = strtok(buffer, del);
		id = (unsigned int)atoi(tok);
		tok = strtok(NULL, del);
		strcpy(date, tok);
		tok = strtok(NULL, del);
		strcpy(owner, tok);
		tok = strtok(NULL, del);
		completion = atof(tok);

		Task* t = createTask(id, date, owner, completion);
		addHT(&ht, t);
	}
	//printHT(ht);


	char name[] = "Andreea";
	printf("No tasks for Andreea: %d", noTasks(ht, name));
	printf("No tasks completed above: %d", noComplLevel(ht, 0.20));

	return 0;
}
