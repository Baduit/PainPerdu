#pragma once

#include <vector>
#include <string_view>

#include <PainPerdu/Definitions.hpp>

namespace PainPerdu
{

namespace parserv2
{

class Parser
{
	public:
		Definitions operator()(std::string_view input);
};

} // namespace parserv2

} // namespace PainPerdu
