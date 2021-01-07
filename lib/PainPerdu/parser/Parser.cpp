#include <concepts>
#include <iostream>
#include <ranges>

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

} // namespace


Parser::Parser():
	_tokeniser(create_config())
{
}


Definitions Parser::operator()(std::string_view input)
{
	using namespace crepuscule;

	Definitions defs;
	Result tokenize_result = _tokeniser(input);
	Expression& main_expression = tokenize_result.expression;

	ParsingState state;
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
			using namespace crepuscule;
			logger[LogCategory::Parser].error("Invalid code at token : todo make a way to print t the token");
			break;
		}
	}

	return defs;
}


} // namespace parser


} // namespace PainPerdu
