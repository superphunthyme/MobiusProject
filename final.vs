#version 330 core

layout (location=0) in vec4 position;
layout (location=1) in vec3 normal;
//layout (location=1) in vec4 color;
uniform vec4 color;

uniform mat4 ModelMatrix;	
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 colorVertFrag; // Pass the color on to rasterization
out vec3 Normal;
out vec3 FragPos;

void main() {
  // map the vertex position into clipping space 
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * position;
  FragPos = vec3(ModelMatrix * position);
  colorVertFrag = vec3(color.x, color.y, color.z);
  Normal = normal;
}
