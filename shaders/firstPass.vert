#version 440
layout(location=0) in vec3 pos_model;
layout(location=1) in vec3 normal_model;
out vec3 normal_world;
out vec3 pos_world;
uniform mat4 VP;
void main(){
  gl_Position=VP*vec4(pos_model,1);
  // No model matrix used
  normal_world=normal_model;
  pos_world=pos_model;
}