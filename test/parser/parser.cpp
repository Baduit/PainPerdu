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

template <typename T>
bool operator==(const PainPerdu::Annotation& a, const T& b)
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
	
	expect((defs.recipe.size() == 4) >> fatal);
	expect(defs.recipe[0] == PainPerdu::instructions::MoveRight{1});
	expect(defs.recipe[1] == PainPerdu::instructions::MoveLeft{1});
	expect(defs.recipe[2] == PainPerdu::instructions::Increment{255});
	expect(defs.recipe[3] == PainPerdu::instructions::Decrement{1});
}

void all()
{
	std::string_view code = ":label >1 <1 +255 -1 #ref @ref *label ? ?5 !aa [ ] $lol .sauce >12";
	PainPerdu::Parser parser;
	auto defs = parser(code);

	expect((defs.annotations.size() == 1) >> fatal);
	expect(defs.annotations[0] == PainPerdu::annotations::DefineLabel{"label", 0});

	expect((defs.recipe.size() == 15) >> fatal);
	expect(defs.recipe[0] == PainPerdu::instructions::MoveRight{1});
	expect(defs.recipe[1] == PainPerdu::instructions::MoveLeft{1});
	expect(defs.recipe[2] == PainPerdu::instructions::Increment{255});
	expect(defs.recipe[3] == PainPerdu::instructions::Decrement{1});
	expect(defs.recipe[4] == PainPerdu::instructions::DefineReference{"ref"});
	expect(defs.recipe[5] == PainPerdu::instructions::MoveToReference{"ref"});
	expect(defs.recipe[6] == PainPerdu::instructions::GoToLabel{"label"});
	expect(defs.recipe[7] == PainPerdu::instructions::IfCurrentValueEquals0{});
	expect(defs.recipe[8] == PainPerdu::instructions::IfCurrentValueEqualsN{5});
	expect(defs.recipe[9] == PainPerdu::instructions::IfCursorIsAtReference{"aa"});
	expect(defs.recipe[10] == PainPerdu::instructions::GetChar{});
	expect(defs.recipe[11] == PainPerdu::instructions::PutChar{});
	expect(defs.recipe[12] == PainPerdu::instructions::IfReferenceExists{"lol"});
	expect(defs.recipe[13] == PainPerdu::instructions::UndefineReference{"sauce"});
	expect(defs.recipe[14] == PainPerdu::instructions::MoveRight{12});
}

int main()
{
	"basic"_test = basic;
	"all"_test = all;
}