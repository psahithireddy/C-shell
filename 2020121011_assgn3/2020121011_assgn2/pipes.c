#include "headers.h"
#include "pipes.h"
char commands[20][1024];  //FORK a child write using it , read from parent.
                                           
void pipes(int i)        
{
    int n=0; //NO.OF PIPES
    char *token;
    char newcommands[10][1024];
    token=strtok(commands[i],"|");
    while(token!=NULL)
    {
    	strcpy(newcommands[n],token);
    	printf("%s\n",newcommands[n]);
    	n++;
    	token=strtok(NULL,"|");
    }	
	
    for(int j=0;j<n;j++)
    {                 //for n pipes
    
        int pipefd[2];                                     // 0 is read, 1 is write 
        int retstatus=pipe(pipefd);      // calling to create a pipe 
        if(retstatus==-1)
        {
            perror("pipe");
            return 0;
        }
        int pid2=fork();
        if(pid2==0)                                                          //grand child
        {

        	close(pipefd[0]);
            if(j!=n-1)
        	{dup2(pipefd[1],1); } 
                                                 // write end of file
        	char *execcommand[1024];
			int m=0;
			execcommand[m] = strtok(newcommands[j], " ");
			m=1;
			while(1)
			{    
				execcommand[m] = strtok(NULL, " ");                            
				if(execcommand[m]==NULL){
				break;}
				m+=1;
			}
            int erno=execvp(execcommand[0], execcommand);               
         	if(erno<0)
          	{  perror("execvpwrite");return 0; }
        }
        else if(pid2<0)
        {
            perror("fork");
            return;
        }
        else
        { 
         	close(pipefd[1]);
         	dup2(pipefd[0],0);     //read end of file
            wait(NULL);
         
        }   
        
    } 
    
         	        	
    

}
