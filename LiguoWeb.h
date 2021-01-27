#ifndef _LIGUO_WEB_H_
#define _LIGUO_WEB_H_

#include <errno.h>
#include <stdlib.h>
#include "config.h"
#include "Session.h"


typedef unsigned char uint8;
int binary_semaphore_allocation(key_t key,int sem_flags);
int binary_semaphore_deallocate(int semid);
int binary_semaphore_initialize(int semid);
int binary_semaphore_wait(int semid);
int binary_semaphore_post(int semid);
void writesecurityfile();
uint8 CheckPassword(uint8 *password);
unsigned char LiguoWeb_GET_Method(const char *sstr,json_t * json,json_t *ech,json_t *jaonres,char *estr);
unsigned char LiguoWeb_POST_Method(const unsigned char *sstr,json_t *json,json_t*ech,json_t *jsonres,char * estr);
#endif //_LIGUO_WEB_H_