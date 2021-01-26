#include "Session.h"
static void DisplayOpensslVersion();

struct Session_Version Version={1,0,0};

void DisplayVersion(){
    printf("The Session Version is %d.%d.%d\r\n",Version.Major,Version.Modify,Version.Id);
    DisplayOpensslVersion();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void DisplayOpensslVersion(){
    printf("the openssl version is %s\r\n",OpenSSL_version(OPENSSL_VERSION));
}