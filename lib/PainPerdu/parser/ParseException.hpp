#pragma once

#include <stdexcept>
#include <string>
#include <concepts>

namespace PainPerdu
{

namespace parser
{

class ParseException: public std::runtime_error
{
	public:
		template <std::integral Number>
		ParseException(Number line, const std::string& message):
			std::runtime_error("On line: " + std::to_string(line) + " Message: " + message)
		{}
};

} // namespace parser


} // namespace PainPerdu
