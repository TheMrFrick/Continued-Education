#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
// #include <unistd.h>
// #include <sys/types.h>
#include <sys/stat.h>

typedef struct node
{
    char *code;
    char *rep;
    struct node *next;
} node;

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
        printf("token %s, node's string %s\n", token, curr->rep);
        printf("strcmp %d\n", strcmp(curr->rep, token));
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
//assume that arg1 will be the filename and arg2 will be the huffman codebook
//argv[0] is always ./<filename>
//argv[1 .. n] are the arguments further passed in
int main(int argc, char *argv[])
{
    node *head = NULL, *ptr;
    ptr = head;
    char ch;
    int fd, sz;
    char *c = (char *)calloc(4096, sizeof(char)); // 4KB

    fd = open(argv[1], O_RDONLY); // read in huffman code
    if (fd < 0)
    {
        perror("r1");
        exit(1);
    }

    /*
        CREATE THE ABILITY TO COMPRESS
    */
    char delimit[] = " \t\n";
    sz = read(fd, c, 4096); //4 KB read
    char *token;
    token = strtok(c, delimit);
    int toknum = 1;
    char *code;
    char *word;
    while (token != NULL)
    {
        if (toknum == 1)
        {
            token = strtok(NULL, delimit);
            ++toknum;
            continue;
        }
        if (toknum % 2 == 0)
        {
            //token is a number
            // printf("code: %s\n", token);
            code = (char *)malloc(strlen(token + 1));
            strcpy(code, token);
            // code[strlen(code)] = '\0';
            token = strtok(NULL, delimit);
            ++toknum;
        }
        if (toknum % 2 == 1)
        {
            //token is the word
            // printf("word: %s\n", token);
            word = (char *)malloc(strlen(token + 1));
            strcpy(word, token);
            // word[strlen(word)] = '\0';
            token = strtok(NULL, delimit);
            ++toknum;
        }
        head = insertEndLL(head, code, word);
    }
    // sz = read(fd, c, 4096);
    close(fd);
    printLL(head);

    //start to compress
    fd = open(argv[2], O_RDONLY);

    if (fd < 0)
    {
        perror("r1");
        exit(1);
    }

    free(c);

    c = NULL;
    c = (char *)calloc(4096, sizeof(char));

    sz = read(fd, c, 4096);
    printf("The stuff being read: %s\n", c);
    token = strtok(c, delimit);

    free(code);
    code = NULL;

    //File to Write
    char *fileToWrite = (char *)malloc(sizeof(argv[2]) + 5);
    strcpy(fileToWrite, argv[2]);
    fileToWrite[strlen(argv[2])] = '.';
    fileToWrite[strlen(argv[2]) + 1] = 'h';
    fileToWrite[strlen(argv[2]) + 2] = 'c';
    fileToWrite[strlen(argv[2]) + 3] = 'z';
    // printf("newfile %s\n", fileToWrite);
    int fcreate = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

    while (token != NULL)
    {
        printf("token %s\n", token);
        code = findCode(head, token);
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
            printf("Code for token %s is %s\n", token, code);
        }
        token = strtok(NULL, delimit);
    }
    close(fcreate);
    close(fd);

    exit(1);

    //Try to decompress
    free(fileToWrite);
    fileToWrite = NULL;
    fileToWrite = (char *)malloc(14);
    fileToWrite = "decompress.txt";

    int fdecompress = open(fileToWrite, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    fd = open(argv[3], O_RDONLY);

    if (fd < 0)
    {
        perror("r1");
        exit(1);
    }
    free(c);
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
            // puts("");
            // printf("The word is: %s\n", word);
            // printf("here is its code %s\n", code_read);

            free(code_read);
            code_read = NULL;
            code_read = (char *)malloc(sizeof(char) * 1);

            word[strlen(word) - 1] = ' ';
            // printf("%d", word[strlen(word) - 1]);

            write(fdecompress, word, strlen(word));
            // write(fdecompress, " ", strlen(" "));
        }
        // printf("this is what I have so far: %s\n", totalcode);

        sz = read(fd, c, 1);
    }
    close(fdecompress);
    return 0;
}
