#include <charconv>
#include <format>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

#include <PainPerdu/parser/Parser.hpp>

namespace PainPerdu
{

namespace parser
{

template<class> inline constexpr bool always_false_v = false;	

namespace pegtl = tao::pegtl;

struct AlphaAndUnderscore : pegtl::sor<pegtl::alpha, pegtl::one<'_'>>{};
struct AlphaNumAndUnderscore: pegtl::sor<pegtl::alnum, pegtl::one<'_'>>{};

struct Identifier : pegtl::seq<AlphaAndUnderscore, pegtl::opt<pegtl::plus<AlphaNumAndUnderscore>>> {};
struct Integer : pegtl::plus<pegtl::digit> {};

struct DefineLabel : pegtl::seq<pegtl::one<':'>, Identifier>  {};
struct MoveRight : pegtl::seq<pegtl::one<'>'>, pegtl::sor<Integer, Identifier>> {};
struct MoveLeft : pegtl::seq<pegtl::one<'<'>, pegtl::sor<Integer, Identifier>> {};
struct Increment : pegtl::seq<pegtl::one<'+'>, pegtl::sor<Integer, Identifier>> {};
struct Decrement : pegtl::seq<pegtl::one<'-'>, pegtl::sor<Integer, Identifier>> {};
struct ResetCase : pegtl::one<';'> {};
struct DefineReference : pegtl::seq<pegtl::one<'#'>, Identifier>  {};
struct UndefineReference : pegtl::seq<pegtl::one<'.'>, Identifier> {};
struct MoveToReference : pegtl::seq<pegtl::one<'@'>, Identifier>  {};
struct GoToLabel : pegtl::seq<pegtl::one<'*'>, Identifier>  {};
struct Rewind : pegtl::seq<pegtl::one<'&'>, Identifier> {};
struct IfCurrentValueDifferent : pegtl::seq<pegtl::one<'?'>, pegtl::opt<pegtl::sor<Integer, Identifier>>> {};
struct IfCursorIsAtReference : pegtl::seq<pegtl::one<'!'>, Identifier> {};
struct IfReferenceExists : pegtl::seq<pegtl::one<'$'>, Identifier> {};
struct GetChar : pegtl::one<'['> {};
struct PutChar : pegtl::one<']'> {};
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

struct Visitor
{
	Visitor(Definitions& defs):
		_defs(defs)
	{}

	template <typename InstructionN, typename InstructionId>
	Instruction do_it_right(const pegtl::parse_tree::node& node)
	{
		const auto& child = *(node.children[0]);
		if (child.is_type<Integer>())
		{
			decltype(std::declval<InstructionN>().value) n;
			std::from_chars(child.string_view().data(), child.string_view().data() + child.string_view().size(), n);

			return InstructionN{ n };
		}
		else
		{
			return InstructionId{ child.string() };
		}
	}

	template <typename InstructionAlone, typename InstructionN, typename InstructionId>
	Instruction do_it_right(const pegtl::parse_tree::node& node)
	{
		if (node.children.empty())
		{
			return InstructionAlone{};
		}
		else
		{
			const auto& child = *(node.children[0]);
			if (child.is_type<Integer>())
			{
				decltype(std::declval<InstructionN>().value) n;
				std::from_chars(child.string_view().data(), child.string_view().data() + child.string_view().size(), n);
				return InstructionN{ n };
			}
			else
			{
				return InstructionId{ child.string() };
			}
		}
	}

	void operator()(const pegtl::parse_tree::node* node)
	{
		if (!node)
			return;			

		if (node->is_type<DefineLabel>())
		{
			_defs.annotations.push_back(annotations::DefineLabel{ node->children[0]->string(), _defs.recipe.size() });
		}
		else if (node->is_type<MoveRight>())
		{
			_defs.recipe.push_back(do_it_right<instructions::MoveRight, instructions::MoveRightRef>(*node));
		}
		else if (node->is_type<MoveLeft>())
		{
			_defs.recipe.push_back(do_it_right<instructions::MoveLeft, instructions::MoveLeftRef>(*node));
		}
		else if (node->is_type<Increment>())
		{
			_defs.recipe.push_back(do_it_right<instructions::Increment, instructions::IncrementRef>(*node));
		}
		else if (node->is_type<Decrement>())
		{
			_defs.recipe.push_back(do_it_right<instructions::Decrement, instructions::DecrementRef>(*node));
		}
		else if (node->is_type<ResetCase>())
		{
			_defs.recipe.push_back(instructions::ResetCase{});
		}
		else if (node->is_type<DefineReference>())
		{
			_defs.recipe.push_back(instructions::DefineReference{ node->children[0]->string() });
		}
		else if (node->is_type<UndefineReference>())
		{
			_defs.recipe.push_back(instructions::UndefineReference{ node->children[0]->string() });
		}
		else if (node->is_type<MoveToReference>())
		{
			_defs.recipe.push_back(instructions::MoveToReference{ node->children[0]->string() });
		}
		else if (node->is_type<GoToLabel>())
		{
			_defs.recipe.push_back(instructions::GoToLabel{ node->children[0]->string() });
		}
		else if (node->is_type<Rewind>())
		{
			_defs.recipe.push_back(instructions::Rewind{ node->children[0]->string() });
		}
		else if (node->is_type<IfCurrentValueDifferent>())
		{
			_defs.recipe.push_back(do_it_right<instructions::IfCurrentValueDifferent0, instructions::IfCurrentValueEqualsN, instructions::IfCurrentValueEqualsNRef>(*node));
		}
		else if (node->is_type<IfCursorIsAtReference>())
		{
			_defs.recipe.push_back(instructions::IfCursorIsAtReference{ node->children[0]->string() });
		}
		else if (node->is_type<IfReferenceExists>())
		{
			_defs.recipe.push_back(instructions::IfReferenceExists{ node->children[0]->string() });
		}
		else if (node->is_type<GetChar>())
		{
			_defs.recipe.push_back(instructions::GetChar{});
		}
		else if (node->is_type<PutChar>())
		{
			_defs.recipe.push_back(instructions::PutChar{});
		}
		else if (node->is_type<ReadFile>())
		{
			auto filename = node->string_view();
			filename.remove_prefix(1);
			filename.remove_suffix(1);
			_defs.recipe.push_back(instructions::ReadFile{ std::string(filename) });
		}
		else
		{
			for (const auto& child: node->children)
				(*this)(child.get());
		}
	}

	Definitions& _defs;
};

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

	Visitor v(defs);
	v(root.get());

	return defs;
}

} // namespace parser


} // namespace PainPerdu
