#version 440
in vec2 uv;
out vec4 color;
uniform sampler2D renderedTex;
void main(){
  color=texture(renderedTex,uv);
}