#version 410 compatibility


in vec3 vertCol;

uniform float u_time;
uniform vec2 u_resolution;

void main(){
	gl_FragColor = vec4(vertCol.xyz, 1.0);
}