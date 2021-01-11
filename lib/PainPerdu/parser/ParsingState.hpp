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
			_definitions.instructions.emplace_back(T(args...));
		}

	private:
		Definitions& _definitions;
};

} // namespace parser

} // namespace heart
