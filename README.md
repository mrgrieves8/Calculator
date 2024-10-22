# Calculator Project

## Overview

This project is a simple calculator implemented in C. It evaluates mathematical expressions consisting of numbers, parentheses, and operators such as addition, subtraction, multiplication, division, and exponentiation. It uses a state machine and two stacks (one for numbers and one for operators) to parse and evaluate expressions.

### Key Features
- **State Machine**: Implements a finite state machine to handle various stages of expression parsing and evaluation.
- **Stacks**: Utilizes two stacks to handle operators and numbers during expression evaluation.
- **Operator Precedence**: Correctly handles operator precedence, including parentheses and exponentiation.
- **Error Handling**: Detects and reports syntax errors and mathematical errors (e.g., division by zero).
- **Dynamic Memory Management**: Stacks are dynamically created and destroyed for efficient memory usage.

## Files

- `calculator.c`: Contains the main logic of the calculator, including the state machine, operator handling, and error detection.
- `stack.c`: Implements a stack data structure used to store numbers and operators during evaluation.
- `calculator.h`: Header file for the calculator API.
- `stack.h`: Header file for the stack data structure.

## How It Works

### State Machine

The calculator operates as a state machine with several states:
- `S0_WAIT_FOR_NUMBER`: Waiting for a number or an opening parenthesis.
- `S1_WAIT_FOR_OP`: Waiting for an operator or a closing parenthesis.
- `S2_SYNTAX_ERROR`: A syntax error was detected in the expression.
- `S3_MATH_ERROR`: A mathematical error was detected (e.g., division by zero).
- `S4_DONE`: The expression has been successfully evaluated.
- `S5_DO_NOTHING`: No action is required, used to ignore white spaces.

### Handlers

Each state has associated handlers that handle input characters. These handlers are responsible for:
- Reading numbers and pushing them onto the number stack.
- Reading operators and managing operator precedence by pushing operators onto the operator stack.
- Handling parentheses, which alter operator precedence.
- Executing operations by popping two numbers from the number stack and applying the operator from the operator stack.
- Detecting and reporting syntax or mathematical errors.

### Supported Operators

The following mathematical operators are supported:
- `+` (Addition)
- `-` (Subtraction)
- `*` (Multiplication)
- `/` (Division)
- `^` (Exponentiation)

### Error Handling

The calculator checks for and reports two types of errors:
- **Syntax Errors**: Invalid sequences of characters in the expression, such as mismatched parentheses or misplaced operators.
- **Mathematical Errors**: Errors during the execution of mathematical operations, such as division by zero.

## API

### `e_status_t Calculate(const char *expression, double *result)`

The main function of the calculator. It evaluates a mathematical expression provided as a string and returns the result.

- **Parameters**:
  - `expression`: A string containing the mathematical expression to evaluate.
  - `result`: A pointer to a double where the result of the evaluation will be stored.

- **Returns**: 
  - `CALC_SUCCESS`: The expression was successfully evaluated.
  - `CALC_SYNTAX_ERROR`: There was a syntax error in the expression.
  - `CALC_MATH_ERROR`: A mathematical error occurred during evaluation.
  - `CALC_MALLOC_ERROR`: Memory allocation error occurred while creating stacks.

## Example Usage

```c
#include <stdio.h>
#include "calculator.h"

int main(void) {
    double result;
    const char *expression = "3 + 5 * (2 - 8)";

    e_status_t status = Calculate(expression, &result);

    if (status == CALC_SUCCESS) {
        printf("Result: %f\n", result);
    } else {
        printf("Error: %d\n", status);
    }

    return 0;
}
```

### How to Compile

Make sure to include all source files during compilation:

```bash
gcc -ansi -pedantic-errors -Wall -Wextra -g calculator.c stack.c -o calculator
```

