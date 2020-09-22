#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>


#define MAX_CHAR 300
#define MAX_STRINGS 10
char* currDir;
char** historyList;
int historyLen = 0, historyPointer=0, sizeOfHistory=10;

enum supportedCommands{movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7}; //, repeat =8};

        // allocates space for an array of strings
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

        void addToHistory(char* str)
        {
            if (historyLen < sizeOfHistory) // if history is not full, add at the next open space
            {
                strcpy(historyList[historyLen], str);
                historyLen++;
            }
            else // if history is full, start overriding starting from 0
            {
                strcpy(historyList[historyPointer], str);
                historyPointer++;
                if(historyPointer == sizeOfHistory) // once historyPointer has made the rounds though the whole array, it loops back to element 0
                    historyPointer = 0;
            }
        }
        void input(char* str)
        {
            printf("# ");
            fgets(str, MAX_CHAR, stdin); // stdin = keyboard
            addToHistory(str);
            //str[sizeof(str)/sizeof(char)]= '\0';
        }
        char** tokenizer(char* str, int *numTokens)
        {
            char* token = allocCharArray(MAX_CHAR);
            char* delim = " ";
            char** returnTokens = allocStrArray(MAX_CHAR, MAX_STRINGS);
            int count = 0;
            
            // prints at least 1 token, keeps going if more exist
            token = strtok(str, delim);
            while (token != NULL)
            {
                strcpy(returnTokens[count], token);
                // printf("token: %s\n", token); // check if the input has passed prpoerly
                token = strtok(NULL, delim); // all other tokens need to have "NULL" here instead of the original string (idk why)

                if (count == 10)
                {
                    printf("too many inputs!!\n");
                    exit(0);
                }
                count++;

            }
            *numTokens = count;
            // printf("number of tokens: %d\n", *numTokens);

            // get rid of the newline character in the last input
            returnTokens[count-1][strlen(returnTokens[count-1])-1] = '\0';
            
            free(str);free(token);

            return returnTokens;
        }

        int commandNumber (char* command)
        {
            /* movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7*/
            /* the tokenization sticks a newline into "command" so ie "byebye" needs one as well if it is going to evaluate to the same thing*/

            if (strcmp(command, "movetodir\0") == 0)
                return movetodir;
            else if (strcmp(command, "whereami\0") == 0)
                return whereami;
            else if ( strcmp(command, "history\0") == 0)
                return history;
            else if (strcmp(command, "byebye") == 0)
                return byebye;
            else if (strcmp(command, "start\0") == 0)
                return start;
            else if (strcmp(command, "background\0") == 0)
                return background;
            else if (strcmp(command, "exterminate\0") == 0)
                return exterminate;
            else
            {
                printf("command not recognied\n");
                return 0;
            }                
        }

        char* whereAmIf() // prints current directory
        {
            if (currDir == NULL)
                perror("getcwd() error");
            else
                printf("Current working directory is: %s\n", currDir);
            
            return currDir;
            
        }
        char* currentDirectory() // returns current directory
        {
            char* temp = allocCharArray(MAX_CHAR);
            getcwd(temp, MAX_CHAR);
            
            return temp;
        }

        char* moveToDirf(char* newDir)
        {
            char* temp = allocCharArray(MAX_CHAR); 
            //strcat(newDir, "/");
            strcpy(temp, currDir);
            //printf("newDir: %s\n", newDir); // check the new directory has passed correctly 

            if (newDir[0] != '/') // if it doesn't already have a slash, put one
                strcat(temp, "/");

            strcat(temp, newDir);
            
            if( chdir (temp) == 0 )
            {
                strcpy(currDir, temp);
                printf("sucessfully changed to: %s\n", currentDirectory());
            }
            else
            {
                printf("directory does not exist\n");
            }
            
            return currDir;
        }
        // why i wrote it to just rewrite over the same 10 spaces and just move the starting point once when it filled up when it would've probably 
        // been easier to just make and copy into a new array of size 10 every time is beyond me but it's cool isn't it?? if this were a game in the 80s
        // it would've been super efficent and saved a lot of memory and processing time of which there was not much of. nowadays it probably makes
        // no difference. this took me more a long time to logic out on paper than i should have spent on this homework :_)
        void historyf() 
        {
            printf("Last %d inputs:\n---------------------------\n", historyLen);
            int i, j = 0;
            if (historyLen == sizeOfHistory) // if there are now 10 items in history and we've started overwriting them from the top
            {
                for ( i = historyPointer; i < historyLen; i++)
                {
                    printf("%d %s\n", historyLen-j++, historyList[i]);
                }
                for ( i = 0; i < historyPointer; i++)
                {
                    printf("%d %s\n", historyLen-j++, historyList[i]);
                }
            }else // if nothing has been overwritten yet because there are 10 or less items in history 
            {
                for (i = 0 ; i < historyLen ; i++)
                {
                    printf("%d %s \n", historyLen-j++, historyList[i]);
                }
            }
            printf("---------------------------\n");
            
        }
        void clearHistory(char* c)
        {
            if ( strcmp(c, "-c") == 0)
            {
                int i;
                for (i = 0; i < historyLen ; i++)
                {
                    strcpy(historyList[i], "");
                }
                historyLen = 0;
                historyPointer = 0;
                printf("history cleared\n");
            }
            else
            {
                printf("argument error, displaying history regularly: \n");
                historyf();
            }
        }
        void startf(char** tokenized, int numTokens) // start program [parameters]
        {

            // seperate out all of the arguments including the program name but leave out the command "start"
            char** args = allocStrArray(MAX_CHAR, MAX_STRINGS);
            int i;
            for (i = 1 ; i < numTokens ; i++)
            {
                strcpy(args[i-1], tokenized[i]);
            }
            args[i-1] = NULL; // srgs must be null terminated
            
            /*
            printf("check the contents of the args:\n");
             for(i=0 ; i < numTokens-1 ; i++)
            {
                 printf("%d: %s\n", i, args[i]);
            }
            */

            pid_t pid;
            int status;

            /*
            switch (pid = fork())
            {
                case -1:
                    // On error fork() returns -1.
                    perror("fork failed");
                    exit(EXIT_FAILURE);
                case 0:
                    // On success fork() returns 0 in the child.
                    printf(" CHILD <%ld> I'm alive! My PID is <%ld> and my parent got PID <%ld>.\n", (long) getpid(), (long) getpid(), (long) getppid());
                    printf(" CHILD <%ld> Goodbye!\n", (long) getpid());
                    exit(EXIT_SUCCESS);
                default:
                    // On success fork() returns the pid of the child to the parent.
                    printf("PARENT <%ld> Spawned a child with PID = %ld.\n", (long) getpid(), (long) pid);
                    wait(NULL);
                    printf("PARENT <%ld> Child with PID = %ld terminated.\n", (long) getpid(), (long) pid);
                    printf("PARENT <%ld> Goodbye.\n", (long) getpid());
                    exit(EXIT_SUCCESS);
            }
            */
            pid = fork();
            if ( pid < 0)
            {
                printf("error");
            }
            else if (pid == 0) // child
            {
                if (execvp(args[0], args) < 0) {     // execute the command                                 // ok so here, i have a problem with args, it doesn't seem to pass though 
                    printf("execution failed\n");
                    exit(1);
                }
            }
            else // parent
            {
               while (wait(&status) != pid);
               printf("parent no longer waiting on child of PID %ld\n", (long) pid);
            }
            
        }
        void backgroundf(char** tokenized, int numTokens)
        {
            // seperate out all of the arguments including the program name but leave out the command "start"
            char** args = allocStrArray(MAX_CHAR, MAX_STRINGS);
            int i;
            for (i = 1 ; i < numTokens ; i++)
            {
                strcpy(args[i-1], tokenized[i]);
            }
            args[i-1] = NULL;
            
            pid_t pid;
            //int status;
            
            pid = fork();
            if ( pid < 0)
            {
                printf("error");
            }
            else if (pid == 0) // child
            {
                if (execvp(args[0], args) < 0) {     // execute the command
                    printf("execution failed\n");
                    exit(1);
                }
            }
            else // parent behaves differently if its a background process rather than a regular exicution
            {
               printf("parent started child with PID of %ld\n", (long)pid);
               //return;
            }
        }
        void exterminatef(char* input)
        {
            pid_t PID = atoi(input);
            if (kill(PID, SIGKILL) == -1)
            {
                printf("kill error\n");
            }
            printf("killed PID %ld\n", (long) PID);
        }

        int main()
        {
                int exit = 0;
                printf("welcome to myShell\n");
                currDir = allocCharArray(MAX_CHAR);
                currDir = currentDirectory();
                historyList = allocStrArray(MAX_CHAR, MAX_STRINGS);
                
                do{
                    printf("inside of do/while loop\n");
                    char** tokenized = allocStrArray(MAX_CHAR, MAX_STRINGS);
                    int *numTokens = (int*) calloc(1, sizeof(int));
                    char* command = allocCharArray(MAX_CHAR);//(char *)calloc(MAX_STRINGS, sizeof(char));
                    char* str = allocCharArray(MAX_CHAR);

                    input(str);

                    tokenized = tokenizer(str, numTokens);
                    strcpy(command, tokenized[0]); // first token is always a command
                    /* movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7*/

                    // printf("lets see the tokens in main: \n");
                    // int i;
                    // for (i=0; i < *numTokens; i++)
                    // {
                    //     printf("%d: %s\n", i, tokenized[i]);
                    // }

                    switch (commandNumber(command))
                    {
                        case movetodir:
                                currDir = moveToDirf(tokenized[1]);
                                break;
                        case whereami:
                                whereAmIf();
                                break;
                        case history:
                                if ( strcmp(tokenized[1], "") != 0 )
                                    clearHistory(tokenized[1]);
                                else
                                    historyf();
                                break;
                        case byebye:
                                exit = 1;
                                break;
                        case start:
                                startf(tokenized, *numTokens);
                                break;
                        case background:
                                backgroundf(tokenized, *numTokens);
                                printf(">\n");
                                break;
                        case exterminate:
                                exterminatef(tokenized[1]);
                                break;
                        default:
                            break;
                    }
                    //printf("bottom of switch\n");

                }while (exit == 0);

                printf("end of program\n");
                return 0;
        }