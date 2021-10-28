#include "call_cd.h"
#include "headers.h"        
char pathshell[1024];
char save_dir[1024];
char save_dir1[1024];
void call_cd(char input[1024])
{
    const char s[2]=" ";
    char *token;
    char cwd[1024]={};
    //copying remaining part into string except cd
    char newinput[1024];
    int k=0;
    for(int i=3;i<=strlen(input);i++)
    {
        newinput[k]=input[i];
        k++;
    }
    
     //printf("%s is save_dir\n",save_dir);
     //  printf("%s is save_dir1\n",save_dir1);
    token=strtok(newinput,s);
    if(strcmp(token,".")==0)
     { 
        getcwd(cwd,1024);
        strcpy(save_dir1,cwd);
     }
    else if(strcmp(token,"..")==0)
    {  
        getcwd(cwd,1024);
        save_dir1[1024]=NULL;
        strcpy(save_dir1,cwd);
        chdir("..");
    }
    else if(strcmp(token,"-")==0)
    {
        if(save_dir[2]==NULL)
        {    
           // printf("hai bro");
            strcpy(save_dir,pathshell);
        }
        else
         {   
           // printf("hai sis");
            save_dir[1024]=NULL;
            strcpy(save_dir,save_dir1);    
         }
       // printf("%s is opened\n",save_dir); 
        getcwd(cwd,1024);
        save_dir1[1024]=NULL;
        strcpy(save_dir1,cwd); 
       // printf("current dir is %s",save_dir1);  
        chdir(save_dir);
    } 
    else if(strcmp(token,"~")==0)
    {   
         char *a="/..";
         char pathshell1[1024];
         strcpy(pathshell1,pathshell);
         strcat(pathshell1,a);
         strcpy(save_dir1,pathshell1);
         chdir(pathshell1);
    }
    else 
    {   
        strcpy(cwd,newinput);
        if(chdir(cwd)<0)
        {            
            char *a="/";
            getcwd(cwd,1024);
            strcat(cwd,a);
            strcat(cwd,newinput);
            if(chdir(cwd)<0)
            {
                perror("chdir");
                return EXIT_FAILURE;
            }
            strcpy(save_dir1,cwd);
        }
        else
        {
            chdir(cwd);
            strcpy(save_dir1,cwd);  // to get back in case - is used
        }
        
    }
     
    
}

    


