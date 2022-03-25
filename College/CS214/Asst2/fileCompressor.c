#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "fileCompressor.h"
#include <sys/stat.h>

Node *root = NULL;  //root of AVL tree
Node *tList = NULL; //ll of tokens
int numNode;		//num nodes in tree
Node *huffmanTree;  //huffman tree

node *head = NULL;
char ch;
int fd, sz;
char *c;

//Free the AVL tree
void freeAVL(Node *root)
{
	if (root != NULL)
	{
		free(root->left);
		free(root->right);
		free(root);
	}
}

//Find the height of a node
//Need make sure it is not a NULL node
int findHeight(Node *node)
{
	if (node == NULL)
	{
		return -1;
	}
	return node->h;
}

//Checker to see which has the smaller height
int bigHeight(int node1, int node2)
{
	if (node1 > node2)
	{
		return node1;
	}
	return node2;
}

//Single AVL rotation with left child
Node *rotateLeft(Node *node)
{
	//node is the old root

	Node *newRoot;
	newRoot = node->left;
	node->left = newRoot->right;
	newRoot->right = node;

	int temp1;
	temp1 = findHeight(node->left);
	int temp2;
	temp2 = findHeight(node->right);
	int temp3;
	temp3 = findHeight(newRoot->left);

	node->h = bigHeight(temp1, temp2) + 1;
	newRoot->h = bigHeight(temp3, node->h) + 1;

	return newRoot;
}

//Single AVL rotation with the right child
Node *rotateRight(Node *node)
{
	//node is the old root

	Node *newRoot;
	newRoot = node->right;
	node->right = newRoot->left;
	newRoot->left = node;

	int temp1 = findHeight(node->left);
	int temp2 = findHeight(node->right);
	int temp3 = findHeight(newRoot->right);

	node->h = bigHeight(temp1, temp2) + 1;
	newRoot->h = bigHeight(temp3, node->h) + 1;

	return newRoot;
}

//Double AVL rotation with left
Node *doubleRotateL(Node *node)
{
	node->left = rotateRight(node->left);
	node = rotateLeft(node);
	return node;
}

//Double AVL rotation with right
Node *doubleRotateR(Node *node)
{
	node->right = rotateLeft(node->right);
	node = rotateRight(node);
	return node;
}

//Insert Node into AVL Tree
//Insets a new node into the LL of tokens

Node *insertList(char *toke)
{
	Node *node;
	node = (Node *)malloc(sizeof(Node));
	node->token = toke;

	if (tList == NULL)
	{
		return node;
	}

	node->right = tList;
	return node;
}

Node *insertAVL(char *toke, Node *root)
{
	//printf("inserted: %s\n", toke);
	int cmp;
	if (root != NULL)
	{
		cmp = compare(toke, root->token);
	}

	//the tree is empty, create AVL tree
	if (root == NULL)
	{
		root = (Node *)malloc(sizeof(Node));
		root->freq = 1;
		root->h = 0;
		root->token = toke;
		root->left = NULL;
		root->right = NULL;
		numNode++;
		tList = insertList(toke);
	}
	else if (cmp == 0)
	{
		root->freq = root->freq + 1; //increase the frequency of the token because appeared again
		return root;
	}
	else if (cmp < 0)
	{ //toke is LESS so go LEFT
		root->left = insertAVL(toke, root->left);

		//determine if rotation needed
		int temp;
		temp = abs(findHeight(root->left) - findHeight(root->right));
		//printf("temp is: %d\n", temp);
		if (temp == 2)
		{ //balance factor greater than 1
			int cmpTemp;
			cmpTemp = compare(toke, root->left->token);
			if (cmpTemp < 0)
			{
				//printf("rotated left\n");
				root = rotateLeft(root);
			}
			else
			{
				//printf("doubled rotated left\n");
				root = doubleRotateL(root);
			}
		}
	}
	else if (cmp > 0)
	{ //go right
		root->right = insertAVL(toke, root->right);

		//determine if rotation needed
		int temp;
		temp = abs(findHeight(root->left) - findHeight(root->right));
		if (temp == 2)
		{ //balance factor greater than 1
			int cmpTemp;
			cmpTemp = compare(toke, root->right->token);
			if (cmpTemp > 0)
			{
				//printf("rotate right\n");
				root = rotateRight(root);
			}
			else
			{
				//printf("double rotated right\n");
				root = doubleRotateR(root);
			}
		}
	}

	root->h = bigHeight(findHeight(root->left), findHeight(root->right)) + 1;
	return root;
}

//Print out the AVL tree
void printAVL(Node *root)
{
	if (root == NULL)
	{
		return;
	}
	printf("\t%s %d, %d, %d\n", root->token, root->token[0], root->freq, root->h);

	if (root->left != NULL)
	{
		printf("L: %s, %d, %d\t", root->left->token, root->left->freq, root->left->h);
	}
	if (root->right != NULL)
	{
		printf("R: %s, %d, %d", root->right->token, root->right->freq, root->right->h);
	}
	printf("\n- - - - - -\n");

	printAVL(root->left);
	printAVL(root->right);
}

//Compare strings to determine order for AVL tree
/*
 * 1 means s1 is smaller than s2
 * -1 means s1 is less than s2
 * 0 means both strings are equal
 */
