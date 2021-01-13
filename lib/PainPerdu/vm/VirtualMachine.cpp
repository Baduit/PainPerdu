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
{
	_labels["__start__"] = 0;
	_references["__begin__"] = 0;
}

void VirtualMachine::compile(Definitions definitions)
{
	std::size_t old_recipe_size = _definitions.recipe.size();
	_definitions += optimize(std::move(definitions));

	for (const auto& a: _definitions.annotations)
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
	_labels["__exit__"] = _definitions.recipe.size();
}

void VirtualMachine::run()
{
	bool run = true;
	// I use integers instead of iterators to avoid iterator invalidation
	while (run && _step < _definitions.recipe.size())
	{
		std::visit(
			[&, this](const auto& current_instruction)
			{
				using T = std::decay_t<decltype(current_instruction)>;
				if constexpr (std::same_as<instructions::MoveRight, T>)
				{
					//logger[LogCategory::VM].debug("Move right");
					if (_memory.advance_cursor(current_instruction.value))
						_references["__end__"] = _memory.get_stack_size();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveLeft, T>)
				{
					//logger[LogCategory::VM].debug("Move left");
					_memory.move_back_cursor(current_instruction.value);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::Increment, T>)
				{
					//logger[LogCategory::VM].debug("Incr");
					_memory.incr_current_case(current_instruction.value);
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::Decrement, T>)
				{
					//logger[LogCategory::VM].debug("Decr");
					_memory.decr_current_case(current_instruction.value);
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::DefineReference, T>)
				{
					//logger[LogCategory::VM].debug("Define reference");
					_references[current_instruction.identifier] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::UndefineReference, T>)
				{
					//logger[LogCategory::VM].debug("Undefine reference");
					_references.erase(current_instruction.identifier);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveToReference, T>)
				{
					//logger[LogCategory::VM].debug("Move to reference");
					// No need to check, because if there is a reference, it already exists
					_memory.move_cursor_to_no_check(_references[current_instruction.identifier]);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::GoToLabel, T>)
				{
					//logger[LogCategory::VM].debug("GoToLabel : ", current_instruction.identifier);
					if (auto it = _labels.find(current_instruction.identifier); it != _labels.end())
					{
						_step = it->second;
					}
					else
					{
						logger[LogCategory::VM].error("Label '", current_instruction.identifier, "' does not exist");
						run = false;
					}
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueDifferent0, T>)
				{
					//logger[LogCategory::VM].debug("Condition with 0, current value is == ", _memory.get_current_case(), " cursor is at ", _memory.get_cursor_position());
					if (_memory.get_current_case() != 0)
						_step += 1;
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueEqualsN, T>)
				{
					//logger[LogCategory::VM].debug("Check current value with n");
					if (_memory.get_current_case() == current_instruction.value)
						_step += 1;
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::IfCursorIsAtReference, T>)
				{
					//logger[LogCategory::VM].debug("Check cursor is at reference");
					if (auto it = _references.find(current_instruction.identifier); it != _references.end())
					{
						if (_memory.get_cursor_position() == it->second)
							_step += 1;
						else
							_step += 2;
					}
					else
					{
						logger[LogCategory::VM].error("Reference '", current_instruction.identifier, "' does not exist");
						run = false;	
					}
				}
				else if constexpr (std::same_as<instructions::IfReferenceExists, T>)
				{
					//logger[LogCategory::VM].debug("Check reference exists");
					if (auto it = _references.find(current_instruction.identifier); it != _references.end())
						_step += 1;	
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::GetChar, T>)
				{
					//logger[LogCategory::VM].debug("Get char");
					// todo check _in state
					uint8_t c = 0;
					_in >> c;
					_memory.set_current_case(c);
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::PutChar, T>)
				{
					//logger[LogCategory::VM].debug("Put char");
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
