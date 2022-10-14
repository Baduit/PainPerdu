function set_canvas_size_form_value() {
	var canvas = document.getElementById("outputCanvas");
	document.getElementById("canvasWidth").value = canvas.width;
	document.getElementById("canvasHeight").value = canvas.height;
}

set_canvas_size_form_value();

function resize_canvas() {
	var canvas = document.getElementById("outputCanvas");
	var canvas_context = canvas.getContext("2d");
	canvas_context.clearRect(0, 0, canvas.width, canvas.height);
	canvas_context.beginPath();

	canvas.width = document.getElementById("canvasWidth").value;
	canvas.height = document.getElementById("canvasHeight").value;

	return false;
}

function render_stack_on_canvas(canvas, stack) {
	var canvas_context = canvas.getContext("2d");
	var i = 0;
	for (var y = 0; y < canvas.height; ++y) {
		for (var x = 0; x < canvas.width; ++x) {
			var red_pos_in_stack = i;
			var green_pos_in_stack = red_pos_in_stack + 1
			var blue_pos_in_stack = green_pos_in_stack + 1;
			if (blue_pos_in_stack >= stack.size()) {
				return;
			} else {
				// Maybe I should check the value are between 0 and 255, but I use a vector<uint8_t> on the backend and i'm kinda lazy
				// The line below is ugly :/
				var pixel_color = "rgb(" + stack.get(red_pos_in_stack).toString() + ", " + stack.get(green_pos_in_stack).toString() + ", " + stack.get(blue_pos_in_stack).toString() + ")";
				canvas_context.fillStyle = pixel_color;
				canvas_context.fillRect(x, y, 1, 1)
			}
			i += 3;
		}
	}
}

function ask_nicely_the_api_to_run_the_code() {
	var canvas = document.getElementById("outputCanvas")
	var canvas_context = canvas.getContext("2d");

	canvas_context.clearRect(0, 0, canvas.width, canvas.height);
	canvas_context.beginPath()

	var answer = Module.run_pain_perdu_code(document.getElementById("yololInput").value);

	document.getElementById("yololOutput").value = answer.console_output();
	render_stack_on_canvas(canvas, answer.stack());

	answer.delete();
}