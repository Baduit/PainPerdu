#pragma once

#include <external/brigand/brigand.hpp>

#include <PainPerdu/Definitions.hpp>
#include <PainPerdu/parser/ParseException.hpp>
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
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator(">"), AnyInteger{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		state.emplace_instruction<instructions::MoveRight>(integer.value);
		return end;
	}
};

struct MoveLeft
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("<"), AnyInteger{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		state.emplace_instruction<instructions::MoveLeft>(integer.value);
		return end;
	}
};

struct Increment
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("+"), AnyInteger{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		if (integer.value < 0 || integer.value > 255)
			throw ParseException(integer.line, "Instruction + must be followed by an integer between 0 and 255 included");
		state.emplace_instruction<instructions::Increment>(integer.value);
		return end;
	}
};

struct Decrement
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("-"), AnyInteger{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		if (integer.value < 0 || integer.value > 255)
			throw ParseException(integer.line, "Instruction + must be followed by an integer between 0 and 255 included");
		state.emplace_instruction<instructions::Decrement>(integer.value);
		return end;
	}
};

struct DefineReference
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("#"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::DefineReference>(std::string(identifier.value));
		return end;
	}
};

struct MoveToReference
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("@"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::DefineReference>(std::string(identifier.value));
		return end;
	}
};

struct DefineLabel
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator(":"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_annotation<annotations::DefineLabel>(std::string(identifier.value), state.get_next_instruction_index());
		return end;
	}
};

struct GoToLabel
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("*"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::GoToLabel>(std::string(identifier.value));
		return end;
	}
};

struct IfCurrentValueEqualsN
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("?"), AnyInteger{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		if (integer.value < 0 || integer.value > 255)
			throw ParseException(integer.line, "Instruction + must be followed by an integer between 0 and 255 included");
		state.emplace_instruction<instructions::IfCurrentValueEqualsN>(integer.value);
		return end;
	}
};

struct IfCurrentValueEquals0
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("?"));
	}

	template <typename It>
	static It action(It, It end, ParsingState& state)
	{
		state.emplace_instruction<instructions::IfCurrentValueEquals0>();
		return end;
	}
};

struct IfCursorIsAtReference
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("!"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::IfCursorIsAtReference>(std::string(identifier.value));
		return end;
	}
};

struct IfReferenceExists
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("$"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::IfReferenceExists>(std::string(identifier.value));
		return end;
	}
};

struct IfLabelExists
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("."), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It end, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::IfLabelExists>(std::string(identifier.value));
		return end;
	}
};

struct GetChar
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("["));
	}

	template <typename It>
	static It action(It, It end, ParsingState& state)
	{
		state.emplace_instruction<instructions::GetChar>();
		return end;
	}
};

struct PutChar
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("]"));
	}

	template <typename It>
	static It action(It, It end, ParsingState& state)
	{
		state.emplace_instruction<instructions::PutChar>();
		return end;
	}
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
			patterns::IfCurrentValueEqualsN, // this one must absoluterly be before "IfCurrentValueEquals0"
			patterns::IfCurrentValueEquals0,
			patterns::IfCursorIsAtReference,
			patterns::IfReferenceExists,
			patterns::IfLabelExists,
			patterns::GetChar,
			patterns::PutChar
		>;

} // namespace parser


} // namespace PainPerdu
