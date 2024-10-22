#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

typedef enum status
{
	CALC_SUCCESS,
	CALC_MATH_ERROR,
	CALC_SYNTAX_ERROR,
	CALC_MALLOC_ERROR
} e_status_t;


/*
Description: Calculate the expression recieved as string input.
Input: String expression to be calculated, a double pointer to recieve the result.
Return: Enum represents the status of the calculation.
Time complexity: O(n)
Space complexity: O(n)
Details:
    Can proccess:
        -Decimal point
        -Unaric operators
        -Power (support as well: fractional powers, and negative powers)
        -Parentheses
        -White spaces
    Examples:
    	char str[100] = "7+8"; 				  Result = 15 status = CALC_SUCCESS 
    	char str2[100] = "8+8*3+-2^5";		  Result =  0 status = CALC_SUCCESS	  
    	char str3[100] = "8+8*3-2^";		  Result =  0 status = CALC_SYNTAX_ERROR  
    	char str4[100] = "2/0";				  Result =  0 status = CALC_MATH_ERROR    
    	char str5[100] = "8++8*((3-2)*5)";	  Result = 48 status = CALC_SUCCESS 	   
    	char str6[100] = "3-2)*5";			  Result =  0 status = CALC_SYNTAX_ERROR  
    	char str7[100] = "(3-2)*5+ 5*(4+4+4"; Result =  0 status = CALC_SYNTAX_ERROR 
    	char str8[100] = "4-3-2-1\n";		  Result =  -2 status = CALC_SUCCESS  
    	char str9[100] = "4^3^2^1\n";		  Result =  4096 status = CALC_SUCCESS 
*/
e_status_t Calculate(const char *expresion, double *result);
#endif /*__CALCULATOR_H__*/
