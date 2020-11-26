#include "prompt.h"
#include "headers.h"
 char username[LOGIN_NAME_MAX];
void prompt(int length)
{
    //get hostname
    struct utsname u;
    uname(&u);
    if(u.nodename<0)
    {
        perror("uname");
        return EXIT_FAILURE;
    }
    //get username
   
    int result;
    result=getlogin_r(username,LOGIN_NAME_MAX);
    if(result<0)
    {
        perror("getlogin_r");
        return EXIT_FAILURE;
    }
    //getting curr dir name
    char cwp[1024]={};
    char curr_dirname[1024]={};
    getcwd(cwp,1024);
    int k=0;
    for(int i=length;i<strlen(cwp);i++)
    {
        curr_dirname[k]=cwp[i];
        k++;
    }        
    char interme[1024];
   // printf("%s@%s:~%s$ ",username,u.nodename,curr_dirname);
       sprintf(interme,"%s@%s:~%s$ ",username,u.nodename,curr_dirname);
        write(1,interme,strlen(interme)); 
       
}


