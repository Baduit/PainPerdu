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

function ask_nicely_the_api_to_run_the_code() {
	const options = {
		method: 'POST',
		body: document.getElementById("yololInput").value,
		headers: {
			'Content-Type': 'text/plain'
		}
	}
	fetch("http://127.0.0.1:6574/execute", options)
		.then(function (response) {
			return response.text();
		})
		.then(function (myText) {
			var canvas = document.getElementById("outputCanvas")
			var canvas_context = canvas.getContext("2d");

			canvas_context.clearRect(0, 0, canvas.width, canvas.height);
			canvas_context.beginPath()

			console.log(myText);	

			const answer = JSON.parse(myText);

			document.getElementById("yololOutput").value = answer.out;
		})
		.catch(function (error) {
			console.log("Error: " + error);
		});
}