#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <variant>

namespace PainPerdu
{

namespace annotations
{

struct DefineLabel
{
	std::string identifier;
	std::size_t step_index;
};

} // namespace annotations
using Annotation = std::variant<annotations::DefineLabel>;

namespace instructions
{

struct MoveRight
{
	std::size_t value;
};

struct MoveLeft
{
	std::size_t value;
};

struct Increment
{
	uint8_t value;
};

struct Decrement
{
	uint8_t value;
};

struct DefineReference
{
	std::string identifier;
	std::size_t cursor_index;
};

struct MoveToReference
{
	std::string identifier;
};

struct GoToLabel
{
	std::string identifier;
};

struct IfCurrentValueEquals0
{

};

struct IfCurrentValueEqualsN
{
	uint8_t value;
};

struct IfCursorIsAtReference
{
	std::string identifier;
};

struct IfReferenceExists
{
	std::string identifier;
};

struct IfLabelExists
{
	std::string identifier;
};

struct GetChar
{

};

struct PutChar
{

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
	std::vector<Annotation> annotations;
	std::vector<Instruction> instructions;
};

} // namespace PainPerdu
