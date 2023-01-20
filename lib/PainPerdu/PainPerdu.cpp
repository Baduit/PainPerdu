#include <PainPerdu/PainPerdu.hpp>

namespace PainPerdu
{

Interpreter::Interpreter(std::istream& in, std::ostream& out):
	_parser(),
	_vm(in, out)
{}


void Interpreter::compile_and_run(std::string_view input)
{
	_vm.compile(_parser(input));
	_vm.run();
}

void Interpreter::enable_input()
{
	_vm.enable_input();
}

void Interpreter::disable_input()
{
	_vm.disable_input();
}

const std::vector<uint8_t>& Interpreter::get_stack() const
{
	return _vm.get_stack();
}

} // namespace PainPerdu