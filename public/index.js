// Create canvas context
const canvas = document.getElementById('canvas');
const context = canvas.getContext('2d');

// Render circles on canvas

const render = (dataLength, circleStructSize) => {
	// Get circle data from C - Typed Array
	let circles = new Int16Array(Module.HEAP16.buffer, _getCircles(canvas.width, canvas.height), dataLength);

	// Clear canvas to re-render
	context.clearRect(0, 0, canvas.width, canvas.height);

	// Loop data and get circle chunks
	for (let i = 0; i < circles.length; i += circleStructSize) {
		// Get circle data chunk
		let circle = circles.slice(i, i + circleStructSize);

		const [ x, y, r, cr, cg, cb ] = circle;
		// Draw circle
		context.beginPath();
		context.arc(x, y, r, 0, 2 * Math.PI, false);
		context.fillStyle = `rgba(${cr}, ${cg}, ${cb}, 1)`;
		context.fill();
	}

	console.log('rendered');

	// Request next animation frame and re-render with updated circles
	window.requestAnimationFrame(() => {
		render(dataLength, circleStructSize);
	});
};
