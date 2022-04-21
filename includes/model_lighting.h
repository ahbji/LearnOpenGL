#include <learnopengl/shader.h>

#ifndef MODEL_LIGHTING_H
#define MODEL_LIGHTING_H
class ModelLighting
{
public:
    static void setupMaterial(Shader* shader, float shininess)
    {
        shader->setFloat("material.shininess", 32.0f);

    }
    static void initDirectionalLight(Shader* shader, glm::vec3 viewPos, glm::vec3 lightDirection, glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular)
    {
        shader->setVec3("viewPos", viewPos);
        shader->setVec3("dirLight.direction", lightDirection);
        shader->setVec3("dirLight.ambient", lightAmbient);
        shader->setVec3("dirLight.diffuse", lightDiffuse);
        shader->setVec3("dirLight.specular", lightSpecular);
    }
    static void initPointLight(Shader* shader, unsigned int lightID, glm::vec3 viewPos, glm::vec3 lightPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
    {
        shader->setVec3("viewPos", viewPos);
        shader->setVec3("pointLights[" + std::to_string(lightID) + "].position", lightPos);
        shader->setVec3("pointLights[" + std::to_string(lightID) + "].ambient", ambient);
        shader->setVec3("pointLights[" + std::to_string(lightID) + "].diffuse", diffuse);
        shader->setVec3("pointLights[" + std::to_string(lightID) + "].specular", specular);
        shader->setFloat("pointLights[" + std::to_string(lightID) + "].constant", constant);
        shader->setFloat("pointLights[" + std::to_string(lightID) + "].linear", linear);
        shader->setFloat("pointLights[" + std::to_string(lightID) + "].quadratic", quadratic);
    }
    static void initSpotLight(Shader* shader, unsigned int lightID, glm::vec3 viewPos, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
    {
        shader->setVec3("viewPos", viewPos);
        shader->setVec3("spotLights[" + std::to_string(lightID) + "].position", lightPos);
        shader->setVec3("spotLights[" + std::to_string(lightID) + "].direction", lightDirection);
        shader->setVec3("spotLights[" + std::to_string(lightID) + "].ambient", ambient);
        shader->setVec3("spotLights[" + std::to_string(lightID) + "].diffuse", diffuse);
        shader->setVec3("spotLights[" + std::to_string(lightID) + "].specular", specular);
        shader->setFloat("spotLights[" + std::to_string(lightID) + "].constant", constant);
        shader->setFloat("spotLights[" + std::to_string(lightID) + "].linear", linear);
        shader->setFloat("spotLights[" + std::to_string(lightID) + "].quadratic", quadratic);
        shader->setFloat("spotLights[" + std::to_string(lightID) + "].cutOff", cutOff);
        shader->setFloat("spotLights[" + std::to_string(lightID) + "].outerCutOff", outerCutOff);
    }
};

#endif