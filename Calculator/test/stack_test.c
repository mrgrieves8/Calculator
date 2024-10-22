#include <assert.h> /* assert() */
#include <stdio.h> /* printf() */

#include "stack.h"

int main(void)
{
	char five = '5';
	char four = '4';
	char x = '0';
	stack_t *c_stack = StackCreate(10,sizeof(char));
	
	int six = 6;
	int seven = 7;
	int y = 0;
	stack_t *i_stack = StackCreate(15,sizeof(int));
	
	assert(c_stack);
	assert(i_stack);
	
	assert(0 == StackGetSize(c_stack));
	assert(0 == StackGetSize(i_stack));
	
	StackPush(c_stack, &five);
	assert(StackGetSize(c_stack) == 1);
	StackPush(i_stack, &six);
	assert(StackGetSize(i_stack) == 1);
	
	x = *(char *)StackPeek(c_stack);
	assert('5' == x);
	y = *(int *)StackPeek(i_stack);
	assert(6 == y);
	
	StackPush(c_stack, &four);
	assert(StackGetSize(c_stack) == 2);
	StackPush(i_stack, &seven);
	assert(StackGetSize(i_stack) == 2);
	
	x = *(char *)StackPeek(c_stack);
	assert('4' == x);
	y = *(int *)StackPeek(i_stack);
	assert(7 == y);
	
	StackPop(c_stack);
	assert(StackGetSize(c_stack) == 1);
	StackPop(i_stack);
	assert(StackGetSize(i_stack) == 1);
	
	x = *(char *)StackPeek(c_stack);
	assert('5' == x);
	y = *(int *)StackPeek(i_stack);
	assert(6 == y);
	
	assert(10 == StackGetCapacity(c_stack));
	assert(15 == StackGetCapacity(i_stack));
	
	assert(0 == StackIsEmpty(c_stack));
	assert(0 == StackIsEmpty(i_stack));
	StackPop(c_stack);
	StackPop(i_stack);
	assert(1 == StackIsEmpty(c_stack));
	assert(1 == StackIsEmpty(i_stack));
	
	StackDestroy(c_stack);
	StackDestroy(i_stack);
	printf("ALL TESTS PASSED!\n");
	return 0;
}
