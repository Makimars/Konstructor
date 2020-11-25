#version 330
in vec4 vColor;
out vec4 fColor;

uniform vec3 selectedItemColor;
uniform bool isSelected;
uniform float selectedTransparentValue;

void main()
{
    if(isSelected)
	fColor = vec4(selectedItemColor, selectedTransparentValue);
    else
	fColor = vColor;
}
