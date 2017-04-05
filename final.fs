#version 330 core

in vec4 colorVertFrag;

out vec4 color;

void main() {
  // color the fragment 
  color = colorVertFrag;
}
