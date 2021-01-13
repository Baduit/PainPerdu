#pragma once

#include <vector>
#include <cstdint>

namespace PainPerdu
{

namespace vm
{

class Memory
{
	public:
		Memory();

		bool advance_cursor(std::size_t n);
		void move_back_cursor(std::size_t n);

		bool move_cursor_to(std::size_t position);
		void move_cursor_to_no_check(std::size_t position);

		std::size_t get_cursor_position() const;

		void incr_current_case(uint8_t n);
		void decr_current_case(uint8_t n);

		uint8_t get_current_case() const;
		void set_current_case(uint8_t n);

		std::size_t get_stack_size() const;

		void debug_dump_memory() const;

	private:
		bool resize_if_needed();

	private:
		std::vector<uint8_t> _stack;
		// I use integers instead of iterators to avoid iterator invalidation
		std::size_t _cursor = 0;
};

} // namespace vm


} // namespace PainPerdu
