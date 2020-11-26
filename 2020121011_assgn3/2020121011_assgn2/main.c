#include "prompt.h"
#include "headers.h"
char pathshell[1024];
int ppid;
int cpid;
int sig=0;
int bg;
int his;
char commandhistory[20][1024];
char bgprocessname[100][1024];
int bgprocessid[100];
int yes_out=0,yes_inp=0,yes_outr=0,yes_and=0,yes_pipe;
char *execcommand[1000];
char commands[20][1024]={};
char save_dir[1024];
int proid;
int i;


    
int main()         
{   
    save_dir[1024]=NULL;
    int stout=dup(STDOUT_FILENO);
    int stin=dup(STDIN_FILENO);
    bgprocessname[100][1024]=NULL;
    bgprocessid[100]=0;
    bg=1;
    int cc=0;
    if(getcwd(pathshell,1024)==NULL)
    {
        perror("getcwd");
    }
   
    his=0;
    int len=strlen(pathshell);
    pid_t pidstack[100]={};
    signal(SIGINT,SIG_IGN); //ctrl +c
    signal(SIGTSTP, SIG_IGN);  //ctrl +z
   while(1) 
    {
       //get stdin and out back
        prompt(len);
        sig=0;
        int size=1024;
        char *input1;char buffer[1024];       
        input1 = (char *) malloc (size);
        getdelim (&input1, &size,10, stdin);
        if(feof(stdin)){
            printf("\n");
            return 0;
        }
        if(input1==NULL)
        {
            return 0;
        }
        if(his==20)
           his=0;
        strcpy(commandhistory[his],input1); 
        //need to implement commands separated by ;
        const char s[2]=";";
        char *token;
        int k=0;
        char *seetoken=NULL;               
        seetoken=strtok(input1,"\n");
        if(seetoken==NULL)
        {
            continue;
        }
        token=strtok(seetoken,";");
        while(token!=NULL)       //separate through ; and store 
        {
            strcpy(commands[k],token);
            k++;
            token=strtok(NULL,";");
        } 
         
        for( i=0;i<k;i++)                  // for each command check what it is , else execvp
        {   
            signal(SIGCHLD,signalhandle());                  
            cc=0;
            yes_and=0;yes_out=0;yes_inp=0;yes_outr =0,yes_pipe=0;
            char *andtoken;
            char temp[1024];
            strcpy(temp,commands[i]);  
            int n=0;  
            andtoken=strtok(temp," ");
            while(andtoken!=NULL)           // to check presence of &
            {
                if(strcmp(andtoken,"&")==0)
                    yes_and=1;
                if(strcmp(andtoken,">")==0)
                    yes_out=n; 
                if(strcmp(andtoken,"<")==0)
                    yes_inp=n;
                if(strcmp(andtoken,">>")==0)
                    yes_outr=n;
                if(strcmp(andtoken,"|")==0)
                    yes_pipe=n;
                andtoken=strtok(NULL," ");
                n++;
            }
             if(yes_inp!=0 || yes_out!=0 || yes_outr!=0)        // io redirection
                ioredirect(i,yes_inp,yes_out,yes_outr); 
            
            char *seetoken1;
            seetoken1=strtok(temp," ");
            ppid=getpid();
            if(yes_pipe!=0)
               pipes(i); 
              
            //custom commands    
            else if(strcmp(seetoken1,"quit")==0)
             {cc=1;   return 0;}
            
            //echo command
            else if(strcmp(seetoken1,"echo")==0)
            {    
               cc=1; call_echo(commands[i]);
            }
            
            //pwd command
            else if(strcmp(seetoken1,"pwd")==0)
            {
               cc=1; call_pwd(commands[i]);
            }
            
            else if(strcmp(seetoken1,"cd")==0)
            {
              cc=1;  call_cd(commands[i]);
            }
             else if(strcmp(seetoken1,"pinfo")==0)
            {
              cc=1;  pinfo(commands[i]);
            }
            else if(strcmp(seetoken1,"ls")==0)
            {
                 cc=1;
                 call_ls(commands[i]);
            
            }
            else if(strcmp(seetoken1,"history")==0)
            {
                if(his<10)
                {
                    for(int u=0;u<=his;u++)
                        printf("%s \n",commandhistory[u]);
                }
                else
                {
                     for(int u=10;u<20;u++)
                        printf("%s \n",commandhistory[u]);
                }                            
            }
            else if(strcmp(seetoken1,"setenv")==0 ||strcmp(seetoken1,"getenv")==0 || strcmp(seetoken1,"unsetenv")==0  )  //remaining
            {
                cc=1;
                call_setenv(commands[i]);
            }
            else if(strcmp(seetoken1,"jobs")==0)
            {
                cc=1;
                call_jobs(bg);
            }
            else if(strcmp(seetoken1,"overkill")==0 )
            {
                cc=1;
                call_overkill();
            }
            else if(strcmp(seetoken1,"kjob")==0 || strcmp(seetoken1,"fg")==0 || strcmp(seetoken1,"bg")==0  )
            {
                cc=1;
                kjob(commands[i]);
            }
            else if (yes_pipe == 0 && cc==0)
           {   
            int stat;
            proid=fork(); // child process and not custom command
            cpid=proid;
            if(proid==0 )
            {
                signal(SIGINT,SIG_DFL); //ctrl +c
                signal(SIGTSTP, SIG_DFL);  //ctrl +z
                cpid=getpid();
				int m=0;
				execcommand[m] = strtok(commands[i], " ");
				m=1;
				while(1)
                {                           
                    execcommand[m] = strtok(NULL, " ");                              
					if(execcommand[m]==NULL){
						break;
				    }
					m+=1;
				}
                    if(yes_and==1)
                        execcommand[m-1]=NULL;
                    
                
                setpgid(0,0);
                int erno=execvp(execcommand[0], execcommand);               
                if(erno<0)
                 {   perror("execvp"); }
            
             }   
              
             if(proid>0)
             {
                    strcpy( bgprocessname[bg],commands[i]);                           
                        bgprocessid[bg]=cpid;
                        bg++;
                   
                    
                    printf("\n");
                    setpgid(proid,proid); 
                     if(yes_inp!=0)
                         dup2(stin,STDIN_FILENO); 
                    if(yes_and==0)
                    {
                        int status;
                        signal(SIGTTOU,SIG_IGN);
                        tcsetpgrp(STDIN_FILENO,cpid);
                        waitpid(proid,&status, WUNTRACED);
                        tcsetpgrp(STDIN_FILENO,getpgrp());
                        signal(SIGTTOU,SIG_DFL);
                                 
                 
                    }  
                       
                  
            }      
            else
            {
                {perror("fork");}
            }
            }
            if(yes_out !=0  || yes_outr !=0) 
                dup2(stout,STDOUT_FILENO);
            dup2(stin,STDIN_FILENO); 
          
      }
    his++;
  }
  
}

