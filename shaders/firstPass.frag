#version 440
const vec3 light_color={1,1,1};
const vec3 observer_world={0,0,0};
const float spec=16.0;
const float light_intensity=1.0;
out vec4 color;
in vec3 normal_world;
in vec3 pos_world;
uniform vec3 light_world;
void main(){
  vec3 fragToLight=light_world-pos_world;
  vec3 resColor=light_color;
  float diffuseFactor=dot(normalize(normal_world),normalize(fragToLight));
  vec3 viewVec=normalize(pos_world-observer_world);
	vec3 halfWay=normalize(fragToLight-viewVec);
	float specFactor=pow(max(dot(halfWay,normalize(normal_world)),0.0),spec);
  color=vec4(resColor*light_intensity*diffuseFactor,1)*0.5+
    vec4(resColor*light_intensity,1)*0.1+
    vec4(resColor*light_intensity,1)*specFactor*0.4;
}