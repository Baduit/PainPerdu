#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>
#include <utility>
#include <array>

#include <PainPerdu/parser/Parser.hpp>

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

TEST_CASE("basic")
{
	std::string_view code = ">1 <1+255\t-1";
	PainPerdu::Parser parser;
	auto defs = parser(code);
	
	REQUIRE(defs.recipe.size() == 4);
	CHECK(defs.recipe[0] == PainPerdu::instructions::MoveRight{1});
	CHECK(defs.recipe[1] == PainPerdu::instructions::MoveLeft{1});
	CHECK(defs.recipe[2] == PainPerdu::instructions::Increment{255});
	CHECK(defs.recipe[3] == PainPerdu::instructions::Decrement{1});
}

TEST_CASE("all")
{
	std::string_view code = ":label >1 <1 +255 -1 #ref @ref *label ? ?5 !aa [ ] $lol .sauce >12";
	PainPerdu::Parser parser;
	auto defs = parser(code);

	REQUIRE(defs.annotations.size() == 1);
	CHECK(defs.annotations[0] == PainPerdu::annotations::DefineLabel{"label", 0});

	REQUIRE(defs.recipe.size() == 15);
	CHECK(defs.recipe[0] == PainPerdu::instructions::MoveRight{1});
	CHECK(defs.recipe[1] == PainPerdu::instructions::MoveLeft{1});
	CHECK(defs.recipe[2] == PainPerdu::instructions::Increment{255});
	CHECK(defs.recipe[3] == PainPerdu::instructions::Decrement{1});
	CHECK(defs.recipe[4] == PainPerdu::instructions::DefineReference{"ref"});
	CHECK(defs.recipe[5] == PainPerdu::instructions::MoveToReference{"ref"});
	CHECK(defs.recipe[6] == PainPerdu::instructions::GoToLabel{"label"});
	CHECK(defs.recipe[7] == PainPerdu::instructions::IfCurrentValueDifferent0{});
	CHECK(defs.recipe[8] == PainPerdu::instructions::IfCurrentValueEqualsN{5});
	CHECK(defs.recipe[9] == PainPerdu::instructions::IfCursorIsAtReference{"aa"});
	CHECK(defs.recipe[10] == PainPerdu::instructions::GetChar{});
	CHECK(defs.recipe[11] == PainPerdu::instructions::PutChar{});
	CHECK(defs.recipe[12] == PainPerdu::instructions::IfReferenceExists{"lol"});
	CHECK(defs.recipe[13] == PainPerdu::instructions::UndefineReference{"sauce"});
	CHECK(defs.recipe[14] == PainPerdu::instructions::MoveRight{12});
}

TEST_CASE("token_basic")
{
	using namespace PainPerdu::parser;

	std::string_view code = ">1 <1+255\t-1";
	PainPerdu::Parser parser;
	auto tokens = parser.get_tokens(code);
	REQUIRE(tokens.size() == 8);
	CHECK(tokens[0] == Token{.type = Token::Type::OPERATOR, .line = 1, .start_column = 1, .lenght = 1});
	CHECK(tokens[1] == Token{.type = Token::Type::NUMBER,   .line = 1, .start_column = 2, .lenght = 1});
	CHECK(tokens[2] == Token{.type = Token::Type::OPERATOR, .line = 1, .start_column = 4, .lenght = 1});
	CHECK(tokens[3] == Token{.type = Token::Type::NUMBER,   .line = 1, .start_column = 5, .lenght = 1});
	CHECK(tokens[4] == Token{.type = Token::Type::OPERATOR, .line = 1, .start_column = 6, .lenght = 1});
	CHECK(tokens[5] == Token{.type = Token::Type::NUMBER,   .line = 1, .start_column = 7, .lenght = 3});
	CHECK(tokens[6] == Token{.type = Token::Type::OPERATOR, .line = 1, .start_column = 11, .lenght = 1});
	CHECK(tokens[7] == Token{.type = Token::Type::NUMBER,   .line = 1, .start_column = 12, .lenght = 1});
}

TEST_CASE("token_basic")
{
	using namespace PainPerdu::parser;

	std::string_view code =
		":label\n"
		"ignore\n"
		"#ref";
	PainPerdu::Parser parser;
	auto tokens = parser.get_tokens(code);
	REQUIRE(tokens.size() == 4);
	CHECK(tokens[0] == Token{.type = Token::Type::OPERATOR, .line = 1, .start_column = 1, .lenght = 1});
	CHECK(tokens[1] == Token{.type = Token::Type::LABEL,    .line = 1, .start_column = 2, .lenght = 5});
	CHECK(tokens[2] == Token{.type = Token::Type::OPERATOR, .line = 3, .start_column = 1, .lenght = 1});
	CHECK(tokens[3] == Token{.type = Token::Type::VARIABLE, .line = 3, .start_column = 2, .lenght = 3});
}