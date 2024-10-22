/*
	Code By: Amit Ellenbogen
	Review By: Shahar Marom
	Date: 26.6.24
	Project: Calculator
*/
#include <stdlib.h> /* strtod() */
#include <ctype.h> /* isspace() */
#include <math.h> /* pow() */


#include "calculator.h"
#include "stack.h"

#define NUM_OF_VALID_STATES 2
#define ASCII_RANGE 128
#define STACK_SIZE 100
#define NUM_OF_OPS 5
#define UNUSED(x) (void)(x)

/***************************************************************************
|								Type Defenitions:							|
|	State - an enum for identifying which handler should be called.			|
|	Handler - a function that executes the action triggered by the signal.	|
|	Operator - a function that recieves 2 numbers and returns a number		|
****************************************************************************/

typedef enum state 
{
	S0_WAIT_FOR_NUMBER = 0,
	S1_WAIT_FOR_OP = 1,
	S2_SYNTAX_ERROR = 2,
	S3_MATH_ERROR = 3,
	S4_DONE = 4,
	S5_DO_NOTHING = 5
} e_state;

typedef e_state (*handler_t)(char **str_ptr,
							stack_t *number_stack,
							stack_t *op_stack);
							
typedef double (*operator_t)(double number1, double number2);

/***************************************************************************
|	There are 12 handlers in this implementation.							|
|	Each one executes an action relevant to its trigger, checks for 		|
|	possible errors and returns the correspondent state.					|
****************************************************************************/

