#version 410 compatibility


out vec4 fragColor;
in vec3 vertCol;

void main(){
	fragColor = vec4(vertCol, 1.0);
}