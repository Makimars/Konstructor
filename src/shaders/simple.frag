#version 330
in vec4 vColor;
out vec4 fColor;

uniform vec3 selectedItemColor;
uniform bool itemIsSelected;
uniform bool polygonIsSelected;

void main()
{
    if(itemIsSelected || polygonIsSelected)
	fColor = vec4(selectedItemColor,1);
    else
	fColor = vColor;
}
