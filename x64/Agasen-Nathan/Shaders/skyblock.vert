#version 330 core

//gets data at attrib 0 and stores in vec3
layout(location=0) in vec3 aPos;

uniform mat4 projection;

uniform mat4 view;


//pass the tex coord to the fragment shader
out vec3 texCoord;

void main(){
vec4 pos = projection * view * vec4(aPos, 1.0);

//gl_Pos is predefined
//directly write onto view space
gl_Position = vec4(pos.x, pos.y, pos.w, pos.w); 
//assign to apos

texCoord = aPos;
}