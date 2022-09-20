#include <concepts>
#include <type_traits>

#include <tao/pegtl.hpp>

#include <PainPerdu/parser_v2/Parser.hpp>

namespace PainPerdu
{

namespace parserv2
{

template<class> inline constexpr bool always_false_v = false;	

namespace pegtl = tao::pegtl;

struct AlphaAndUnderscore : pegtl::sor<pegtl::alpha, pegtl::one<'_'>>{};
struct AlphaNumAndUnderscore: pegtl::sor<pegtl::alnum, pegtl::one<'_'>>{};

struct Identifier : pegtl::seq<AlphaAndUnderscore, pegtl::opt<pegtl::plus<AlphaNumAndUnderscore>>> {};

struct Integer : pegtl::plus<pegtl::digit> {};

struct MoveRight : pegtl::seq<pegtl::one<'>'>, Integer> {};
struct MoveRightRef : pegtl::seq<pegtl::one<'>'>, Identifier> {};
struct MoveLeft : pegtl::seq<pegtl::one<'<'>, Integer> {};
struct MoveLeftRef : pegtl::seq<pegtl::one<'<'>, Identifier> {};
struct Increment : pegtl::seq<pegtl::one<'+'>, Integer> {};
struct IncrementRef : pegtl::seq<pegtl::one<'+'>, Identifier> {};
struct Decrement : pegtl::seq<pegtl::one<'-'>, Integer> {};
struct DecrementRef : pegtl::seq<pegtl::one<'-'>, Identifier>  {};
struct ResetCase : pegtl::one<';'> {};
struct DefineReference : pegtl::seq<pegtl::one<'#'>, Identifier>  {};
struct UndefineReference : pegtl::seq<pegtl::one<'.'>, Identifier> {};
struct MoveToReference : pegtl::seq<pegtl::one<'@'>, Identifier>  {};
struct GoToLabel : pegtl::seq<pegtl::one<'*'>, Identifier>  {};
struct Rewind : pegtl::seq<pegtl::one<'&'>, Identifier> {};
struct IfCurrentValueDifferent0 : pegtl::one<'?'> {};
struct IfCurrentValueEqualsN : pegtl::seq<pegtl::one<'?'>, Integer> {};
struct IfCurrentValueEqualsNRef : pegtl::seq<pegtl::one<'?'>, Identifier> {};
struct IfCursorIsAtReference : pegtl::seq<pegtl::one<'!'>, Identifier> {};
struct IfReferenceExists : pegtl::seq<pegtl::one<'$'>, Identifier> {};
struct GetChar : pegtl::one<'['> {};
struct PutChar : pegtl::one<']'> {};
struct ReadFile : pegtl::if_must<pegtl::one<'"'>, pegtl::until<pegtl::one<'"'>, pegtl::any>> {};
struct Comment : pegtl::if_must<pegtl::one<'{'>, pegtl::until<pegtl::one<'}'>, pegtl::any>> {};

struct Grammar:
	pegtl::plus
		<
			pegtl::sor
				<
					MoveRight,
					MoveRightRef,
					MoveLeft,
					MoveLeftRef,
					Increment,
					IncrementRef,
					Decrement,
					DecrementRef,
					ResetCase,
					DefineReference,
					UndefineReference,
					MoveToReference,
					GoToLabel,
					Rewind,
					IfCurrentValueDifferent0,
					IfCurrentValueEqualsN,
					IfCurrentValueEqualsNRef,
					IfCursorIsAtReference,
					IfReferenceExists,
					GetChar,
					PutChar,
					ReadFile,
					Comment
				>
		>
{};


Definitions Parser::operator()(std::string_view input)
{
	Definitions defs;

	pegtl::memory_input mem_input(input.data(), input.size(), "");
	pegtl::parse<Grammar>(mem_input);

	return defs;
}

} // namespace parserv2


} // namespace PainPerdu
