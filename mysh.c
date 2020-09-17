#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 300
#define MAX_INPUTS 10

enum supportedCommands{movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7}; //, repeat =8};

        char** allocStrArray(int stringsize, int totalStrings)
        {
            char **array = malloc(totalStrings * sizeof(char *));
            int i;
            for (i = 0; i < totalStrings; ++i) 
                array[i] = (char *)malloc(stringsize+1);

            return array;
        }
        
        // honestly not a necessary function but it looks nicer alongside allocStrArray()   :)
        char* allocCharArray(int size)
        {
            char* temp = (char *)calloc(size, sizeof(char));
            return temp;
        }

        char** tokenizer(int *numTokens)
        {
            char* str = allocCharArray(MAX_CHAR);
            char* token = allocCharArray(MAX_CHAR);
            char* delim = " ";
            char** returnTokens = allocStrArray(MAX_INPUTS, MAX_CHAR);
            int count = 0;
            
            fgets(str, MAX_CHAR, stdin); // stdin = keyboard
            
            // prints at least 1 token, keeps going if more exist
            token = strtok(str, delim);
            while (token != NULL)
            {
                strcpy(returnTokens[count], token);
                printf("token: %s\n", token);
                token = strtok(NULL, delim); // all other tokens need to have "NULL" here instead of the original string (idk why)

                if (count == 10)
                {
                    printf("too many inputs!!\n");
                    exit(0);
                }
                count++;

            }
            *numTokens = count;

           
            free(str);free(token);

            return returnTokens;
        }

        int commandNumber (char* command)
        {
            /* movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7*/
            /* the tokenization sticks a newline into "command" so ie "byebye" needs one as well if it is going to evaluate to the same thing*/

            if (strcmp(command, "movetodir\n") == 0)
                return movetodir;
            else if (strcmp(command, "whereami\n") == 0)
                return whereami;
            else if (strcmp(command, "history\n") == 0)
                return history;
            else if (strcmp(command, "byebye\n") == 0)
                return byebye;
            else if (strcmp(command, "start\n") == 0)
                return start;
            else if (strcmp(command, "background\n") == 0)
                return background;
            else if (strcmp(command, "exterminate\n") == 0)
                return exterminate;
            else
                return 0;
        }
        int main()
        {
                int exit = 0;
                printf("welcome to fake baby shell\n");
                
                do{
                    char** tokenized = allocStrArray(MAX_INPUTS, MAX_CHAR);
                    int *numTokens = (int*) calloc(1, sizeof(int));
                    char* command = (char *)calloc(MAX_INPUTS, sizeof(char));

                    printf("# ");
                    tokenized = tokenizer(numTokens);
                    strcpy(command, tokenized[0]); // first token is always a command
                    /* movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7*/
                    switch (commandNumber(command))
                    {
                        switch movetodir:
                                movetodir();
                                break;
                        switch whereami:
                                whereami();
                                break;
                        switch history:
                                history();
                                break;
                        switch byebye:
                                exit = 1;
                                break;
                        switch start:
                                start();
                                break;
                        switch background:
                                background();
                                break;
                        switch exterminate:
                                break;
                        default:
                            break;
                    }
                    

                }while (exit == 0);

                printf("made it to the end");
                return 0;
        }
