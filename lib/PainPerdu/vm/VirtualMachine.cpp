#include <PainPerdu/vm/VirtualMachine.hpp>

namespace PainPerdu
{

namespace vm
{

VirtualMachine::VirtualMachine(std::istream& in, std::ostream& out):
	_in(in),
	_out(out)
{}

void VirtualMachine::compile(Definitions definitions)
{
	_definitions += optimize(std::move(definitions));
}

void VirtualMachine::run()
{
	// I use integers instead of iterators to avoid iterator invalidation
	while (_step < _definitions.recipe.size())
	{
		++_step;
	}
}

Definitions VirtualMachine::optimize(Definitions&& definitions)
{
	// todo : optimize cursor movement
	// todo : optimize step movement
	// todo : optimize incr/decr
	return definitions;
}

} // namespace vm


} // namespace PainPerdu
