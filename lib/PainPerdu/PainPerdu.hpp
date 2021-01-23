#pragma once

#include <string_view>

#include <PainPerdu/parser/Parser.hpp>
#include <PainPerdu/vm/VirtualMachine.hpp>

namespace PainPerdu
{

class Interpreter
{
	public:
		Interpreter(std::istream& in, std::ostream& out):
			_parser(),
			_vm(in, out)
		{}


		void compile_and_run(std::string_view input)
		{
			_vm.compile(_parser(input));
			_vm.run();
		}

		void enable_get_char()
		{
			_vm.enable_get_char();
		}

		void disable_get_char()
		{
			_vm.disable_get_char();
		}

		const std::vector<uint8_t>& get_stack() const
		{
			return _vm.get_stack();
		}

	private:
		Parser _parser;
		vm::VirtualMachine _vm;
};

} // namespace PainPerdu