#include"headers.h"
#include "ioredir.h"                 
char commands[20][1024];

int ioredirect(int i,int inp,int out,int outr)
{
 //  printf("hai redirect receievd\n");
    
    char temp2[10][1024]={};
    char *temp3;
    int n=0;
    char temp1[1024];
    strcpy(temp1,commands[i]);
    temp3=strtok(temp1," ");
    while(temp3!=NULL)
    {
        strcpy(temp2[n],temp3);
        n++;
        temp3=strtok(NULL," ");
        
    }
    //printf("check3\n");    
    if(inp!=0)
    {
        int fd0=open(temp2[inp+1],O_RDONLY,0);
        if(fd0==-1)
            perror("inputfile");   
        dup2(fd0,STDIN_FILENO);
        close(fd0);
    }
    if(out!=0)
    {  
     //   printf("outfile successful  and %s is file\n",temp2[out+1]);
        int fd1=open(temp2[out+1], O_WRONLY | O_CREAT |O_TRUNC,0644);
        if(fd1==-1)
            perror("outfile");
        dup2(fd1,STDOUT_FILENO);  
        close(fd1);
    }    
    if(outr!=0)
    {  
       // printf("outfile successful  and %s is file\n",temp2[outr+1]);
        int fd1=open(temp2[outr+1], O_WRONLY | O_CREAT |O_APPEND ,0644); // if present open else create
        //lseek(fd1,0,SEEK_END);                               //go till end                       
        if(fd1==-1)
            perror("outfile");
        dup2(fd1,STDOUT_FILENO);  
        close(fd1);
    }
    strcpy(commands[i]," ");       // send the actual command removing < , > ,>>
    for(int k=0;k<n;k++)
    {
      
         if(inp!=0)
        {  
              strcpy(temp2[inp],"");
           // strcpy(temp2[inp+1]," ");
        }   
        if(out!=0)
        {  
            strcpy(temp2[out]," ");
            strcpy(temp2[out+1]," ");
        }   
        if(outr!=0)
        {   
            strcpy(temp2[outr]," ");
            strcpy(temp2[outr+1]," ");
        }   
        strcat(commands[i],temp2[k]);
        strcat(commands[i]," ");
    }
    strcat(commands[i],"\0");
    //printf("command is %s\n",commands[i]); 
       
}           
