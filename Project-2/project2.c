/**
@file: project2.c
@author: Maria Pacifico
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

struct idList {
    pthread_t thID;
    struct idList *next;
};

int index_num = 0;
int unordered[10];
sem_t mysemaphore;

// CMP EXCHANGE FUNCTION
void *cmp_exch(void *param){

    sem_wait(&mysemaphore);

    int curr_index = index_num;
    int temp;
    index_num += 2;

    if ((unordered[curr_index] > unordered[curr_index + 1])&& ((curr_index + 1 )< 10)){
            temp = unordered[curr_index];
            unordered[curr_index] = unordered[curr_index+1];
            unordered[curr_index + 1] = temp;
    }

    sem_post(&mysemaphore);

    pthread_exit(NULL);

}

int main(){

    int x;

    // INPUT NUMBERS USING STANDARD INPUT
    printf("Enter 10 numbers:\n");
    for (int i=0; i < 10; i++){
            x = scanf("%d", &unordered[i]);
    }

    // PRINT THE INTEGERS IN ORDER ENTER
    printf("Numbers entered:\n");
    for (int i=0; i < 10; i++){
            printf("%d ", unordered[i]);
    }
    printf("\n");

    // IMPLEMENTATION

    int *param;
    int thCrEr;
    pthread_t *threadID;

    // INITALIZE SEMAPHORE
    sem_init( &mysemaphore, 0, 1);

    struct idList *root;            // Points to a linked list of thread IDs
    struct idList *currNode, *temp;

    for (int i = 0; i < 10; i++){
        // EVEN PHASE
        if (i % 2 == 0){
            index_num = 1;

            // CREATING THREADS
            for (int j = 0; j < 5; j++){
                temp = (struct idList *)  malloc(sizeof(struct idList));

                if (!j) { root = temp;  currNode = temp; }
                threadID =  (pthread_t *)malloc(sizeof(pthread_t));

                int thCrEr = pthread_create( threadID , NULL, cmp_exch,  NULL);

                if ( thCrEr )
                    printf("Error in creating a new thread!!!");


                temp->thID = *threadID;
                temp->next = NULL;
                currNode->next = temp;
                currNode = temp;
            }
            currNode = root;

            // JOINING THE THREADS
            for (int j = 0; j < 4; j++){
                pthread_join( currNode->thID, NULL);

                currNode = currNode->next;
            }

        }
        // ODD PHASE
        else{
            index_num = 0;

            // CREATING THREADS
            for (int j = 0; j < 5; j++){
                temp = (struct idList *)  malloc(sizeof(struct idList));
                if (!j) { root = temp;  currNode = temp; }
                threadID =  (pthread_t *)malloc(sizeof(pthread_t));

                int thCrEr = pthread_create( threadID , NULL, cmp_exch,  NULL);

                if ( thCrEr )
                    printf("Error in creating a new thread!!!");


                temp->thID = *threadID;
                temp->next = NULL;
                currNode->next = temp;
                currNode = temp;
            }
            currNode = root;

            // JOINING THREADS
            for (int j = 0; j < 5; j++){
                pthread_join( currNode->thID, NULL);

                currNode = currNode->next;
            }
        }
    }

    // PRINT SORTED ARRAY
    printf("Sorted array:\n");
    for (int i=0; i < 10; i++){
        printf("%d ", unordered[i]);
    }
    printf("\n");

}

