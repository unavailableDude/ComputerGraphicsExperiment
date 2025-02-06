#version 410 compatibility


uniform float u_time;
uniform vec2 u_resolution;

vec3 palette(float t){
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

//ran for every pixel
void main(){
    //setup uv to -1~1 on xy
    vec2 uv = gl_FragCoord.xy / u_resolution.xy  * 2. - 1.;
    //track original uv
    vec2 uv0 = uv;
    
    vec3 finalCol = vec3(0.);
    
    for(int i = 0; i < 3; i++){
    //fract uv
        uv = fract(uv * 1.2) - 0.5;

        //get circle shape
        float d = length(uv) * exp(length(uv0) * cos(u_time * 0.5));
        vec3 col = palette(length(uv0) + (float(i) * 0.3) + (u_time * 0.3));
        //make it repeat and move with time
        d = sin(d * 8. + u_time) / 8.;
        //make it double sided
        d = abs(d);
        //make it neony
        d = 0.015 / d;
        //increase contrast
        d = pow(d, 1.2);
        
        finalCol += col * d;
    }
    
    gl_FragColor = vec4(finalCol, 1.);
}