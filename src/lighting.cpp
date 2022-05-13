#include <lighting.h>

Material::Material(float shininess):shininess(shininess){}

void Material::applyLightingMapMaterial(Shader* shader)
{
    shader->use();
    shader->setFloat("material.shininess", shininess);
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    :ambient(ambient),diffuse(diffuse), specular(specular), shininess(shininess)
{
}

void Material::applyMaterial(Shader* shader)
{
    shader->use();
    shader->setVec3("material.ambient", ambient);
    shader->setVec3("material.diffuse", diffuse);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}

Lighting::Lighting(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    :viewPos(viewPos),ambient(ambient),diffuse(diffuse), specular(specular){}

void Lighting::apply(Shader* shader, glm::vec3 lightPos)
{
    shader->use();
    shader->setVec3("viewPos", viewPos);
    shader->setVec3("light.position", lightPos);
    shader->setVec3("light.ambient", ambient);
    shader->setVec3("light.diffuse", diffuse);
    shader->setVec3("light.specular", specular);
}

DirectionalLight::DirectionalLight(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    :Lighting(viewPos, ambient, diffuse, specular)
{
}

void DirectionalLight::apply(Shader* shader, glm::vec3 lightDirection)
{
    shader->use();
    shader->setVec3("viewPos", viewPos);
    shader->setVec3("dirLight.direction", lightDirection);
    shader->setVec3("dirLight.ambient", ambient);
    shader->setVec3("dirLight.diffuse", diffuse);
    shader->setVec3("dirLight.specular", specular);
}

PointLight::PointLight(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
    :Lighting(viewPos, ambient, diffuse, specular),
    constant(constant),linear(linear),quadratic(quadratic){}

void PointLight::apply(Shader* shader, unsigned int lightID, glm::vec3 lightPos)
{
    shader->use();
    shader->setVec3("viewPos", viewPos);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].position", lightPos);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].ambient", ambient);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].diffuse", diffuse);
    shader->setVec3("pointLights[" + std::to_string(lightID) + "].specular", specular);
    shader->setFloat("pointLights[" + std::to_string(lightID) + "].constant", constant);
    shader->setFloat("pointLights[" + std::to_string(lightID) + "].linear", linear);
    shader->setFloat("pointLights[" + std::to_string(lightID) + "].quadratic", quadratic);
}

SpotLight::SpotLight(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
    :Lighting(viewPos, ambient, diffuse, specular),
    constant(constant),linear(linear),quadratic(quadratic),cutOff(cutOff),outerCutOff(outerCutOff){}

void SpotLight::apply(Shader* shader, unsigned int lightID, glm::vec3 lightPos, glm::vec3 lightDirection)
{
    shader->use();
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