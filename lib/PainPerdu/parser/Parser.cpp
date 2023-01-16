#include <charconv>

#include <PainPerdu/parser/Parser.hpp>
#include <PainPerdu/parser/TreeToDefVisitor.hpp>
#include <PainPerdu/parser/TreeToToken.hpp>
#include <PainPerdu/parser/Grammar.hpp>

namespace PainPerdu
{

namespace parser
{

template<typename Rule>
using Selector = pegtl::parse_tree::selector
	<
		Rule,
		pegtl::parse_tree::store_content::on
			<
				Identifier,
				Integer,
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
				ReadFile
			>
	>;


template<typename Rule>
struct ParserAction {};

template<>
struct ParserAction<SyntaxError>
{
	template<typename ParseInput>
	static void apply(const ParseInput& in)
	{
		// It does not work on emscripten, I dunno why
		//throw std::runtime_error(std::format("Error at line = {}, column = {}.\nUnknown symbols : {}",  in.position().line, in.position().column, in.string()));
		throw std::runtime_error("Error at line = " + std::to_string(in.position().line) + ", column = " + std::to_string(in.position().column) + ".\nUnknown symbols : " + in.string());
	}
};


Definitions Parser::operator()(std::string_view input)
{
	Definitions defs;

	pegtl::memory_input mem_input(input.data(), input.size(), "");
	const auto root = pegtl::parse_tree::parse<Grammar, Selector, ParserAction>(mem_input);

	TreeToDefVisitor v(defs);
	v(root.get());

	return defs;
}

std::vector<Token> Parser::get_tokens(std::string_view input)
{
	TreeToTokenState token_state;

	pegtl::memory_input mem_input(input.data(), input.size(), "");
	pegtl::parse<Grammar, TreeToTokenAction>(mem_input, token_state);

	return token_state.tokens;
}

} // namespace parser


} // namespace PainPerdu
