#include "Image.h"
#include "stb/stb_image.h"

#include "GLFW/glfw3.h"

static uint32_t BytesPerPixel(ImageFormat format)
{
    switch (format)
    {
        case ImageFormat::RGBA:    return 4;
        case ImageFormat::RGBA32F: return 16;
    }
    return 0;
}

GL_RGB 

static unsigned int ImageFormatToOpenGLFormat(ImageFormat format)
{
    switch (format)
    {
        case ImageFormat::RGBA:    return GL_RGBA;
        case ImageFormat::RGBA32F: return GL_RGBA32F;
    }
    return 0;
}


Image::Image(std::string_view path);
    : m_Filepath(path)
{
    int width, height, channels;
    unit8_t* data = nullptr;

    if stbi_is_hdr(m_Filepath.c_str())
    {
        data = (unit8_t*)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 4);
        m_Format = ImageFormat::RGBA32F;
    } else {
        data = stbi_load(m_Filepath.c_str(), &widht, &height, &channels, 4);
        m_Format = ImageFormat::RGBA;
    }

    m_Width  = width;
    m_Hieght = height;

    AllocateMemory(m_Width * m_height * BytesPerPixel(m_Format));
    SetData(data);
}
Image::Image(uint32_t width, uint32_t height, ImageFormat format, const void* data = nullptr)
    : m_Width(width), m_Height(height), m_Format(format)
{
	AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
	if (data)
		SetData(data);
}
Image::~Image()
{
    Release();
}
Image::AllocateMemory(uint64_t size)
{
    // temporary as it we may not need to do this for OpenGL
    return size;
}
Image::SetData(const void* data)
{
    unsigned int texture_id;
    unsigned int open_gl_format = ImageFormatToOpenGLFormat(m_Format);
    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, open_gl_format, m_width, m_height, 0, open_gl_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    


}

    void SetData(const void* data);
    void Resize(uint32_t width, uint32_t height);
    unsigned int GetTextureID() const {return m_TextureID;}

private:
    uint32_t m_Width =0, m_Height =0;
    std::string m_Filepath;
    unsigned int m_TextureID;


};