#include <concepts>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <PainPerdu/vm/VirtualMachine.hpp>
#include <PainPerdu/misc/Log.hpp>

namespace
{

template<class> inline constexpr bool always_false_v = false;

std::string		readAllContent(std::string_view filename)
{
	namespace fs = std::filesystem;

	std::ifstream	file(filename.data(), std::ios::binary);
	if (!file)
		throw std::runtime_error("Invalid file");

	auto file_size = fs::file_size(filename);

	std::string file_content;
	file_content.resize(file_size);
	file.read(file_content.data(), static_cast<std::streamsize>(file_size));
	return file_content;
}

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
	_references["__here__"] = 0;
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
	// I use integers instead of iterators to avoid iterator invalidation
	while (_step < _definitions.recipe.size())
	{
		std::visit(
			[&, this](const auto& current_instruction)
			{
				using T = std::decay_t<decltype(current_instruction)>;
				if constexpr (std::same_as<instructions::MoveRight, T>)
				{
					if (_memory.advance_cursor(current_instruction.value))
						_references["__end__"] = _memory.get_stack_size();
					_references["__here__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveRightRef, T>)
				{
					if (_memory.advance_cursor(get_value_at_reference(current_instruction.reference)))
						_references["__end__"] = _memory.get_stack_size();
					_references["__here__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveLeft, T>)
				{
					_memory.move_back_cursor(current_instruction.value);
					_references["__here__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveLeftRef, T>)
				{
					_memory.move_back_cursor(get_value_at_reference(current_instruction.reference));
					_references["__here__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::Increment, T>)
				{
					_memory.incr_current_case(current_instruction.value);
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::IncrementRef, T>)
				{
					_memory.incr_current_case(get_value_at_reference(current_instruction.reference));
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::Decrement, T>)
				{
					_memory.decr_current_case(current_instruction.value);
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::DecrementRef, T>)
				{
					_memory.decr_current_case(get_value_at_reference(current_instruction.reference));
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::ResetCase, T>)
				{
					_memory.reset_current_case();
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::DefineReference, T>)
				{
					_references[current_instruction.identifier] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::UndefineReference, T>)
				{
					auto it = _references.find(current_instruction.identifier);
					if (it == _references.end())
						throw std::runtime_error("Reference '" + current_instruction.identifier + "' does not exist");
					_references.erase(it);
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::MoveToReference, T>)
				{
					_memory.move_cursor_to_no_check(get_reference(current_instruction.identifier));
					_references["__here__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::GoToLabel, T>)
				{
					_labels_rewind[current_instruction.identifier] = _step;
					_step = get_label(current_instruction.identifier);
				}
				else if constexpr (std::same_as<instructions::Rewind, T>)
				{
					if (auto it = _labels_rewind.find(current_instruction.identifier); it != _labels_rewind.end())
						_step = it->second + 1;
					else
						throw std::runtime_error("Can't rewind with the identifier : '" + current_instruction.identifier + "'.");
				}
				else if constexpr (std::same_as<instructions::IfCurrentValueDifferent0, T>)
				{
					if (_memory.get_current_case() != 0)
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
				else if constexpr (std::same_as<instructions::IfCurrentValueEqualsNRef, T>)
				{
					if (_memory.get_current_case() == get_value_at_reference(current_instruction.reference))
						_step += 1;
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::IfCursorIsAtReference, T>)
				{
					if (_memory.get_cursor_position() == get_reference(current_instruction.identifier))
						_step += 1;
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::IfReferenceExists, T>)
				{
					if (auto it = _references.find(current_instruction.identifier); it != _references.end())
						_step += 1;	
					else
						_step += 2;
				}
				else if constexpr (std::same_as<instructions::GetChar, T>)
				{
					if (!_input_enabled)
						throw std::runtime_error("Get char instruction has been disabled.");
					// todo check _in state
					uint8_t c = 0;
					_in >> c;
					_memory.set_current_case(c);
					_references["__last_modified__"] = _memory.get_cursor_position();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::PutChar, T>)
				{
					// todo check _out state
					_out << _memory.get_current_case();
					_step += 1;
				}
				else if constexpr (std::same_as<instructions::ReadFile, T>)
				{
					std::cout << "here" << std::endl;
					std::cout << "current_instruction.filename" << current_instruction.filename << std::endl;
					if (!_input_enabled)
						throw std::runtime_error("Read file instruction has been disabled.");
					std::string str = readAllContent(current_instruction.filename);
					if (!str.empty())
					{
						if (_memory.write_data(str))
							_references["__end__"] = _memory.get_stack_size();
						_references["__here__"] = _memory.get_cursor_position();
						_references["__last_modified__"] = _memory.get_cursor_position();
					}

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

void VirtualMachine::enable_input()
{
	_input_enabled = true;
}

void VirtualMachine::disable_input()
{
	_input_enabled = false;
}

const std::vector<uint8_t>& VirtualMachine::get_stack() const
{
	return _memory.get_stack();
}

std::size_t VirtualMachine::get_label(const std::string& identifier) const
{
	auto it = _labels.find(identifier); 
	if (it == _labels.end())
		throw std::runtime_error("Label '" + identifier + "' does not exist");
	return it->second;
}

std::size_t VirtualMachine::get_reference(const std::string& identifier) const
{
	auto it = _references.find(identifier); 
	if (it == _references.end())
		throw std::runtime_error("Reference '" + identifier + "' does not exist");
	return it->second;
}

uint8_t VirtualMachine::get_value_at_reference(const std::string& identifier) const
{
	return _memory.get_stack()[get_reference(identifier)];
}

} // namespace vm


} // namespace PainPerdu
