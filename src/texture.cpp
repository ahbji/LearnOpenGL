#include <stb_image.h>

#include <texture.h>

using namespace learnGL;

Texture::Texture(const char* texturePath, bool gammaCorrection, bool flipY)
{
    loadMipMap(texturePath, gammaCorrection, flipY);
}

void Texture::setupTextureUnit(Shader* shader, const std::string& sampler2DVariableName ,unsigned int textureUnitID)
{
    shader->use();
    shader->setInt(sampler2DVariableName, textureUnitID);
}

void Texture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::unBind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    unBind();
    glDeleteTextures(1, &this->texture);
}

void Texture::setupDataFormat(int nrChannels, GLenum *out_i_format, GLenum *out_d_format, bool gammaCorrection)
{
    if (nrChannels == 1)
    {
        *out_i_format = GL_RED;
        *out_d_format = GL_RED;
    }
        
    else if (nrChannels == 3)
    {
        *out_i_format = gammaCorrection ? 
            GL_SRGB // 重校纹理（将素材的色域从 sRGB 变回线性色域）
            : GL_RGB;
        *out_d_format = GL_RGB;
    }

    else if (nrChannels == 4)
    {
        *out_i_format = gammaCorrection ? 
            GL_SRGB_ALPHA // 重校纹理（将素材的色域从 sRGB 变回线性色域）
            : GL_RGBA;
        *out_d_format = GL_RGBA;
    }
}

void Texture::loadMipMap(const char* texturePath, bool gammaCorrection, bool flipY)
{
    // 创建纹理对象
    glGenTextures(1, &texture);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flipY);
    // 读取素材，并获取其宽、高、通道数
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        // 根据通道数设置纹理格式
        GLenum internalFormat, dataFormat;
        setupDataFormat(nrChannels, &internalFormat, &dataFormat, gammaCorrection);
        // 绑定纹理对象到 2D 纹理目标
        glBindTexture(GL_TEXTURE_2D, texture);
        // 为 2D 纹理目标生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        // 为 2D 纹理目标生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 设置2D纹理在 X 轴的包围方式
        // 使用 GL_CLAMP_TO_EDGE 防止半透明的边界。 由于插值，它需要从下一次重复的texels
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        // 设置2D纹理在 Y 轴的包围方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        // 设置缩小操作时使用的纹理过滤方式，GL_LINEAR_MIPMAP_LINEAR 在两个邻近的多级渐变纹理之间使用线性插值进行采样 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // 设置放大操作时使用的纹理过滤方式， GL_LINEAR 生成更加羽化的纹理
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 释放素材数据
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        stbi_image_free(data);
    }
}

CubemapTexture::CubemapTexture(std::vector<std::string> faces, bool gammaCorrection, bool flipY)
{
    loadCubemap(faces, gammaCorrection, flipY);
}

void CubemapTexture::setupTextureUnit(Shader* shader, const std::string& sampler2DVariableName ,unsigned int textureUnitID)
{
    Texture::setupTextureUnit(shader, sampler2DVariableName, textureUnitID);
}

void CubemapTexture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
}

void CubemapTexture::unBind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubemapTexture::~CubemapTexture()
{
    unBind();
    glDeleteTextures(1, &this->texture);
}

void CubemapTexture::loadCubemap(std::vector<std::string> faces, bool gammaCorrection, bool flipY)
{
    glGenTextures(1, &texture);
    stbi_set_flip_vertically_on_load(flipY);
    
    int width, height, nrChannels;
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    // 遍历 cubemap 各个面的素材
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // 根据通道数设置纹理格式
            GLenum internalFormat, dataFormat;
            setupDataFormat(nrChannels, &internalFormat, &dataFormat, gammaCorrection);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}