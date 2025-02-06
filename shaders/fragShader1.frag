#version 410 compatibility


out vec4 fragColor;
in vec3 vertCol;

uniform float u_time;

void main(){
	fragColor = vec4(vertCol * fract(u_time), 1.0);
}