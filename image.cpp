#include "image.h"
#include <stdexcept>
#include <fstream>
#include <limits>

using namespace std;

GreyscaleImage::GreyscaleImage(int width, int height) :
    Image<byte>(width, height, ImageType::Bitmap, 8, 0xFF, 0xFF, 0xFF)
{
    buildPalette();
}

GreyscaleImage::GreyscaleImage(FIBITMAP* fi) :
    Image<byte>(fi)
{
    buildPalette();
}

GreyscaleImage::~GreyscaleImage() { }

GreyscaleImage::GreyscaleImage(GreyscaleImage const& other) :
    Image<byte>(other)
{
    buildPalette();
}

GreyscaleImage& GreyscaleImage::operator=(GreyscaleImage const& other) {
    *static_cast<Image<byte>*>(this) = other;
    return *this;
}

byte GreyscaleImage::getPixel(int x, int y) const {
    byte pixel;
    if(!FreeImage_GetPixelIndex(m_image, x, y, &pixel)) {
        throw runtime_error("Cannot read pixel");
    }
    return pixel;
}

void GreyscaleImage::setPixel(int x, int y, byte pixel) {
    if(!FreeImage_SetPixelIndex(m_image, x, y, &pixel)) {
        throw runtime_error("Cannot set pixel value");
    }
}

GreyscaleImage GreyscaleImage::fromRawData(vector<byte> vec, int width, int height, bool flip) {
    if(vec.size() != static_cast<size_t>(width * height)) {
        throw runtime_error("Input vector has wrong size");
    }
    GreyscaleImage img(width, height);
    for(int y = 0 ; y != height ; ++y) {
        int scanlineOffset = width * y;
        for(int x = 0 ; x != width ; ++x) {
            byte pixel = vec[x + scanlineOffset];
            img.setPixel(x, flip? height-y : y, pixel);
        }
    }

    return img;
}

GreyscaleImage GreyscaleImage::load(string const& filename)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
        if(fif == FIF_UNKNOWN)
            throw runtime_error("Cannot open image");
    }
    auto fi = FreeImage_Load(fif, filename.c_str());
    if(!fi) {
        throw runtime_error("Cannot open image");
    }
    return GreyscaleImage(fi);
}

void GreyscaleImage::buildPalette() {
    RGBQuad* palette = FreeImage_GetPalette(m_image);
    for(int i = 0 ; i != 256 ; ++i) {
        palette[i].rgbRed = i;
        palette[i].rgbGreen = i;
        palette[i].rgbBlue = i;
    }
}

RGBImage::RGBImage(int width, int height) :
    Image<RGBTriple>(width, height, ImageType::Bitmap, 24, 0x0000FF, 0x00FF00, 0xFF0000)
{ }

RGBImage::RGBImage(FIBITMAP* fi) :
    Image<RGBTriple>(fi)
{  }

RGBImage::~RGBImage() { }

RGBImage::RGBImage(RGBImage const& other) :
    Image<RGBTriple>(other)
{ }

RGBImage& RGBImage::operator=(RGBImage const& other) {
    *static_cast<Image<RGBTriple>*>(this) = other;
    return *this;
}

RGBTriple RGBImage::getPixel(int x, int y) const {
    RGBQUAD quad;
    if(!FreeImage_GetPixelColor(m_image, x, y, &quad)) {
        throw runtime_error("Cannot read pixel");
    }
    return {quad.rgbRed, quad.rgbGreen, quad.rgbBlue};
}

void RGBImage::setPixel(int x, int y, RGBTriple pixel) {
    RGBQUAD quad = {pixel.rgbtRed, pixel.rgbtGreen, pixel.rgbtBlue, 0};
    if(!FreeImage_SetPixelColor(m_image, x, y, &quad)) {
        throw runtime_error("Cannot set pixel value");
    }
}

RGBImage RGBImage::fromRawData(vector<RGBTriple> vec, int width, int height, bool flip) {
    if(vec.size() != static_cast<size_t>(width * height)) {
        throw runtime_error("Input vector has wrong size");
    }
    RGBImage img(width, height);
    for(int y = 0 ; y != height ; ++y) {
        int scanlineOffset = width * y;
        for(int x = 0 ; x != width ; ++x) {
            RGBTriple pixel = vec[x + scanlineOffset];
            img.setPixel(x, flip? height-y : y, pixel);
        }
    }

    return img;
}

RGBImage RGBImage::load(string const& filename)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
        if(fif == FIF_UNKNOWN)
            throw runtime_error("Cannot open image");
    }
    auto fi = FreeImage_Load(fif, filename.c_str());
    if(!fi) {
        throw runtime_error("Cannot open image");
    }
    return RGBImage(fi);
}

BinaryImage::BinaryImage(int width, int height) :
    Image<bool>(width, height, ImageType::Bitmap, 1, 0xFF, 0xFF, 0xFF)
{
    buildPalette();
}

BinaryImage::BinaryImage(FIBITMAP* fi) :
    Image<bool>(fi)
{
    buildPalette();
}

BinaryImage::~BinaryImage() { }

BinaryImage::BinaryImage(BinaryImage const& other) :
    Image<bool>(other)
{
    buildPalette();
}

BinaryImage& BinaryImage::operator=(BinaryImage const& other) {
    *static_cast<Image<bool>*>(this) = other;
    return *this;
}

bool BinaryImage::getPixel(int x, int y) const {
    byte b;
    if(!FreeImage_GetPixelIndex(m_image, x, y, &b)) {
        throw runtime_error("Cannot read pixel");
    }
    return b;
}

void BinaryImage::setPixel(int x, int y, bool pixel) {
    byte b = pixel ? 1 : 0;
    if(!FreeImage_SetPixelIndex(m_image, x, y, &b)) {
        throw runtime_error("Cannot set pixel value");
    }
}

BinaryImage BinaryImage::fromRawData(vector<bool> vec, int width, int height, bool flip) {
    if(vec.size() != static_cast<size_t>(width * height)) {
        throw runtime_error("Input vector has wrong size");
    }
    BinaryImage img(width, height);
    for(int y = 0 ; y != height ; ++y) {
        int scanlineOffset = width * y;
        for(int x = 0 ; x != width ; ++x) {
            bool pixel = vec[x + scanlineOffset];
            img.setPixel(x, flip? height-y : y, pixel);
        }
    }

    return img;
}

BinaryImage BinaryImage::load(string const& filename)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
        if(fif == FIF_UNKNOWN)
            throw runtime_error("Cannot open image");
    }
    auto fi = FreeImage_Load(fif, filename.c_str());
    if(!fi) {
        throw runtime_error("Cannot open image");
    }
    return BinaryImage(fi);
}

void BinaryImage::buildPalette() {
    RGBQuad* palette = FreeImage_GetPalette(m_image);
    palette[0].rgbRed = 0;
    palette[0].rgbGreen = 0;
    palette[0].rgbBlue = 0;
    palette[1].rgbRed = 255;
    palette[1].rgbGreen = 255;
    palette[1].rgbBlue = 255;
}