int compare(char *s1, char *s2)
{

	int i, k, max;
	i = 0;
	k = 0;
	if (strlen(s1) > strlen(s2))
	{
		max = strlen(s1);
	}
	else if (strlen(s1) < strlen(s2))
	{
		max = strlen(s2);
	}
	else
	{
		max = strlen(s1);
	}

	while (i < max && k < max)
	{
		if (s1[i] == s2[k])
		{
			i++;
			k++;
			continue;
		}
		else if (isupper(s1[i]) && islower(s2[k]))
		{
			return 1;
		}
		else if (isupper(s2[k]) && islower(s1[i]))
		{
			return -1;
		}
		else if (islower(s1[i]) && islower(s2[k]))
		{
			if (s1[i] > s2[k])
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if (isupper(s1[i]) && isupper(s2[k]))
		{
			if (s1[i] > s2[k])
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if (s1[i] > s2[k])
		{
			return 1;
		}
		else if (s1[i] < s2[k])
		{
			return -1;
		}
	}

	//When the prefix characters are all the same
	//but one of them is longer than the other
	if (i == max && k != max)
	{
		return 1;
	}
	else if (k == max && i != max)
	{
		return -1;
	}

	return 0;
}

char *concat(char *str1, char buf)
{
	int length = strlen(str1);
	char *str2 = (char *)malloc(length + 1);
	strcpy(str2, str1);
	str2[length] = buf;
	str2[length + 1] = '\0';
	str1 = str2;
	return str1;
}

//Find the target node
int search(char *toke, Node *root)
{

	if (root == NULL)
	{
		return -1;
	}

	int cmp;
	cmp = compare(toke, root->token);

	if (cmp < 0)
	{
		return search(toke, root->left);
	}
	else if (cmp > 0)
	{
		return search(toke, root->right);
	}
	else
	{
		return root->freq;
	}
}

//Insert node into queue T
tNode *insertT(Node *huff, tNode *T)
{
	tNode *temp;
	temp = (tNode *)malloc(sizeof(tNode));
	temp->node = huff;
	temp->next = NULL;

	if (T == NULL)
	{
		return temp;
	}

	tNode *ptr;
	tNode *prev;
	ptr = T;
	prev = NULL;
	while (ptr != NULL)
	{
		if (huff->freq < ptr->node->freq)
		{
			if (prev == NULL)
			{
				temp->next = ptr;
				return temp;
			}
			else
			{
				temp->next = ptr;
				prev->next = temp;
				break;
			}
		}

		prev = ptr;
		ptr = ptr->next;
	}

	if (ptr == NULL)
	{
		prev->next = temp;
		temp->next = NULL;
	}

	return T;
}

//print huffman tree
void print(Node *huff, int arr[], int top)
{

	if (huff == NULL)
	{
		return;
	}

	if (huff->right != NULL)
	{
		arr[top] = 1;
		//printf("arr[top]: %d %d\n", arr[top], top);
		print(huff->right, arr, top + 1);
	}

	if (huff->left != NULL)
	{
		arr[top] = 0;
		print(huff->left, arr, top + 1);
	}

	if (huff->left == NULL && huff->right == NULL)
	{
		printf("%s: ", huff->token);
		int i;
		i = 0;
		for (; i < top; i++)
		{
			printf("%d", arr[i]);
		}
		printf("\n");
	}

	return;
}

//Write into codebook
void writeCodebook(Node *huff, int arr[], int top, int file)
{
	// printf("token:%s\n", huff->token);
	if (huff == NULL)
	{
		return;
	}

	if (huff->right != NULL)
	{
		// printf("went right\n");
		arr[top] = 1;
		//printf("arr[top]: %d %d\n", arr[top], top);
		writeCodebook(huff->right, arr, top + 1, file);
	}

	if (huff->left != NULL)
	{
		// printf("went left\n");
		// printf("token left %s\n", huff->left->token);
		arr[top] = 0;
		writeCodebook(huff->left, arr, top + 1, file);
	}

	if (huff->left == NULL && huff->right == NULL)
	{

		char *str;
		str = (char *)malloc(sizeof(char) * top + 1);
		int i;
		i = 0;
		for (; i < top + 1; i++)
		{
			str[i] = '\0';
		}
		i = 0;
		printf("token: %s\n", huff->token);
		for (; i < top; i++)
		{
			char temp;
			temp = arr[i] + '0';
			//printf("temp: %c\n", temp);
			str[i] = temp;
			if (i == 0 && top == 1)
			{
				break;
			}
		}
		//printf("----\n");
		//printf("here is the string %s\n", huff->token);
		write(file, str, strlen(str));
		char *tempstr;
		tempstr = "\t";
		write(file, tempstr, strlen(tempstr));
		write(file, huff->token, strlen(huff->token));
		write(file, "\n", strlen("\n"));
	}

	return;
}
//Tokenizer method
void tokenize(int fd, int end)
{
	int status;
	status = 0;
	int bytes;
	bytes = 0;
	char buffer;
	char *tempToke;
	tempToke = "";

	do
	{
		status = read(fd, &buffer, 1);

		//printf("byte: %d\n", bytes);

		if (bytes == end)
		{ //at the end of file
			printf("inserted: %s\n", tempToke);
			if (strcmp(tempToke, "") != 0)
			{
				root = insertAVL(tempToke, root);
			}
			//printf("end\n");
			break;
		}
		bytes++;
		//printf("buffer is %c %d\n", buffer, buffer);
		if (isspace(buffer))
		{
			//printf("temptoken is %s\n", tempToke);
			if (strcmp(tempToke, "") != 0)
			{
				root = insertAVL(tempToke, root);
			}
			// printAVL(root);
			//printAVL(root);
			//printf("token:%s\n", tempToke);
			tempToke = ""; //tokenize the delimiter
			if (buffer == 32)
			{ //buffer is a space
				//printf("space\n");
				tempToke = concat(tempToke, '\\');
				tempToke = concat(tempToke, ' ');
				//printAVL(root);
			}
			else if (buffer == '\t')
			{ //buffer is a tab
				//printf("tab\n");
				tempToke = concat(tempToke, '\\');
				tempToke = concat(tempToke, 't');
			}
			else if (buffer == '\n')
			{ //buffer is a new line
				//printf("new line\n");
				tempToke = concat(tempToke, '\\');
				tempToke = concat(tempToke, 'n');
			}
			else if (buffer == '\v')
			{ //vertical tab
				tempToke = concat(tempToke, '\\');
				tempToke = concat(tempToke, 'v');
			}
			else if (buffer == '\f')
			{ //feed
				tempToke = concat(tempToke, '\\');
				tempToke = concat(tempToke, 'f');
			}
			else if (buffer == '\r')
			{ //
				tempToke = concat(tempToke, '\\');
				tempToke = concat(tempToke, 'r');
			}
			// printf("temptoken is %s\n", tempToke);
			root = insertAVL(tempToke, root);
			tempToke = ""; //reset token
		}
		else if (buffer == 0 || buffer == '\0')
		{ //null char
			if (strcmp(tempToke, "") != 0)
			{
				root = insertAVL(tempToke, root);
			}
			tempToke = "";
			tempToke = concat(tempToke, '\\');
			tempToke = concat(tempToke, '0');
			root = insertAVL(tempToke, root);
			tempToke = ""; //reset token
		}
		else if (buffer == 7 || buffer == '\a')
		{ // \a ==> bell cntrl char
			if (strcmp(tempToke, "") != 0)
			{
				root = insertAVL(tempToke, root);
			}
			tempToke = "";
			tempToke = concat(tempToke, '\\');
			tempToke = concat(tempToke, 'a');
			root = insertAVL(tempToke, root);
			tempToke = ""; //reset token
		}
		else if (buffer == 8 || buffer == '\b')
		{ // \b ==> back space
			if (strcmp(tempToke, "") != 0)
			{
				root = insertAVL(tempToke, root);
			}
			tempToke = "";
			tempToke = concat(tempToke, '\\');
			tempToke = concat(tempToke, 'b');
			root = insertAVL(tempToke, root);
			tempToke = ""; //reset token
		}
		else
		{
			tempToke = concat(tempToke, buffer);
		}

	} while (status > 0);
	return;
}

//Make the min heap & huffman tree
Node *huffman()
{

	//printAVL(root);
	//initalize min heap
	Node **heap = (Node **)malloc(sizeof(Node *) * numNode);
	int i;
	i = 0;

	for (; i < numNode; i++)
	{
		heap[i] = (Node *)malloc(sizeof(Node));
		heap[i] = NULL;
	}

	heap = createHeap(heap);

	i = 0;
	while (i < numNode)
	{
		printf("%s %d\n", heap[i]->token, heap[i]->freq);
		i++;
	}
	printf("- - - - - - - \n");

	/* HUFFMAN CODING */
	tNode *t = NULL; //Queue T; only contains trees

	//T Queue is empty, dequeue first two from min heap
	Node *huffTemp;
	huffTemp = (Node *)malloc(sizeof(Node));

	if (heap[0] != NULL)
	{
		huffTemp->freq = heap[0]->freq;
		huffTemp->left = heap[0];
	}

	heap = siftDown(heap);

	i = 0;
	while (i < numNode)
	{
		printf("%s %d\n", heap[i]->token, heap[i]->freq);
		i++;
	}
	printf("- - - - - - - \n");

	if (heap[0] != NULL)
	{
		huffTemp->freq = huffTemp->freq + heap[0]->freq;
		huffTemp->right = heap[0];
		heap = siftDown(heap);
	}

	//add to t queue
	t = (tNode *)malloc(sizeof(tNode));
	t->node = huffTemp;
	t->next = NULL;

	/*
	i = 0;
	while (i < numNode)
	{
		printf("%s %d\n", heap[i]->token, heap[i]->freq);
		i++;
	}*/

	while (heap[0] != NULL)
	{
		//printf("bellolo\n");
		Node *huffTemp; //for building new tree
		huffTemp = (Node *)malloc(sizeof(Node));
		tNode *tptr;
		tptr = t;

		Node *A; //first node
		Node *B; //second node

		/*
		tNode *tempptr = t;
		while (tempptr != NULL)
		{
			printf("%d\t", tempptr->node->freq);
			tempptr = tempptr->next;
		}
		printf("\n");*/
		printf("hello----\n");

		//Get Node A
		if (tptr != NULL && heap[0] != NULL)
		{ //T queue is not empty
			if (heap[0]->freq <= tptr->node->freq)
			{
				printf("hello1\n");
				A = heap[0];
				printf("-----\n");
				heap = siftDown(heap);
			}
			else
			{
				A = tptr->node;
				t = tptr->next; //delete the node
				tptr = t;
			}
		}
		else if (heap[0] != NULL)
		{ //T Queue is empty
			printf("hello2\n");
			A = heap[0];
			heap = siftDown(heap);
		}
		else
		{ //heap is empty
			A = tptr->node;
			t = tptr->next;
			tptr = t;
		}

		/*printf("- - - - - - -> \n");
		i = 0;
		while (i < numNode)
		{
			if (heap[i] == NULL)
			{
				printf("heap is empty\n");
				break;
			}
			printf("%s %d\n", heap[i]->token, heap[i]->freq);
			i++;
		}
		printf("- - - - - - -> \n");*/

		//Get Node B
		if (tptr != NULL && heap[0] != NULL)
		{ //both are not empty
			if (heap[0]->freq <= tptr->node->freq)
			{
				printf("hello3\n");
				B = heap[0];
				heap = siftDown(heap);
			}
			else
			{
				B = tptr->node;
				t = tptr->next;
				tptr = t;
			}
		}
		else if (heap[0] != NULL)
		{ //T Queue is empty
			printf("hello4\n");
			B = heap[0];
			printf("entered here\n");
			heap = siftDown(heap);
		}
		else if (tptr != NULL)
		{ //heap is empty
			B = tptr->node;
			t = tptr->next;
			tptr = t;
		}
		else
		{ //t queue is empty
			printf("----> here?\n");
			B = NULL;
			break;
		}

		/*
		i = 0;
		while (i < numNode)
		{
			if (heap[i] == NULL)
			{
				printf("heap is empty\n");
				break;
			}
			printf("%s %d\n", heap[i]->token, heap[i]->freq);
			i++;
		}
		printf("- - - - - - ->\n");*/

		if (A != NULL)
		{
			huffTemp->freq = A->freq;

			if (A->token == NULL)
			{
				printf("---->here left\n");
				huffTemp->left = A;
				huffTemp->right = B;
			}
			else
			{
				printf("---->here right\n");
				huffTemp->left = B;
				huffTemp->right = A;
			}
		}

		if (B != NULL)
		{
			huffTemp->freq = B->freq;
		}

		//huffTemp->left = A;
		//huffTemp->right = B;
		t = insertT(huffTemp, t);
		printf("at the end now?\n");
		printf("freq: %d\n", A->freq);
		//printf("freq: %d\n", B->freq);
	}

	tNode *tempptr = t;
	while (tempptr != NULL)
	{
		printf("%d\t", tempptr->node->freq);
		tempptr = tempptr->next;
	}
	printf("\n");

	int arr[1000];
	print(t->node, arr, 0);

	return t->node;
}

//Create the heap
Node **createHeap(Node **heap)
{

	Node *ptr;
	ptr = tList;
	Node *tempNode;

	//get all the tokens and insert into the minheap
	int i;
	i = 0;
	while (ptr != NULL)
	{
		printf("ptr is: %s\n", ptr->token);
		tempNode = (Node *)malloc(sizeof(Node));
		tempNode->token = ptr->token;
		tempNode->freq = search(ptr->token, root);

		if (heap[0] == NULL)
		{ //root of heap is null
			heap[0] = tempNode;
		}
		else if (heap[i] == NULL)
		{
			heap[i] = tempNode;

			//Sift Up
			Node *x;
			x = heap[(i - 1) / 2]; //grab parent
			int j;
			j = i;
			while (j != 0)
			{ //j == 0 means at the root, can't go futher
				if (heap[j]->freq < x->freq)
				{ //sifts up the node

					heap[j] = x;
					heap[(i - 1) / 2] = tempNode; //put
					x = tempNode;
					j = (j - 1) / 2;
				}
				else
				{
					break;
				}
			}
		}
		// printf("%s\n", ptr->token);
		ptr = ptr->right;
		i++;
	}

	printf("ended\n");

	return heap;
}

Node **siftDown(Node **heap)
{
	if (numNode == 1)
	{
		heap[0] = NULL;
		return heap;
	}
	int j;
	j = 0;
	heap[0] = heap[numNode - 1];
	heap[numNode - 1] = NULL;
	numNode--;

	int l;
	int r;
	l = 2 * j + 1;
	r = 2 * j + 2;

	while (heap[l] != NULL && heap[r] != NULL)
	{

		Node *smaller;

		//setting smaller
		if (heap[l] != NULL && heap[r] != NULL)
		{
			printf("hello?\n");
			if (heap[l]->freq > heap[r]->freq)
			{
				smaller = heap[r];

				//parent > child
				if (heap[j]->freq > smaller->freq)
				{
					heap[r] = heap[j];
					heap[j] = smaller;
					j = r;
					l = 2 * j + 1;
					r = 2 * j + 2;
				}
				else
				{
					break;
				}
			}
			else
			{
				smaller = heap[l];

				//parent > child
				if (heap[j]->freq > smaller->freq)
				{
					heap[l] = heap[j];
					heap[j] = smaller;
					j = l;
					l = 2 * j + 1;
					r = 2 * j + 2;
				}
				else
				{
					break;
				}
			}
		}
		else if (heap[l] != NULL && heap[r] == NULL)
		{
			smaller = heap[l];

			//parent > child
			if (heap[j]->freq > smaller->freq)
			{
				heap[l] = heap[j];
				heap[j] = smaller;
				j = l;
				l = 2 * j + 1;
				r = 2 * j + 2;
			}
			else
			{
				break;
			}
		}
		else if (heap[r] != NULL && heap[l] == NULL)
		{
			smaller = heap[r];

			//parent > child
			if (heap[j]->freq > smaller->freq)
			{
				heap[r] = heap[j];
				heap[j] = smaller;
				j = r;
				l = 2 * j + 1;
				r = 2 * j + 2;
			}
			else
			{
				break;
			}
		}
		else
		{ //both equal null
			break;
		}

		if (l > numNode && r > numNode)
		{
			break;
		}
	}

	printf("end siftDown\n");
	return heap;
}

/*
void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
    
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2);
        } else {
        	
            printf("%*s- %s\n", indent, "", entry->d_name);
            
            
        }
    }
    closedir(dir);
}*/

void listdir(char *dirpath, char flag, int huffbook)
{
	DIR *dirOp = opendir(dirpath);
	struct dirent *d;

	if (dir(dirpath))
	{ //path passed is a directory

		if (!dirOp)
		{
			printf("ERROR: CANNOT OPEN THE DIRECTORY\n");
			return;
		}
		else
		{ //directory succesfully opened
			while (d = readdir(dirOp))
			{

				int dirname = strlen(dirpath);
				int current = strlen(d->d_name);
				char *currentdir = (char *)malloc(sizeof(current));

				if (currentdir == NULL)
				{
					printf("ERROR: FAILED TO GO THROUGH DIRECTORY");
					return;
				}

				currentdir = d->d_name;

				char *p = (char *)malloc(sizeof(char) * dirname + current + 2);
				if (p == NULL)
				{
					printf("ERROR: FAILED TO GO TRHOUGH DIRECTORY");
					return;
				}
				printf("%s\n", p);
				memcpy(p, dirpath, dirname);
				memcpy(p + dirname, "/", 1);
				memcpy(p + dirname + 1, currentdir, current + 1);

				if (strncmp(d->d_name, ".", sizeof(char)) == 0)
				{
					continue;
				}
				else if (d->d_type == DT_DIR)
				{
					listdir(dirpath, flag, huffbook);
				}
				else if (d->d_type == DT_REG)
				{
					if (flag == 'b')
					{
						int fd = open(p, O_RDONLY);
						struct stat fpStat;
						int end;
						if (stat(p, &fpStat) == -1)
						{
							printf("ERROR: CANNOT OPEN FILE\n");
						}
						else
						{
							end = fpStat.st_size;
						}

						/*build codebook*/
						tokenize(fd, end - 1);
						/*huffmanTree = huffman();
						int array[1000];
						char* escpChar;
						escpChar = "\n";
						writeCodebook(huffmanTree, array, 0, huffbook);
						write(huffbook, escpChar, strlen(escpChar));*/
						//end file
					}
					else if (flag == 'c')
					{
						//compress
						//start to compress
						fd = open(p, O_RDONLY);

						if (fd < 0)
						{
							perror("r1");
							exit(1);
						}
						struct stat fpStat;
						int end;

						if (stat(p, &fpStat) == -1)
						{
							printf("Error\n");
						}
						else
						{
							end = fpStat.st_size;
							// printf("file size: %lld bytes\n", (long long)end);
						}

						//File to Write
						char *fileToWrite = (char *)malloc(sizeof(p) + 5);
						strcpy(fileToWrite, p);
						fileToWrite[strlen(p)] = '.';
						fileToWrite[strlen(p) + 1] = 'h';
						fileToWrite[strlen(p) + 2] = 'c';
						fileToWrite[strlen(p) + 3] = 'z';
						// printf("newfile %s\n", fileToWrite);
						int fcreate = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

						int bytesRead = 0;
						char *tempToke = "";
						char *code;
						char *word;
						do
						{
							sz = read(fd, &d, 1);
							// printf("byte %c\n", d);
							// printf("bytes read %d\n", bytesRead);
							if (bytesRead == end)
							{
								if (strcmp(tempToke, "") != 0)
								{
									code = findCode(head, tempToke);
									if (code == NULL)
									{
										printf("no code found\n");
										printf("huffmancodebook incorrect\n");
										close(fd);
										close(fcreate);
										exit(1);
									}
									else
									{
										write(fcreate, code, strlen(code));
										// printf("Code for token %s is %s\n", token, code);
									}
								}
							}
							bytesRead++;
							// if(bytesRead == 1){
							// 	continue;
							// }
							if (isspace(d))
							{
								if (strcmp(tempToke, "") != 0)
								{
									code = findCode(head, tempToke);
									if (code == NULL)
									{
										printf("no code found\n");
										printf("huffmancodebook incorrect\n");
										close(fd);
										close(fcreate);
										exit(1);
									}
									else
									{
										write(fcreate, code, strlen(code));
										// printf("Code for token %s is %s\n", token, code);
									}
								}

								tempToke = "";

								if (d == 32)
								{
									tempToke = concat(tempToke, '\\');
									tempToke = concat(tempToke, ' ');
								}
								else if (d == '\t')
								{ //buffer is a tab
									//printf("tab\n");
									tempToke = concat(tempToke, '\\');
									tempToke = concat(tempToke, 't');
								}
								else if (d == '\n')
								{ //buffer is a new line
									//printf("new line\n");
									tempToke = concat(tempToke, '\\');
									tempToke = concat(tempToke, 'n');
								}
								else if (d == '\v')
								{ //vertical tab
									tempToke = concat(tempToke, '\\');
									tempToke = concat(tempToke, 'v');
								}
								else if (d == '\f')
								{ //feed
									tempToke = concat(tempToke, '\\');
									tempToke = concat(tempToke, 'f');
								}
								else if (d == '\r')
								{ //
									tempToke = concat(tempToke, '\\');
									tempToke = concat(tempToke, 'r');
								}
								code = findCode(head, tempToke);
								if (code == NULL)
								{
									printf("no code found\n");
									printf("huffmancodebook incorrect\n");
									close(fd);
									close(fcreate);
									exit(1);
								}
								else
								{
									write(fcreate, code, strlen(code));
									// printf("Code for token %s is %s\n", token, code);
								}
								tempToke = "";
							}
							else
							{
								tempToke = concat(tempToke, d);
							}

						} while (sz > 0);

						free(fileToWrite);
						close(fcreate);
						close(fd);
					}
					else if (flag == 'd')
					{
						//decompress
						//Try to decompress
			char *fileToWrite = (char *)malloc(strlen(p));
			// fileToWrite =  (char *)malloc(strlen(command[i - 2]));
			strncpy(fileToWrite, p, strlen(p) - 4);
			// fileToWrite = "decompress.txt";

			printf("%s\n", fileToWrite);

			int fdecompress = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
			fd = open(p, O_RDONLY);

			if (fd < 0)
			{
				perror("r1");
				exit(1);
			}
			c = NULL;
			c = (char *)calloc(1, sizeof(char));

			sz = read(fd, c, 1);
			char *code_read = (char *)calloc(sizeof(char) * 1, sizeof(char));
			char *totalcode = (char *)malloc(sizeof(char) * 1);
			while (sz > 0)
			{
				free(totalcode);
				totalcode = NULL;
				totalcode = (char *)malloc(strlen(code_read) + strlen(c));
				if (totalcode != NULL && c != NULL)
				{
					// totalcode = (char *)malloc(strlen(code_read) + strlen(c) + 1);
					// totalcode[0] = '\0';
					// printf("code_Read %s, c %s\n", code_read, c);
					strcat(totalcode, code_read);
					// printf("do i get here?\n");
					strcat(totalcode, c);
				}
				else
				{
					printf("couldn't malloc\n");
					exit(1);
				}
				free(code_read);
				code_read = NULL;
				code_read = (char *)malloc(sizeof(totalcode));
				strcpy(code_read, totalcode);
				char *word = compareCode(code_read, head);
				if (word != NULL)
				{
					free(code_read);
					code_read = NULL;
					code_read = (char *)malloc(sizeof(char) * 1);
					printf("word %s\n", word);
					if (strcmp(word, "\\ ") == 0)
					{
						write(fdecompress, " ", 1);
					}
					else if (strcmp(word, "\\n") == 0)
					{
						write(fdecompress, "\n", 1);
					}
					else if (strcmp(word, "\\t") == 0)
					{
						write(fdecompress, "\t", 1);
					}
					else
					{
						write(fdecompress, word, strlen(word));
					}
					// write(fdecompress, " ", 1);
					// free(word);
					// word = NULL;
					// write(fdecompress, " ", strlen(" "));
				}
				// printf("this is what I have so far: %s\n", totalcode);

				sz = read(fd, c, 1);
			}
			close(fdecompress);
					}
				}
				else if (d->d_type == DT_UNKNOWN)
				{
					printf("ERROR: UNKNOWN DIRECTORY\n");
					return;
				}
			}

			closedir(dirOp);
		}
	}
	else if (file(dirpath))
	{ //path passed is a file
		printf("WARNING: RECURSE FLAG SET OFF WITH FILE\n");

		if (flag == 'b')
		{
			int fd = open(dirpath, O_RDONLY);
			struct stat fpStat;
			int end;
			if (stat(dirpath, &fpStat) == -1)
			{
				printf("ERROR: CANNOT OPEN FILE\n");
			}
			else
			{
				end = fpStat.st_size;
			}

			/*build codebook*/
			tokenize(fd, end - 1);
			/*huffmanTree = huffman();
			int array[1000];
			char* escpChar;
			escpChar = "\n";
			writeCodebook(huffmanTree, array, 0, huffbook);
			write(huffbook, escpChar, strlen(escpChar)); //end file*/
		}
		else if (flag == 'c')
		{
			//start to compress
			fd = open(dirpath, O_RDONLY);

			if (fd < 0)
			{
				perror("r1");
				exit(1);
			}
			struct stat fpStat;
			int end;

			if (stat(dirpath, &fpStat) == -1)
			{
				printf("Error\n");
			}
			else
			{
				end = fpStat.st_size;
				// printf("file size: %lld bytes\n", (long long)end);
			}

			//File to Write
			char *fileToWrite = (char *)malloc(sizeof(dirpath) + 5);
			strcpy(fileToWrite, dirpath);
			fileToWrite[strlen(dirpath)] = '.';
			fileToWrite[strlen(dirpath) + 1] = 'h';
			fileToWrite[strlen(dirpath) + 2] = 'c';
			fileToWrite[strlen(dirpath) + 3] = 'z';
			// printf("newfile %s\n", fileToWrite);
			int fcreate = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

			int bytesRead = 0;
			char *tempToke = "";
			char *code;
			char *word;
			do
			{
				sz = read(fd, &d, 1);
				// printf("byte %c\n", d);
				// printf("bytes read %d\n", bytesRead);
				if (bytesRead == end)
				{
					if (strcmp(tempToke, "") != 0)
					{
						code = findCode(head, tempToke);
						if (code == NULL)
						{
							printf("no code found\n");
							printf("huffmancodebook incorrect\n");
							close(fd);
							close(fcreate);
							exit(1);
						}
						else
						{
							write(fcreate, code, strlen(code));
							// printf("Code for token %s is %s\n", token, code);
						}
					}
				}
				bytesRead++;
				// if(bytesRead == 1){
				// 	continue;
				// }
				if (isspace(d))
				{
					if (strcmp(tempToke, "") != 0)
					{
						code = findCode(head, tempToke);
						if (code == NULL)
						{
							printf("no code found\n");
							printf("huffmancodebook incorrect\n");
							close(fd);
							close(fcreate);
							exit(1);
						}
						else
						{
							write(fcreate, code, strlen(code));
							// printf("Code for token %s is %s\n", token, code);
						}
					}

					tempToke = "";

					if (d == 32)
					{
						tempToke = concat(tempToke, '\\');
						tempToke = concat(tempToke, ' ');
					}
					else if (d == '\t')
					{ //buffer is a tab
						//printf("tab\n");
						tempToke = concat(tempToke, '\\');
						tempToke = concat(tempToke, 't');
					}
					else if (d == '\n')
					{ //buffer is a new line
						//printf("new line\n");
						tempToke = concat(tempToke, '\\');
						tempToke = concat(tempToke, 'n');
					}
					else if (d == '\v')
					{ //vertical tab
						tempToke = concat(tempToke, '\\');
						tempToke = concat(tempToke, 'v');
					}
					else if (d == '\f')
					{ //feed
						tempToke = concat(tempToke, '\\');
						tempToke = concat(tempToke, 'f');
					}
					else if (d == '\r')
					{ //
						tempToke = concat(tempToke, '\\');
						tempToke = concat(tempToke, 'r');
					}
					code = findCode(head, tempToke);
					if (code == NULL)
					{
						printf("no code found\n");
						printf("huffmancodebook incorrect\n");
						close(fd);
						close(fcreate);
						exit(1);
					}
					else
					{
						write(fcreate, code, strlen(code));
						// printf("Code for token %s is %s\n", token, code);
					}
					tempToke = "";
				}
				else
				{
					tempToke = concat(tempToke, d);
				}

			} while (sz > 0);

			free(fileToWrite);
			close(fcreate);
			close(fd);
		}
		else if (flag == 'd')
		{
			//Try to decompress
			char *fileToWrite = (char *)malloc(strlen(dirpath));
			// fileToWrite =  (char *)malloc(strlen(command[i - 2]));
			strncpy(fileToWrite, dirpath, strlen(dirpath) - 4);
			// fileToWrite = "decompress.txt";

			printf("%s\n", fileToWrite);

			int fdecompress = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
			fd = open(dirpath, O_RDONLY);

			if (fd < 0)
			{
				perror("r1");
				exit(1);
			}
			c = NULL;
			c = (char *)calloc(1, sizeof(char));

			sz = read(fd, c, 1);
			char *code_read = (char *)calloc(sizeof(char) * 1, sizeof(char));
			char *totalcode = (char *)malloc(sizeof(char) * 1);
			while (sz > 0)
			{
				free(totalcode);
				totalcode = NULL;
				totalcode = (char *)malloc(strlen(code_read) + strlen(c));
				if (totalcode != NULL && c != NULL)
				{
					// totalcode = (char *)malloc(strlen(code_read) + strlen(c) + 1);
					// totalcode[0] = '\0';
					// printf("code_Read %s, c %s\n", code_read, c);
					strcat(totalcode, code_read);
					// printf("do i get here?\n");
					strcat(totalcode, c);
				}
				else
				{
					printf("couldn't malloc\n");
					exit(1);
				}
				free(code_read);
				code_read = NULL;
				code_read = (char *)malloc(sizeof(totalcode));
				strcpy(code_read, totalcode);
				char *word = compareCode(code_read, head);
				if (word != NULL)
				{
					free(code_read);
					code_read = NULL;
					code_read = (char *)malloc(sizeof(char) * 1);
					printf("word %s\n", word);
					if (strcmp(word, "\\ ") == 0)
					{
						write(fdecompress, " ", 1);
					}
					else if (strcmp(word, "\\n") == 0)
					{
						write(fdecompress, "\n", 1);
					}
					else if (strcmp(word, "\\t") == 0)
					{
						write(fdecompress, "\t", 1);
					}
					else
					{
						write(fdecompress, word, strlen(word));
					}
					// write(fdecompress, " ", 1);
					// free(word);
					// word = NULL;
					// write(fdecompress, " ", strlen(" "));
				}
				// printf("this is what I have so far: %s\n", totalcode);

				sz = read(fd, c, 1);
			}
			close(fdecompress);
		}
	}
}

//Method to check if this is a directory
int dir(const char *dir)
{
	DIR *directory = opendir(dir);
	if (directory)
	{
		closedir(directory);
		return 1;
	}
	else
	{
		return 0;
	}
}

//Method to check if this is a file
int file(const char *to_read)
{
	struct stat fd;
	if (stat(to_read, &fd) == 0)
	{
		if (fd.st_mode & S_IFDIR)
		{
			return 0;
		}
		else if (fd.st_mode & S_IFREG)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

/*
*/

int main(int argc, char **argv)
{

	/* Flag commands:
	 * -b = build codebook
	 * -c = compress
	 * -d = decompress
	 * -R = recursive
	 * when there is just one file --> can have one or two flags
	 * where there is a directory --> two flags, one HAS TO BE -R
	*/

	numNode = 0;
	int fd; //file desciptor

	int i; //used to iterate through argv
	i = 1;
	int j; //used for command
	j = 0;
	int f; //used for flags
	f = 0;
	char flags[2];
	flags[0] = '0';
	flags[1] = '0';
	char *command[2]; //temp var for command
	command[0] = "0";
	command[1] = "0";
	int compress = 0;
	int decompress = 0;
	int build = 0;
	int recur = 0;

	for (; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			flags[f] = argv[i][1];
			f++;
		}
		else
		{
			command[j] = argv[i];
			j++;
		}
	}

	f = 0;
	for (; f < 2; f++)
	{
		printf("flag: %c\n", flags[f]);
	}
	/*
	j = 0;
	for(; j < 2; j++){
		printf("command: %s\n", command[j]);
	}*/

	//initalize codebook
	char *codebook = (char *)malloc(sizeof(char) * 11);
	int fileCreate;
	char *escpChar;
	//No codebook
	if (command[1] == "0")
	{

		codebook = strcpy(codebook, "HuffmanCodebook");
		fileCreate = open(codebook, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		escpChar = "\\";
		write(fileCreate, escpChar, strlen(escpChar)); //escape char
		escpChar = "\n";
		write(fileCreate, escpChar, strlen(escpChar)); //new line
	}
	else
	{ //codebook has been inserted
		free(codebook);
		fileCreate = open(command[1], O_WRONLY, S_IRUSR | S_IWUSR);
	}

	if (flags[0] == 'R' || flags[1] == 'R')
	{

		//Need to recurse
		if (flags[0] == 'b' || flags[1] == 'b')
		{
			listdir(command[0], 'b', fileCreate);
			printf("out here\n");
			huffmanTree = huffman();
			int array[1000];
			char *escpChar;
			escpChar = "\n";
			writeCodebook(huffmanTree, array, 0, fileCreate);
			write(fileCreate, escpChar, strlen(escpChar));
		}
		else if (flags[0] == 'c' || flags[1] == 'c')
		{
			//check to see if HuffmanCodeBook exists
			if (strcmp(command[1], "./HuffmanCodebook") != 0)
			{
				puts("./HuffmanCodebook does not exist");
				exit(1);
			}
			fd = open(command[1], O_RDONLY); // read in huffman code
			if (fd < 0)
			{
				perror("r1");
				exit(1);
			}

			/*
        CREATE THE ABILITY TO COMPRESS
    	*/
			struct stat fpStat;
			int end;
			if (stat(command[1], &fpStat) == -1)
			{
				printf("Error\n");
			}
			else
			{
				end = fpStat.st_size;
				// printf("file size: %lld bytes\n", (long long)end);
			}

			if (head != NULL)
			{
				freeLL(head);
			}
			sz = 0;
			char *tempToke;
			tempToke = "";
			int bytesRead = 0;
			char *code_word[2];
			int cread = 0;
			int wread = 0;
			char d;
			do
			{
				sz = read(fd, &d, 1);
				if (bytesRead == end)
				{
					if (strcmp(tempToke, "") != 0)
					{
						if (strcmp(code_word[0], "") == 0)
						{
							continue;
						}
						else
						{
							code_word[1] = (char *)malloc(sizeof(tempToke));
							strcpy(code_word[1], tempToke);
							head = insertEndLL(head, code_word[0], code_word[1]);
							break;
						}
					}
				}
				++bytesRead;
				if (bytesRead == 1)
				{
					continue;
				}
				if (isspace(d))
				{
					if (strcmp(tempToke, "") != 0)
					{
						if (cread == 0)
						{
							if (strcmp(tempToke, "\\") == 0)
							{
								tempToke = "";
								continue;
							}
							code_word[0] = (char *)malloc(sizeof(tempToke));
							strcpy(code_word[0], tempToke);
							++cread;
							tempToke = "";
						}
						else if (wread == 0)
						{
							if (strcmp(tempToke, "\\") == 0)
							{
								tempToke = concat(tempToke, ' ');
							}
							code_word[1] = (char *)malloc(sizeof(tempToke));
							strcpy(code_word[1], tempToke);
							tempToke = "";
							++wread;
						}
						if (cread == 1 && wread == 1)
						{
							head = insertEndLL(head, code_word[0], code_word[1]);
							cread = 0;
							wread = 0;
							free(code_word[0]);
							free(code_word[1]);
						}
					}
				}
				else
				{
					tempToke = concat(tempToke, d);
				}
			} while (sz > 0);
			close(fd);
			printLL(head);

			listdir(command[0], 'c', fileCreate);
		}
		else if (flags[0] == 'd' || flags[1] == 'd')
		{
			//check to see if HuffmanCodeBook exists
			if (strcmp(command[1], "./HuffmanCodebook") != 0)
			{
				puts("./HuffmanCodebook does not exist");
				exit(1);
			}
			fd = open(command[1], O_RDONLY); // read in huffman code
			if (fd < 0)
			{
				perror("r1");
				exit(1);
			}

			/*
        CREATE THE ABILITY TO COMPRESS
    	*/
			struct stat fpStat;
			int end;
			if (stat(command[1], &fpStat) == -1)
			{
				printf("Error\n");
			}
			else
			{
				end = fpStat.st_size;
				// printf("file size: %lld bytes\n", (long long)end);
			}

			if (head != NULL)
			{
				freeLL(head);
			}
			sz = 0;
			char *tempToke;
			tempToke = "";
			int bytesRead = 0;
			char *code_word[2];
			int cread = 0;
			int wread = 0;
			char d;
			do
			{
				sz = read(fd, &d, 1);
				if (bytesRead == end)
				{
					if (strcmp(tempToke, "") != 0)
					{
						if (strcmp(code_word[0], "") == 0)
						{
							continue;
						}
						else
						{
							code_word[1] = (char *)malloc(sizeof(tempToke));
							strcpy(code_word[1], tempToke);
							head = insertEndLL(head, code_word[0], code_word[1]);
							break;
						}
					}
				}
				++bytesRead;
				if (bytesRead == 1)
				{
					continue;
				}
				if (isspace(d))
				{
					if (strcmp(tempToke, "") != 0)
					{
						if (cread == 0)
						{
							if (strcmp(tempToke, "\\") == 0)
							{
								tempToke = "";
								continue;
							}
							code_word[0] = (char *)malloc(sizeof(tempToke));
							strcpy(code_word[0], tempToke);
							++cread;
							tempToke = "";
						}
						else if (wread == 0)
						{
							if (strcmp(tempToke, "\\") == 0)
							{
								tempToke = concat(tempToke, ' ');
							}
							code_word[1] = (char *)malloc(sizeof(tempToke));
							strcpy(code_word[1], tempToke);
							tempToke = "";
							++wread;
						}
						if (cread == 1 && wread == 1)
						{
							head = insertEndLL(head, code_word[0], code_word[1]);
							cread = 0;
							wread = 0;
							free(code_word[0]);
							free(code_word[1]);
						}
					}
				}
				else
				{
					tempToke = concat(tempToke, d);
				}
			} while (sz > 0);
			close(fd);
			printLL(head);

			listdir(command[0], 'd', fileCreate);
		}
		else
		{
			printf("ERROR: INVALID FLAG\n");
		}
	}
	else if (flags[0] == 'b')
	{
		fd = open(command[0], O_RDONLY); //file descriptor

		if (fd == -1)
		{
			printf("ERROR: FAILED TO READ FILE\n");
		}

		struct stat fpStat;
		int end;
		if (stat(command[i - 1], &fpStat) == -1)
		{
			printf("Error\n");
		}
		else
		{
			end = fpStat.st_size;
		}

		/*build codebook*/
		tokenize(fd, end - 1);
		huffmanTree = huffman();
		int array[1000];
		writeCodebook(huffmanTree, array, 0, fileCreate);
		write(fileCreate, escpChar, strlen(escpChar)); //end file
	}
	else if (flags[0] == 'c')
	{
		//check to see if HuffmanCodeBook exists
		if (strcmp(command[1], "./HuffmanCodebook") != 0)
		{
			puts("Huffman Code Book doesn't exist");
			exit(1);
		}
		fd = open(command[1], O_RDONLY); // read in huffman code
		if (fd < 0)
		{
			perror("r1");
			exit(1);
		}

		/*
        CREATE THE ABILITY TO COMPRESS
    	*/
		struct stat fpStat;
		int end;
		if (stat(command[1], &fpStat) == -1)
		{
			printf("Error\n");
		}
		else
		{
			end = fpStat.st_size;
			// printf("file size: %lld bytes\n", (long long)end);
		}

		if (head != NULL)
		{
			freeLL(head);
		}
		sz = 0;
		char *tempToke;
		tempToke = "";
		int bytesRead = 0;
		char *code_word[2];
		int cread = 0;
		int wread = 0;
		char d;
		do
		{
			sz = read(fd, &d, 1);
			if (bytesRead == end)
			{
				if (strcmp(tempToke, "") != 0)
				{
					if (strcmp(code_word[0], "") == 0)
					{
						continue;
					}
					else
					{
						code_word[1] = (char *)malloc(sizeof(tempToke));
						strcpy(code_word[1], tempToke);
						head = insertEndLL(head, code_word[0], code_word[1]);
						break;
					}
				}
			}
			++bytesRead;
			if (bytesRead == 1)
			{
				continue;
			}
			if (isspace(d))
			{
				if (strcmp(tempToke, "") != 0)
				{
					if (cread == 0)
					{
						if (strcmp(tempToke, "\\") == 0)
						{
							tempToke = "";
							continue;
						}
						code_word[0] = (char *)malloc(sizeof(tempToke));
						strcpy(code_word[0], tempToke);
						++cread;
						tempToke = "";
					}
					else if (wread == 0)
					{
						if (strcmp(tempToke, "\\") == 0)
						{
							tempToke = concat(tempToke, ' ');
						}
						code_word[1] = (char *)malloc(sizeof(tempToke));
						strcpy(code_word[1], tempToke);
						tempToke = "";
						++wread;
					}
					if (cread == 1 && wread == 1)
					{
						head = insertEndLL(head, code_word[0], code_word[1]);
						cread = 0;
						wread = 0;
						free(code_word[0]);
						free(code_word[1]);
					}
				}
			}
			else
			{
				tempToke = concat(tempToke, d);
			}
		} while (sz > 0);
		close(fd);
		printLL(head);

		// exit(1);

		//start to compress
		fd = open(command[0], O_RDONLY);

		if (fd < 0)
		{
			perror("r1");
			exit(1);
		}

		if (stat(command[0], &fpStat) == -1)
		{
			printf("Error\n");
		}
		else
		{
			end = fpStat.st_size;
			// printf("file size: %lld bytes\n", (long long)end);
		}

		//File to Write
		char *fileToWrite = (char *)malloc(sizeof(command[0]) + 5);
		strcpy(fileToWrite, command[0]);
		fileToWrite[strlen(command[0])] = '.';
		fileToWrite[strlen(command[0]) + 1] = 'h';
		fileToWrite[strlen(command[0]) + 2] = 'c';
		fileToWrite[strlen(command[0]) + 3] = 'z';
		// printf("newfile %s\n", fileToWrite);
		int fcreate = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

		bytesRead = 0;
		tempToke = "";
		char *code;
		char *word;
		do
		{
			sz = read(fd, &d, 1);
			// printf("byte %c\n", d);
			// printf("bytes read %d\n", bytesRead);
			if (bytesRead == end)
			{
				if (strcmp(tempToke, "") != 0)
				{
					code = findCode(head, tempToke);
					if (code == NULL)
					{
						printf("no code found\n");
						printf("huffmancodebook incorrect\n");
						close(fd);
						close(fcreate);
						exit(1);
					}
					else
					{
						write(fcreate, code, strlen(code));
						// printf("Code for token %s is %s\n", token, code);
					}
				}
			}
			bytesRead++;
			// if(bytesRead == 1){
			// 	continue;
			// }
			if (isspace(d))
			{
				if (strcmp(tempToke, "") != 0)
				{
					code = findCode(head, tempToke);
					if (code == NULL)
					{
						printf("no code found\n");
						printf("huffmancodebook incorrect\n");
						close(fd);
						close(fcreate);
						exit(1);
					}
					else
					{
						write(fcreate, code, strlen(code));
						// printf("Code for token %s is %s\n", token, code);
					}
				}

				tempToke = "";

				if (d == 32)
				{
					tempToke = concat(tempToke, '\\');
					tempToke = concat(tempToke, ' ');
				}
				else if (d == '\t')
				{ //buffer is a tab
					//printf("tab\n");
					tempToke = concat(tempToke, '\\');
					tempToke = concat(tempToke, 't');
				}
				else if (d == '\n')
				{ //buffer is a new line
					//printf("new line\n");
					tempToke = concat(tempToke, '\\');
					tempToke = concat(tempToke, 'n');
				}
				else if (d == '\v')
				{ //vertical tab
					tempToke = concat(tempToke, '\\');
					tempToke = concat(tempToke, 'v');
				}
				else if (d == '\f')
				{ //feed
					tempToke = concat(tempToke, '\\');
					tempToke = concat(tempToke, 'f');
				}
				else if (d == '\r')
				{ //
					tempToke = concat(tempToke, '\\');
					tempToke = concat(tempToke, 'r');
				}
				code = findCode(head, tempToke);
				if (code == NULL)
				{
					printf("no code found\n");
					printf("huffmancodebook incorrect\n");
					close(fd);
					close(fcreate);
					exit(1);
				}
				else
				{
					write(fcreate, code, strlen(code));
					// printf("Code for token %s is %s\n", token, code);
				}
				tempToke = "";
			}
			else
			{
				tempToke = concat(tempToke, d);
			}

		} while (sz > 0);

		free(fileToWrite);
		close(fcreate);
		close(fd);
	}
	else if (flags[0] == 'd')
	{
		//check to see if HuffmanCodeBook exists
		if (strcmp(command[1], "./HuffmanCodebook") != 0)
		{
			puts("./HuffmanCodebook does not exist");
			exit(1);
		}
		fd = open(command[1], O_RDONLY); // read in huffman code
		if (fd < 0)
		{
			perror("r1");
			exit(1);
		}

		/*
        CREATE THE ABILITY TO COMPRESS
    	*/
		struct stat fpStat;
		int end;
		if (stat(command[1], &fpStat) == -1)
		{
			printf("Error\n");
		}
		else
		{
			end = fpStat.st_size;
			// printf("file size: %lld bytes\n", (long long)end);
		}

		if (head != NULL)
		{
			freeLL(head);
		}
		sz = 0;
		char *tempToke;
		tempToke = "";
		int bytesRead = 0;
		char *code_word[2];
		int cread = 0;
		int wread = 0;
		char d;
		do
		{
			sz = read(fd, &d, 1);
			if (bytesRead == end)
			{
				if (strcmp(tempToke, "") != 0)
				{
					if (strcmp(code_word[0], "") == 0)
					{
						continue;
					}
					else
					{
						code_word[1] = (char *)malloc(sizeof(tempToke));
						strcpy(code_word[1], tempToke);
						head = insertEndLL(head, code_word[0], code_word[1]);
						break;
					}
				}
			}
			++bytesRead;
			if (bytesRead == 1)
			{
				continue;
			}
			if (isspace(d))
			{
				if (strcmp(tempToke, "") != 0)
				{
					if (cread == 0)
					{
						if (strcmp(tempToke, "\\") == 0)
						{
							tempToke = "";
							continue;
						}
						code_word[0] = (char *)malloc(sizeof(tempToke));
						strcpy(code_word[0], tempToke);
						++cread;
						tempToke = "";
					}
					else if (wread == 0)
					{
						if (strcmp(tempToke, "\\") == 0)
						{
							tempToke = concat(tempToke, ' ');
						}
						code_word[1] = (char *)malloc(sizeof(tempToke));
						strcpy(code_word[1], tempToke);
						tempToke = "";
						++wread;
					}
					if (cread == 1 && wread == 1)
					{
						head = insertEndLL(head, code_word[0], code_word[1]);
						cread = 0;
						wread = 0;
						free(code_word[0]);
						free(code_word[1]);
					}
				}
			}
			else
			{
				tempToke = concat(tempToke, d);
			}
		} while (sz > 0);
		close(fd);
		printLL(head);

		//Try to decompress
		char *fileToWrite = (char *)malloc(strlen(command[0]));
		// fileToWrite =  (char *)malloc(strlen(command[i - 2]));
		strncpy(fileToWrite, command[0], strlen(command[0]) - 4);
		// fileToWrite = "decompress.txt";

		printf("%s\n", fileToWrite);

		int fdecompress = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		fd = open(command[0], O_RDONLY);

		if (fd < 0)
		{
			perror("r1");
			exit(1);
		}
		c = NULL;
		c = (char *)calloc(1, sizeof(char));

		sz = read(fd, c, 1);
		char *code_read = (char *)calloc(sizeof(char) * 1, sizeof(char));
		char *totalcode = (char *)malloc(sizeof(char) * 1);
		while (sz > 0)
		{
			free(totalcode);
			totalcode = NULL;
			totalcode = (char *)malloc(strlen(code_read) + strlen(c));
			if (totalcode != NULL && c != NULL)
			{
				// totalcode = (char *)malloc(strlen(code_read) + strlen(c) + 1);
				// totalcode[0] = '\0';
				// printf("code_Read %s, c %s\n", code_read, c);
				strcat(totalcode, code_read);
				// printf("do i get here?\n");
				strcat(totalcode, c);
			}
			else
			{
				printf("couldn't malloc\n");
				exit(1);
			}
			// printf("this is the byte read in %s\n", c);
			free(code_read);
			code_read = NULL;
			code_read = (char *)malloc(sizeof(totalcode));
			strcpy(code_read, totalcode);
			char *word = compareCode(code_read, head);
			if (word != NULL)
			{

				free(code_read);
				code_read = NULL;
				code_read = (char *)malloc(sizeof(char) * 1);

				printf("word %s\n", word);
				if (strcmp(word, "\\ ") == 0)
				{
					write(fdecompress, " ", 1);
				}
				else if (strcmp(word, "\\n") == 0)
				{
					write(fdecompress, "\n", 1);
				}
				else if (strcmp(word, "\\t") == 0)
				{
					write(fdecompress, "\t", 1);
				}
				else
				{
					write(fdecompress, word, strlen(word));
				}
			}
			sz = read(fd, c, 1);
		}
		close(fdecompress);
	}

	return 1;
}

/*





*/

static node *createNode(char *code, char *rep)
{
	node *n1;
	n1 = (node *)malloc(sizeof(node));
	n1->code = (char *)malloc(strlen(code + 1));
	n1->rep = (char *)malloc(strlen(rep + 1));
	strcpy(n1->code, code);
	strcpy(n1->rep, rep);
	// n1->code[strlen(code)] = '\0';
	n1->next = NULL;
}

void freeLL(node *root)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		freeLL(root->next);
		free(root);
	}
}

static node *insertEndLL(node *head, char *code, char *rep)
{
	if (head == NULL)
	{
		head = createNode(code, rep);
		return head;
	}
	node *curr = head;
	for (; curr != NULL; curr = curr->next)
	{
		if (curr->next == NULL)
		{
			node *n = createNode(code, rep);
			curr->next = n;
			return head;
		}
	}
	return head;
}

void printLL(node *head)
{
	if (head == NULL)
	{
		return;
	}
	else
	{
		printf("Code: %s\t Word: %s\n", head->code, head->rep);
		printLL(head->next);
	}
}

char *findCode(node *root, char *token)
{
	if (root == NULL)
	{
		return NULL;
	}
	node *curr = root;
	for (; curr != NULL; curr = curr->next)
	{
		// printf("token %s, node's string %s\n", token, curr->rep);
		// printf("strcmp %d\n", strcmp(curr->rep, token));
		if (strcmp(curr->rep, token) == 0)
		{
			return curr->code;
		}
	}
}

char *compareCode(char *code_in, node *root)
{
	if (root == NULL)
	{
		return NULL;
	}
	node *curr = root;
	for (; curr != NULL; curr = curr->next)
	{
		// printf("code_in %s, currnode_code %s, cmp %d\n", code_in, curr->code, strcmp(code_in, curr->code));
		if (strcmp(code_in, curr->code) == 0)
		{
			return curr->rep;
		}
	}
}
