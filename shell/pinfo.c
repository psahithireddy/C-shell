#include "pinfo.h"
#include "headers.h"
int ppid;
char state[100]={}; 
void pinfo(char input[])
{
     //to break the words at spaces
    const char s[2]=" ";
    char *token;
    int k=0;
    FILE *fd1, *fd2;
    //input has pinfo has first few characters hence making new string
    char newinput[1024]={};
    for(int i=6;i<strlen(input);i++)
    {
       newinput[k]=input[i];
       k++;
    } 
    token=strtok(newinput,s);
   //printf("entered\n");
    char path1[100],path2[100]={};
    
    char pid[16]={};
    char exe[256]={};
    char vm[100]={};    
        //get current process id
        //proc - process information pseudo-filesystem
        if(token==NULL)
       { 
        
        sprintf(path1,"/proc/%d/status",getpid());
        sprintf(path2,"/proc/%d/exe",getpid());
        }
        else 
        {
         sprintf(path1,"/proc/%s/status",token);
         sprintf(path2,"/proc/%s/exe",token);   
        }
        //printf(" %d is pid\n");
        int i=0;
        char temp[1024];
        int rnum=readlink(path2,exe,256);
        if(rnum<0)
        {
            printf("no process with given  pid\n");
            return 0;
        }
        fd1=fopen(path1,"r");
        if(fd1==NULL)
         {
            printf("no process with given  pid\n");
           // return 0;
        }    
        
        while(fgets(temp,sizeof(temp),fd1)!=NULL)
        {
            if(i==5)
                strcpy(pid,temp);
            if(i == 2)
                strcpy(state,temp);
            if(i== 17)
                strcpy(vm,temp); 
            i++;                
		    
	     }
         fclose(fd1);  

         printf("%8s",pid);
         printf("%s",state);
         printf("%s",vm);         
         printf("executable : %s \n",exe); 
         
              
 }
    
    
            
    

