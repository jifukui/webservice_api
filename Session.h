#ifndef __SESSION_H_
#define __SESSION_H_
#include <openssl/ssl.h>
struct Session_Management{
    unsigned int temid;
};
struct Session_Version{
    unsigned int Major;
    unsigned int Modify;
    unsigned int Id;
};
void DisplayVersion();
#endif