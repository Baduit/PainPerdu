#include <emscripten/bind.h>

#include <PainPerdu/PainPerdu.hpp>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
	function("run_pain_perdu_code", &PainPerdu::run_pain_perdu_code_no_input);

	register_vector<uint8_t>("vector<uint8_t>");
	class_<PainPerdu::PainPerduResult>("PainPerduResult")
		.function("console_output", &PainPerdu::PainPerduResult::get_console_output)
		.function("stack", &PainPerdu::PainPerduResult::get_stack);
}