#include <iostream>
#include <utility>
#include <array>

#include <boost_ut/ut.hpp>

#include <PainPerdu/parser/Patterns/PatternMatchHelper.hpp>

using namespace boost::ut;

using namespace PainPerdu::parser::patterns;

void basic_success()
{
	auto list = {1, 2, 3, 4, 5};
	bool res = match(list.begin(), list.end(), 1, 2, 3);
	expect(res) << "basic_success";
}

void fit_success()
{
	auto list = {1, 2, 3};
	bool res = match(list.begin(), list.end(), 1, 2, 3);
	expect(res) << "fit_success";
}


void basic_failure()
{
	auto list = {1, 2, 3, 4, 5};
	bool res = match(list.begin(), list.end(), 1, 2, 4);
	expect(!res) << "basic_failure";
}

void pattern_too_big()
{
	auto list = {1, 2, 3};
	bool res = match(list.begin(), list.end(), 1, 2, 3, 4);
	expect(!res) << "pattern_too_big";
}

void failure_empty_range()
{
	std::array<int, 0> list;
	bool res = match(list.begin(), list.end(), 1, 2);
	expect(!res) << "failure_empty_range";
}

int main()
{
	"basic_success"_test = basic_success;
	"fit_success"_test = fit_success;
	"basic_failure"_test = basic_failure;
	"pattern_too_big"_test = pattern_too_big;
	"failure_empty_range"_test = failure_empty_range;
}