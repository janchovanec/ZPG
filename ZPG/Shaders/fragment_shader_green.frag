#version 410
out vec4 frag_colour;
in vec3 color;
void main () { frag_colour = vec4(color * vec3(0.1, 0.5, 0.1), 1.0); }