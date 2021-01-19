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
			try
			{
				std::stringstream out;
				std::stringstream in;
				PainPerdu::Interpreter interpreter(in, out);
				interpreter.disable_get_char();

				interpreter.compile_and_run(std::move(req.body));

				nlohmann::json answer_body;
				answer_body["out"] = out.str();
				res.set_content(answer_body.dump(), "application/json");
				//res.set_header("Access-Control-Allow-Origin", "*");
			}
			catch (std::exception& e)
			{
				nlohmann::json answer_body;
				answer_body["error"] = e.what();
				res.set_content(answer_body.dump(), "application/json");
			}
		});

	server.set_error_handler([]([[maybe_unused]] const auto& req, auto& res) {
			auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
			char buf[BUFSIZ];
			snprintf(buf, sizeof(buf), fmt, res.status);
			res.set_content(buf, "text/html");
		});

	auto mount_point = (argc > 1) ? argv[1] : "../public";
	auto ret = server.set_mount_point("/", mount_point);
	if (!ret)
	{
		std::cout << "Error while mouting ../public as /" << std::endl;
	}

	server.listen("0.0.0.0", 6574);
}