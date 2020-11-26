#include "headers.h"
#include "call_overkill.h"
int bgprocessid[100];
int bg;
void call_overkill()           //convert all into group or take all and run loop on pids
{
    for(int i=0 ;i<bg ;i++)
    {
        if(bgprocessid[i]!=NULL)
            kill(bgprocessid[i],9);
    }
}
