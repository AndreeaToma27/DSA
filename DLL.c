#define _CRT_SECURE_NO_WARNINGS
#define line_size 256
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

typedef struct Reteta {
	unsigned int no;
	char* patientName;
	char* doctorName;
	char* specialStatus;
	unsigned char noMeds;
	float discount;
}Reteta;

typedef struct Node {
	Reteta* info;
	struct Node* prev;
	struct Node* next;
}Node;


Reteta* createReteta(unsigned int no, char* patientName, char* doc, char* status, unsigned char noMeds, float disc) {
	Reteta* r = (Reteta*)malloc(sizeof(Reteta));
	r->no = no;
	r->patientName = (char*)malloc(strlen(patientName) + 1);
	strcpy(r->patientName, patientName);
	r->doctorName = (char*)malloc(strlen(doc) + 1);
	strcpy(r->doctorName, doc);
	r->specialStatus = (char*)malloc(strlen(status) + 1);
	strcpy(r->specialStatus, status);
	r->noMeds = noMeds;
	r->discount = disc;
	return r;
}

void printReteta(Reteta* r) {
	printf("\n%i, %s, %s, %s, %u, %.2f", r->no, r->patientName, r->doctorName, r->specialStatus, r->noMeds, r->discount);
}

Node* createNode(Reteta* r) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = r;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

void addToListPosition(Node** list, Node* node, int pos) {
	if (*list == NULL || pos == 0) {
		node->next = *list;
		if (*list) (*list)->prev = node;
		*list = node;
		return;
	}

	Node* current = *list;
	int currentPosition = 0;

	while (current->next != NULL && currentPosition < pos - 1) {
		current = current->next;
		currentPosition++;
	}

	node->next = current->next;
	if (current->next) current->next->prev = node;
	node->prev = current;
	current->next = node;

}

void addToList(Node** list, Node* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

void printList(Node* list) {
	while (list->next != NULL) {
		printReteta(list->info);
		list = list->next;
	}
	printReteta(list->info);
	printf("\n---------");

	while (list->prev != NULL) {
		printReteta(list->info);
		list = list->prev;
	}
	printReteta(list->info);
}

int nrRetete(Node* list, int prag) {
	int count = 0;
	while (list->next != NULL) {
		if (list->info->noMeds > prag) {
			count++;
		}
		list = list->next;
	}
	if (list->info->noMeds > prag) {
		count++;
	}
	return count;
}

void changeDiscount(Node* list, char* name, float perc) {
	while (list->next != NULL) {
		if (strcmp(list->info->patientName, name) == 0) {
			list->info->discount = perc;
			break;
		}
		list = list->next;
	}
	if (strcmp(list->info->patientName, name) == 0) {
		list->info->discount = perc;
	}
}

void freeReteta(Reteta* r)
{
	free(r->patientName);
	free(r->doctorName);
	free(r->specialStatus);

}

void freeList(Node** list) {
	while (*list)
	{
		Node* freeMe = *list;
		*list = (*list)->next;
		freeReteta(freeMe->info);
		free(freeMe);
	}
}

int main() {
	FILE* fp = fopen("Data.txt", "r");
	if (fp == NULL) return -1;

	char* tok;
	char buffer[line_size];
	char del[] = ",\n";

	unsigned int no;
	char patientName[line_size];
	char doctorName[line_size];
	char specialStatus[line_size];
	unsigned char noMeds;
	float discount;

	Node* list = NULL;

	while (fgets(buffer, line_size, fp)) {
		tok = strtok(buffer, del);
		no = (unsigned int)atoi(tok);

		tok = strtok(NULL, del);
		strcpy(patientName, tok);

		tok = strtok(NULL, del);
		strcpy(doctorName, tok);

		tok = strtok(NULL, del);
		strcpy(specialStatus, tok);

		tok = strtok(NULL, del);
		noMeds = (unsigned char)atoi(tok);

		tok = strtok(NULL, del);
		discount = atof(tok);

		Reteta* r = createReteta(no, patientName, doctorName, specialStatus, noMeds, discount);
		//printReteta(r);

		Node* n = createNode(r);
		addToListPosition(&list, n, 1);

	}

	char name[] = "Jane Roe";
	float discountNou = 22.33;
	changeDiscount(list, name, discountNou);

	int prag = 2;
	int nrRet = nrRetete(list, prag);
	//printf("Nr: %d", nrRet);

	printList(list);
	freeList(&list);
	free(tok);
	fclose(fp);
	return 0;
}
