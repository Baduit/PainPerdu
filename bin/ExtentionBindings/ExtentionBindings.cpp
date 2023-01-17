#include <vector>
#include <string>

#include <emscripten/bind.h>

#include <PainPerdu/parser/Parser.hpp>

using namespace emscripten;

std::vector<PainPerdu::parser::Token> get_tokens(const std::string& input)
{
    return PainPerdu::Parser().get_tokens(input);
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
}