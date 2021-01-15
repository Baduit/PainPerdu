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

struct AnyInteger
{
	bool operator==(const crepuscule::Token& token)
	{
		return std::holds_alternative<crepuscule::Integer>(token);
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

namespace helper
{

template <typename It, typename... Args>
bool match_impl(It)
{
	return true;
}

template <typename It, typename T, typename... Args>
bool match_impl(It it, T&& t, Args&&... args)
{
	if (*it == t)
	{
		return match_impl(it + 1, args...);
	}
	else
	{
		return false;
	}
}

template <typename It, typename... Args>
bool match(It begin, It end, Args&&... args)
{
	static_assert(sizeof...(args) != 0);
	if (begin == end)
		return false;
	else if (static_cast<std::size_t>(end - begin) < sizeof...(args))
		return false;
	else
		return match_impl(begin, args...);
}

} // namespace helper

} // namespace patterns

} // namespace parser

} // namespace PainPerdu