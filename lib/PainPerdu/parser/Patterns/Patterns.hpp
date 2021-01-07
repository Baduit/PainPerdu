#pragma once

#include <external/brigand/brigand.hpp>

#include <PainPerdu/parser/ParsingState.hpp>
#include <PainPerdu/parser/Patterns/PatternMatchHelper.hpp>

namespace PainPerdu
{

namespace parser
{

struct DummyPattern
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

using Patterns =
	brigand::list
		<
			DummyPattern
		>;

} // namespace parser


} // namespace PainPerdu
