#include "headers.h"
#include "call_jobs.h"
char bgprocessname[100][1024];
int bgprocessid[100];
char state[100];
void call_jobs(int num)
{
    for(int i=1;i<num;i++)
    {  
        if(bgprocessid[i]!=NULL)
        {    
        int  id;
        FILE *fd1;char path1[100];char temp[2048];
        id=bgprocessid[i];    
        sprintf(path1,"/proc/%d/stat",id);
		fd1=fopen(path1,"r");
        if(fd1==NULL){
            continue; 	 
        }  
        else
        {
            int j=0;
            while(fgets(temp,sizeof(temp),fd1)!=NULL)
            { 
                j++;                    
            }
            fclose(fd1);  
            printf("[%d]   ",i);
            int c=1;
            char *token;
            token=strtok(temp," ");
            while(token!=NULL)
            {
                if(c==3)
                    strcpy(state,token);
                c++;
                token=strtok(NULL," ");
            }
            if(strcmp(state,"T")==0)
                printf("Stopped ");
            else
                printf("Running ");    
            printf("%s  [%d]\n ",bgprocessname[i],bgprocessid[i]);
        } 
        }					           
	}
}
