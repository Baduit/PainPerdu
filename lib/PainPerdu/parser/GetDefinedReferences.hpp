#pragma once

#include <vector>
#include <string>

#include <PainPerdu/parser/Grammar.hpp>

namespace PainPerdu::parser
{

template<typename Rule>
struct GetDefinedReferencesAction {};

template<>
struct GetDefinedReferencesAction<operators::DefineReference>
{
	template <typename ParseInput>
	static void apply(const ParseInput&, std::vector<std::string>&, bool& is_next_reference)
	{
		is_next_reference = true;
	}
};

template<>
struct GetDefinedReferencesAction<Identifier>
{
	template <typename ParseInput>
	static void apply(const ParseInput& in, std::vector<std::string>& references, bool& is_next_reference)
	{
		if (is_next_reference)
		{
			references.push_back(in.string());
			is_next_reference = false;
		}
	}
};

} // namespace PainPerdu::parser