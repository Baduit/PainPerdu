#include <concepts>
#include <iostream>
#include <ranges>
#include <sstream>

#include <crepuscule/TokenPrinter.hpp>

#include <PainPerdu/parser/Parser.hpp>
#include <PainPerdu/parser/ParseException.hpp>
#include <PainPerdu/parser/ParsingState.hpp>
#include <PainPerdu/parser/Patterns/Patterns.hpp>
#include <PainPerdu/misc/Log.hpp>

namespace PainPerdu
{

namespace parser
{


namespace
{

crepuscule::Config create_config()
{
	crepuscule::Config conf;

	conf.comment_delimiters.emplace_back("{", "}");

	conf.delimiters.emplace_back(" ");
	conf.delimiters.emplace_back("\t");
	conf.delimiters.emplace_back("\n");

	conf.operators.emplace_back(":");
	conf.operators.emplace_back(">");
	conf.operators.emplace_back("<");
	conf.operators.emplace_back("+");
	conf.operators.emplace_back("-");
	conf.operators.emplace_back("#");
	conf.operators.emplace_back("@");
	conf.operators.emplace_back("*");
	conf.operators.emplace_back("?");
	conf.operators.emplace_back("!");
	conf.operators.emplace_back("[");
	conf.operators.emplace_back("]");
	conf.operators.emplace_back("$");
	conf.operators.emplace_back(".");

	conf.integer_reader =
		[](std::string_view str) -> std::optional<int>
		{
			// TODO: improve this, I don't think use a stringstream is ideal
			for (auto c: str)
				if (!isdigit(c))
					return {};
			std::stringstream ss;
			ss << str;
			int result;
			ss >> result;
			return result;
		};

	return conf;
}

template <typename It>
auto get_line(It begin, It end)
{
	return std::find_if(begin, end,
		[](const auto& token)
		{
			if (const auto* endline = std::get_if<crepuscule::Operator>(&token); endline)
				return endline->value == "\n";
			else
				return false;
		});
}

std::string token_to_string(const crepuscule::Token& token)
{
	std::stringstream ss;
	std::visit(
			[&](const auto& t)
			{
				using namespace crepuscule;
				ss << t;
			}, token);
	return ss.str();
}

} // namespace


Parser::Parser():
	_tokeniser(create_config())
{
}


Definitions Parser::operator()(std::string_view input)
{
	return (*this)(std::string(input));
}

Definitions Parser::operator()(std::string&& input)
{
		using namespace crepuscule;

	Definitions defs;
	Result tokenize_result = _tokeniser(input);
	Expression& main_expression = tokenize_result.expression;

	ParsingState state(defs);
	auto it = main_expression.value.begin();
	auto end = main_expression.value.end();
	while (it != end)
	{
		bool pattern_matched = false;
		brigand::for_each<Patterns>(
			[&](auto type)
			{
				if (!pattern_matched)
				{
					using P = typename decltype(type)::type;
					if (P::match(it, end, state))
					{
						pattern_matched = true;
						it = P::action(it, end, state);
					}
				}
			});

		if (!pattern_matched)
		{
			logger[LogCategory::Parser].error("Invalid code at token : " + token_to_string(*it));
			break;
		}
	}

	return defs;
}

} // namespace parser


} // namespace PainPerdu
