#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */

typedef struct stack stack_t;

/*
StackCreate
description: Creates a new stack_t instance. 
input: wanted capacity of new stack, the size of each element in the stack in bytes
return: a pointer to the newly created stack
time complexity: O(1)
space complexity: O(n)

*/
stack_t *StackCreate(size_t capacity, size_t element_size);

/*
StackDestroy
description: destroys entire stack.
input: a pointer to the stack
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
void StackDestroy(stack_t *stack);

/*
StackPop
description: Removes the top element of the stack, decrements size.
input: a pointer to the stack
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
void StackPop(stack_t *stack);

/*
StackPush
description: add new element to the top of the stack and increments size.
input: a pointer to the stack and a pointer to the new element
return: nothing
time complexity: O(1)
space complexity: O(1)

*/
void StackPush(stack_t *stack, void *new_element);

/*
StackIsEmpty
description: Checks if the stack is empty.
input: a stack
return: 1 for empty stack, 0 otherwise
time complexity: O(1)
space complexity: O(1)

*/
int StackIsEmpty(const stack_t *stack);

/*
StackGetSize
description: returns the number of element currently in the stack.
input: a stack
return: the number of element currently in the stack.
time complexity: O(1)
space complexity: O(1)

*/
size_t StackGetSize(const stack_t *stack);

/*
StackGetCapacity
description: returns the maximum size allowed for the stack.
input: a stack
return: the maximum size allowed for the stack
time complexity: O(1)
space complexity: O(1)

*/
size_t StackGetCapacity(const stack_t *stack);

/*
StackPeek
description: Returns the top element of the stack without removing it.
input: a stack
return: a pointer to the top element
time complexity: O(1)
space complexity: O(1)

*/
void *StackPeek(const stack_t *stack);


#endif
