#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<string.h>


int headCommand(char **command);//function to display first 10 lines of a file
int makeNewDirectory(char **command);// function to create a directory
int makeNewFile(char **command); // function to create a new file
int changeDirectory(char **command); //function to change directory
int echoCLI(char **command); //function to output something in the Shell
int presentWorkingDirectory(); //function to display the present working directory in the Shell
int exitCLI(char **command); //function to exit the Shell
int helpCLI(char **command); //function to display all the commands Shell reacts to
int displayHistory(char **command); //function to output the history of operations by the user
void addToHistory(const char *inputLine); //function to add the recent operation to history
int launchCLI(char **command); 

//parsing functions
char** splitLine(char *inputLine);
char* readLine();

int headCommand(char **command){

    if(command[1] == NULL) fprintf(stderr, "Some Argument expected to \"head\" \n");
    else{
        FILE* temp;
        temp = fopen(command[1] , "r");

        int n = 10;
        char inputLine[100];

        while(fgets(inputLine, sizeof(inputLine), temp) && n){
            printf("%s", inputLine);
            n--;
        }

        fclose(temp);
    }

    return 1;
}


int makeNewDirectory(char **command)
{
    if(command[1] == NULL)fprintf(stderr, "Some Argument expected to \"mkdir\" \n");
    else mkdir(command[1], 0777);
    return 1;
}


int makeNewFile(char **command)
{
    if(command[1] == NULL)fprintf(stderr, "Some Argument expected to \"touch\" \n");
    else
    {
        FILE* temp = fopen(command[1], "w");
        fclose(temp);
    }
    return 1;
}



int changeDirectory(char **command)
{
    if(command[1] == NULL)fprintf(stderr, "Some Argument expected to \"cd\" \n");
    else if(chdir(command[1]) != 0)perror("ByteShell");
    return 1;
}



int echoCLI(char **command)
{
    int i = 0;
    while(command[i]!=NULL)
    {
        if(i!=0)printf("%s ",command[i]);
        i++;
    }
    printf("\n");
    return 1;
}



int presentWorkingDirectory()
{
    char myPwd[1024];
    if(getcwd(myPwd, 1024) == NULL)
    {
        perror("");
        exit(0);
    }
    printf("%s\n", myPwd);
    return 1;
}



int exitCLI(char **command)
{
  return 0;
}



int helpCLI(char **command)
{
  char *builtin_string[] = { "cd", "help", "history", "exit", "echo", "pwd", "mkdir", "touch", "head"};
  int i;
  printf("<<ByteShell Project by Sanidhya>>\n");
  printf("The following Commands are Supported Builtin: \n");
  for (i = 0; i < sizeof(builtin_string) / sizeof(char *); i++)printf("  %s\n", builtin_string[i]);
  return 1;
}



struct  Node
{
    char *str;
    struct Node* next;    
};

struct Node* head = NULL;
struct Node* cur = NULL;

int displayHistory(char **command)
{
    struct Node* ptr = head;
    int i = 1;
    while(ptr != NULL)
    {
        printf(" %d %s\n", i++,ptr->str);
        ptr = ptr->next;
    }
    return 1;
}

void addToHistory(const char *inputLine)
{
    char* temp = (char *)malloc(strlen(inputLine));
    strcpy(temp, inputLine);
    char* temp_token = strtok(temp, " ");
    if(temp_token == NULL) return;
    
    if(head == NULL)
    {
        head = (struct Node *)malloc(sizeof(struct Node));
        head->str = (char *)malloc(strlen(inputLine));
        strcpy(head->str, inputLine);
        head->next = NULL;
        cur = head;
    }
    else
    {
        struct Node *ptr = (struct Node *)malloc(sizeof(struct Node));
        cur->next = ptr;
        ptr->str = (char *)malloc(strlen(inputLine));
        strcpy(ptr->str, inputLine);
        ptr->next = NULL;
        cur=ptr;
    }
}



int launchCLI(char **command)
{
    pid_t pid;
    int status;
    pid = fork();
    if(pid == 0)
    {
        if(execvp(command[0], command) == -1)perror("ByteShell");
        exit(EXIT_FAILURE);
    }
    else do{
        waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));   

    return 1;
}



char** splitLine(char *inputLine)
{
    int buffer_size = 1024, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token = strtok(inputLine, " ");
    if(!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(token != NULL)
    {
        // printf("%s\n", token);
        tokens[position] = token;
        position++;
        if(position >= buffer_size)
        {
            buffer_size += buffer_size;
            tokens = realloc(tokens, buffer_size * sizeof(char *));
            if(!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
    }
    tokens[position] = NULL;
    return tokens;
}

char* readLine()
{
    int buffer_size = 1024;
    int position = 0;
    char *buffer = malloc(sizeof(char) *buffer_size);
    int c;

    if(!buffer)
    {
        fprintf(stderr, "Allocation error");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        c = getchar();
        if (c == '\n' )
        {
            buffer[position] = '\0';
            return buffer; 
        }
        else
        {
            buffer[position] = c;
            position++;
        }
        if (position >= buffer_size)
        {
            buffer_size += buffer_size; //doubtful
            buffer = realloc(buffer, buffer_size);
        }
        if (!buffer)
        {
            fprintf(stderr, "allocation error\n");
            exit(EXIT_FAILURE); 
        }
    }
}