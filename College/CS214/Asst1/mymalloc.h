/*
    Program written by: Kyle Frick & Vivian Trieu
*/

#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

// DEFINITIONS AND INCLUDES
#include<stdio.h>
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
#define true 1
#define false -1
#define memSize 4096

// TYPEDEFS
typedef int boolean;

//CREATION OF STATICS AND FINALS
static char arr[memSize];

//METHODS
void * mymalloc(size_t bytes, char* file, int line); // mymalloc() version of malloc()

void myfree(void *p, char* file, int line); // myfree() version of free()

boolean canHold(size_t bytes, int metadata); // determining if the arr can hold the new data being inputted. 
#endif
