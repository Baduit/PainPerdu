#pragma once

#include <tao/pegtl.hpp>

namespace PainPerdu::parser
{

namespace pegtl = tao::pegtl;

struct AlphaAndUnderscore : pegtl::sor<pegtl::alpha, pegtl::one<'_'>>{};
struct AlphaNumAndUnderscore: pegtl::sor<pegtl::alnum, pegtl::one<'_'>>{};

struct Identifier : pegtl::seq<AlphaAndUnderscore, pegtl::opt<pegtl::plus<AlphaNumAndUnderscore>>> {};
struct Integer : pegtl::plus<pegtl::digit> {};

namespace operators
{
    
struct DefineLabel : pegtl::one<':'> {};
struct MoveRight : pegtl::one<'>'> {};
struct MoveLeft : pegtl::one<'<'> {};
struct Increment : pegtl::one<'+'> {};
struct Decrement : pegtl::one<'-'> {};
struct ResetCase : pegtl::one<';'> {};
struct DefineReference : pegtl::one<'#'> {};
struct UndefineReference : pegtl::one<'.'> {};
struct MoveToReference : pegtl::one<'@'> {};
struct GoToLabel : pegtl::one<'*'> {};
struct Rewind : pegtl::one<'&'> {};
struct IfCurrentValueDifferent : pegtl::one<'?'> {};
struct IfCursorIsAtReference : pegtl::one<'!'> {};
struct IfReferenceExists : pegtl::one<'$'> {};
struct GetChar : pegtl::one<'['> {};
struct PutChar : pegtl::one<']'> {};
}

struct DefineLabel : pegtl::seq<operators::DefineLabel, Identifier>  {};
struct MoveRight : pegtl::seq<operators::MoveRight, pegtl::sor<Integer, Identifier>> {};
struct MoveLeft : pegtl::seq<operators::MoveLeft, pegtl::sor<Integer, Identifier>> {};
struct Increment : pegtl::seq<operators::Increment, pegtl::sor<Integer, Identifier>> {};
struct Decrement : pegtl::seq<operators::Decrement, pegtl::sor<Integer, Identifier>> {};
struct ResetCase : operators::ResetCase {};
struct DefineReference : pegtl::seq<operators::DefineReference, Identifier>  {};
struct UndefineReference : pegtl::seq<operators::UndefineReference, Identifier> {};
struct MoveToReference : pegtl::seq<operators::MoveToReference, Identifier>  {};
struct GoToLabel : pegtl::seq<operators::GoToLabel, Identifier>  {};
struct Rewind : pegtl::seq<operators::Rewind, Identifier> {};
struct IfCurrentValueDifferent : pegtl::seq<operators::IfCurrentValueDifferent, pegtl::opt<pegtl::sor<Integer, Identifier>>> {};
struct IfCursorIsAtReference : pegtl::seq<operators::IfCursorIsAtReference, Identifier> {};
struct IfReferenceExists : pegtl::seq<operators::IfReferenceExists, Identifier> {};
struct GetChar : operators::GetChar {};
struct PutChar : operators::PutChar {};
struct ReadFile : pegtl::if_must<pegtl::one<'"'>, pegtl::until<pegtl::one<'"'>, pegtl::any>> {};
struct Comment : pegtl::if_must<pegtl::one<'{'>, pegtl::until<pegtl::one<'}'>, pegtl::any>> {};
struct Skip : pegtl::plus<pegtl::sor<pegtl::blank, pegtl::eol>> {};

struct Expression : 
	pegtl::sor
		<
			DefineLabel,
			MoveRight,
			MoveLeft,
			Increment,
			Decrement,
			ResetCase,
			DefineReference,
			UndefineReference,
			MoveToReference,
			GoToLabel,
			Rewind,
			IfCurrentValueDifferent,
			IfCursorIsAtReference,
			IfReferenceExists,
			GetChar,
			PutChar,
			ReadFile,
			Comment,
			Skip
		>
{};

struct SyntaxError : pegtl::if_must<pegtl::any, pegtl::until<Expression>> {};

struct Grammar : pegtl::plus<pegtl::sor<Expression, SyntaxError>> {};

} // namespace PainPerdu::parser