#pragma once

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

	private:
		std::istream& _in;
		std::ostream& _out;
		Definitions _definitions;

		Memory _memory;
		// I use integers instead of iterators to avoid iterator invalidation
		std::size_t _step = 0;
};

} // namespace vm


} // namespace PainPerdu
