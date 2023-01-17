#pragma once

#include <vector>
#include <string>

#include <PainPerdu/parser/Grammar.hpp>

namespace PainPerdu::parser
{

template<typename Rule>
struct GetDefinedLabelsAction {};    

template <>                                                                       
struct GetDefinedLabelsAction<operators::DefineLabel>
{
    template <typename ParseInput>
    static void apply(const ParseInput& in, std::vector<std::string>&, bool& is_next_label)
    {
        is_next_label = true;
    }
};

template <>                                                                       
struct GetDefinedLabelsAction<Identifier>
{
    template <typename ParseInput>
    static void apply(const ParseInput& in, std::vector<std::string>& labels, bool& is_next_label)
    {
        if (is_next_label)
        {
            labels.push_back(in.string());
            is_next_label = false;
        }
    }
};

} // namespace PainPerdu::parser