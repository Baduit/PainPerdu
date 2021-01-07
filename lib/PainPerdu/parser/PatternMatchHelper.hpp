#pragma once

#include <crepuscule/Token.hpp>

namespace PainPerdu
{

namespace parser
{

namespace patterns
{

struct AnyWord
{
	bool operator==(const crepuscule::Token& token)
	{
		return std::holds_alternative<crepuscule::Word>(token);
	}
};

struct Operator
{
	Operator(std::string v):
		value(v) // RVO
	{}

	bool operator==(const crepuscule::Token& token)
	{
		auto* ope = std::get_if<crepuscule::Operator>(&token);
		return (ope && ope->value == value);
	}

	std::string value;
};

struct Keyword
{
	Keyword(std::string v):
		value(v) // RVO
	{}

	bool operator==(const crepuscule::Token& token)
	{
		auto* k = std::get_if<crepuscule::Keyword>(&token);
		return (k && k->value == value);
	}

	std::string value;
};

template <typename... Args>
bool match(Args&&... args)
{

	return false;
}

}

}

}