#version 330
in vec4 vColor;
out vec4 fColor;

uniform bool isSelected;

void main(void)
{
    if(isSelected)
	fColor = vColor;
    else
	fColor = vec4(0,0,0,0);
}
