#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define SIZE 1000

typedef struct Exam {
	char* examCode;
	unsigned int noStudents;
	char examDate[10];
	float weightFinalGrade;
}Exam;

typedef struct Node {
	Exam* info;
	struct Node* next;
}Node;

Exam* createExam(char* examCode, unsigned int noStudents, char* examDate, float weightFinalGrade);
Node* createNode(Exam* exam);
void addNode(Node** list, Node* node);
void printList(const Node* list);
int noExams(Node* list, float weight);
void deleteExamsBeforeDate(Node** list, const char* date);


int main() {

	FILE* f = fopen("data.txt", "r");
	if (f == NULL) {
		return -1;
	}

	Node* list = NULL;
	char buffer[SIZE];
	char* tok = NULL;
	char del[] = ",";

	char examCode[SIZE];
	unsigned int noStudents;
	char examDate[SIZE];
	float weightFinalGrade;


	while (fgets(buffer, sizeof(buffer), f) != NULL) {
		tok = strtok(buffer, del);
		strcpy(examCode, tok);

		tok = strtok(NULL, del);
		noStudents =(unsigned int)atoi(tok);

		tok = strtok(NULL, del);
		strcpy(examDate, tok);

		tok = strtok(NULL, del);
		weightFinalGrade = atof(tok);

		Exam* exam = createExam(examCode, noStudents, examDate, weightFinalGrade);
		Node* node = createNode(exam);
		addNode(&list, node);
	}

	//printList(list);




//3
	
	int a=noExams(list, 23.3);
	printf("%d",a);
	


	deleteExamsBeforeDate(&list, "2023-12-12");
	printList(list);
	
	return 0;
}

Exam* createExam(char* examCode, unsigned int noStudents, char* examDate, float weightFinalGrade) {
	Exam* exam = NULL;
	exam = (Exam*)malloc(sizeof(Exam));
	if (exam) {
		exam->examCode = (char*)malloc(strlen(examCode) + 1);
		strcpy(exam->examCode, examCode);

		exam->noStudents = noStudents;

		//char[ceva] cu strcpy atat
		strcpy(exam->examDate, examDate);

		exam->weightFinalGrade = weightFinalGrade;
	}
	return exam;
}

void printExam(Exam* exam) {

	printf("Exam Code: %s, No Students: %d, Exam Date: %s, Weight Final Grade: %.2f\n", exam->examCode, exam->noStudents, exam->examDate, exam->weightFinalGrade);

}

Node* createNode(Exam* exam) {
	Node* node = NULL;
	node = (Node*)malloc(sizeof(Node));

	node->info = exam;
	node->next = NULL;
	return node;
}

void addNode(Node** list,Node* node) {
	if (*list == NULL) {

		*list = node;
	}
	else {
		if (node->info->noStudents < 100) {
			node->next = *list;
			*list = node;
		}
		else {
			
			Node* tmp = *list;
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			tmp->next = node;
		}
	}
}

void printList(const Node* list) {
	printf("Category < 100\n");
	const Node* tmp = list;
	while (tmp!=NULL) {
		if (tmp->info->noStudents < 100) {
			
			printExam(tmp->info);
			
		}tmp = tmp->next;
			
	}
	tmp = list;
	printf("Category > 100\n");
	while (tmp) {
		if (tmp->info->noStudents > 100) {
			
			printExam(tmp->info);
			
		}tmp = tmp->next;
	}

	
}

int noExams(Node* list,float weight) {
	int count = 0;
	while (list) {
		if (list->info->weightFinalGrade = weight) {
			count++;
		}
		list = list->next;
	}
	return count;
}


void deleteExamsBeforeDate(Node** list, const char* date) {
	Node* current = *list;
	Node* prev = NULL;

	while (current != NULL) {
		if (strcmp(current->info->examDate, date) < 0) {
			// Exam date is before the specified date, delete this node
			Node* temp = current;
			if (prev == NULL) {
				// Deleting the head node
				*list = current->next;
			}
			else {
				prev->next = current->next;
			}
			current = current->next;
			free(temp->info->examCode);
			free(temp->info);
			free(temp);
		}
		else {
			// Move to the next node
			prev = current;
			current = current->next;
		}
	}
}
