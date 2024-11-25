#version 410
out vec4 frag_colour;
in vec3 color;
void main () { frag_colour = vec4(vec3(0.565, 0.353, 0.0), 1.0); }