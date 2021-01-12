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

template<class> inline constexpr bool always_false_v = false;

void VirtualMachine::run()
{
	bool run = true;
	// I use integers instead of iterators to avoid iterator invalidation
	while (run && _step < _definitions.recipe.size())
	{
		std::visit(
			[&](const auto& current_instruction)
			{
				using T = std::decay_t<decltype(current_instruction)>;
				if constexpr (std::same_as<instructions::MoveRight, T>)
				{
					_memory.advance_cursor(current_instruction.value);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveLeft, T>)
				{
					_memory.move_back_cursor(current_instruction.value);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::Increment, T>)
				{
					_memory.incr_current_case(current_instruction.value);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::Decrement, T>)
				{
					_memory.decr_current_case(current_instruction.value);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::DefineReference, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::UndefineReference, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::MoveToReference, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::GoToLabel, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueEquals0, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueEqualsN, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::IfCursorIsAtReference, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::IfReferenceExists, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::GetChar, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else if constexpr (std::same_as<instructions::PutChar, T>)
				{
					logger[LogCategory::VM].error("Not implemented yet.");
					run = false;
				}
				else
				{
					static_assert(always_false_v<T>, "non-exhaustive visitor!");
				}
			}, _definitions.recipe[_step]);
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
