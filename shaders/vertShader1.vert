#version 410 compatibility


layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;

out vec3 vertCol;

uniform float u_time;

void main(){
	gl_Position = vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
	vertCol = vertexColor;
}