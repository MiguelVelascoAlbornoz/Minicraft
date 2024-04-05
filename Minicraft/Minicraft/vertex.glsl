#version 330 

layout(location = 0) in vec4 vPosition;
uniform mat4 model;
uniform vec3 playerPos;
uniform float time;
// Variables de salida
//out vec3 vertexColor; // Color que se pasará al fragment shader
out vec3 pos;
// Importar el ruido de Perlin
/*float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 co) {
    vec2 i = floor(co);
    vec2 f = fract(co);

    // Four corners in 2D of a tile
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
           (c - a)* u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}*/
void main() {
   
    //vertexColor = vec3(0,0,0);
   
    //float distance = sqrt(vPosition.x*vPosition.x+vPosition.y*vPosition.y+vPosition.z*vPosition.z);
    //float factor = sin(distance);
    //vec3 playerD = vec3(vPosition.x-playerPos.x,0,vPosition.z-playerPos.z);
    //float distance = sqrt(playerD.x*playerD.x+playerD.z*playerD.z);
    //float positionY = vPosition.y+pow(2,-0.01*distance*distance+2);
    //float  positionY = noise(vec2(vPosition.x/50,vPosition.z/50))*30;
    pos = vec3(vPosition.x,vPosition.y,vPosition.z);
    gl_Position = model*vec4(vPosition.x,vPosition.y,vPosition.z,1.0f);
  
}