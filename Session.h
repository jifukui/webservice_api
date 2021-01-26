#ifndef __SESSION_H_
#define __SESSION_H_
#include <openssl/ssl.h>
struct Session_Management{

}
struct Session_Version{
    unsigned int Major;
    unsigned int Modify;
    unsigned int Id;
}
void DisplayVersion();
#endif