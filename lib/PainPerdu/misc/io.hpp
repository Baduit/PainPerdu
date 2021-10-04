#pragma once

#include <iostream>


template <typename ...Args>
void print(Args&&... args)
{
	(std::cout << ... << args);
}

template <typename ...Args>
void print_and_flush(Args&&... args)
{
	print(args...);
	std::cout.flush();
}

// Note : it also flushes
template <typename ...Args>
void println(Args&&... args)
{
	(std::cout << ... << args) << std::endl;
}
