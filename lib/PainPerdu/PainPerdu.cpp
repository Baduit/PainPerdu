#include <sstream>

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

PainPerduResult run_pain_perdu_code_no_input(const std::string& input)
{
	PainPerduResult result;
	try
	{
		std::stringstream out;
		std::stringstream in;
		PainPerdu::Interpreter interpreter(in, out);
		interpreter.disable_input();

		interpreter.compile_and_run(input);

		result.console_output = out.str();
		result.stack = interpreter.get_stack();
	}
	catch (std::exception& e)
	{
		result.console_output = std::string("Error : ") + e.what();
	}
	return result;
}

} // namespace PainPerdu