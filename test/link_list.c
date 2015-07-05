/*
 * =====================================================================================
 *
 *       Filename:  link_list.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/01/2015 04:13:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "link_list.h"

int main()
{
	int i;
	char *a="aa",*b="bb",*c="cc";

	link_list_t link_list;
	link_list_init(&link_list);
	link_list_add_head(&link_list,a);
	link_list_add_head(&link_list,b);
	link_list_add_head(&link_list,c);
	printf("totla list length %d\n-----------------------\n",link_list.list_len);
	for (;link_list.list_len;)
		printf("%s\n",(char*)link_list_get_head(&link_list));
	printf("head and tail pointers and list length %d %d %d\n",link_list.tail,link_list.head,link_list.list_len);
	return 0;
}
