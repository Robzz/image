#ifndef IMAGE_H
#include "image.h"
#endif

#include <stdexcept>

template <class T>
Image<T>::Image(int width, int height, ImageType t, int bpp, unsigned int rMask,
                unsigned int gMask, unsigned int bMask) :
    m_image(FreeImage_AllocateT(static_cast<FREE_IMAGE_TYPE>(t), width, height, bpp, rMask, gMask, bMask)),
    m_width(width),
    m_height(height)
{
    if(!m_image) {
        throw std::runtime_error("Cannot allocate image");
    }
}

template <class T>
Image<T>::Image(FIBITMAP* fi) :
    m_image(fi),
    m_width(FreeImage_GetWidth(fi)),
    m_height(FreeImage_GetHeight(fi))
{ }

template <class T>
Image<T>::Image(Image const& other) :
    m_image(FreeImage_Clone(other.m_image)),
    m_width(other.m_width),
    m_height(other.m_height)
{
    if(!m_image) {
        throw std::runtime_error("Cannot allocate image");
    }
}

template <class T>
Image<T>::Image(Image&& other) :
    m_image(other.m_image),
    m_width(other.m_width),
    m_height(other.m_height)
{
    other.m_image = nullptr;
}

template <class T>
Image<T>& Image<T>::operator=(Image<T> const& other) {
    FIBITMAP* temp = FreeImage_Clone(other.m_image);
    if(!temp) {
        throw std::runtime_error("Cannot allocate image");
    }
    FreeImage_Unload(m_image);
    m_image = temp;
    m_width = other.m_width;
    m_height = other.m_height;
    return *this;
}

template <class T>
Image<T>& Image<T>::operator=(Image<T>&& other) {
    FreeImage_Unload(m_image);
    m_image = other.m_image;
    m_width = other.m_width;
    m_height = other.m_height;
    return *this;
}

template <class T>
bool Image<T>::operator==(Image const& other) {
    if(m_width != other.m_width || m_height != other.m_height)
        return false;
    for(int y = 0 ; y != m_height ; ++y) {
        for(int x = 0 ; x != m_width ; ++x) {
            if(getPixel(x, y) != other.getPixel(x, y))
                return false;
        }
    }
    return true;
}

template <class T>
Image<T>::~Image() {
    FreeImage_Unload(m_image);
}

template <class T>
int Image<T>::width() const {
    return m_width;
}

template <class T>
int Image<T>::height() const {
    return m_height;
}

template <class T>
const T* Image<T>::getBits() const {
    return reinterpret_cast<const T*>(FreeImage_GetBits(m_image));
}

template <class T>
const T* Image<T>::getScanline(int scanline) const {
    return reinterpret_cast<const T*>(FreeImage_GetScanLine(m_image, scanline));
}

template <class T>
void Image<T>::flipX() {
    auto w = m_width / 2;
    for(int y = 0 ; y != m_height ; ++y) {
        for(int x = 0 ; x != w ; ++x) {
            T t = getPixel(x, y);
            int xFlip = m_width - x - 1;
            setPixel(x, y, getPixel(xFlip, y));
            setPixel(xFlip, y, t);
        }
    }
}

template <class T>
void Image<T>::flipY() {
    auto h = m_height / 2;
    for(int y = 0 ; y != h ; ++y) {
        for(int x = 0 ; x != m_width ; ++x) {
            T t = getPixel(x, y);
            int yFlip = m_height - y - 1;
            setPixel(x, y, getPixel(x, yFlip));
            setPixel(x, yFlip, t);
        }
    }
}

template <class T>
void Image<T>::blit(ImageCoords c, Rect r, Image<T> const& other) {
    int h = m_height - c.y;
    int w = m_width - c.x;
    h = (r.height > h) ? h : r.height;
    w = (r.width > w) ? w : r.width;

    for(int ix = 0 ; ix != w ; ++ix) {
        for(int iy = 0 ; iy != h ; ++iy) {
            setPixel(c.x + ix, c.y + iy, other.getPixel(r.x + ix, r.y + iy));
        }
    }
}

template <class T>
void Image<T>::crop(Rect r) {
    FIBITMAP* croppedImg = FreeImage_Copy(m_image, r.x, r.y + r.height, r.x + r.width, r.y);
    m_width = r.width;
    m_height = r.height;
    FreeImage_Unload(m_image);
    m_image = croppedImg;
}

template <class T>
void Image<T>::save(std::string const& filename, ImageFormat f) const {
    int flags = (f == ImageFormat::BmpRle) ? BMP_SAVE_RLE : 0;
    if(!FreeImage_Save(static_cast<FREE_IMAGE_FORMAT>(f),
                      m_image, filename.c_str(), flags)) {
        throw std::runtime_error("Cannot save image");
    }
}
