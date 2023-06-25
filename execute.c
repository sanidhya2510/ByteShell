#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.c"

typedef int (*BuiltinFunction)(char**);
BuiltinFunction func[] = 
{
        &changeDirectory,
        &helpCLI,
        &displayHistory,
        &exitCLI,
        &echoCLI,
        &presentWorkingDirectory,
        &makeNewDirectory,
        &makeNewFile,
        &headCommand
    };

int executeCLI(char **command)
{
    
    char* allCommands[] = {"cd", "help","history","exit","echo", "pwd", "mkdir", "touch", "head"};
    if(command[0] == NULL)
    {
        return 1;
    }

    int i;
    for(i = 0; i < sizeof(allCommands)/sizeof(allCommands[0]) ; i++)
    {
        if(strcmp(command[0], allCommands[i]) == 0)
        {
            return (*func[i])(command);
        }
    }
    return launchCLI(command);
}