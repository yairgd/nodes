/*
 * =====================================================================================
 *
 *       Filename:  list.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/09/13 21:42:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "list.h"
#include <memory.h>
#include <stdlib.h>

char list_add(list_t *list,void *data)
{
	if (list->len==list->max) {
		list->data = realloc(list->data,list->size*255 + list->size*list->len);
		if (list->data==0) // error, bad allocation
			return 0;
		list->max+=255;
	}
	memcpy (list->data+list->len*list->size ,data,list->size);
	list->len++;
	return 1;
}
void list_remove(list_t *list,int len)
{
	void *dst = list->data+len*list->size;
	void *src =list->data+(len+1)*list->size; 
	memcpy (dst,src ,(list->max-len-1)*list->size);
	list->len--;
}
void *list_get(list_t *list,int idx)
{
	return list->data+idx*list->size;
}

void *list_get_last(list_t *list)
{
	return list->data+list->len*list->size;
}

void list_clear(list_t *list)
{
	list->len = 0;
}

void list_deinit(list_t *list)
{
	free (list->data);
}

int list_size(list_t *list)
{
	return list->len;
}
typedef struct _test {
	int a;
	int b;
} test_t;

#ifdef TEST_LIST
main()
{
	list_t l;
	test_t t;
	int i;
	int n;
	LIST_INIT(l,test_t,20);
	t.a=1;t.b=10;list_add(&l,&t);
	t.a=2;t.b=20;list_add(&l,&t);
	t.a=3;t.b=30;list_add(&l,&t);

n=1;
list_remove(&l,n);	
}
#endif
