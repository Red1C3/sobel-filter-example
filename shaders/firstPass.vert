#version 440
layout(location=0) in vec3 pos_model;
layout(location=1) in vec3 normal_model;
uniform mat4 MVP;
void main(){
  gl_Position=MVP*vec4(pos_model,1); 
}