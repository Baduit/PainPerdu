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

// Alternative API
struct PainPerduResult
{
	const auto& get_console_output() const { return console_output; }
	const auto& get_stack() const { return stack; }

	std::string console_output;
	std::vector<uint8_t> stack;
};

PainPerduResult run_pain_perdu_code_no_input(const std::string& input);

} // namespace PainPerdu