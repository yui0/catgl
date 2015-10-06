var currentProgram;
var c, gl;
var aLoc = [];
var uLoc = [];
var texture = [];

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

function checkSize(img) {
	var w = img.naturalWidth, h = img.naturalHeight;
	var size = Math.pow(2, Math.log(Math.min(w, h)) / Math.LN2 | 0); // largest 2^n integer that does not exceed s
	if (w !== h || w !== size) {
		var canv = document.createElement('canvas');
		canv.height = canv.width = size;
		canv.getContext('2d').drawImage(img, 0, 0, w, h, 0, 0, size, size);
		img = canv;
	}
	return img;
}
function create_texture(n, source) {
	var img = new Image();

	img.onload = function() {
		var tex = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, tex);
		//gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
//		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, checkSize(img));
		//gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		//gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
		gl.generateMipmap(gl.TEXTURE_2D);
		gl.bindTexture(gl.TEXTURE_2D, null);
		texture[n] = tex;
	};

	img.src = source;
}

function render() {
	parameters.time = Date.now() - parameters.startTime;

	gl.useProgram( currentProgram );
	gl.uniform1f( gl.getUniformLocation(currentProgram, 'time'), parameters.time / 1000 );
	gl.uniform2f( gl.getUniformLocation(currentProgram, 'mouse'), parameters.mouseX, parameters.mouseY );
	gl.uniform2f( gl.getUniformLocation(currentProgram, 'resolution'), parameters.screenWidth, parameters.screenHeight );

	if (texture[0]!=null) {
		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, texture[0]);
		gl.uniform1i(gl.getUniformLocation(currentProgram, 'iChannel0'), 0);
	}
	if (texture[1]!=null) {
		gl.activeTexture(gl.TEXTURE1);
		gl.bindTexture(gl.TEXTURE_2D, texture[1]);
		gl.uniform1i(gl.getUniformLocation(currentProgram, 'iChannel1'), 1);
	}

	// Create vertex buffer (2 triangles)
	buffer = gl.createBuffer();
	gl.bindBuffer( gl.ARRAY_BUFFER, buffer );
	gl.bufferData( gl.ARRAY_BUFFER, new Float32Array( [ -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0 ] ), gl.STATIC_DRAW );
	gl.vertexAttribPointer( aLoc[0], 3, gl.FLOAT, false, 0, 0 );
	gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );
	gl.drawArrays( gl.TRIANGLES, 0, 6 );
//	gl.flush();
}

//var canvas = document.createElement('canvas');
var canvas = $('c');
var parameters = { startTime: Date.now(), time: 0, mouseX: 0.5, mouseY: 0.5, screenWidth: 0, screenHeight: 0 };
parameters.screenWidth = canvas.width;
parameters.screenHeight = canvas.height;

initWebGL();
initShaders();

function $(e) { return document.getElementById(e); }
if ($('webgljs').getAttribute('data-texture0')) {
	gl.activeTexture(gl.TEXTURE0);
	create_texture(0, $('webgljs').getAttribute('data-texture0'));
}
if ($('webgljs').getAttribute('data-texture1')) {
	gl.activeTexture(gl.TEXTURE1);
	create_texture(1, $('webgljs').getAttribute('data-texture1'));
}

document.addEventListener( 'mousemove', function ( event ) {
	var clientX = event.clientX;
	var clientY = event.clientY;

	parameters.mouseX = clientX / window.innerWidth;
	parameters.mouseY = 1 - clientY / window.innerHeight;
}, false );

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
