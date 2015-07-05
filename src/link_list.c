/*
 * =====================================================================================
 *
 *       Filename:  link_list.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/01/2015 11:17:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *i
 * =====================================================================================
 */

#include "link_list.h"
#include <stdlib.h>
#include <string.h>

//todo: memory managemt.  what happend if malloc is failed

link_list_t * link_list_new()
{
	link_list_t *link_list =  (link_list_t*)malloc(sizeof(link_list_t ));
	link_list_init(link_list);
	return link_list;
}

void link_list_init(link_list_t *link_list)
{
//	link_list->next =  (link_list_t*)malloc(sizeof(link_list_t *));
//	link_list->next->next=0;
//	link_list->prev = link_list->next;
	link_list->head=0 ;
	link_list->tail=0 ;
	link_list->current=0 ;
	link_list->list_len = 0;
	
	
}

/**
 * Created  05/01/2015
 * @brief   add new element to list
 * @param   link_list - initiated link_list_t
 * @param   data - data to link
 * @return  
 */
void link_list_add_head(link_list_t *link_list,void *data)
{

		link_list_data_t *tmp =  (link_list_data_t*)malloc(sizeof(link_list_data_t *));
		if (link_list->head ==0) 
		{	

			link_list->head = tmp;
			link_list->tail = tmp;
			link_list->head->next = 0;
			link_list->head->prev = 0;
			link_list->head->data = data;
		}
		else
		{
			tmp = link_list->head;
		        link_list->head = (link_list_data_t*)malloc(sizeof(link_list_data_t *));
        		link_list->head->next = 0;
			link_list->head->prev = tmp;
			link_list->head->data = data;
			tmp->next = link_list->head;
		}
		link_list->list_len++;
		
}

/**
 * @created 05/21/15
 * @brief   
 * @param   
 * @return  
 */
void link_list_add_tail(link_list_t *link_list,void *data)
{

		link_list_data_t *tmp =  (link_list_data_t*)malloc(sizeof(link_list_data_t *));
		if (link_list->head ==0) 
		{	

			link_list->head = tmp;
			link_list->tail = tmp;
			link_list->tail->prev = 0;
			link_list->tail->next = 0;
			link_list->tail->data = data;
		}
		else
		{
			tmp = link_list->tail;
		        link_list->tail = (link_list_data_t*)malloc(sizeof(link_list_data_t *));
        		link_list->tail->prev = 0;
			link_list->tail->next = tmp;
			link_list->tail->data = data;
			tmp->prev = link_list->tail;
		}
		link_list->list_len++;
		
}
/**
 * Created  05/01/2015
 * @brief   return current head and remove it and the previous element is set to be new head
 * @return  void * pointer to data
 */
void * link_list_get_head(link_list_t *link_list)
{
	if (link_list->list_len==0)
		return 0;
	link_list_data_t *tmp = link_list->head;
	link_list->head = link_list->head->prev;
	void *data = tmp->data;
	free(tmp);
	link_list->list_len--;
	if (link_list->list_len==0) {
		link_list->head=0;
		link_list->tail=0;
	}
	return data;
}


/**
 * @created 05/12/15
 * @brief   return current tail and remove it and the next element is set to be new tail: todo !!!!!!!!!!!!!!!!!!!!!
 * @param   link_list - pointer to link list
 * @return   void * pointer to data
 */
void * link_list_get_tail(link_list_t *link_list)
{
	if (link_list->list_len==0)
		return 0;
	link_list_data_t *tmp = link_list->tail;
	link_list->tail = link_list->tail->next;
	void *data = tmp->data;
	free(tmp);
	link_list->list_len--;
	if (link_list->list_len==0) {
		link_list->head=0;
		link_list->tail=0;
	}
	return data;
}

void *  link_list_set_current_to_head(link_list_t *link_list)
{
	link_list->current=link_list->head;
	if (link_list->current)
		return link_list->current->data;
	else
		return 0;
}

void * link_list_get_current_and_next(link_list_t *link_list)
{
	if (link_list->current)
	{
		link_list_data_t *tmp = link_list->current;
		link_list->current = link_list->current->prev;
		return tmp->data;
	}
	return 0;
	
}

void *  link_list_set_current_to_tail(link_list_t *link_list)
{
	link_list->current=link_list->tail;
	if (link_list->current)
		return link_list->current->data;
	else
		return 0;
}

void * link_list_get_current_and_prev(link_list_t *link_list)
{
	if (link_list->current)
	{
		link_list_data_t *tmp = link_list->current;
		link_list->current = link_list->current->next;
		return tmp->data;
	}
	return 0;
}


/**
 * @created 05/21/15
 * @brief   converts string list to token string as the following: aa.bb.cc 
 * 	    works only for (char *) types.   
 * @param   
 * @return  
 */
char *link_list_covert_to_string(link_list_t *link_list,char *sep)
{
	char *str;
	if (link_list->list_len==0)
		return 0;
	// find left length
	int len =0;
	link_list_for_each(str,link_list)
		len+=strlen(str);
	str = (char*)malloc(len);


	// copy values to strings
	str[0]=0;
	link_list_for_each(char *tmp_str,link_list) {
		strcat (str,tmp_str);
		strcat (str,sep);
	}
	str[strlen(str)-1]=0;
	return str;

}

void link_list_deinit(link_list_t *link_list)
{
	while (link_list->list_len) {
		link_list_get_head(link_list);
	}
}
