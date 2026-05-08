#version 330 core

in vec3 Position;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform float ambientStrength;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() 
{    
    //ambient
    float Ia = ambientStrength; 

    //diffuse
    vec3 N = normalize(Normal); 
    vec3 Lm = normalize(lightPos - Position);
    float Id = max(dot(N, Lm), 0.0);

    //specular 
    vec3 V = normalize(cameraPos - Position);
    vec3 Rm = reflect(-Lm, N);
    float Is = pow(max(dot(Rm, V), 0.0), 16) * 0.5;

    vec3 finalColor = (Ia + Id + Is) * lightColor * Color; 

    FragColor = vec4(finalColor, 1.0);
}