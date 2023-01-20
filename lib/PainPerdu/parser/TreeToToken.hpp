#pragma once

#include <vector>

#include <PainPerdu/parser/Grammar.hpp>
#include <PainPerdu/parser/Token.hpp>

namespace PainPerdu::parser
{

struct TreeToTokenState
{
	enum NextIdentifierType
	{
		NONE, // If there is an identifier with NONE, this is a bug
		LABEL, // Equivalent of Token::Type::LABEL
		REFERENCE // Equivalent of Token::Type::VARIABLE
	};

	std::vector<Token> tokens;
	NextIdentifierType next_identifier_type;
};

inline Token::Type get_token_type(TreeToTokenState::NextIdentifierType next_identifier_type)
{
	switch (next_identifier_type)
	{
		case TreeToTokenState::NextIdentifierType::LABEL :
		{
			return Token::Type::LABEL;
		}
		case TreeToTokenState::NextIdentifierType::REFERENCE :
		{
			return Token::Type::VARIABLE;
		}
		// Weird, even when the switch was exhaustive (there was also the NONE case)
		// MSVC complained about not all path having a return value (warning C4715)
		default:
		{
			throw std::runtime_error("Someone did a shitty code trololol");
		}
	}
}

template<typename Rule>
struct TreeToTokenAction {};

#define DefineAction(Match, TokenType, IdentifierType)                                \
	template <>                                                                       \
	struct TreeToTokenAction<Match>                                                   \
	{                                                                                 \
		template <typename ParseInput>                                                \
		static void apply(const ParseInput& in, TreeToTokenState& state)              \
		{                                                                             \
			state.tokens.push_back(                                                   \
				Token                                                                 \
				{                                                                     \
					.type = TokenType,                                                \
					.line = in.position().line,                                       \
					.start_column = in.position().column,                             \
					.length = in.size()                                               \
				});                                                                   \
			state.next_identifier_type = IdentifierType;                              \
		}                                                                             \
	};

DefineAction(operators::DefineLabel, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::LABEL)
DefineAction(operators::MoveRight, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::MoveLeft, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::Increment, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::Decrement, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(ResetCase, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::NONE)
DefineAction(operators::DefineReference, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::UndefineReference, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::MoveToReference, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::GoToLabel, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::LABEL)
DefineAction(operators::Rewind, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::LABEL)
DefineAction(operators::IfCurrentValueDifferent, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::IfCursorIsAtReference, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::IfReferenceExists, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::REFERENCE)
DefineAction(GetChar, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::NONE)
DefineAction(PutChar, Token::Type::OPERATOR, TreeToTokenState::NextIdentifierType::NONE)

DefineAction(Comment, Token::Type::COMMENT, TreeToTokenState::NextIdentifierType::NONE)
DefineAction(ReadFile, Token::Type::STRING, TreeToTokenState::NextIdentifierType::NONE)
DefineAction(Integer, Token::Type::NUMBER, TreeToTokenState::NextIdentifierType::NONE)

template <>                                                                       
struct TreeToTokenAction<Identifier>
{
	template <typename ParseInput>
	static void apply(const ParseInput& in, TreeToTokenState& state)
	{
		if (state.next_identifier_type != TreeToTokenState::NextIdentifierType::NONE)
		{
			state.tokens.push_back(
				Token
				{
					.type = get_token_type(state.next_identifier_type),
					.line = in.position().line,
					.start_column = in.position().column,
					.length = in.size()
				});
			state.next_identifier_type = TreeToTokenState::NextIdentifierType::NONE;
		}
	}
};

#undef DefineAction

} // namespace PainPerdu::parser