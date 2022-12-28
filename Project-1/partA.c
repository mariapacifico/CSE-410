/*
 file: partA.c
 author: Maria Pacifico
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
        // Creates pipe 1
        int     num_array[2];
        int pip1[2], fpid1;

        if (pipe(pip1) == -1){
                perror("pipe");
                exit(EXIT_FAILURE);
        }

        // forks child 1
        fpid1 = fork();

        // child 1
        if (fpid1 == 0){
                // creates second pipe
                int pip2[2], fpid2;
                if (pipe(pip2) == -1)
                {
                        perror("pipe2");
                        exit(EXIT_FAILURE);
                }
                // forks child 2
                fpid2 = fork();

                // child 2
                if (fpid2 == 0){
                        // writes "child2" and pid
                        printf("child2 pid: %d\n", fpid2);

                        // close pipe1
                        close(pip1[0]);
                        close(pip1[1]);
                    
                        // read from pipe2
                        read(pip2[0], num_array, sizeof(num_array));
                        printf("child2 read %d, %d\n", num_array[0], num_array[1]);
                        exit(0);
                }

                // child 1 again
                else{
                        // writes "child1" and pid to stdout
                        printf("child1 pid: %d\n", fpid2);

                        // closes stdout
                        fclose(stdout);

                        // read from parent
                        read(pip1[0], num_array, sizeof(num_array));

                        // multiplies each value by 4
                        // write to second child
                        num_array[0] = num_array[0] * 4;
                        num_array[1] = num_array[1] * 4;
                        write(pip2[1], num_array, sizeof(num_array));
                }
        }
        // parent
        else{
                // closes stdout
                fclose(stdout);
                // reads two numbers from stdin
                int x = scanf("%d%d", &num_array[0], &num_array[1]);
                // writes two numbers to pipe p1
                write(pip1[1], num_array, sizeof(num_array));
        }
        return 0;
}
        
