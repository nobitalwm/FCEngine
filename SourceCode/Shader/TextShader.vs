attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord0;

#ifndef USE_UBO
	uniform mat4 u_MVPMatrix;
#endif

varying vec4 color;
varying vec2 texCoord0;

void main()
{
	gl_Position = u_MVPMatrix * a_position;
	color = a_color;
	texCoord0 = a_texCoord0;
}
