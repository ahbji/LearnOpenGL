#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
  
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1; 
    vec3 ambient = ambientStrength * lightColor; // 环境因子 * 光颜色
  	
    FragColor = vec4(ambient * objectColor, 1.0);
} 