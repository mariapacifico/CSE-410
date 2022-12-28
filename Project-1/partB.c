/**
@file: partB.c
@author: Maria Pacifico
**/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

        int matrix1[4][5];
        int matrix2_t[3][5]; // transposed
        int matrix3[4][3];
        int x;

        // INPUT NUMBERS USING STANDARD INPUT
        // MATRIX 1
        for (int i=0; i < 4; i++){
                for (int j=0; j < 5; j++){
                        x = scanf("%d",&matrix1[i][j]);
                        }
        }

        // MATRIX2
        for (int i=0; i < 5; i++){
                for (int j=0; j < 3; j++){
                        x = scanf("%d",&matrix2_t[j][i]);
                        }
        }

        // STDOUT
        // MATRIX1
        printf("Matrix1\n");
        for (int i=0; i < 4; i++){
                for (int j=0; j < 5; j++){
                        printf("%d  ",matrix1[i][j]);
                        }
                printf("\n");
        }
         
        // MATRIX2
        printf("Matrix2\n");
        for (int i=0; i < 5; i++){
                for (int j=0; j < 3; j++){
                        printf("%d  ",matrix2_t[j][i]);
                        }
                printf("\n");
        }

        printf("FINAL RESULT\n");

        // PIPES

        // DIVIDE THE PIPES INTO ROWS AND COLUMNS
        int pipes_col[4][3][2];
        int pipes_row[4][3][2];

        // ONE PIPE TO SEND BACK TO PARENT
        int final_pipe[2];

        for (int i = 0; i < 4; i++){
                for (int j = 0; j < 3; j++){
                        if (pipe(pipes_col[i][j]) == -1){
                                perror("pipe");
                                exit(EXIT_FAILURE);
                        }
                        if (pipe(pipes_row[i][j]) == -1){
                                perror("pipe");
                                exit(EXIT_FAILURE);
                        }
                }
        }

        if (pipe(final_pipe) == -1){
                perror("pipe");
                exit(EXIT_FAILURE);
        }

        int xpos, ypos, fpid;
        int num_array[3];
        int row_nums[5];
        int col_nums[5];
        
        // CREATE 12 PROCESSES
        for (int i = 0; i < 4; i++){
                for (int j = 0; j < 3; j++){
                        xpos = i;
                        ypos = j;
                        fpid = fork();
                        if (fpid == 0){
                                break;
                        }
                }
                if (fpid ==0){
                        break;
                }
        }
    
        // PARENT
        if (fpid != 0){
                // WRITE IN ALL OF THE ROWS
                for (int i = 0; i < 4; i++){
                        write(pipes_row[i][0][1], matrix1[i], sizeof(matrix1[i]));
                }
                    
                // WRITE IN ALL OF THE COLUMNS
                for (int i = 0; i < 3; i++){
                        write(pipes_col[0][i][1], matrix2_t[i], sizeof(matrix2_t[i]));
                }
                    
                // READ FROM PIPE 12 TIMES
                for (int i = 0; i < 12; i ++){
                        read(final_pipe[0], num_array, sizeof(num_array));
                            
                        // PUT INTO FINAL MATRIX
                        matrix3[num_array[0]][num_array[1]] = num_array[2];
                }
                
                // PRINT MATRIX 3
                for(int i = 0; i < 4; i++){
                        for(int j = 0; j < 3; j++){
                                printf("%d  ", matrix3[i][j]);
                        }
                        printf("\n");
                }
        }
        else{
                    // READ IN FROM ROW PIPE
                    read(pipes_row[xpos][ypos][0], row_nums, sizeof(row_nums));

                    // READ IN FROM COLUMN PIPE
                    read(pipes_col[xpos][ypos][0], col_nums, sizeof(col_nums));

                    // WRITE TO NEXT ROW PIPE
                    if (ypos < 2){
                            write(pipes_row[xpos][ypos+1][1], row_nums, sizeof(row_nums));
                    }
                    // WRITE TO NEXT COLUMN PIPE
                    if (xpos < 3){
                            write(pipes_col[xpos+1][ypos][1], col_nums, sizeof(col_nums));
                    }

                    // CALCULATE
                    num_array[0] = xpos;
                    num_array[1] = ypos;
                    num_array[2] = row_nums[0] * col_nums[0];
                    num_array[2] += row_nums[1] * col_nums[1];
                    num_array[2] += row_nums[2] * col_nums[2];
                    num_array[2] += row_nums[3] * col_nums[3];
                    num_array[2] += row_nums[4] * col_nums[4];

                    // WRITE TO FINAL PIPE
                    write(final_pipe[1], num_array, sizeof(num_array));
        }
}
          

