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

	private:
		Parser _parser;
		vm::VirtualMachine _vm;
};

} // namespace PainPerdu