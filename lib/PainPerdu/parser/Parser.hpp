#pragma once

#include <vector>
#include <string_view>

#include <crepuscule/crepuscule.hpp>
#include <PainPerdu/Definitions.hpp>

namespace PainPerdu
{

namespace parser
{

class Parser
{
	public:
		Parser();

		Definitions operator()(std::string_view input);

	private:
		crepuscule::Tokenizer _tokeniser;
};

} // namespace parser

using parser::Parser;

} // namespace PainPerdu
