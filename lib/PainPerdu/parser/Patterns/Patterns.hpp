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
	static It action(It begin, It, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		state.emplace_instruction<instructions::MoveRight>(static_cast<std::size_t>(integer.value));
		return begin + 2;
	}
};

struct MoveRightRef
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator(">"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto word = std::get<crepuscule::Word>(*(begin + 1));
		state.emplace_instruction<instructions::MoveRightRef>(std::string(word.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		state.emplace_instruction<instructions::MoveLeft>(static_cast<std::size_t>(integer.value));
		return begin + 2;
	}
};

struct MoveLeftRef
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("<"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto word = std::get<crepuscule::Word>(*(begin + 1));
		state.emplace_instruction<instructions::MoveLeftRef>(std::string(word.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		if (integer.value < 0 || integer.value > 255)
			throw ParseException(integer.line, "Instruction + must be followed by an integer between 0 and 255 included");
		state.emplace_instruction<instructions::Increment>(integer.value);
		return begin + 2;
	}
};

struct IncrementRef
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("+"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto word = std::get<crepuscule::Word>(*(begin + 1));
		state.emplace_instruction<instructions::IncrementRef>(std::string(word.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		if (integer.value < 0 || integer.value > 255)
			throw ParseException(integer.line, "Instruction + must be followed by an integer between 0 and 255 included");
		state.emplace_instruction<instructions::Decrement>(integer.value);
		return begin + 2;
	}
};

struct DecrementRef
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("-"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto word = std::get<crepuscule::Word>(*(begin + 1));
		state.emplace_instruction<instructions::DecrementRef>(std::string(word.value));
		return begin + 2;
	}
};

struct ResetCase
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator(";"));
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		state.emplace_instruction<instructions::ResetCase>();
		return begin + 1;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::DefineReference>(std::string(identifier.value));
		return begin + 2;
	}
};

struct UndefineReference
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("."), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::UndefineReference>(std::string(identifier.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::MoveToReference>(std::string(identifier.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_annotation<annotations::DefineLabel>(std::string(identifier.value), state.get_next_instruction_index());
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::GoToLabel>(std::string(identifier.value));
		return begin + 2;
	}
};

struct Rewind
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("&"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::Rewind>(std::string(identifier.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto integer = std::get<crepuscule::Integer>(*(begin + 1));
		if (integer.value < 0 || integer.value > 255)
			throw ParseException(integer.line, "Instruction + must be followed by an integer between 0 and 255 included");
		state.emplace_instruction<instructions::IfCurrentValueEqualsN>(integer.value);
		return begin + 2;
	}
};

struct IfCurrentValueEqualsNRef
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("?"), AnyWord{});
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto word = std::get<crepuscule::Word>(*(begin + 1));
		state.emplace_instruction<instructions::IfCurrentValueEqualsNRef>(std::string(word.value));
		return begin + 2;
	}
};

struct IfCurrentValueDifferent0
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, Operator("?"));
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		state.emplace_instruction<instructions::IfCurrentValueDifferent0>();
		return begin + 1;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::IfCursorIsAtReference>(std::string(identifier.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		auto identifier = std::get<crepuscule::Word>(*(begin + 1));
		// todo check identifier does not contain any forbidden character
		state.emplace_instruction<instructions::IfReferenceExists>(std::string(identifier.value));
		return begin + 2;
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
	static It action(It begin, It, ParsingState& state)
	{
		state.emplace_instruction<instructions::GetChar>();
		return begin + 1;
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
	static It action(It begin, It, ParsingState& state)
	{
		state.emplace_instruction<instructions::PutChar>();
		return begin + 1;
	}
};

struct ReadFile
{
	template <typename It>
	static bool match(It begin, It end, const ParsingState&)
	{
		return helper::match(begin, end, String("\"", "\""));
	}

	template <typename It>
	static It action(It begin, It, ParsingState& state)
	{
		auto filename = std::get<crepuscule::String>(*begin);
		state.emplace_instruction<instructions::ReadFile>(filename.value);
		return begin + 1;
	}
};

} // namespace patterns

using Patterns =
	brigand::list
		<
			patterns::MoveRight,
			patterns::MoveRightRef,
			patterns::MoveLeft,
			patterns::MoveLeftRef,
			patterns::Increment,
			patterns::IncrementRef,
			patterns::Decrement,
			patterns::DecrementRef,
			patterns::ResetCase,
			patterns::DefineReference,
			patterns::UndefineReference,
			patterns::MoveToReference,
			patterns::DefineLabel,
			patterns::GoToLabel,
			patterns::Rewind,
			patterns::IfCurrentValueEqualsN, // this one must absoluterly be before "IfCurrentValueDifferent0"
			patterns::IfCurrentValueEqualsNRef, // this one must absoluterly be before "IfCurrentValueDifferent0"
			patterns::IfCurrentValueDifferent0,
			patterns::IfCursorIsAtReference,
			patterns::IfReferenceExists,
			patterns::GetChar,
			patterns::PutChar
		>;

} // namespace parser


} // namespace PainPerdu
