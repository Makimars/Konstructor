#version 330
out vec4 fColor;

uniform vec3 selectedItemColor;
uniform bool isSelected;
uniform float selectedTransparentValue;
uniform vec4 itemColor;

void main()
{
    if(isSelected)
	fColor = vec4(selectedItemColor, selectedTransparentValue);
    else
	fColor = itemColor;
}
