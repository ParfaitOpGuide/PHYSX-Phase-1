#version 330 core

//gets data at attrib 0 and stores in vec3
layout(location=0) in vec3 aPos;

//declare a var to hold the data to be passed 
//uniform float x;
//uniform float y;

//create transform variable
uniform mat4 transform;

uniform mat4 projection;

uniform mat4 view;

//tex coord is at 0
//accesses the uv and assigns to aTex
layout(location = 2) in vec2 aTex;
//pass the tex coord to the fragment shader
out vec2 texCoord;

//normals have attrib pos 1
layout(location = 1) in vec3 vertexNormal;
out vec3 normCoord;
out vec3 fragPos;

layout(location = 3) in vec3 m_tan;
layout(location = 4) in vec3 m_btan;
out mat3 TBN;

void main(){
//create vec3 for the new position
//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);

//gl_Pos is predefined
//denotes final pos of the vertex
gl_Position = projection * view * transform * vec4(aPos, 1.0); 
//turns the above vec3 into a vec4

texCoord = aTex;

mat3 modelMat = mat3(transpose(inverse(transform)));

vec3 T = normalize(modelMat * m_tan);
vec3 B = normalize(modelMat * m_btan);
vec3 N = normalize(normCoord);
TBN = mat3(T, B, N);

//get normal matrix and convert it into a 3x3
normCoord = modelMat * vertexNormal;
fragPos = vec3(transform * vec4(aPos,1.0));
}