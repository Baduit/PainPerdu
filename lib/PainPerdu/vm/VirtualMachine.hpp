#pragma once

#include <map>
#include <iostream>

#include <PainPerdu/Definitions.hpp>
#include <PainPerdu/vm/Memory.hpp>

namespace PainPerdu
{

namespace vm
{

class VirtualMachine
{
	public:
		VirtualMachine(std::istream& in, std::ostream& out);

		void compile(Definitions definitions);
		void run();

		Definitions optimize(Definitions&& definitions);

		void enable_get_char();
		void disable_get_char();

	private:
		std::istream& _in;
		std::ostream& _out;
		Definitions _definitions;

		Memory _memory;
		// I use integers instead of iterators to avoid iterator invalidation
		std::size_t _step = 0;

		std::map<std::string, std::size_t> _references;
		std::map<std::string, std::size_t> _labels;

		bool _get_char_enabled = true;
};

} // namespace vm


} // namespace PainPerdu
