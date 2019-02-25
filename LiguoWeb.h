#ifndef _LIGUO_WEB_H_
#define _LIGUO_WEB_H_
unsigned char LiguoWeb_GET_Method(const char *sstr,json_t * json,json_t *ech,json_t *jaonres,char *estr);
unsigned char LiguoWeb_POST_Method(const unsigned char *sstr,json_t *json,json_t*ech,json_t *jsonres,char * estr);
#endif _LIGUO_WEB_H_
