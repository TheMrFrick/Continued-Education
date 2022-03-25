#ifndef _FILECOMPRESSOR_H_
#define _FILECOMPRESSOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct Node Node;

struct Node {

	int freq;
	int h;
	char* token;
	Node * left;
	Node * right;
	
};

typedef struct tNode tNode;

struct tNode{
	Node* node;
	tNode* next;
};

/* AVL Tree Prototypes */
void freeAVL(Node * root);
Node* insertAVL(char* token, Node* root);
int findHeight(Node* node);
int bigHeight(int node1, int node2);
Node* rotateLeft(Node* node);
Node* rotateRight(Node* node);
Node* doubleRotateL(Node* node);
Node* doubleRotateR(Node* node);
void printAVL(Node* root);
int compare(char* s1, char* s2);
int search(char* toke, Node* root);
Node* insertList( char*toke);
void tokenizer(int fd, int end);

/* Min Heap & Huffman Prototypes */
tNode* insertT(Node* huff, tNode* T);
void print(Node* huff, int arr[], int top);
Node* huffman();
Node** createHeap(Node** heap);
Node** siftDown(Node** heap);
void writeCodebook(Node* huff, int arr[], int top, int file);

/* General Helper Methods Prototypes*/
char* concat(char* str1, char buf);

/* Compression and Decompression */
//typedefs
typedef struct node
{
    char *code;
    char *rep;
    struct node *next;
} node;

//methods
static node *createNode(char *code, char *rep);
void freeLL(node *root);
static node *insertEndLL(node *head, char *code, char *rep);
void printLL(node *head);
char *findCode(node *root, char *token);
char *compareCode(char *code_in, node *root);


#endif
