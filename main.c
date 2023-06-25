#include <stdlib.h>
#include "execute.c"

void loop()
{
    char *inputLine;
    char **command;
    int status;

    do{
        printf("<<Custom Byteshell June 2023>>");
        inputLine = readLine();
        addToHistory(inputLine);
        command = splitLine(inputLine);
        status = executeCLI(command);
    }while (status);
    
}

int main(int argc, char **argv)
{   
    printf("Enter \'help\' for available commands\n");
    loop();
    return 0;
}

