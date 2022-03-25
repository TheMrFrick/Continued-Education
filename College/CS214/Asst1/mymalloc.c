#include <stdio.h>
#include "mymalloc.h"

int const magicnum = 5223;

void * mymalloc(size_t bytes, char* file, int line){
    long int det = canHold(bytes, 4);
    if(det == -1){ //check to see if fails first,
        printf("Failed to find correct storage: %s %d", __FILE__, __LINE__);
    } else { // continue on as if it didn't fail
        /* LETS CREATE THE META DATA */
        char int1; // 1 because that will mean that it will be active, hence first byte of metadata stating inuse
        char int2;
        /* ENCRYPT THE METADATA INTO TWO BYTES */
        if(bytes >= 100){
            int1 = (bytes/100);
            int2 = bytes %100;
        } else {
            int1 = 0;
            int2 = bytes;
        }
        long int i = det;
        arr[i] = 52;
        arr[i + 1] = 23;
        arr[i + 2] = int1;
        arr[i + 3] = int2;
        i = i + 4;
        int j = 0;
        for(j; j < (bytes); j++){
            arr[i + j] = '-';
        }
    }
    if(det != -1){
        det = det + 4;
        void* final = (void*) det;
        return final;
    } else {
        return NULL;
    }
}

void myfree(void *p, char* file, int line){
	/* Cases:
	- good pointer
	- pointer out of range
	- pointer to middle of data
	*/
	
    long int addr = (long int)p; //address of the pointer
    
  
    if(addr < 0 || addr > memSize){
        //addr out of range
    	printf("Error, address out of range: %s %d\n", __FILE__, __LINE__);
    	
    } else if(arr[addr-4] == 52 && arr[addr-3] == 23){
    	//good pointer case
    	int i = addr;
    	
    	int stop = addr + (arr[addr-2]*100 + arr[addr-1]);
    	
    	//free the data!
    	for(i; i < stop; i++){
    		arr[i] = 0;
    	}
    	
    	//reset metadata
    	arr[addr-4] = 0;
    	arr[addr-3] = 0;
    	arr[addr-2] = 0;
    	arr[addr-1] = 0;
    } else if(arr[addr-4] != 52 && arr[addr-3] != 23){
		//the magic number has not been found
		//cases:
		//middle of allocated space
		//give pointer we did not allocate
		printf("Error, not a pointer: %s %d\n", __FILE__, __LINE__);
    }
    return;
}

boolean canHold(size_t bytes, int metadata){ // will return the index at which it is at.
    int  i = 0;
    long int j = bytes + metadata;
    for (i; i < memSize; i++){
        //test to see if the next number is greater than memSize without NULLPOINTEREXCEPT
        if((i+1) >= memSize){
            break;
        }
        //end of test
        // check to see if the arr[i] is 52 and if arr[i+1] = 23
        if(arr[i] == 52 && arr[i+1] == 23){
            //assume that arr[i] is the first digit
            int int1 = arr[i + 2]; // get first size bit
            int int2 = arr[i + 3]; // get second size bit
            long int p_bytes = (int1 * 100) + int2; // find the size
            //printf("size of bytes :%ld\ti before: %d\n", p_bytes, i);
            i = i + p_bytes + 3; // add that to i
            //printf("i after: %d\n", i);
            j = bytes + metadata; // set j to default
        } else {
            j--; 
            if(j == 0){
                return (i - (bytes + (metadata - 1)));
            }
        }
    }
    
    return false;
}

