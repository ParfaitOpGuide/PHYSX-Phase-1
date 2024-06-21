#version 330 core

//Return a color
out vec4 FragColor;

//take vevc3 texcoord
in vec3 texCoord;
//add cubemap
uniform samplerCube skybox;

void main(){
//call the texture func to get the color
FragColor = texture(skybox, texCoord);
}