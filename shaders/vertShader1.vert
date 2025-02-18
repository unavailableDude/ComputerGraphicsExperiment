#version 410 compatibility


layout(location = 0) in vec4 vertexPos;
layout(location = 1) in vec3 vertexColor;

uniform float u_time;
uniform mat4 u_MVP;
out vec3 vertCol;

void main(){
	gl_Position = vec4(vertexPos.xyz, 1.0);
	vertCol = vertexColor;
}