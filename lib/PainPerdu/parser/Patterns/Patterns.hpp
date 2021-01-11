#pragma once

#include <external/brigand/brigand.hpp>

#include <PainPerdu/Definitions.hpp>
#include <PainPerdu/parser/ParsingState.hpp>
#include <PainPerdu/parser/Patterns/PatternMatchHelper.hpp>

namespace PainPerdu
{

namespace parser
{

namespace patterns
{

struct MoveRight
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct MoveLeft
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct Increment
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct Decrement
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct DefineReference
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct MoveToReference
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct DefineLabel
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct GoToLabel
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct IfCurrentValueEquals0
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct IfCurrentValueEqualsN
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct IfCursorIsAtReference
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct IfReferenceExists
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct IfLabelExists
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct GetChar
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

struct PutChar
{
	template <typename It>
	static bool match(It, It, const ParsingState&) { return false; }

	template <typename It>
	static It action(It, It end, ParsingState&) { return end; }
};

} // namespace patterns

using Patterns =
	brigand::list
		<
			patterns::MoveRight,
			patterns::MoveLeft,
			patterns::Increment,
			patterns::Decrement,
			patterns::DefineReference,
			patterns::MoveToReference,
			patterns::DefineLabel,
			patterns::GoToLabel,
			patterns::IfCurrentValueEquals0,
			patterns::IfCurrentValueEqualsN,
			patterns::IfCursorIsAtReference,
			patterns::IfReferenceExists,
			patterns::IfLabelExists,
			patterns::GetChar,
			patterns::PutChar
		>;

} // namespace parser


} // namespace PainPerdu
