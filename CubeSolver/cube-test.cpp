/** 
 * \file cube-test.cpp
 */

#include "cube.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

int main()
{
	Cube::initChoose();
	
	clock_t t;
	t = clock();

	Cube::test();

	std::cout << std::endl << std::endl;

	t = clock() - t;
	std::cout << "time: " << (float)t / CLOCKS_PER_SEC << std::endl;

	std::cout << std::endl << std::endl;
	
	std::system("pause");
	
	return 0;
}