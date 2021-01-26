#include "Session.h"


struct Session_Version Version={1,0,0}

void DisplayVersion(){
    printf("The Session Version is %d.%d.%d\r\n",Version.Major,Version.Modify,Version.Id);
}