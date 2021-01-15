#include <iostream>
#include <filesystem>
#include <fstream>

#include <PainPerdu/PainPerdu.hpp>

namespace fs = std::filesystem;

inline std::string		readAllContent(std::string_view filename)
{
	std::ifstream	file(filename.data(), std::ios::binary);
	if (!file)
		throw std::runtime_error("Invalid file");

	auto file_size = fs::file_size(filename);

	std::string file_content;
	file_content.resize(file_size);
	file.read(file_content.data(), static_cast<std::streamsize>(file_size));
	std::erase(file_content, '\r');
	return file_content;
}

int main(int argc, char** argv)
{
	PainPerdu::Interpreter interpreter(std::cin, std::cout);

	if (argc < 2)
	{
		std::string line;
		while (std::getline(std::cin, line))
		{
			interpreter.compile_and_run(std::move(line));
		}
	}
	else
	{
		interpreter.compile_and_run(readAllContent(argv[1]));
	}
}