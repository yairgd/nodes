#ifndef STACK_H
#define STACK_H


typedef struct stack_s {
	void *stack;
	int sp;
} parser_stack_t;

parser_stack_t *stack_new(parser_stack_t **stack,int max_stack_size);
void *stack_get_top(parser_stack_t *stack,int size);
void *stack_pop(parser_stack_t *stack,int size);
void stack_push(parser_stack_t *stack,void *value,int size);
void stack_free(parser_stack_t *stack);
#endif
