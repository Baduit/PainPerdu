#pragma once

#include <random>

struct RandomEngine
{
	RandomEngine():
		_mt(_rd())
	{}

	std::size_t operator()(std::size_t max_value_included)
	{
		std::uniform_int_distribution<std::size_t> dist(0, max_value_included);
		return dist(_mt);
	}

	std::random_device	_rd;
	std::mt19937		_mt;
};

struct Random
{
	static std::size_t rand(std::size_t max_value_included)
	{
		return random_engine(max_value_included);
	}
	
	inline static RandomEngine random_engine = {};
};
