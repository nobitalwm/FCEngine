#ifndef RENDER_TEXTURE_H__INCLUDE
#define RENDER_TEXTURE_H__INCLUDE

#include "Resource/Resource.h"
#include "RenderPublic.h"

namespace cocos2d
{
    struct _MipmapInfo;
}
typedef struct cocos2d::_MipmapInfo MipmapInfo;

class CTexture : public CResource
{
    DECLARE_RESOURCE_TYPE(eRT_Texture)
public:
    CTexture();
    virtual ~CTexture();

    virtual bool Load();
    virtual bool Unload();

    GLuint ID() const;

    bool InitWithMipmaps(MipmapInfo* mipmaps, int mipmapsNum, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh);
    bool InitWithData(const void *data, ssize_t dataLen, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh);
    bool UpdateSubImage(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const GLvoid *data);
    PixelFormat ConvertDataToFormat(const unsigned char* data, 
                                                        ssize_t dataLen, 
                                                        PixelFormat originFormat,
                                                        PixelFormat format,
                                                        unsigned char** outData, 
                                                        ssize_t* outDataLen);

    int Width() const;
    int Height() const;

    bool operator==( const CTexture& other );

    bool operator!=( const CTexture& other );

private:
    bool m_bPremultipliedAlpha;
    int m_iWidth;
    int m_iHeight;
    size_t m_uMipmapCount;
    GLuint m_uId;
    PixelFormat m_pixelFormat;
    static const PixelFormatInfoMap _pixelFormatInfoTables;

};

#endif