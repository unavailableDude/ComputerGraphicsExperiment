#version 410 compatibility


layout(location = 0) in vec4 vertexPos;
layout(location = 1) in vec3 vertexColor;

uniform float u_time;
uniform mat4 u_MVP;
out vec3 vertCol;

void main(){
	vec4 vert = vertexPos;
	vert = u_MVP * vert;
	gl_Position = vec4(vert.xyz, 1.0);
	vertCol = vertexColor;
}