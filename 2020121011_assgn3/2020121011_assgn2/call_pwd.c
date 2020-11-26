#include "call_pwd.h"
#include "headers.h"

void call_pwd(char input[1024])
{
    char buff[1024];
    char *result=getcwd(buff,1023);
    // a space was printing extra 
    if(result==NULL)
    {
        perror("getcwd");
        return EXIT_FAILURE;
    }   
    else
        write(1,result,strlen(result));
        printf("\n");
}
