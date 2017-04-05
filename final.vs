#version 330 core

layout (location=0) in vec4 position;
//layout (location=1) in vec4 color;
uniform vec4 color;

uniform mat4 ModelMatrix;	
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 colorVertFrag; // Pass the color on to rasterization

void main() {
  // map the vertex position into clipping space 
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * position;
  colorVertFrag = color;
}
