#pragma once

#include <PainPerdu/Definitions.hpp>

namespace PainPerdu
{

namespace parser
{

class ParsingState
{
	public:
		ParsingState(Definitions& definitions):
			_definitions(definitions)
		{}

		template <typename T, typename... Args>
		void emplace_annotation(Args&&... args)
		{
			_definitions.annotations.emplace_back(T(args...));
		}

		template <typename T, typename... Args>
		void emplace_instruction(Args&&... args)
		{
			_definitions.recipe.emplace_back(T(args...));
		}

		std::size_t get_next_instruction_index() const
		{
			return _definitions.recipe.size();
		}

	private:
		Definitions& _definitions;
};

} // namespace parser

} // namespace heart
