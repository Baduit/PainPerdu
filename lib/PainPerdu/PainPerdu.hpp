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

		void enable_input()
		{
			_vm.enable_input();
		}

		void disable_input()
		{
			_vm.disable_input();
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