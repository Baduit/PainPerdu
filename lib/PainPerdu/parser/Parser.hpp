#pragma once

#include <vector>
#include <string_view>

#include <PainPerdu/Definitions.hpp>
#include <PainPerdu/parser/Token.hpp>

namespace PainPerdu
{

namespace parser
{

class Parser
{
	public:
		Definitions operator()(std::string_view input);

		std::vector<Token> get_tokens(std::string_view input);
		std::vector<std::string> get_defined_labels(std::string_view input);
		std::vector<std::string> get_defined_references(std::string_view input);
};

} // namespace parser

using parser::Parser;

} // namespace PainPerdu
