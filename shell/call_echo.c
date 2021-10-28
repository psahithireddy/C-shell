#include "call_echo.h"
#include"headers.h"

void call_echo(char input[])
{
    //to break the words at spaces
    const char s[2]=" ";
    char *token;
    int k=0;
    //input has echo has first few characters hence making new string
    char newinput[1024]={};
    for(int i=5;i<strlen(input);i++)
    {
        newinput[k]=input[i];
        k++;
    } 
    // a space was printing extra 
    printf("\b");
    token=strtok(newinput,s);
    while(token!=NULL)
    {
        write(1,token,strlen(token));
        write(1," ",1);
        token=strtok(NULL,s);
    }
    printf("\n");
 //   return main();
}
