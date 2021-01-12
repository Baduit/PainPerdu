#include <PainPerdu/vm/Memory.hpp>

namespace PainPerdu
{

namespace vm
{

Memory::Memory()
{
	_stack.resize(1);
}

void Memory::advance_cursor(std::size_t n)
{
	_cursor += n;
	resize_if_needed();
}

void Memory::move_back_cursor(std::size_t n)
{
	_cursor -= n;
}

void Memory::move_cursor_to(std::size_t position)
{
	_cursor = position;
	resize_if_needed();
}

void Memory::move_cursor_to_no_check(std::size_t position)
{
	_cursor = position;
}

std::size_t Memory::get_cursor_position() const
{
	return _cursor;
}

void Memory::incr_current_case(uint8_t n)
{
	// overflow if defined behavior
	_stack[_cursor] += n;
}
void Memory::decr_current_case(uint8_t n)
{
	// underflow if defined behavior
	_stack[_cursor] -= n;
}

uint8_t Memory::get_current_case() const
{
	return _stack[_cursor];
}

void Memory::set_current_case(uint8_t n)
{
	_stack[_cursor] = n;
}

void Memory::resize_if_needed()
{
	if (_cursor >= _stack.size())
	{
		_stack.resize(_cursor);
	}
}

} // namespace vm


} // namespace PainPerdu
