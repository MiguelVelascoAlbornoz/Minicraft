#version 330

out vec4 fragColor;
uniform vec3 playerPos;
uniform vec3 color;
uniform float time;


in vec3 pos;
//in vec3 vertexColor;
// Función para calcular el valor de ruido Perlin en 2D

void main() {
    //vec2 pixelPosition = gl_FragCoord.xy;
    
    fragColor = vec4(cos(pos.y*3.14-3.14/2)*0.5+0.5,0.8,0.2,1.0f);
}