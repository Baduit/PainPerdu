#include <concepts>

#include <PainPerdu/vm/VirtualMachine.hpp>
#include <PainPerdu/misc/Log.hpp>

namespace
{

template<class> inline constexpr bool always_false_v = false;

} // namespace


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
	std::size_t old_recipe_size = _definitions.recipe.size();
	_definitions += optimize(std::move(definitions));

	for (const auto& a: definitions.annotations)
	{
		std::visit(
			[&](const auto& annotation)
			{
				using T = std::decay_t<decltype(annotation)>;
				if constexpr (std::same_as<annotations::DefineLabel, T>)
				{
					_labels[annotation.identifier] = annotation.step_index + old_recipe_size;
				}
				else
				{
					static_assert(always_false_v<T>, "non-exhaustive visitor!");
				}
			}, a);
	}
}

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
					_references[current_instruction.identifier] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::UndefineReference, T>)
				{
					_references.erase(current_instruction.identifier);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveToReference, T>)
				{
					// No need to check, because if there is a reference, it already exists
					_memory.move_cursor_to_no_check(_references[current_instruction.identifier]);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::GoToLabel, T>)
				{
					_step = _labels[current_instruction.identifier];
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueEquals0, T>)
				{
					if (_memory.get_current_case() == 0)
						_step += 1;
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueEqualsN, T>)
				{
					if (_memory.get_current_case() == current_instruction.value)
						_step += 1;
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::IfCursorIsAtReference, T>)
				{
					if (auto it = _references.find(current_instruction.identifier); it != _references.end())
					{
						if (_memory.get_cursor_position() == it->second)
							_step += 1;
						else
							_step += 2;
					}
					else
					{
						logger[LogCategory::VM].error("Reference does not exist");
						run = false;	
					}
				}
				else if constexpr (std::same_as<instructions::IfReferenceExists, T>)
				{
					if (auto it = _references.find(current_instruction.identifier); it != _references.end())
					{
						_step += 1;	
					}
					else
					{
						_step += 2;
					}
				}
				else if constexpr (std::same_as<instructions::GetChar, T>)
				{
					// todo check _in state
					uint8_t c = 0;
					_in >> c;
					_memory.set_current_case(c);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::PutChar, T>)
				{
					// todo check _out state
					_out << _memory.get_current_case();
					_step += 1;
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
	// Do not forget to update the labels's index
	return definitions;
}

} // namespace vm


} // namespace PainPerdu
