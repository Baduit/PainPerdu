#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <variant>
#include <compare>
#include <algorithm>
#include <iterator>

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
	
	std::size_t value;
};

struct MoveLeft
{
	bool operator==(const MoveLeft&) const = default;
	
	std::size_t value;
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

struct UndefineReference
{
	bool operator==(const UndefineReference&) const = default;
	
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

struct IfCurrentValueDifferent0
{
	bool operator==(const IfCurrentValueDifferent0&) const = default;
	

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
			instructions::UndefineReference,
			instructions::MoveToReference,
			instructions::GoToLabel,
			instructions::IfCurrentValueDifferent0,
			instructions::IfCurrentValueEqualsN,
			instructions::IfCursorIsAtReference,
			instructions::IfReferenceExists,
			instructions::GetChar,
			instructions::PutChar
		>;

struct Definitions
{
	bool operator==(const Definitions&) const = default;

	Definitions& operator+=(const Definitions& other)
	{
		annotations.insert(annotations.end(), other.annotations.begin(), other.annotations.end());
		recipe.insert(recipe.end(), other.recipe.begin(), other.recipe.end());
		return *this;
	}

	Definitions& operator+=(Definitions&& other)
	{
		annotations.insert(annotations.end(), std::make_move_iterator(other.annotations.begin()), std::make_move_iterator(other.annotations.end()));
		recipe.insert(recipe.end(), std::make_move_iterator(other.recipe.begin()), std::make_move_iterator(other.recipe.end()));
		return *this;
	}
	
	std::vector<Annotation> annotations;
	std::vector<Instruction> recipe;
};
// TODO: define operator >> to be able to dump Definitions into a stream (so I can dump optimized code)

} // namespace PainPerdu
