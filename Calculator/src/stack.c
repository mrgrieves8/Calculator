#include <stdlib.h> /* size_t(), malloc(), free() */
#include <stdio.h> /* printf() */
#include <assert.h> /* assert() */

#include "stack.h"

struct stack
{
    size_t capacity;
    size_t size;
    size_t element_size;
    char *buffer;
};

stack_t *StackCreate(size_t capacity, size_t element_size)
{
	size_t buffer_size = sizeof(char) * capacity * element_size;
	stack_t *stack_ptr = (stack_t *)malloc(sizeof(stack_t) + buffer_size);
	
	if(NULL == stack_ptr)
	{
		return NULL;
	}
	
	stack_ptr->capacity = capacity;
	stack_ptr->element_size = element_size;
	stack_ptr->size = 0;
	stack_ptr->buffer = (char *)stack_ptr + sizeof(stack_t);
	
	return stack_ptr;
}

void StackDestroy(stack_t *stack)
{
	assert(stack);
	free(stack);
}

void StackPop(stack_t *stack)
{
	assert(stack);
	assert(!StackIsEmpty(stack));
	--(stack->size);
}

void StackPush(stack_t *stack, void *new_element)
{
	char *temp_p = (char *)new_element;
	size_t i = 0;
	size_t new_index = stack->size * stack->element_size;
	
	assert(stack);
	assert(stack->size < stack->capacity);
	
	for(; i < stack->element_size; ++i , ++temp_p)
	{
		stack->buffer[new_index + i] = *temp_p;
	}

	++(stack->size);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(stack);
	return (stack->size == 0);
}

size_t StackGetSize(const stack_t *stack)
{
	assert(stack);
	return stack->size;
}

size_t StackGetCapacity(const stack_t *stack)
{
	assert(stack);
	return stack->capacity;
}

void *StackPeek(const stack_t *stack)
{
	size_t peek_index = (stack->size - 1) * stack->element_size;
	assert(stack);
	assert(!StackIsEmpty(stack));
	return stack->buffer+peek_index;
}

