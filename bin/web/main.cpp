#include <iostream>
#include <sstream>

#include <external/httplib.h>
#include <external/json/json.hpp>

#include <PainPerdu/PainPerdu.hpp>

int main(int argc, char** argv)
{
	httplib::Server server;

	server.Post("/execute",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json answer_body;

			try
			{
				std::stringstream out;
				std::stringstream in;
				PainPerdu::Interpreter interpreter(in, out);
				interpreter.disable_input();

				interpreter.compile_and_run(std::move(req.body));

				answer_body["out"] = out.str();
				answer_body["stack"] = interpreter.get_stack();
			}
			catch (std::exception& e)
			{
				answer_body["out"] = std::string("Error : ") + e.what();
				answer_body["stack"] = std::vector<uint8_t>{};	
			}

			res.set_content(answer_body.dump(), "application/json");
			res.set_header("Access-Control-Allow-Origin", "*");
		});

	server.set_error_handler([]([[maybe_unused]] const auto& req, auto& res) {
			auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
			char buf[BUFSIZ];
			snprintf(buf, sizeof(buf), fmt, res.status);
			res.set_content(buf, "text/html");
			res.set_header("Access-Control-Allow-Origin", "*");
		});

	auto mount_point = (argc > 1) ? argv[1] : "../public";
	auto ret = server.set_mount_point("/", mount_point);
	if (!ret)
	{
		std::cout << "Error while mouting ../public as /" << std::endl;
	}

	server.listen("0.0.0.0", 6574);
}