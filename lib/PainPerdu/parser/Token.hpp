#pragma once

#include <string_view>
#include <compare>

namespace PainPerdu::parser
{

struct Token
{
    // There will be some bindings,
    // that'w why I prefer having a specified type
	enum class Type: std::size_t
	{
        // I know that I don't need to specify the value
        // But in this case I prefer to be specific
		VARIABLE = 0,
		LABEL = 1,
		COMMENT = 2,
		STRING = 3,
		NUMBER = 4,
		OPERATOR = 5
	};

    bool operator==(const Token&) const = default;

	Type type;
	std::size_t line;
	std::size_t start_column;
	std::size_t length;

	// Getter for the bindings
	Type get_type() const { return type; }
	std::size_t get_line() const { return line; }
	std::size_t get_start_column() const { return start_column; }
	std::size_t get_length() const { return length; }
};


} // namespace PainPerdu::parser