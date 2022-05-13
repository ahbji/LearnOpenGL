#ifndef TEXTURE_H
#define TEXTURE_H

#include <learnopengl/shader.h>
#include <vector>

namespace learnGL{
    class Texture
    {
    public:
        enum MIPMAP_TYPE
        {
            DIFFUSE, // 对应 GL_TEXTURE0
            SPECULAR // 对应 GL_TEXTURE1
        };

        /**
         * @brief Construct a new Texture object
         * 
         * @param texturePath 纹理素材路径
         * @param gammaCorrection 是否开启伽马校正 
         * @param flipY 是否反转 Y 轴，默认需要，如果顶点数据的 texture 坐标的原点在左下角，则不需要反转 Y 轴
         */
        Texture(const char* texturePath, bool gammaCorrection = true, bool flipY = true);

        /**
         * @brief 
         * 
         * @param shader 着色器指针
         * @param sampler2DVariableName sampler2D 采样器名
         * @param textureUnitID 纹理单元 ID ，取值 0 -- 15 ，或 Texture::DIFFUSE + n ，n 为 0 -- 15
         */
        void setupTextureUnit(Shader* shader, const std::string& sampler2DVariableName ,unsigned int textureUnitID);

        /**
         * @brief 绑定纹理纹理单元
         * 
         * @param textureUnit GL_TEXTURE0 到 GL_TEXTURE15
         */
        void bind(GLenum textureUnit);

        ~Texture();
    protected:
        unsigned int texture;
        Texture() = default;
        void setupDataFormat(int nrChannels, GLenum *out_i_format, GLenum *out_d_format, bool gammaCorrection = false);
    private:
        /**
         * @brief 解绑纹理纹理单元
         */
        void unBind();
        /**
         * @brief 
         * 
         * @param texturePath 纹理素材路径
         * @param gammaCorrection 是否开启伽马校正
         * @return 返回纹理对象指针
         */
        void loadMipMap(const char* texturePath, bool gammaCorrection, bool flipY);
    };

    class CubemapTexture : protected Texture
    {
    public:
        /**
         * @brief Construct a new Cubemap Texture object
         * 
         * @param texturePath 纹理素材路径
         * @param gammaCorrection 是否开启伽马校正 
         * @param flipY 是否反转 Y 轴，默认不需要
         */
        CubemapTexture(std::vector<std::string> faces, bool gammaCorrection = false, bool flipY = false);
        /**
         * @brief 
         * 
         * @param shader 着色器指针
         * @param sampler2DVariableName sampler2D 采样器名
         * @param textureUnitID 纹理单元 ID ，取值 0 -- 15 ，或 Texture::DIFFUSE + n ，n 为 0 -- 15
         */
        void setupTextureUnit(Shader* shader, const std::string& sampler2DVariableName ,unsigned int textureUnitID);

        /**
         * @brief 绑定纹理纹理单元
         * 
         * @param textureUnit GL_TEXTURE0 到 GL_TEXTURE15
         */
        void bind(GLenum textureUnit);

        unsigned int getTexture() { return texture; }

        ~CubemapTexture();
    private:
        /**
         * @brief 解绑纹理纹理单元
         */
        void unBind();
        void loadCubemap(std::vector<std::string> faces, bool gammaCorrection, bool flipY);
    };
};
#endif