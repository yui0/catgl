var currentProgram;
var c, gl;
var aLoc = [];
var uLoc = [];

function initWebGL() {
	c = document.getElementById("c");
	gl = c.getContext("experimental-webgl");
}

function initShaders() {
	var p = gl.createProgram();
	var v = document.getElementById("vs").textContent;
	var f = document.getElementById("fs").textContent;
	var vs = gl.createShader(gl.VERTEX_SHADER);
	var fs = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(vs, v);
	gl.shaderSource(fs, f);
	gl.compileShader(vs);
	gl.compileShader(fs);
	gl.attachShader(p, vs);
	gl.attachShader(p, fs);
	gl.linkProgram(p);
	gl.useProgram(p);
	aLoc[0] = gl.getAttribLocation(p, "position");
	//aLoc[1] = gl.getAttribLocation(p, "color");
	gl.enableVertexAttribArray(aLoc[0]);
	//gl.enableVertexAttribArray(aLoc[1]);
	currentProgram = p;
}

function render() {
	parameters.time = Date.now() - parameters.startTime;

	gl.useProgram( currentProgram );
	gl.uniform1f( gl.getUniformLocation(currentProgram, 'time'), parameters.time );
	gl.uniform2f( gl.getUniformLocation(currentProgram, 'resolution'), parameters.screenWidth, parameters.screenHeight );

	// Create vertex buffer (2 triangles)
	buffer = gl.createBuffer();
	gl.bindBuffer( gl.ARRAY_BUFFER, buffer );
	gl.bufferData( gl.ARRAY_BUFFER, new Float32Array( [ -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0 ] ), gl.STATIC_DRAW );
	gl.vertexAttribPointer( aLoc[0], 3, gl.FLOAT, false, 0, 0 );
	gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );
	gl.drawArrays( gl.TRIANGLES, 0, 6 );

//	gl.flush();
}

var canvas = document.createElement( 'canvas' );
var parameters = { startTime: Date.now(), time: 0, mouseX: 0.5, mouseY: 0.5, screenWidth: 0, screenHeight: 0 };
parameters.screenWidth = canvas.width;
parameters.screenHeight = canvas.height;

initWebGL();
initShaders();
//render();

if ( !window.requestAnimationFrame ) {
	window.requestAnimationFrame = ( function() {
		return window.webkitRequestAnimationFrame ||
			window.mozRequestAnimationFrame ||
			window.oRequestAnimationFrame ||
			window.msRequestAnimationFrame ||
			function ( callback, element ) {
				window.setTimeout( callback, 1000 / 60 );
			};
	} )();
}
function animate() {
	requestAnimationFrame( animate );
	render();
}
animate();
