#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <external/Rang/Rang.hpp>
#include <external/magic_enum/magic_enum.hpp>
#include <external/nameof/nameof.hpp>

enum class LogCategory
{
	Parser,
	VM,
	Misc
};

class Sink
{
	public:
		Sink(LogCategory category):
			_category(category)
		{}


	template <typename ...Args>
	void info(Args&&... args)
	{
		print(rang::fg::green, "INFO", std::forward<Args>(args)...)	;
	}

	template <typename ...Args>
	void weird(Args&&... args)
	{
		print(rang::fg::magenta, "WEIRD", std::forward<Args>(args)...)	;
	}

	template <typename ...Args>
	void warning(Args&&... args)
	{
		print(rang::fg::yellow, "WARNING", std::forward<Args>(args)...)	;
	}

	template <typename ...Args>
	void error(Args&&... args)
	{
		print(rang::fg::red, "ERROR", std::forward<Args>(args)...)	;
	}

	template <typename ...Args>
	void debug(Args&&... args)
	{
		print(rang::fg::blue, "DEBUG", std::forward<Args>(args)...)	;
	}

	LogCategory get_category() const { return _category; }

	private:
		template <typename ...Args>
		void print(rang::fg fg_color, std::string_view importance, Args&&... args)
		{
			std::cout << "[" << fg_color << importance;
			std::cout << rang::style::reset << "] [" << magic_enum::enum_name(_category) << "] ";
			(std::cout << ... << args) << std::endl;
		}

	private:
		LogCategory _category;
};

class Logger
{
	public:	
		Logger()
		{
			const auto& values = magic_enum::enum_values<LogCategory>();
			for (const auto& c : values)
			{
				sinks.emplace_back(c);
			}
		}

		Sink& operator[](LogCategory category)
		{
			auto it = std::find_if(sinks.begin(), sinks.end(),
				[category](const Sink& s)
				{
					return s.get_category() == category;
				});
			return *it;
		}

	private:
		std::vector<Sink> sinks;
};

inline Logger logger = {};