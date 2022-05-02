#ifndef LIGHTING_H
#define LIGHTING_H

#include <learnopengl/shader.h>

class Material
{
public:
    /**
     * @brief 使用光照贴图时使用此构造函数创建 Material 对象
     * 
     * @param shininess 光泽度
     */
    Material(float shininess);

    /**
     * @brief 属性完整的 Material 对象
     * 
     * @param ambient 环境光反射率
     * @param diffuse 漫反射率
     * @param specular 镜面光反射率
     * @param shininess 光泽度
     */
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

    /**
     * @brief 应用 Material 属性
     * 
     * @param shader 
     */
    void applyMaterial(Shader* shader);

    /**
     * @brief 为光照贴图应用 Material 属性
     * 
     * @param shader 
     */
    void applyLightingMapMaterial(Shader* shader);
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

/**
 * @brief 基础光照
 * 
 */
class Lighting
{
public:
    Lighting(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    /**
     * @brief 应用基础光照
     * 
     * @param shader 着色器
     * @param lightPos 光源位置
     */
    void apply(Shader* shader, glm::vec3 lightPos);
protected:
    glm::vec3 viewPos;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

/**
 * @brief 平行光
 * 
 */
class DirectionalLight: protected Lighting
{
public:
    /**
     * @brief 创建平行光
     * 
     * @param viewPos 观察位置 
     * @param ambient 环境光反射率
     * @param diffuse 漫反射率
     * @param specular 镜面光反射率
     */
    DirectionalLight(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    /**
     * @brief 应用平行光
     * 
     * @param shader 着色器
     * @param lightDirection 片段到光源的光线方向（一般为全局光照方向向量取反）
     */
    void apply(Shader* shader, glm::vec3 lightDirection);
protected:
};

/**
 * @brief 点光源
 * 
 */
class PointLight : protected Lighting
{
public:
    /**
     * @brief 创建点光源
     * 
     * @param viewPos 观察位置 
     * @param ambient 环境光反射率
     * @param diffuse 漫反射率
     * @param specular 镜面光反射率
     * @param constant 常量衰减参数
     * @param linear 线性衰减参数
     * @param quadratic 二次衰减参数
     */
    PointLight(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
    void apply(Shader* shader, unsigned int lightID, glm::vec3 lightPos);
protected:
    float constant;
    float linear;
    float quadratic;
};

/**
 * @brief 聚光光源
 * 
 */
class SpotLight : protected Lighting
{
public:
    /**
     * @brief 创建聚光光源
     * 
     * @param viewPos 观察位置
     * @param lightDirection 片段到光源的光线方向（一般为全局光照方向向量取反）
     * @param ambient 环境光反射率
     * @param diffuse 漫反射率
     * @param specular 镜面光反射率
     * @param constant 常量衰减参数
     * @param linear 线性衰减参数
     * @param quadratic 二次衰减参数
     * @param cutOff 内圆锥角度
     * @param outerCutOff 外圆锥角度
     */
    SpotLight(glm::vec3 viewPos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
    void apply(Shader* shader, unsigned int lightID, glm::vec3 lightPos, glm::vec3 lightDirection);
protected:
    glm::vec3 lightDirection;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

#endif // LIGHTING_H