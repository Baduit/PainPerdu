#include <concepts>

#include <PainPerdu/vm/VirtualMachine.hpp>
#include <PainPerdu/misc/Log.hpp>

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
		std::visit(
			[&](const auto& current_instruction)
			{
				using T = std::decay_t<decltype(current_instruction)>;
				if constexpr (std::same_as<instructions::MoveRight, T>)
				{
					_memory.advance_cursor(current_instruction.value);
				}
				else if constexpr (std::same_as<instructions::MoveLeft, T>)
				{
					_memory.move_back_cursor(current_instruction.value);
				}
				else if constexpr (std::same_as<instructions::Increment, T>)
				{
					_memory.incr_current_case(current_instruction.value);
				}
				else if constexpr (std::same_as<instructions::Decrement, T>)
				{
					_memory.decr_current_case(current_instruction.value);
				}
				else
				{
					logger[LogCategory::VM].error("Not implemented yet.");
				}
			}, _definitions.recipe[_step]);
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
