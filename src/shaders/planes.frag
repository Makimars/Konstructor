#version 330
out vec4 fColor;

uniform vec4 planeColor;
uniform bool isSelected;

void main(void)
{
    if(isSelected)
	fColor = planeColor;
    else
	fColor = vec4(0,0,0,0);
}
