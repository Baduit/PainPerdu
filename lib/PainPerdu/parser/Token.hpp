#pragma once

#include <string_view>
#include <compare>
#include <type_traits>

namespace PainPerdu::parser
{

struct Token
{
	// Enum totally bugged so I'm making a struct with constante to simulate it
	// And also string type because this will be used in javascript as string values
	/* struct Type
	{
		// The variable name correspond to its PainPerdu meaning
		// The variable value correspond to how I want to color it in vscode
		inline static std::string VARIABLE = "variable";
		inline static std::string LABEL = "function";
		inline static std::string COMMENT = "comment";
		inline static std::string STRING = "string";
		inline static std::string NUMBER = "number";
		inline static std::string OPERATOR = "operator";
	}; */
	enum class Type
	{
		VARIABLE,
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