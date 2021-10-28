#include "headers.h"
#include "call_kjob.h"


char bgprocessname[100][1024];
int bgprocessid[100];
int j;



void kjob(char input[])
{
     //printf("command receive\n");
    int count=0;
    char *newcommands[3]={};
    char *token;
    token=strtok(input," ");
    while(token != NULL)
    {
        newcommands[count]=token;
        count++;
        token=strtok(NULL," ");
    }
    if(count>3)
    {
        printf("invalid arguments to kjob\n");
        return 0;
    }
    int sig;
    int id=*newcommands[1];
    if(strcmp(newcommands[0],"kjob")==0)
         sig=*newcommands[2];

    if(bgprocessid[id-48]!=NULL)
    {
        //printf("command receive\n");
        if(strcmp(newcommands[0],"kjob")==0)
            kill(bgprocessid[id-48],sig-48);
        
        else if(strcmp(newcommands[0],"fg")==0)          //brings background processes to fg and changes state from running to stopped
        {
            if(count > 2)
                printf("invalid aruguments to fg\n");
            int retstatus= kill(bgprocessid[id-48],SIGCONT);
            if(retstatus==-1)
            {
                perror(kill);
                return 0;
            }
            else
            {   
                pid_t pid;
                int status;
                signal(SIGTTOU,SIG_IGN);
                tcsetpgrp(0,bgprocessid[id-48]);
                waitpid(bgprocessid[id-48],&status,WUNTRACED);
                tcsetpgrp(0,getpid());
                signal(SIGTTOU,SIG_DFL);

            }    

        } 
        else if(strcmp(newcommands[0],"bg")==0)         //starts the stopped processes in bg and no affect on running processes
        {
           if(count > 2)
                printf("invalid aruguments to fg\n");
           //printf("bg received\n");
           int retstatus= kill(bgprocessid[id-48],SIGCONT);
           if(retstatus==-1)
           {
               printf("already running\n");
               return 0;
           }
        }    
          
    }
    else
    {
        printf("No process with given job number\n");
    }

}
