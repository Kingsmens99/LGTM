#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wordsProcessorLL.h"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[]){
        FILE *fp;		//file pointer
	int cp[2];		//file descriptor for child pipe
        int i, y, x = 0;	//local variables to hold values
	
        pid_t *pid = (pid_t *) malloc((argc-1) * sizeof(pid_t));//pid_t array that holds all cpid

        if(argc < 2){
                printf("Not enough arguements!\n");
                exit(1);
        }

        if(pipe(cp) == -1){
                fprintf(stderr, "pipe failed");
                return 1;
        }

        for(i=0; i < argc-1; i++){
                fp = fopen(argv[i+1], "r");
                if(fp == NULL){ perror("read file error!\n"); exit(1);}
                
		if((pid[i] = fork()) == 0){
			close(cp[READ_END]);

                        List *list = countWordsWithLinkedList(fp); //First required function
                        list->filename = (char *) malloc(strlen(argv[i+1]) + 1); //dynamically allocates file name array
                        strcpy(list->filename, argv[i+1]);		    //puts filename in header linknode
                        //printf("Filename: %s\n", list->filename);
			int dumb = printWordsLinkedList(list); //Second required function
			y = list->size_tot;
			write(cp[WRITE_END], &y, sizeof(y));	//sends total amount of words in file to parent through pipe
				
			close(cp[WRITE_END]);
			exit(0);
                }
		else if(pid[i] == -1){ perror("Fork failed!\n"); exit(EXIT_FAILURE); }
		else{ fclose(fp); }
        }
        for(i=0; i < argc-1; i++){
		if (i == 0){ close(cp[WRITE_END]);}
		read(cp[READ_END], &y, sizeof(y)); //reads total amount of words in file from child
		x = x + y;
		wait();
        }
	free(pid);
	close(cp[READ_END]);
	printf("The total number of words from all files is %d.\n", x);	
        return 0;
}
