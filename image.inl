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
void Image<T>::save(std::string const& filename, ImageFormat f) const {
    int flags = (f == ImageFormat::BmpRle) ? BMP_SAVE_RLE : 0;
    if(!FreeImage_Save(static_cast<FREE_IMAGE_FORMAT>(f),
                      m_image, filename.c_str(), flags)) {
        throw std::runtime_error("Cannot save image");
    }
}
