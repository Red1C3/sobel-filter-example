#version 440
in vec2 vertex_model;
out vec2 uv;
void main(){
  gl_Position=vec4(vertex_model,0,1);
  uv=(vertex_model+1.0)/2.0;
  uv.y*=-1;
}