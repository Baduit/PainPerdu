#include <vector>
#include <string>

#include <emscripten/bind.h>

#include <PainPerdu/parser/Parser.hpp>

using namespace emscripten;

static std::vector<std::string> TOKEN_TYPE_STRINGS =
    {
        "variable",
        "label",
        "comment",
        "string",
        "number",
        "operator",
    };

std::size_t to_index(PainPerdu::parser::Token::Type type)
{
    return static_cast<std::size_t>(type);
}

std::vector<PainPerdu::parser::Token> get_tokens(const std::string& input)
{
    return PainPerdu::Parser().get_tokens(input);
}

EMSCRIPTEN_BINDINGS(PainPerduParserModule) {
    register_vector<std::string>("VectorString");
    constant("TOKEN_TYPE_STRINGS", TOKEN_TYPE_STRINGS);

    enum_<PainPerdu::parser::Token::Type>("PainPerduTokenType");

    class_<PainPerdu::parser::Token>("PainPerduToken")
        .function("get_type", &PainPerdu::parser::Token::get_type)
        .function("get_line", &PainPerdu::parser::Token::get_line)
        .function("get_start_column", &PainPerdu::parser::Token::get_start_column)
        .function("get_length", &PainPerdu::parser::Token::get_length);
    
    register_vector<PainPerdu::parser::Token>("VectorToken");
    function("get_tokens", &get_tokens);
}