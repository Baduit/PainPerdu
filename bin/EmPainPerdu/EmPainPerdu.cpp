#include <sstream>

#include <emscripten/bind.h>

#include <PainPerdu/PainPerdu.hpp>

using namespace emscripten;

struct PainPerduResult
{
	const auto& get_console_output() const { return console_output; }
	const auto& get_stack() const { return stack; }

	std::string console_output;
	std::vector<uint8_t> stack;
};

PainPerduResult run_pain_perdu_code(const std::string& input)
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


EMSCRIPTEN_BINDINGS(my_module) {
	function("run_pain_perdu_code", &run_pain_perdu_code);

	register_vector<uint8_t>("vector<uint8_t>");
	class_<PainPerduResult>("PainPerduResult")
		.function("console_output", &PainPerduResult::get_console_output)
		.function("stack", &PainPerduResult::get_stack);
}