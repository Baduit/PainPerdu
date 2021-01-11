#include <iostream>
#include <utility>
#include <array>

#include <boost_ut/ut.hpp>

#include <PainPerdu/parser/Parser.hpp>

using namespace boost::ut;

template <typename T>
bool operator==(const PainPerdu::Instruction& a, const T& b)
{
	if (!std::holds_alternative<T>(a))
		return false;
	
	return std::get<T>(a) == b;
}

void basic()
{
	std::string_view code = ">1 <1+255\t-1";
	PainPerdu::Parser parser;
	auto defs = parser(code);
	
	std::cout << defs.instructions.size() << std::endl;
	expect((defs.instructions.size() == 4) >> fatal);
	expect(defs.instructions[0] == PainPerdu::instructions::MoveRight{1});
	expect(defs.instructions[1] == PainPerdu::instructions::MoveLeft{1});
	expect(defs.instructions[2] == PainPerdu::instructions::Increment{255});
	expect(defs.instructions[3] == PainPerdu::instructions::Decrement{1});
}

int main()
{
	"basic"_test = basic;
}