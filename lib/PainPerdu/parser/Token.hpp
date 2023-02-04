#pragma once

#include <string_view>
#include <compare>
#include <type_traits>

namespace PainPerdu::parser
{

struct Token
{
	enum class Type
	{
		REFERENCE,
		LABEL,
		COMMENT,
		STRING,
		NUMBER,
		OPERATOR
	};
	using TypeIntType = std::underlying_type_t<Type>;

    bool operator==(const Token&) const = default;

	Type type;
	std::size_t line;
	std::size_t start_column;
	std::size_t length;

	// Getter for the bindings
	Type get_type() const { return type; }
	TypeIntType get_type_index() const { return static_cast<TypeIntType>(type); }
	std::size_t get_line() const { return line; }
	std::size_t get_start_column() const { return start_column; }
	std::size_t get_length() const { return length; }

};


} // namespace PainPerdu::parser