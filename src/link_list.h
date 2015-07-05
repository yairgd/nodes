/*
 * =====================================================================================
 *
 *       Filename:  link_list.h
 *
 *    Description:  `
 *
 *        Version:  1.0
 *        Created:  05/01/2015 11:17:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef LINK_LIST_H
#define LINK_LIST_H


typedef struct  link_list_data_s {
	struct link_list_data_s *next;
	struct link_list_data_s *prev;
	void *data;
} link_list_data_t;


typedef struct  link_list_s {
	link_list_data_t *head;
	link_list_data_t *tail;
	link_list_data_t *current;
	int list_len;

} link_list_t;


link_list_t * link_list_new();
void link_list_add_head(link_list_t *link_list,void *data);
void link_list_free_to_end(link_list_t *link_list);
void link_list_init(link_list_t *link_list);
void * link_list_get_head(link_list_t *link_list);
void * link_list_get_tail(link_list_t *link_list);
void * link_list_set_current_to_head(link_list_t *link_list);
void * link_list_get_current_and_next(link_list_t *link_list);
void *  link_list_set_current_to_tail(link_list_t *link_list);

void * link_list_get_current_and_prev(link_list_t *link_list);
char *link_list_covert_to_string(link_list_t *link_list,char *sep);
void link_list_deinit(link_list_t *link_list);
void link_list_add_tail(link_list_t *link_list,void *data);





#define  link_list_for_each(item,list) link_list_set_current_to_head(list);\
	for( int keep=1;(list)->current;keep=1) \
	      for(item = link_list_get_current_and_next(list); keep;keep=0 )


#endif


	
