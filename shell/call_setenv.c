#include "headers.h"
#include "call_setenv.h"

void call_setenv(char command[])
{
    int count=0;
    char *newcommands[3]={};
    char *token;
    token=strtok(command," ");
    while(token !=NULL)
    {
        newcommands[count]=token;
        count++;
        token=strtok(NULL," ");
    }
    if(count > 3)
    {
        printf("too many arguments for setenv\n");
        return 0;
    }
    if(strcmp(newcommands[0],"setenv")==0)
    {
        int retstatus=setenv(newcommands[1],newcommands[2],1);
        if(retstatus==-1){
            perror("setenv");
        }
    }
     if(strcmp(newcommands[0],"getenv")==0)
     {
        char *retstatus=getenv(newcommands[1]);
        if(retstatus==NULL){
            perror("getenv");
        }
        printf(" %s \n",getenv(newcommands[1]));
     }
      if(strcmp(newcommands[0],"unsetenv")==0)
     {
        int retstatus=unsetenv(newcommands[1]);
        if(retstatus==-1){
            perror("unsetenv");
        }
     }
     
}
