#version 420
in vec2 texpos;
uniform sampler2D tex;
uniform vec4 color;
void main(){
    gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).r) * color;
}