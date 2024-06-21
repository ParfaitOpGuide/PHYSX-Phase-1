#version 330 core

out vec4 FragColor;
//texture to be passed
uniform sampler2D tex0;

//recieve the texCoord from the vertex shader
in vec2 texCoord;

//normal data and world space
in vec3 normCoord;
in vec3 fragPos;

//self explanatory
uniform vec3 Point_lightPos;
uniform vec3 Point_lightColor;
uniform vec3 Dir_lightPos;
uniform vec3 Dir_lightColor;

//ambiennt light color and strength
uniform float Point_ambientStr;
uniform vec3 Point_ambientColor;
uniform float Dir_ambientStr;
uniform vec3 Dir_ambientColor;

uniform vec3 cameraPos;
// specular strength and phong
uniform float Point_specStr;
uniform float Point_specPhong;
uniform float Dir_specStr;
uniform float Dir_specPhong;

//both variable changable strengths
uniform float Point_strength;
uniform float Dir_strength;

uniform float opacity;

void main(){

//get color of pixel
vec4 pixelColor = texture(tex0, texCoord);
//if alpha low enough, drop pixel
if(pixelColor.a <0.1){
discard;
}

vec3 normal = normalize(normCoord);
//get view direction from camera to fragment
vec3 viewDir = normalize(cameraPos - fragPos);

//point calc
//get direction of light to fragment
vec3 Point_lightDir = normalize(Point_lightPos - fragPos);
float attenuation = Point_strength / (length(Point_lightPos - fragPos)*length(Point_lightPos - fragPos));

//apply diffuse formula
float Point_diff = max(dot(normal, Point_lightDir), 0.0);
//apply color
vec3 diffusePoint = Point_diff * Point_lightColor;

//get ambient lightby multiplying col and str
vec3 ambientPoint = Point_ambientColor * Point_ambientStr;

//get reflection vector
vec3 Point_reflectDir = reflect(-Point_lightDir, normal);

//get the specular light
float Point_spec = pow(max(dot(Point_reflectDir, viewDir), 0.1), Point_specPhong);
vec3 specularPoint = Point_spec * Point_specStr * Point_lightColor;

//dir calc
//get direction of light to fragment
vec3 Dir_lightDir = normalize(-Dir_lightPos);

//apply diffuse formula
float Dir_diff = max(dot(normal, Dir_lightDir), 0.0);
//apply color
vec3 diffuseDir = Dir_diff * Dir_lightColor;

//get ambient lightby multiplying col and str
vec3 ambientDir = Dir_ambientColor * Dir_ambientStr;

//get reflection vector
vec3 Dir_reflectDir = reflect(-Dir_lightDir, normal);

//get the specular light
float Dir_spec = pow(max(dot(Dir_reflectDir, viewDir), 0.1), Dir_specPhong);
vec3 specularDir = Dir_spec * Dir_specStr * Dir_lightColor;

//end of dir calc


//assign the texture color using the function, also adds the light and muktiplies intensity
//FragColor = vec4((1*(specularDir + diffuseDir + ambientDir)),1.0) * texture(tex0, texCoord);
//FragColor = vec4(attenuation*(specularPoint + diffusePoint + ambientPoint),1.0) * texture(tex0, texCoord);
FragColor = vec4((attenuation*(specularPoint + diffusePoint + ambientPoint)) + (Dir_strength*(specularDir + diffuseDir + ambientDir)),opacity) * texture(tex0, texCoord);


//fog calc, introduce distance limits
float FogMax = 50.0;
float FogMin = 0.1;
//fog color
vec4  FogColor = vec4(0.15, 0.2, 0.4, 1.0);

// Calculate fog
float FogDist = length(cameraPos - fragPos);
float fog = (FogMax - FogDist)/(FogMax - FogMin);
fog= clamp(fog, 0.0, 1.0);

FragColor = mix(FogColor, FragColor, fog);
}