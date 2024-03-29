#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <sstream>

#include <PainPerdu/PainPerdu.hpp>

TEST_CASE("hello_world_one_case")
{
	std::string_view code = "+72] +29] +7]] +3] -79] +55] +24] +3] -6] -8] -67] -23]";

	std::stringstream in;
	std::stringstream out;

	PainPerdu::Interpreter interpreter(in, out);
	interpreter.compile_and_run(code);
	
	CHECK(out.str() == "Hello World!\n");
}

TEST_CASE("hello_world_multiple_cases")
{
	std::string_view code =
		"+72     ]"
		">1 +101 ]"
		">1 +108 ]]"
		">1 +111 ]"
		">1 +32  ]"
		">1 +87  ]"
		">1 +111 ]"
		">1 +114 ]"
		">1 +108 ]"
		">1 +100 ]"
		">1 +33  ]"
		">1 +10  ]";

	std::stringstream in;
	std::stringstream out;

	PainPerdu::Interpreter interpreter(in, out);
	interpreter.compile_and_run(code);
	
	CHECK(out.str() == "Hello World!\n");
}

TEST_CASE("hello_world_labels")
{
	std::string_view code =
		"#end_of_string"
		">1 +10 #end_of_line"
		">1 +13 #carriage_return"
		">1 +32 #space"
		">1 +33 #exclamation_mark"
		">1 +34 #double_quote"
		">1 +35 #hashtag"
		">1 +36 #dollar_sign"
		">1 +37 #pourcent_sign"
		">1 +38 #ampersand"
		">1 +39 #simple_quote"
		">1 +40 #open_parenthesis"
		">1 +41 #close_parenthesis"
		">1 +42 #asterisk"
		">1 +43 #plus"
		">1 +44 #comma"
		">1 +45 #minus"
		">1 +46 #dot"
		">1 +47 #slash"
		">1 +48 #zero"
		">1 +49 #one"
		">1 +50 #two"
		">1 +51 #three"
		">1 +52 #four"
		">1 +53 #five"
		">1 +54 #six"
		">1 +55 #seven"
		">1 +56 #eight"
		">1 +57 #nine"
		">1 +58 #colon"
		">1 +59 #semi_colon"
		">1 +60 #less"
		">1 +61 #equal"
		">1 +62 #greater"
		">1 +63 #question_marl"
		">1 +64 #at"
		">1 +65 #A"
		">1 +66 #B"
		">1 +67 #C"
		">1 +68 #D"
		">1 +69 #E"
		">1 +70 #F"
		">1 +71 #G"
		">1 +72 #H"
		">1 +73 #I"
		">1 +74 #J"
		">1 +75 #K"
		">1 +76 #L"
		">1 +77 #M"
		">1 +78 #N"
		">1 +79 #O"
		">1 +80 #P"
		">1 +81 #Q"
		">1 +82 #R"
		">1 +83 #S"
		">1 +84 #T"
		">1 +85 #U"
		">1 +86 #V"
		">1 +87 #W"
		">1 +88 #X"
		">1 +89 #Y"
		">1 +90 #Z"
		">1 +91 #opening_bracket"
		">1 +92 #back_slash"
		">1 +93 #closing_bracket"
		">1 +94 #circumflex"
		">1 +95 #underscore"
		">1 +96 #back_quote"
		">1 +97 #a"
		">1 +98 #b"
		">1 +99 #c"
		">1 +100 #d"
		">1 +101 #e"
		">1 +102 #f"
		">1 +103 #g"
		">1 +104 #h"
		">1 +105 #i"
		">1 +106 #j"
		">1 +107 #k"
		">1 +108 #l"
		">1 +109 #m"
		">1 +110 #n"
		">1 +111 #o"
		">1 +112 #p"
		">1 +113 #q"
		">1 +114 #r"
		">1 +115 #s"
		">1 +116 #t"
		">1 +117 #u"
		">1 +118 #v"
		">1 +119 #w"
		">1 +120 #x"
		">1 +121 #y"
		">1 +122 #z"
		">1 +123 #opening_brace"
		">1 +124 #pipe"
		">1 +125 #closing_brace"
		">1 +126 #tilde"
		"@H] @e] @l]] @o] @space] @W] @o] @r] @l] @d] @exclamation_mark] @end_of_line]";

	std::stringstream in;
	std::stringstream out;

	PainPerdu::Interpreter interpreter(in, out);
	interpreter.compile_and_run(code);
	
	CHECK(out.str() == "Hello World!\n");
}

TEST_CASE("hello_print")
{
	std::string_view code =
		"+72     "
		">1 +101 "
		">1 +108 "
		">1 +108 "
		">1 +111 "
		">1 +32  "
		">1 +87  "
		">1 +111 "
		">1 +114 "
		">1 +108 "
		">1 +100 "
		">1 +33  "
		">1 +10 "
		"@__begin__"

		"*print"

		"*print_end"
		":print"
		"?*print_impl"
		"*print_end"
		":print_impl"
		"]>1*print"
		":print_end"
		">1#this_is_to_add_a_backslash_zero";

	std::stringstream in;
	std::stringstream out;

	PainPerdu::Interpreter interpreter(in, out);
	interpreter.compile_and_run(code);
	
	CHECK(out.str() == "Hello World!\n");
}

TEST_CASE("new_operators")
{
	std::string_view code = "+50 +__here__ ] ; +50 +__here__ ] >1 +100 ?__begin__]";

	std::stringstream in;
	std::stringstream out;

	PainPerdu::Interpreter interpreter(in, out);
	interpreter.compile_and_run(code);
	
	CHECK(out.str() == "ddd");
}
