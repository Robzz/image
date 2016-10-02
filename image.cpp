#include "image.h"
#include <stdexcept>
#include <fstream>
#include <limits>

using namespace std;

Image::Image() :
    m_image(nullptr)
{ }


Image::Image(unsigned int width, unsigned int height, Image::Type t, int bpp) :
    m_image(nullptr)
{
    if(t == Image::Type::Unknown)
        throw runtime_error("Invalid image type");
    m_image = FreeImage_AllocateT(static_cast<FREE_IMAGE_TYPE>(t), static_cast<int>(width),
                                  static_cast<int>(height), static_cast<int>(bpp));
    if(!m_image)
        throw runtime_error("Cannot create image");
}

Image::Image(string const& filename) :
    m_image(nullptr)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
        if(fif == FIF_UNKNOWN)
            throw runtime_error("Cannot open image");
    }
    m_image = FreeImage_Load(fif, filename.c_str());
}

Image::Image(FIBITMAP* other) :
    m_image(FreeImage_Clone(other))
{ }

Image::~Image() {
    if(m_image)
        FreeImage_Unload(m_image);
}

Image::Image(Image const& other) :
    m_image(FreeImage_Clone(other.m_image))
{ }

Image& Image::operator=(Image const& other) {
    m_image = FreeImage_Clone(other.m_image);
    return *this;
}

bool Image::hasPixels() const {
    return FreeImage_HasPixels(m_image);
}

unsigned int Image::width() const {
    return FreeImage_GetWidth(m_image);
}

unsigned int Image::height() const {
    return FreeImage_GetHeight(m_image);
}

unsigned int Image::bpp() const {
    return FreeImage_GetBPP(m_image);
}

Image::Type Image::type() const {
    return Image::Type(FreeImage_GetImageType(m_image));
}

RGBQUAD Image::getPixel(unsigned int x, unsigned int y) const {
    RGBQUAD r;
    if(!FreeImage_GetPixelColor(m_image, x, y, &r)) {
        throw runtime_error("Cannot read pixel");
    }
    return r;
}

void Image::setPixel(unsigned int x, unsigned int y, RGBQUAD color) {
    if(!FreeImage_SetPixelColor(m_image, x, y, &color)) {
        throw runtime_error("Cannot set pixel value");
    }
}

const byte* Image::getBits() const {
    return FreeImage_GetBits(m_image);
}

const byte* Image::getScanLine(int scanLine) const {
    return FreeImage_GetScanLine(m_image, scanLine);
}

Image Image::from_greyscale(vector<unsigned short> vec, int width, int height, bool flip) {
    if(vec.size() != static_cast<size_t>(width * height * 2)) {
        throw runtime_error("Input vector has wrong size");
    }
    FIBITMAP* img = FreeImage_AllocateT(FIT_UINT16, width, height, 16);
    int scanline_iter = flip? height-1 : 0,
        scanline_iter_incr = flip? -1 : 1;
    for(int i = 0 ; i != height ; ++i, scanline_iter += scanline_iter_incr) {
        unsigned short* scanline = reinterpret_cast<unsigned short*>(FreeImage_GetScanLine(img, i));
        for(int j = 0 ; j != width ; ++j)
            scanline[j] = vec[static_cast<size_t>(scanline_iter*width + j)];
    }

    Image i(img);

    return i;
}

bool Image::save(string const& filename, Format f) const {
    switch(f) {
        case Format::Bmp:
            return FreeImage_Save(FIF_BMP, m_image, filename.c_str(), BMP_DEFAULT);
        case Format::BmpRle:
            return FreeImage_Save(FIF_BMP, m_image, filename.c_str(), BMP_SAVE_RLE);
        case Format::Png:
            return FreeImage_Save(FIF_PNG, m_image, filename.c_str(), PNG_DEFAULT);
        default:
            throw runtime_error("Unknown image format");
    }
}
