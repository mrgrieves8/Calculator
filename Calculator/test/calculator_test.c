#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */
#include <stdlib.h> /* free() */
#include <assert.h>

#include "calculator.h"

#define TESTNUM 25

static char failed_tests_print[300] = {'\0'};

static void AddFailedTest(const char *str);

static int TestCalculator(void);

int main(void)
{
	int failed_tests_num = 0;
	
	failed_tests_num += TestCalculator();
	printf("Tested Calculator\n");
	
	if (failed_tests_num)
	{
		printf("%d out %d tests failed\nFailed tests:\n%s"
		, failed_tests_num, TESTNUM, failed_tests_print);
		return failed_tests_num;
	}
	
	printf("All Tests Passed!\n");
	
	return 0;
}

static void AddFailedTest(const char *str)
{
	strcat(failed_tests_print, str);
}

static int TestCalculator(void)
{
	double result = 0;
	int status = 0;
	int num_failed = 0;
	char str1[100] = "7+8";
	char str2[100] = "8+8*3+-2^5";
	char str3[100] = "8+8*3-2^";
	char str4[100] = "2/0";
	char str5[100] = "8++8*((3-2)*5)";
	char str6[100] = "3-2)*5";
	char str7[100] = "(3-2)*5+ 5*(4+4+4";
	char str8[100] = "4-3-2-1\n";
	char str9[100] = "4^3^2^1\n";
	char str10[100] = "1+1*";
	char str11[100] = "2^3^2"; 
	char str12[100] = "1-2-3"; 
	char str13[100] = "2^0";   
	char str14[100] = "0^0";
	char str15[100] = "2^1";
	char str16[100] = "(2+3)*4";
	char str17[100] = "2*(3+4*2)/2";
	char str18[100] = "5/2";
	char str19[100] = "5.5*2";
	char str20[100] = "-2+3";
	char str21[100] = "3+-2";
	char str22[100] = "4/2+2";
	char str23[100] = "(2+3)*(4-1)";
	char str24[100] = "10/(5-5)"; 
	char str25[100] = "((2+3)*4)"; 

	/* Test case 1 */
	status = Calculate(str1, &result);
	if (CALC_SUCCESS != status || result < 14.99 || result > 15.01)
	{
		AddFailedTest("TestCalculator1\n");
		++num_failed;
	}

	/* Test case 2 */
	status = Calculate(str2, &result);
	if (CALC_SUCCESS != status || result < -0.01 || result > 0.01)
	{
		AddFailedTest("TestCalculator2\n");
		++num_failed;
	}

	/* Test case 3 */
	status = Calculate(str3, &result);
	if (CALC_SYNTAX_ERROR != status)
	{
		AddFailedTest("TestCalculator3\n");
		++num_failed;
	}

	/* Test case 4 */
	status = Calculate(str4, &result);
	if (CALC_MATH_ERROR != status)
	{
		AddFailedTest("TestCalculator4\n");
		++num_failed;
	}

	/* Test case 5 */
	status = Calculate(str5, &result);
	if (CALC_SUCCESS != status || result < 47.99 || result > 48.01)
	{
		AddFailedTest("TestCalculator5\n");
		++num_failed;
	}

	/* Test case 6 */
	status = Calculate(str6, &result);
	if (CALC_SYNTAX_ERROR != status)
	{
		AddFailedTest("TestCalculator6\n");
		++num_failed;
	}

	/* Test case 7 */
	status = Calculate(str7, &result);
	if (CALC_SYNTAX_ERROR != status)
	{
		AddFailedTest("TestCalculator7\n");
		++num_failed;
	}

	/* Test case 8 */
	status = Calculate(str8, &result);
	if (CALC_SUCCESS != status || result < -2.01 || result > -1.99)
	{
		AddFailedTest("TestCalculator8\n");
		++num_failed;
	}

	/* Test case 9 */
	status = Calculate(str9, &result);
	if (CALC_SUCCESS != status || result < 262143.99 || result > 262144.01)
	{
		AddFailedTest("TestCalculator9\n");
		++num_failed;
	}

	/* Test case 10 */
	status = Calculate(str10, &result);
	if (CALC_SYNTAX_ERROR != status)
	{
		AddFailedTest("TestCalculator10\n");
		++num_failed;
	}

	/* Test case 11 */
	status = Calculate(str11, &result);
	if (CALC_SUCCESS != status || result < 511.99 || result > 512.01)
	{
		AddFailedTest("TestCalculator11\n");
		++num_failed;
	}

	/* Test case 12 */
	status = Calculate(str12, &result);
	if (CALC_SUCCESS != status || result < -4.01 || result > -3.99)
	{
		AddFailedTest("TestCalculator12\n");
		++num_failed;
	}

	/* Test case 13 */
	status = Calculate(str13, &result);
	if (CALC_SUCCESS != status || result < 0.99 || result > 1.01)
	{
		AddFailedTest("TestCalculator13\n");
		++num_failed;
	}

	/* Test case 14 */
	status = Calculate(str14, &result);
	if (CALC_SUCCESS != status || result != 1)
	{
		AddFailedTest("TestCalculator14\n");
		++num_failed;
	}

	/* Test case 15 */
	status = Calculate(str15, &result);
	if (CALC_SUCCESS != status || result < 1.99 || result > 2.01)
	{
		AddFailedTest("TestCalculator15\n");
		++num_failed;
	}

	/* Test case 16 */
	status = Calculate(str16, &result);
	if (CALC_SUCCESS != status || result < 19.99 || result > 20.01)
	{
		AddFailedTest("TestCalculator16\n");
		++num_failed;
	}

	/* Test case 17 */
	status = Calculate(str17, &result);
	if (CALC_SUCCESS != status || result < 10.99 || result > 11.01)
	{
		AddFailedTest("TestCalculator17\n");
		++num_failed;
	}

	/* Test case 18 */
	status = Calculate(str18, &result);
	if (CALC_SUCCESS != status || result < 2.49 || result > 2.51)
	{
		AddFailedTest("TestCalculator18\n");
		++num_failed;
	}

	/* Test case 19 */
	status = Calculate(str19, &result);
	if (CALC_SUCCESS != status || result < 10.99 || result > 11.01)
	{
		AddFailedTest("TestCalculator19\n");
		++num_failed;
	}

	/* Test case 20 */
	status = Calculate(str20, &result);
	if (CALC_SUCCESS != status || result < 0.99 || result > 1.01)
	{
		AddFailedTest("TestCalculator20\n");
		++num_failed;
	}

	/* Test case 21 */
	status = Calculate(str21, &result);
	if (CALC_SUCCESS != status || result < 0.99 || result > 1.01)
	{
		AddFailedTest("TestCalculator21\n");
		++num_failed;
	}

	/* Test case 22 */
	status = Calculate(str22, &result);
	if (CALC_SUCCESS != status || result < 3.99 || result > 4.01)
	{
		AddFailedTest("TestCalculator22\n");
		++num_failed;
	}

	/* Test case 23 */
	status = Calculate(str23, &result);
	if (CALC_SUCCESS != status || result < 14.99 || result > 15.01)
	{
		AddFailedTest("TestCalculator23\n");
		++num_failed;
	}

	/* Test case 24 */
	status = Calculate(str24, &result);
	if (CALC_MATH_ERROR != status)
	{
		AddFailedTest("TestCalculator24\n");
		++num_failed;
	}

	/* Test case 25 */
	status = Calculate(str25, &result);
	if (CALC_SUCCESS != status || result < 19.99 || result > 20.01)
	{
		AddFailedTest("TestCalculator25\n");
		++num_failed;
	}

	return num_failed;
}







