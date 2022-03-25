#include <stdio.h>
#include <sys/time.h>
#include "mymalloc.h"

double avg (double time[], int i){
	
	double tot = 0;
	
	while(i >= 0){
		tot = tot + time[i];
		i--;
	}
	
	tot = tot/100;
	
	return tot;

}

int main(void){

	struct timeval start;
	struct timeval end;
	double timeA[100], timeB[100], timeC[100], timeD[100], timeE[100], timeF[100];
	
	int v;
	int i = 0;
	int j = 0;
	int k = 0;
	
	//Case A -- Pass
	for(v = 0; v < 100; v++){ 
		gettimeofday(&start, NULL);
		
		char* A;
		for(i; i < 150; i++){
			A = malloc(sizeof(char) * 1);
			free(A);
		}
		
		gettimeofday(&end, NULL);
		timeA[v] = end.tv_usec - start.tv_usec;
	}
	
	
	//Case B -- Pass
	char* pArr[50];
	for(v = 0; v < 100; v++){ 
		gettimeofday(&start, NULL);
		i = 0;
		char* B;
		
		while(i < 50){
		
			if(j == 3){ //runs 3 times, so allocates 150 times
				break;
			}
			
			B = malloc(sizeof(char) * 1);
			pArr[i] = B;
			
			if(i == 49){
				j++;
				
				for(k = 0; k < 50; k++){
					free(pArr[k]);
				}
				i = -1;
			}
			i++;
		}
		
		gettimeofday(&end, NULL);
		timeB[v] = end.tv_usec - start.tv_usec;
	}
		
	
	//Case C -- Pass
	for(v = 0; v < 100; v++){ 
		gettimeofday(&start, NULL);
		i = 0;
		j = 0;
		char* pArr2[50];
		while(i < 50){
			int j = rand();
			
			//malloc when even
			if(j % 2 == 0){
				//printf("i is: %d\n", i);
				pArr2[i] = malloc(sizeof(char) * 1);
				i += 1;
			} else if(j % 2 != 0){ //free when odd
				if(i == 0){
					continue;
				}
				i--;
				free(pArr2[i]);
			} 
		}
		
		i = 0;
		//free pointers
		while(i < 50){
			free(pArr2[i]);
			i++;
		}
		
		gettimeofday(&end, NULL);
		timeC[v] = end.tv_usec - start.tv_usec;
	}
		
	
	//Case D -- Pass
	int x = 0;
	for(v = 0; v < 100; v++){
		gettimeofday(&start, NULL);
		i = 0;
		j = 0;
		k = 0;
		char* pArr3[50];
		while(i < 50){
		
			int j = (rand() % (64)) + 1;
			
			//malloc when even
			if(j % 2 == 0){
				k = (rand() % (64)) + 1; //rand bytes
				pArr3[i] = malloc(sizeof(char) * k);
				i += 1;
			} else if(j % 2 != 0){ //free when odd
				
				if(i == 0){
					continue;
				}
				i--;
				free(pArr3[i]);
			} 
		
		}
		
		i = 0;
		//free pointers
		while(i < 50){
			free(pArr3[i]);
			i++;
		}
		
		gettimeofday(&end, NULL);
		timeD[v] = end.tv_usec - start.tv_usec;
	}
	
	
	//Case E -- Pass
	for(v = 0; v < 100; v++){ 
		i = 0; 
		char* pArr4[5];
		gettimeofday(&start, NULL);
		while (i < 50){
			for (j = 0; j < 5; j++){
				pArr4[j] = malloc(sizeof(char) * (100 + rand()% 701));
			}
			for(j = 0; j < 5; j++){
				free(pArr4[j]);
			}
			i++;
		}
		
		gettimeofday(&end, NULL);
		timeE[v] = end.tv_usec - start.tv_usec;
	}
		
	
	//Case F -- Pass
	for(v = 0; v < 100; v++){ 
		gettimeofday(&start, NULL);
		i = 0;
		char* total;
		while (i < 100){
			total = malloc(sizeof(char) * 4000);
			free(total);
			i = i + 1;

		}
		gettimeofday(&end, NULL);
		timeF[v] = end.tv_usec - start.tv_usec;
	}
	
	
	double wA = avg(timeA, 99);
	printf("Average time for workload A: %lf\n", wA);
	
	double wB = avg(timeB, 99);
	printf("Average time for workload B: %lf\n", wB);
	
	double wC = avg(timeC, 99);
	printf("Average time for workload C: %lf\n", wC);
	
	double wD = avg(timeD, 99);
	printf("Average time for workload D: %lf\n", wD);
	
	double wE = avg(timeE, 99);
	printf("Average time for workload E: %lf\n", wE);
	
	double wF = avg(timeF, 99);
	printf("Average time for workload F: %lf\n", wF);
		
}
