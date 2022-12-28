/***********************************************************************/
/* Sample Program for POSIX semaphore operations
/***********************************************************************/

/*  compile using  "gcc sample.c -lpthread -o sample" */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define  MAX  10
#define  UPPER  100000

struct idList {
	 pthread_t thID;
	struct idList *next;
};

long counter = 0;        /* variable to be incremented by each thread */
sem_t mysemaphore;	/* global semaphore */

/* This function specifies the body of each thread */

void *some_thread( void *param) 	/* routine should do counter += UPPER */
{                          		/* if the semaphores work! */
  int j , temp;
  int id = *(int *)param;

  free( param);
  for( j = 0; j < UPPER; j++ ) {

 //    sem_wait( &mysemaphore );  	/*  semaphore WAIT operation */
			// The wait systems call waits on mysemaphore until it becomes non-zero. 
			// Then, it atomically decreases mysemaphore.
	temp = 2 * counter;	// Start of the critical section
    	temp = temp +2;
	counter = temp / 2;				
 // 	sem_post( &mysemaphore );   	/*  semaphore SIGNAL operation */
  }
pthread_exit( NULL );
}

int main( int argc, char *argv[] )
{
  int i, n = 0;
  int *param;
  int thCrEr;

  /* Check for a command-line argument (a positive integer) */
  if( argc != 2 ) {
    fprintf( stderr, "Usage: %s num_threads\n", argv[0] );
    exit( 1 );
  }
  sscanf( argv[1], "%d", &n );
  if( n <= 0 || n >= MAX ) {
    fprintf( stderr, "Invalid thread count -- defaulting to 4\n" );
    n = 4;
  }
	pthread_t *threadID;		// The ID of the newly created thread
	
  /* Create and initialize the mutual exclusion semaphore */
  sem_init( &mysemaphore, 0, 1);   // the second parameter identifies whether the semaphore 
			// is shared between several processes. For Linux, it must be 0 

  struct idList *root;		// Points to a linked list of thread IDs
  struct idList *currNode, *temp;

  /* Create n identical threads */
  for( i = 0; i < n; i++ ) {
    param = (int *)malloc(sizeof(int)); 	/* each thread requires a */
    *param = i;					/* SEPARATE copy of its parameter */
     
    temp = (struct idList *)  malloc(sizeof(struct idList));   // Allocate some memory for a new node
    if (!i) { root = temp;	currNode = temp; }	// initialize the root of the linked list

	threadID =  (pthread_t *)malloc(sizeof(pthread_t));

    int thCrEr = pthread_create( threadID , NULL, some_thread,  param);

	if ( thCrEr ) 
		printf("Error in creating a new thread!!!");


   temp->thID = *threadID;
   temp->next = NULL;
   currNode->next = temp;      
   currNode = temp;

	printf("A new thread created with ID %d \n", *threadID);
  }
  
   currNode = root;
  /* Wait for all the threads to exit */
  

   for( i = 0; i < n; i++ ) {
    printf( "\n Joining thread: %d", currNode->thID );
    pthread_join( currNode->thID, NULL);
  
    currNode = currNode->next;

  }

  
  /* Display final contents of counter */
  printf( "\nFinal value of counter: %d", counter );
  printf( " (should be %d)\n", n*UPPER );
exit(0);

}
