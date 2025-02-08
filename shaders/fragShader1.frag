#version 410 compatibility


in vec3 vertCol;

uniform float u_time;
uniform vec2 u_resolution;

void main(){
	gl_FragColor = vec4(vertCol * fract(u_time), 1.0);
}