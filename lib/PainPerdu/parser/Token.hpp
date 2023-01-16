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

    static constexpr std::string_view TYPE_STRINGS[] =
        {
            "variable",
            "label",
            "comment",
            "string",
            "number",
            "operator",
        };

    std::size_t to_index(Type type)
    {
        return static_cast<std::size_t>(type);
    }

    bool operator==(const Token&) const = default;

	Type type;
	std::size_t line;
	std::size_t start_column;
	std::size_t lenght;
};


} // namespace PainPerdu::parser