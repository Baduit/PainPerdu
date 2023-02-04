#pragma once

#include <vector>

#include <PainPerdu/parser/Grammar.hpp>
#include <PainPerdu/parser/Token.hpp>

namespace PainPerdu::parser
{

struct ToTokenState
{
	enum NextIdentifierType
	{
		NONE, // If there is an identifier with NONE, this is a bug
		LABEL,
		REFERENCE
	};

	std::vector<Token> tokens;
	NextIdentifierType next_identifier_type;
};

inline Token::Type get_token_type(ToTokenState::NextIdentifierType next_identifier_type)
{
	switch (next_identifier_type)
	{
		case ToTokenState::NextIdentifierType::LABEL :
		{
			return Token::Type::LABEL;
		}
		case ToTokenState::NextIdentifierType::REFERENCE :
		{
			return Token::Type::REFERENCE;
		}
		default:
		{
			throw std::runtime_error("Someone did a shitty code trololol");
		}
	}
}

template<typename Rule>
struct ToTokenAction {};

#define DefineAction(Match, TokenType, IdentifierType)                                \
	template <>                                                                       \
	struct ToTokenAction<Match>                                                       \
	{                                                                                 \
		template <typename ParseInput>                                                \
		static void apply(const ParseInput& in, ToTokenState& state)              \
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

DefineAction(operators::DefineLabel, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::LABEL)
DefineAction(operators::MoveRight, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::MoveLeft, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::Increment, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::Decrement, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(ResetCase, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::NONE)
DefineAction(operators::DefineReference, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::UndefineReference, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::MoveToReference, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::GoToLabel, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::LABEL)
DefineAction(operators::Rewind, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::LABEL)
DefineAction(operators::IfCurrentValueDifferent, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::IfCursorIsAtReference, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(operators::IfReferenceExists, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::REFERENCE)
DefineAction(GetChar, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::NONE)
DefineAction(PutChar, Token::Type::OPERATOR, ToTokenState::NextIdentifierType::NONE)

DefineAction(Comment, Token::Type::COMMENT, ToTokenState::NextIdentifierType::NONE)
DefineAction(ReadFile, Token::Type::STRING, ToTokenState::NextIdentifierType::NONE)
DefineAction(Integer, Token::Type::NUMBER, ToTokenState::NextIdentifierType::NONE)

template <>                                                                       
struct ToTokenAction<Identifier>
{
	template <typename ParseInput>
	static void apply(const ParseInput& in, ToTokenState& state)
	{
		if (state.next_identifier_type != ToTokenState::NextIdentifierType::NONE)
		{
			state.tokens.push_back(
				Token
				{
					.type = get_token_type(state.next_identifier_type),
					.line = in.position().line,
					.start_column = in.position().column,
					.length = in.size()
				});
			state.next_identifier_type = ToTokenState::NextIdentifierType::NONE;
		}
	}
};

#undef DefineAction

} // namespace PainPerdu::parser