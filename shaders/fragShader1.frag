#version 410 compatibility


out vec4 fragColor;
in vec4 vertCol;

void main(){
	fragColor = vertCol;
}