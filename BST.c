#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ARRAY_SIZE 1024

typedef struct Rezervare {
	unsigned int id;
	char* denumire;
	unsigned char nrCamere;
	char* numeClient;
	float suma;
}Rezervare;


typedef struct BinarySearchTree {
	Rezervare* data;
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
}BST;

Rezervare* createRezervare(unsigned int id, const char* denumire, unsigned char nrCamere, const char* numeClient, float suma)
{
	Rezervare* rez = (Rezervare*)malloc(sizeof(Rezervare));
	rez->id = id;
	rez->suma = suma;
	rez->nrCamere = nrCamere;
	rez->denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(rez->denumire, denumire);
	rez->numeClient = (char*)malloc(strlen(numeClient) + 1);
	strcpy(rez->numeClient, numeClient);
	return rez;
}

void printRezervare(Rezervare* rez)
{
	printf("\nRezervare #%d, denumire: %s, nume client: %s, nrCamere: %d, suma: %.2f",
		rez->id, rez->denumire, rez->numeClient, rez->nrCamere, rez->suma);
}

BST* createNode(Rezervare* rez)
{
	BST* node = (BST*)malloc(sizeof(BST));
	node->left = node->right = NULL;
	node->data = rez;
	return node;
}

void addBST(BST** bst, Rezervare* rez)
{
	if (*bst == NULL)
		*bst = createNode(rez);
	else
	{
		if (rez->id > (*bst)->data->id)
			addBST(&(*bst)->right, rez);
		else if (rez->id < (*bst)->data->id)
			addBST(&(*bst)->left, rez);
		else
		{
			if ((*bst)->data)
				free((*bst)->data);
			(*bst)->data = rez;
		}
	}
}

void inorder(BST* bst)
{
	if (bst)
	{
		inorder(bst->left);
		printRezervare(bst->data);
		inorder(bst->right);
	}
}

void totalCam(BST* bst, int* cnt)
{
	if (bst)
	{
		if (bst->left == NULL && bst->right == NULL)
		{
			*cnt += bst->data->nrCamere;
		}
		totalCam(bst->left, cnt);
		totalCam(bst->right, cnt);
	}
}

int getHeight(BST* bst)
{
	if (bst)
		return 1 + max(getHeight(bst->left), getHeight(bst->right));
	else
		return 0;
}

void freeRez(Rezervare* rez)
{
	free(rez->denumire);
	free(rez->numeClient);
}

void main()
{
	FILE* fp = fopen("data.txt", "r");
	if (fp == NULL) return -1;

	char buffer[ARRAY_SIZE];
	char* tok;
	char del[] = ",\n";

	BST* bst = NULL;

	unsigned int id;
	char denumire[ARRAY_SIZE];
	unsigned char nrCamere;
	char numeClient[ARRAY_SIZE];
	float suma;

	while (fgets(buffer, ARRAY_SIZE, fp))
	{
		tok = strtok(buffer, del);
		strcpy(denumire, tok);
		tok = strtok(NULL, del);
		strcpy(numeClient, tok);
		tok = strtok(NULL, del);
		id = atoi(tok);
		tok = strtok(NULL, del);
		nrCamere = atoi(tok);
		tok = strtok(NULL, del);
		suma = atof(tok);

		Rezervare* rezervare = createRezervare(id, denumire, nrCamere, numeClient, suma);
		addBST(&bst, rezervare);
	}

	// ex 1
	printf("Prima afisare a arborelui:\n");
	inorder(bst);

	// ex 2
	int totalCamere = 0;
	totalCam(bst, &totalCamere);
	printf("\n\nTotal camere pentru nodurile frunza: %d", totalCamere);

	// ex 3
	int height = getHeight(bst);
	printf("\n\nInaltime arbore: %d", height);

	/*
	// ex 4
	deleteRoot(&bst, bst->data->id);
	printf("\n\nAfisare a aroberului dupa stergere radacina:\n");
	inorder(bst);

	// ex 5

	int idx = 4;
	unsigned int* ids = getIds(bst, idx);
	printf("\n\nId-urile de la radacina pana la nodul %d: ", idx);
	for (int i = 0; i < ARRAY_SIZE && ids[i] != 0; i++)
		printf("%d ", ids[i]);

	printf("\n\n");
	free(ids);
	free(tok);*/
	fclose(fp);
}
