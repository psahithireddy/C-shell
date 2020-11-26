#include "headers.h"
#include "signal.h"
int cpid;
char bgprocessname[100][1024];
int bgprocessid[100];
int j;
void call_name(pid_t pid)
{
  for(j=1;j<=100;j++)
  {
    if(pid==bgprocessid[j])
      break;
  }
}

  
void signalhandle()
{
    
  pid_t pid;
  int status;
    while((pid=waitpid(-1,&status,WNOHANG | WUNTRACED | WCONTINUED))>0)
    {
      call_name(pid);
      if(WIFEXITED(status))
      {
        int exit_status = WEXITSTATUS(status);
        if(exit_status == 0)
			  {
				  printf("%s with pid %d exited normally \n",bgprocessname[j],pid);
          bgprocessid[j]=NULL;
			  }
      }
       else if(WIFSTOPPED(status))
        {
          int exit_status = WIFSTOPPED(status);
          if(exit_status == 0)
				    printf("%s with pid %d suspended normally \n",bgprocessname[j],pid);	
        }
        else if(WIFCONTINUED(status))
        {
          int exit_status = WIFCONTINUED(status);
          if(exit_status == 0)
				    printf("%s with pid %d continued normally \n",bgprocessname[j],pid);	
        }
        else 

			  {
				printf("%s with pid %d was not terminated normally\n" ,bgprocessname[j],pid);
        bgprocessid[j]=NULL;
			  }
    }
    
    
}
