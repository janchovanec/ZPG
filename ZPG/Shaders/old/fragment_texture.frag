#version 330
out vec4 frag_colour;
uniform sampler2D textureUnitID;
in vec2 uvc;
void main () {
     frag_colour = texture(textureUnitID, uvc);
}