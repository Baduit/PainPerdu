#pragma once

enum class Result
{
	SUCCESS,
	FAILURE
};

inline Result operator&&(const Result& a, const Result& b)
{
	if (a == Result::SUCCESS && b == Result::SUCCESS)
		return Result::SUCCESS;
	else
		return Result::FAILURE;
}

inline Result operator||(const Result& a, const Result& b)
{
	if (a == Result::SUCCESS || b == Result::SUCCESS)
		return Result::SUCCESS;
	else
		return Result::FAILURE;
}