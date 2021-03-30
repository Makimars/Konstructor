#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec3 vNormal;

uniform mat4 itemToSpace;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform mat4 itemToRotate;

void main()
{
    gl_Position = cameraToView * worldToCamera * itemToRotate * itemToSpace * vec4(position, 1.0);
    vNormal = normal;
}
