#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>


#define MAX_CHAR 300
#define MAX_INPUTS 10
char* currDir;
char** historyList;
int historyLen = 0, historyPointer=0;

enum supportedCommands{movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7, historyc = 8}; //, repeat =8};

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
            if (historyLen < 10)
            {
                strcpy(historyList[historyLen], str);
                historyLen++;
            }
            else
            {
                strcpy(historyList[historyPointer], str);
                historyPointer++;
                if(historyPointer==10)
                    historyPointer = 0;
            }
            

        }
        char** tokenizer(int *numTokens)
        {
            char* str = allocCharArray(MAX_CHAR);
            char* token = allocCharArray(MAX_CHAR);
            char* delim = " ";
            char** returnTokens = allocStrArray(MAX_INPUTS, MAX_CHAR);
            int count = 0;
            
            fgets(str, MAX_CHAR, stdin); // stdin = keyboard

            addToHistory(str);
            
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

            // gets rid of extranious '\0'
            int i;
            for (i = 1; i < count; i++)
            {
                returnTokens[i][strlen(returnTokens[i])-1] = '\0';
            }
                
            free(str);free(token);

            return returnTokens;
        }

        int commandNumber (char* command)
        {
            /* movetodir = 1, whereami = 2, history  = 3, byebye = 4, start = 5, background = 6, exterminate = 7*/
            /* the tokenization sticks a newline into "command" so ie "byebye" needs one as well if it is going to evaluate to the same thing*/

            if (strcmp(command, "movetodir\0") == 0)
                return movetodir;
            else if (strcmp(command, "whereami\n") == 0)
                return whereami;
            else if ( strcmp(command, "history\n") == 0) // show history
                return history;
            else if (strcmp(command, "history\0-\0") == 0) // clear history
                return historyc;
            else if (strcmp(command, "byebye\n") == 0)
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

        char* whereAmIf() // returns current directory
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
            strcpy(temp, currDir);

            if (newDir[0] != '/') // if it doesn't already have a slash, put one
                strcat(temp, "/");

            strcat(temp, newDir);
            
            if( chdir (temp) == 0)
            {
                strcpy(currDir, temp);
                printf("sucessfully changed to %s\n", currentDirectory());
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
            int i, j = 1;
            if (historyLen == 10) // if there are now 10 items in history and we've started overwriting them from the top
            {
                for ( i = historyPointer; i < historyLen; i++)
                {
                    printf("%d %s\n", j++, historyList[i]);
                }
                for ( i = 0; i < historyPointer; i++)
                {
                    printf("%d %s\n", j++, historyList[i]);
                }
            }else // if nothing has been overwritten yet because there are 10 or less items in history 
            {
                for (i = 0 ; i < historyLen ; i++)
                {
                    printf("%d %s \n", j++, historyList[i]);
                }
            }
            
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
            }
            else
            {
                printf("argument error, displaying history regularly: \n");
                historyf();
            }
        }
        void startf(char** tokenized, int numTokens) // start program [parameters]
        {
            // seperate out the program name from the rest of the parameters
            char* programName = allocCharArray(MAX_CHAR);
            strcpy(programName, tokenized[1]);

            // seperate out all of the arguments including the program name but leave out the command "start"
            char** args = allocStrArray(MAX_INPUTS, MAX_CHAR);
            int i;
            for (i = 1 ; i < numTokens ; i++)
            {
                strcpy(args[i], tokenized[i]);
            }
            args[i] = NULL;
            
            pid_t pid;
            int status;

            pid = fork();
            if ( pid < 0)
            {
                printf("error");
            }
            else if (pid == 0) // child
            {
                printf("Child PID is %ld\n", (long) getpid());
                if (execvp(programName, args) < 0) {     /* execute the command  */
                    printf("*** ERROR: exec failed\n");
                    exit(1);
                }
            }
            else // parent
            {
               while (wait(&status) != pid);
               printf("not waiting no more\n");
            }
            
        }
        void backgroundf(char** tokenized, int numTokens)
        {
            // seperate out the program name from the rest of the parameters
            char* programName = allocCharArray(MAX_CHAR);
            strcpy(programName, tokenized[1]);

            // seperate out all of the arguments including the program name but leave out the command "start"
            char** args = allocStrArray(MAX_INPUTS, MAX_CHAR);
            int i;
            for (i = 1 ; i < numTokens ; i++)
            {
                strcpy(args[i], tokenized[i]);
            }
            args[i] = NULL;
            
            pid_t pid;
            int status;

            pid = fork();
            if ( pid < 0)
            {
                printf("error");
            }
            else if (pid == 0) // child
            {
                printf("Child PID is %ld\n", (long) getpid());
                if (execvp(programName, args) < 0) {     /* execute the command  */
                    printf("*** ERROR: exec failed\n");
                    exit(1);
                }
                printf("# ");
            }
            else // parent behaves differently if its a background process rather than a regular exicution
            {
               while (waitpid(-1, &status, WNOHANG));
               printf("not waiting no more\n");
            }
        }
        void exterminatef(char* input)
        {
            int PID = atoi(input);
            if (kill(PID, SIGKILL) == -1)
            {
                printf("kill error\n");
            }
            printf("killed PID %d\n", PID);
        }

        int main()
        {
                int exit = 0;
                printf("welcome to fake baby shell\n");
                currDir = allocCharArray(MAX_CHAR);
                currDir = currentDirectory();
                historyList = allocStrArray(MAX_INPUTS, MAX_CHAR);
                
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
                        case movetodir:
                                currDir = moveToDirf(tokenized[1]);
                                break;
                        case whereami:
                                whereAmIf();
                                break;
                        case history:
                                historyf();
                                break;
                        case historyc:
                                clearHistory(tokenized[1]);
                                break;
                        case byebye:
                                exit = 1;
                                break;
                        case start:
                                startf(tokenized, *numTokens);
                                break;
                        case background:
                                backgroundf(tokenized, *numTokens);
                                break;
                        case exterminate:
                                exterminatef(tokenized[1]);
                                break;
                        default:
                            break;
                    }
                    

                }while (exit == 0);

                printf("made it to the end\n");
                return 0;
        }