/*
 * =====================================================================================
 *
 *       Filename:  stack.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/30/15 16:31:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "stack.h"
#include <stdlib.h>
#include "string.h"

parser_stack_t *stack_new(parser_stack_t **stack,int max_stack_size)
{
	parser_stack_t *p = (parser_stack_t*)malloc(sizeof(parser_stack_t));
	stack[0] = p;
	p->stack = malloc(max_stack_size);
	p->sp = 0;
	return stack[0];

}

void stack_push(parser_stack_t *stack,void *value,int size)
{
	stack->sp += size;
	memcpy (stack->stack+stack->sp , value,size);
}

void *stack_pop(parser_stack_t *stack,int size)
{
	void *p = stack->stack+stack->sp;
	stack->sp-=size;
	return p;
}
void *stack_get_top(parser_stack_t *stack,int size)
{
	void *p = stack->stack+stack->sp;
	return p;
}

void stack_free(parser_stack_t *stack)
{
	free(stack->stack);
	free(stack);
}

