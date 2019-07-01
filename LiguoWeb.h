#ifndef _LIGUO_WEB_H_
#define _LIGUO_WEB_H_
#include "config.h"
typedef struct 
{
    char username[15];
    char password[PASSWORDLEN];
}authenticate;              
typedef struct{
    int security;
    authenticate Auth[AUTH_NUM];
}Auth_liguo;
void writesecurityfile();
unsigned char LiguoWeb_GET_Method(const char *sstr,json_t * json,json_t *ech,json_t *jaonres,char *estr);
unsigned char LiguoWeb_POST_Method(const unsigned char *sstr,json_t *json,json_t*ech,json_t *jsonres,char * estr);
#endif _LIGUO_WEB_H_