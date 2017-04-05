#version 330 core

in vec3 colorVertFrag;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 lightPos;

void main() {
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(Normal, lightDir), 0.0);
  // White light
  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
  vec3 diffuse = diff * lightColor;
  vec3 result = diffuse * colorVertFrag;
  // color the fragment 
  color = vec4(result, 1.0f);
}
