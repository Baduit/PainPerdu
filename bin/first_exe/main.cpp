#include <iostream>

#include <my_lib/my_lib.hpp>

int main()
{
	auto i = my_lib::add(9, 4);
	std::cout << "The result is: " << i << std::endl;
}