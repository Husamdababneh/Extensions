/*
*
*	Dll does not need an entry point
*
*	@Note: Since the 
*/

#include <iostream>
#include <extensions_functions.h>


int update(int a, int b) {
	return a + b;
}

void init() {
	std::cout << "Init\n";
}

void destroy() {
	std::cout << "Destroy\n";
}

