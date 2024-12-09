#version 450


in vec3 FragPos;


uniform samplerCube UISky;


out vec4 FragColour;



void main () {
	 
    FragColour = texture(UISky, FragPos);
}

