#include "oak.h"


enum class ImageFormat
{
	None = 0,
	RGBA,
	RGBA32F
};

class Image 
{
public:
    Image(std::string_view path);
    Image(uint32_t width, uint32_t height, ImageFormat format, const void* data = nullptr);
    ~Image();

    void SetData(const void* data);
    void Resize(uint32_t width, uint32_t height);
    unsigned int GetTextureID() const {return m_TextureID;}

private:
    void AllocateMemory(uint64_t size);
    void Release();

private:
    uint32_t m_Width =0, m_Height =0;
    std::string m_Filepath;
    unsigned int m_TextureID;
    ImageFormat m_Format = ImageFormat::None;

};