static e_state HandleOpenParenthesis(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleNumber(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);


static e_state HandleAdd(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleSubtract(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleMultiply(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleDivide(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandlePower(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleCloseParenthesis(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);


static e_state HandleIgnore(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleExecute(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleError(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);
static e_state HandleEnd(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack);

/***************************************************************************
|	There are 5 operators in this implementation.							|
|	Each one executes an operation relevant to its trigger and returns a	|
|	result.																	|
****************************************************************************/

static double Add(double number1, double number2);
static double Subtract(double number1, double number2);
static double Multiply(double number1, double number2);
static double Divide(double number1, double number2);
static double Power(double number1, double number2);

/***************************************************************************
|	The state machine handler is responsible for calling the current state	|
|	and transition it to the next state.							 		|
****************************************************************************/

static e_state StateMachineHandler(char **str_ptr, 
									e_state curr_state, 
									stack_t *number_stack,
									stack_t *op_stack);
									
static int IsStateValid(e_state state);
static int CreateStacks(stack_t **number_stack, stack_t **op_stack);

/***************************************************************************
|	The calculation was successfull if it executed every operation in the	|
|	correct order without returning any error and only one number is left	|
|	in the number stack.													|
****************************************************************************/
static int IsSuccessful(stack_t *number_stack, stack_t *op_stack);

/***************************************************************************
|	The handler look up table is responsible for calling the correspondent	|
|	action for every state and signal.										|
****************************************************************************/
static handler_t handlerLUT[NUM_OF_VALID_STATES][ASCII_RANGE] = {NULL};
static void InitializerLUTs(void);

/***************************************************************************
|	The IsPriorityHigher function recieves the operator of the handler and	|
|	the operator from the top of the stack, returns 1 if the stack_op	 	|
|	should be executed before pushing the handler_op to the stack_op		|
****************************************************************************/
typedef enum operator 
{
	ADD = 1,
	SUB = 1,
	MULT = 2,
	DIV = 2,
	POW = 3
} e_operator;

typedef struct priority_table_row 
{
	operator_t operator;
	e_operator priority;
} priority_row_t;

static priority_row_t operatorLUT[ASCII_RANGE] = {{NULL, 0}};

static int IsPriorityHigher(unsigned char handler_op, unsigned char stack_op);

/***************************************************************************
|	The GetStatus function recieves the current state and returns the 		|
|	status that should be returned in a case of an error.					| 
****************************************************************************/
static e_status_t GetStatus(e_state curr_state);

/***************************************************************************
|	The Calculate function initializes the handler LUT, creates the stacks	|
|	and calls the state	machine handler until it reaches an invalid state.	|
|	when the event loop is done the function checks the correct status to	|
|	return.																	|
****************************************************************************/

static e_state starting_state = S0_WAIT_FOR_NUMBER;
static char dummy = '\0';

e_status_t Calculate(const char *expression, double *result)
{
	char **str_ptr = (char **)&expression;
	e_state curr_state = starting_state;
	e_status_t status = CALC_SUCCESS;
	
	stack_t *number_stack = NULL;
	stack_t *op_stack = NULL;
	
	if (CreateStacks(&number_stack, &op_stack))
	{
		return CALC_MALLOC_ERROR;
	}
	
	InitializerLUTs();
	
	while (IsStateValid(curr_state))
	{
		curr_state = StateMachineHandler(str_ptr, 
										curr_state, 
										number_stack, 
										op_stack);
	}
	
	status = GetStatus(curr_state);
	
	if (status != CALC_SUCCESS)
	{
		StackDestroy(number_stack);
		StackDestroy(op_stack);
		return status;
	}
	
	*result = *(double *)StackPeek(number_stack);
	StackDestroy(number_stack);
	StackDestroy(op_stack);
	return CALC_SUCCESS;
}

/**************************STATIC HELPER FUNCTIONS**************************/

static e_state StateMachineHandler(char **str_ptr, 
									e_state curr_state, 
									stack_t *number_stack,
									stack_t *op_stack)
{
	e_state new_state = handlerLUT[curr_state][(unsigned)**str_ptr]
										(str_ptr, number_stack, op_stack);
	
	if (new_state == S5_DO_NOTHING)
	{
		return curr_state;
	}
	
	return new_state;
}


static int IsSuccessful(stack_t *number_stack, stack_t *op_stack)
{
	return (1 == StackGetSize(number_stack) && 1 == StackGetSize(op_stack));
}

static int IsStateValid(e_state state)
{
	return (state == S0_WAIT_FOR_NUMBER || state == S1_WAIT_FOR_OP);
}

static void InitializerLUTs(void)
{
	size_t i = 0;
	size_t j = 0;
	
	if(handlerLUT[i][j])
	{
		return;
	}
	
	/* init handler LUT */
	for (i = 0; i < NUM_OF_VALID_STATES; ++i) 
	{
		for (j = 0; j < ASCII_RANGE; ++j) 
		{
			if (isspace(j))
			{
				handlerLUT[i][j] = HandleIgnore;
			}
			else
			{
				handlerLUT[i][j] = HandleError;
			}
		}
	}

	handlerLUT[S0_WAIT_FOR_NUMBER]['('] = HandleOpenParenthesis;
	for (j = '0'; j <= '9'; ++j) 
	{
		handlerLUT[S0_WAIT_FOR_NUMBER][j] = HandleNumber;
	}
	handlerLUT[S0_WAIT_FOR_NUMBER]['+'] = HandleNumber;
	handlerLUT[S0_WAIT_FOR_NUMBER]['-'] = HandleNumber;

	handlerLUT[S1_WAIT_FOR_OP]['+'] = HandleAdd;
	handlerLUT[S1_WAIT_FOR_OP]['-'] = HandleSubtract;
	handlerLUT[S1_WAIT_FOR_OP]['*'] = HandleMultiply;
	handlerLUT[S1_WAIT_FOR_OP]['/'] = HandleDivide;
	handlerLUT[S1_WAIT_FOR_OP]['^'] = HandlePower;
	handlerLUT[S1_WAIT_FOR_OP][')'] = HandleCloseParenthesis;
	handlerLUT[S1_WAIT_FOR_OP]['\0'] = HandleEnd;
	
	/* init operator LUT */
	for (j = 0; j < ASCII_RANGE; ++j) 
	{
		operatorLUT[j].operator = NULL;
		operatorLUT[j].priority = 0;
	}
	
	operatorLUT['+'].operator = Add;
	operatorLUT['+'].priority = ADD;
	operatorLUT['-'].operator = Subtract;
	operatorLUT['-'].priority = SUB;
	operatorLUT['*'].operator = Multiply;
	operatorLUT['*'].priority = MULT;
	operatorLUT['/'].operator = Divide;
	operatorLUT['/'].priority = DIV;
	operatorLUT['^'].operator = Power;
	operatorLUT['^'].priority = POW;

}

static int IsPriorityHigher(unsigned char handler_op, unsigned char stack_op)
{
	int handler_priority = operatorLUT[handler_op].priority;
	int stack_priority = operatorLUT[stack_op].priority;

	return (stack_priority >= handler_priority);
}

static e_state HandleOperator(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	while (IsPriorityHigher(**str_ptr, *(char *)StackPeek(op_stack)))
	{
		if (HandleExecute(str_ptr, number_stack, op_stack) == S3_MATH_ERROR)
		{
			return S3_MATH_ERROR;
		}
	}
	StackPush(op_stack, *str_ptr);
	
	++(*str_ptr);
	
	return S0_WAIT_FOR_NUMBER;
}


static e_status_t GetStatus(e_state curr_state)
{
	switch(curr_state)
	{
		case S2_SYNTAX_ERROR:
			return CALC_SYNTAX_ERROR;
		case S3_MATH_ERROR:
			return CALC_MATH_ERROR;
		default:
			return CALC_SUCCESS;
	}
}
			

/*****************************WAIT FOR NUM HANDLERS****************************/

static e_state HandleOpenParenthesis(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	UNUSED(number_stack);
	
	StackPush(op_stack, *str_ptr);
	++(*str_ptr);
	
	return S0_WAIT_FOR_NUMBER;
}

static e_state HandleNumber(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	char *curr_str = *str_ptr;
	double number = strtod(curr_str, str_ptr);

	UNUSED(op_stack);
	StackPush(number_stack, &number);

	return S1_WAIT_FOR_OP;
}

/*****************************WAIT FOR OP HANDLERS****************************/

static e_state HandleAdd(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	return HandleOperator(str_ptr, number_stack, op_stack);
}

static e_state HandleSubtract(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	return HandleOperator(str_ptr, number_stack, op_stack);
}

static e_state HandleMultiply(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	return HandleOperator(str_ptr, number_stack, op_stack);
}

static e_state HandleDivide(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	return HandleOperator(str_ptr, number_stack, op_stack);
}

static e_state HandlePower(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	(void)number_stack;
	StackPush(op_stack, *str_ptr);
	++(*str_ptr);
	return S0_WAIT_FOR_NUMBER;
}

static e_state HandleCloseParenthesis(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	char op = *(char *)StackPeek(op_stack);
	while (op != '(' && op != dummy)
	{
		if (HandleExecute(str_ptr, number_stack, op_stack) == S3_MATH_ERROR)
		{
			return S3_MATH_ERROR;
		}
		op = *(char *)StackPeek(op_stack);
	}

	if (*(char *)StackPeek(op_stack) == '(')
	{
		StackPop(op_stack);
	}
	else
	{
		return S2_SYNTAX_ERROR;
	}

	++(*str_ptr);
	return S1_WAIT_FOR_OP;
}

/*****************************GENERAL HANDLERS****************************/

static e_state HandleIgnore(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	++(*str_ptr);
	UNUSED(number_stack);
	UNUSED(op_stack);
	
	return S5_DO_NOTHING;
}

static e_state HandleError(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	UNUSED(str_ptr);
	UNUSED(number_stack);
	UNUSED(op_stack);
	
	return S2_SYNTAX_ERROR;
}


static e_state HandleExecute(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	double number1 = 0;
	double number2 = 0;
	unsigned char operator = dummy;

	if (IsSuccessful(number_stack, op_stack))
	{
		return S4_DONE;
	}
	
	number2 = *(double *)StackPeek(number_stack);
	StackPop(number_stack);
	number1 = *(double *)StackPeek(number_stack);
	StackPop(number_stack);

	operator = *(unsigned char *)StackPeek(op_stack);
	StackPop(op_stack);
	
	if ('/' == operator && 0 == number2)
	{
		return S3_MATH_ERROR;
	}
	
	if (!operatorLUT[operator].operator)
	{
		return S2_SYNTAX_ERROR;
	}
	
	number1 = operatorLUT[operator].operator(number1,number2);

	StackPush(number_stack, &number1);

	if (**str_ptr == '\0')
	{
		return S4_DONE;
	}

	return S5_DO_NOTHING;
}

static e_state HandleEnd(char **str_ptr, 
									stack_t *number_stack, 
									stack_t *op_stack)
{
	e_state curr_state = S4_DONE;
	while (!IsSuccessful(number_stack, op_stack) && GetStatus(curr_state) == CALC_SUCCESS)
	{
		curr_state = HandleExecute(str_ptr, number_stack, op_stack);
	}
	
	return curr_state;
}

static int CreateStacks(stack_t **number_stack, stack_t **op_stack)
{
	*number_stack = StackCreate(STACK_SIZE, sizeof(double));
	if (!*number_stack)
	{
		return 1;
	}
	
	*op_stack = StackCreate(STACK_SIZE, sizeof(char));
	if (!*op_stack)
	{
		StackDestroy(*number_stack);
		return 1;
	}
	
	StackPush(*op_stack, &dummy);
	return 0;
}


static double Add(double number1, double number2)
{
	return (number1+number2);
}
static double Subtract(double number1, double number2)
{
	return (number1-number2);
}
static double Multiply(double number1, double number2)
{
	return (number1*number2);
}
static double Divide(double number1, double number2)
{
	return (number1/number2);
}
static double Power(double number1, double number2)
{
	return pow(number1,number2);
}




