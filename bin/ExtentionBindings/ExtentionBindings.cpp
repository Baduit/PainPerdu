#include <vector>
#include <string>

#include <emscripten/bind.h>

#include <PainPerdu/PainPerdu.hpp>

using namespace emscripten;

std::vector<PainPerdu::parser::Token> get_tokens(const std::string& input)
{
	return PainPerdu::Parser().get_tokens(input);
}

std::vector<std::string> get_defined_labels(const std::string& input)
{
	return PainPerdu::Parser().get_defined_labels(input);
}

std::vector<std::string> get_defined_references(const std::string& input)
{
	return PainPerdu::Parser().get_defined_references(input);
}

EMSCRIPTEN_BINDINGS(PainPerduParserModule) {
	register_vector<std::string>("VectorString");

	class_<PainPerdu::parser::Token>("PainPerduToken")
		.function("get_type_index", &PainPerdu::parser::Token::get_type_index)
		.function("get_line", &PainPerdu::parser::Token::get_line)
		.function("get_start_column", &PainPerdu::parser::Token::get_start_column)
		.function("get_length", &PainPerdu::parser::Token::get_length);

	register_vector<PainPerdu::parser::Token>("VectorToken");
	function("get_tokens", &get_tokens);
	function("get_defined_labels", &get_defined_labels);
	function("get_defined_references", &get_defined_references);

	function("run_pain_perdu_code", &PainPerdu::run_pain_perdu_code_no_input);

	register_vector<uint8_t>("vector<uint8_t>");
	class_<PainPerdu::PainPerduResult>("PainPerduResult")
		.function("console_output", &PainPerdu::PainPerduResult::get_console_output)
		.function("stack", &PainPerdu::PainPerduResult::get_stack);
}