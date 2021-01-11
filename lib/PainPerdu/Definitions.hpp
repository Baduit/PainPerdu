#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <variant>
#include <compare>

namespace PainPerdu
{

namespace annotations
{

struct DefineLabel
{
	bool operator==(const DefineLabel&) const = default;
	
	std::string identifier;
	std::size_t step_index;
};

} // namespace annotations
using Annotation = std::variant<annotations::DefineLabel>;

namespace instructions
{

struct MoveRight
{
	bool operator==(const MoveRight&) const = default;
	
	int value;
};

struct MoveLeft
{
	bool operator==(const MoveLeft&) const = default;
	
	int value;
};

struct Increment
{
	bool operator==(const Increment&) const = default;
	
	uint8_t value;
};

struct Decrement
{
	bool operator==(const Decrement&) const = default;
	
	uint8_t value;
};

struct DefineReference
{
	bool operator==(const DefineReference&) const = default;
	
	std::string identifier;
};

struct MoveToReference
{
	bool operator==(const MoveToReference&) const = default;
	
	std::string identifier;
};

struct GoToLabel
{
	bool operator==(const GoToLabel&) const = default;
	
	std::string identifier;
};

struct IfCurrentValueEquals0
{
	bool operator==(const IfCurrentValueEquals0&) const = default;
	

};

struct IfCurrentValueEqualsN
{
	bool operator==(const IfCurrentValueEqualsN&) const = default;
	
	uint8_t value;
};

struct IfCursorIsAtReference
{
	bool operator==(const IfCursorIsAtReference&) const = default;
	
	std::string identifier;
};

struct IfReferenceExists
{
	bool operator==(const IfReferenceExists&) const = default;
	
	std::string identifier;
};

struct IfLabelExists
{
	bool operator==(const IfLabelExists&) const = default;
	
	std::string identifier;
};

struct GetChar
{
	bool operator==(const GetChar&) const = default;
	

};

struct PutChar
{
	bool operator==(const PutChar&) const = default;
	

};

} // namespace instructions
using Instruction =
	std::variant
		<
			instructions::MoveRight,
			instructions::MoveLeft,
			instructions::Increment,
			instructions::Decrement,
			instructions::DefineReference,
			instructions::MoveToReference,
			instructions::GoToLabel,
			instructions::IfCurrentValueEquals0,
			instructions::IfCurrentValueEqualsN,
			instructions::IfCursorIsAtReference,
			instructions::IfReferenceExists,
			instructions::IfLabelExists,
			instructions::GetChar,
			instructions::PutChar
		>;

struct Definitions
{
	bool operator==(const Definitions&) const = default;
	
	std::vector<Annotation> annotations;
	std::vector<Instruction> instructions;
};

} // namespace PainPerdu
