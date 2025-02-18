#version 410 compatibility


float SDFCircle(float radius, vec2 uv, vec2 position){
    return 1.0 - step(radius, length(uv - position));
}

float SDFSmoothCircle(float radius, vec2 uv, vec2 position, float edgeWidth){
    return 1.0 - smoothstep(radius - edgeWidth, radius + edgeWidth, length(uv - position));
}


uniform vec2 u_resolution;
uniform float u_time;

const float PI = 3.141592;
const float numStripes = 6.0;
const float timeScale = 0.1;

vec3 colorUp = vec3(1., 1., 0.);
vec3 colorDown = vec3(1., 0.4, 0.0);

void main(){
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;

    vec3 color = mix(colorDown, colorUp, uv.y);

    color *= SDFCircle(0.5, uv, vec2(0.5, 0.5));
    float edgeMask = (1. - SDFSmoothCircle(0.49, uv, vec2(0.5, 0.5), 0.01)) 
                     * SDFSmoothCircle(0.5, uv, vec2(0.5, 0.5), 0.01);
    
    float strips = fract(((uv.y * 0.5 * (1. + uv.y)) - fract(u_time * timeScale)) * numStripes);
    color *= smoothstep(0.4, 0.6, abs(strips * 2. -1.));
    color += (color * edgeMask * 0.2) * 0.96 + 0.04;

    gl_FragColor = vec4(color, 1);
}
