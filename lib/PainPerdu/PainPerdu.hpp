#pragma once

#include <string_view>

#include <PainPerdu/parser/Parser.hpp>
#include <PainPerdu/vm/VirtualMachine.hpp>

namespace PainPerdu
{

class Interpreter
{
	public:
		Interpreter(std::istream& in, std::ostream& out);

		void compile_and_run(std::string_view input);
		void enable_input();
		void disable_input();
		const std::vector<uint8_t>& get_stack() const;

	private:
		Parser _parser;
		vm::VirtualMachine _vm;
};

} // namespace PainPerdu