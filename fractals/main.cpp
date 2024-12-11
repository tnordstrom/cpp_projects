/* Tony Nordstrom */
/* December 11 02024 */
/* Project name: Fractals */
/* Project path: /cygdrive/c/dev/c++/fractals */
/* To build: g++ -Wall -o "main" "main.cpp" */
/* To run: ./main.exe */
/* 
 Version 0.1
 - 
 */

#include <iostream>

double calc_iteration(double z, double c)
{
	double retval{ 0.0 };
	
	retval = (z * z) + c;
	
	return(retval);
}

int main(void)
{
	return(0);
}
