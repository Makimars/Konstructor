#version 330
layout(location = 0) in vec3 position;

uniform mat4 itemToSpace;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform mat4 itemToRotate;

void main()
{
    gl_Position = cameraToView * worldToCamera * itemToRotate * itemToSpace * vec4(position, 1.0);
}
