/*
 * =====================================================================================
 *
 *       Filename:  list.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/09/13 21:42:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#pragma once
typedef struct {
	int len,max,size;
	void *data;
} list_t;

#define LIST_INIT(list,type,n)\
	(list).data=(void*)malloc(sizeof(type)*n);\
	(list).len=0;list.max=n;\
	(list).size=sizeof( type);
//void list_init(list_t *list,int max);
char list_add(list_t *list,void *data);
void list_remove(list_t *list,int idx);
void *list_get(list_t *list,int idx);
void *list_get_last(list_t *list);
void list_deinit(list_t *list);
int list_size(list_t *list);


#define list_foreach(item,list ) \
    for(int keep = 1, \
            count = 0,\
            size =list_size( (list) ); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = list_get( (list),count); keep; keep = !keep)
