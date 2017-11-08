/* Micah Tashima
 * Homework 2
 *
 * * * * * * * * * * * * * * * * * * * * * * * * 
 * To run:
 * 	compile with cc merge468b.c -lpthread 
 *	a.out
 *
 * Only works for N = 16, not sure why I can't
 * get it to work for N = 32.
 * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This is an implementation of merge sort assuming the
 * data is an array a[] of length N, and N is a power of 2.
 *
 * The main() program uses a function called merge() which
 * merges subarrays in sorted order.  Rewrite the program
 * so that it uses threads to do the merging of the
 * subarrays.  In particular, each call to merge() creates
 * a thread.  
 *
 * The merging is done in a for-loop.  Each
 * pass through the for-loop causes subarrays of the same size
 * to be merged together.  The mergings within a pass should 
 * be done in parallel using one thread per merge. However,
 * the main program must wait for all the threads to complete
 * before doing another pass of the for-loop.  Thus, mergings
 * of subarrays of size 1 can be done in parallel, then
 * mergings of subarrays of size 2 can be done in parallel and
 * so forth.
 *
 * Use the pthread API
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 16 
#define MAX_THREAD 50

typedef struct
{
	int i;
	int arraysize;
}Parameter;

/* Global variables -- be careful */
int a[N];     /* Array to sort */
int temp[N];  /* Temporary storage */

void genvalues(int n); /* Initializes array a[] */
void prnvalues(int n); /* Prints array a[] */
void *merge(void *context); /* Merges subarrays */

main() 
{

int arraysize;     /* Size of subarrays to merge */
int numarr;      /* Number of subarrays */
int newarrsize;  /* New subarray size */
int newnumarr;   /* New number of subarrays */
int i;
int thread_count;
Parameter A[MAX_THREAD];
pthread_t thread_index[MAX_THREAD];

genvalues(N);    /* Initialize a[] with random values */
printf("Initial values:\n");
prnvalues(N);    /* Display the values */

numarr = 2;
arraysize = 1;

while (numarr > 1) 
{
   thread_count = 0;
   arraysize= 2*arraysize;
   numarr = N/arraysize;
   	for (i=0; i<N; i+=arraysize) 
	{
      		A[thread_count].i = i;
      		A[thread_count].arraysize = arraysize;
   
      		if(pthread_create(&thread_index[thread_count], NULL, (void *)&merge, (void *)&A[thread_count]))
      		{
      			printf("Error with creating thread!\n"); 
              		return;
      		}
      		else
      		{
			printf("Thread %d created!\n", thread_count + 1);
			thread_count = thread_count + 1;
      		}			
	
	}
	for(i = 0; i < thread_count; i++)
	{
		printf("Waiting for thread %d\n", i + 1);
			
		if(pthread_join(thread_index[i], NULL))
		{
			printf("Error with joining thread!\n");
			return;
		}

	}
}
printf("\nOutput:\n");
prnvalues(N); /* Display the sorted values */

}

/*
 * Merges subarrays (a[first], ..., a[midpt-1])
 *    and (a[midpt],..., a[last-1]) into
 *    subarray temp[first],..., temp[last-1] in increasing order.
 *    Then it copies temp[first],..., temp[last-1] back into
 *    a[first],..., a[last-1].
 */
void *merge(void *context)
{
int leftptr;   /* Pointers used in array a[ ] */
int rightptr;
int midpoint;
int last;
int k;         /* pointer used in array temp[ ] */
int delay;

Parameter *A = context;
/* 
 * Do not delete the next three lines.  They cause the function to
 * delay by a amount that is proportional to the subarray it is merging
 */
if (A->arraysize > 0) delay = A->arraysize;
else delay = 1;
usleep(delay*250000);

midpoint = A->i + (A->arraysize/2);
last = A->i + A->arraysize;
leftptr = A->i;
rightptr = midpoint; 

/* Merge values in the two arrays of a[] into temp[] */
for(k=A->i; k<last; k++) {
   if (leftptr >= midpoint) temp[k] = a[rightptr++];
   else if (rightptr >= last) temp[k] = a[leftptr++];
   else if (a[leftptr] < a[rightptr]) temp[k] = a[leftptr++];
   else if (a[leftptr] >= a[rightptr]) temp[k] = a[rightptr++];
   else printf("There's a bug \n");
}

/* Copy temp[] back to a[] */
for(k=A->i; k<last; k++) {a[k] = temp[k];}

}



/*
 * Initializes array a[] with random values.
 */
void genvalues(int n)
{
int k;
int i;
int current; 

k = 2*n;
current = 0;
	for (i=0; i<n; i++) 
	{
   		current = (current*73 + 19)%k;
   		a[i] = current;
	}
}

/*
 * Prints the values in the array a[]
 */
void prnvalues(int n)
{
int i;
	for (i=0; i<n; i++) 
	{
   		printf(" %d ", a[i]);
   		if ((i+1)%10 == 0) 
		{
			printf("\n");
		}
	}
	printf("\n");

}
