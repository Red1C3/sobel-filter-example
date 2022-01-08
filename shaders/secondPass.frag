#version 440
const float offset=1.0/1000.0;
in vec2 uv;
out vec4 color;
uniform sampler2D renderedTex;
uniform sampler2D renderedDepthMap;
vec3 applyKernel(float kernel[9],sampler2D tex);
void main(){
    // Soble operator kernels
  float Gx[9]={1,0,-1,
               2,0,-2,
               1,0,-1};
  float Gy[9]={1,2,1,
               0,0,0,
               -1,-2,-1};
  vec3 pixelX=applyKernel(Gx,renderedTex);
  vec3 pixelY=applyKernel(Gy,renderedTex);
  for(int i=0;i<9;i++){
    Gx[i]*=300;
    Gy[i]*=300;
  }
  float depthX=applyKernel(Gx,renderedDepthMap).r;
  float depthY=applyKernel(Gy,renderedDepthMap).r;
  float depthFactor=sqrt(depthX*depthX+depthY*depthY);
  color.r=sqrt(pixelX.r*pixelX.r+pixelY.r*pixelY.r);
  color.g=sqrt(pixelX.g*pixelX.g+pixelY.g*pixelY.g);
  color.b=sqrt(pixelX.b*pixelX.b+pixelY.b*pixelY.b);
  color=color*0.5+vec4(depthFactor)*0.5;
  color.a=1.0;
}
vec3 applyKernel(float kernel[9],sampler2D tex){
   vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(tex, uv + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    return col;
}