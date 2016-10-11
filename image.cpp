#include "image.h"
#include <stdexcept>
#include <fstream>
#include <limits>

using namespace std;

bool Rect::operator==(Rect const& other) {
    return (x == other.x) && (y == other.y) &&
           (width == other.width) && (height == other.height);
}

bool Rect::isIn(Rect const& other) {
    ImageCoords p1({x, y}), p2({x + width, y + height});
    return p1.isIn(other) && p2.isIn(other);
}

bool ImageCoords::operator==(ImageCoords const& other) {
    return (x == other.x) && (y == other.y);
}

bool ImageCoords::isIn(Rect const& r) {
    return (x >= r.x) && (x < (r.x + r.width)) &&
           (y >= r.y) && (y < (r.y + r.height));
}

ostream& operator<<(ostream& s, ImageCoords const& c) {
    s << "(" << c.x << ", " << c.y << ")";
    return s;
}

ostream& operator<<(ostream& s, Rect const& r) {
    s << "(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height <<  ")";
    return s;
}

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

bool BinaryImage::isImmediateInterior(int x, int y) const {
    if(getPixel(x, y)) {
        bool top, left, bottom, right;
        top = (y == 0) ? true : getPixel(x, y - 1);
        left = (x == 0) ? true : getPixel(x - 1, y);
        bottom = (y == (m_height - 1)) ? true : getPixel(x, y + 1);
        right = (x == (m_width - 1)) ? true : getPixel(x + 1, y);

        return !(top && left && bottom && right);
    }
    return false;
}

bool BinaryImage::isImmediateExterior(int x, int y) const {
    if(!getPixel(x, y)) {
        bool top, left, bottom, right;
        top = (y == 0) ? false : getPixel(x, y - 1);
        left = (x == 0) ? false : getPixel(x - 1, y);
        bottom = (y == (m_height - 1)) ? false : getPixel(x, y + 1);
        right = (x == (m_width - 1)) ? false : getPixel(x + 1, y);

        return top || left || bottom || right;
    }
    return false;
}

void BinaryImage::setPixel(int x, int y, bool pixel) {
    byte b = pixel ? 1 : 0;
    if(!FreeImage_SetPixelIndex(m_image, x, y, &b)) {
        throw runtime_error("Cannot set pixel value");
    }
}
GreyscaleImage BinaryImage::deadReckoning3x3(bool symmetry) const {
    std::vector<std::vector<ImageCoords>> p;
    p.reserve(m_height);
    GreyscaleImage out(m_width, m_height);

    /* Initialization
     * Set the immediate interior and immediate exterior to 0 and the rest
     * to the "infinity"
     */
    for(int x = 0 ; x != m_height ; ++x) {
        p.emplace_back(m_width);
        for(int y = 0 ; y != m_width ; ++y) {
            if(isImmediateInterior(x, y) || (symmetry && isImmediateExterior(x, y))) {
                p[x][y] = {x, y};
                out.setPixel(x, y, 0);
            }
            else {
                p[x][y] = {-1, -1};
                out.setPixel(x, y, 255);
            }
        }
    }

    float d1 = 1.f, d2 = sqrt(2.f);
    /* Forward pass
     * Since we use a different coordinate system than the authors of the
     * algorithm, the windows must be slightly modified:
     * Forward:    -     -    -       Backward:  sqrt(2)  1  sqrt(2)
     *             1     C    -                     -     C     1
     *          sqrt(2)  1  sqrt(2)                 -     -     -
     */
    for(int y = 1 ; y != m_height ; ++y) {
        for(int x = 1 ; x != m_width - 1 ; ++x) {
            int i1, i2;
            if(out.getPixel(x-1, y-1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x-1][y-1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x, y-1) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x][y-1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x+1, y-1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x+1][y-1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x-1, y) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x-1][y];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
        }
    }

    // Backward pass
    for(int y = m_height- 2 ; y >= 0 ; --y) {
        for(int x = m_width - 2 ; x != 0 ; --x) {
            int i1, i2;
            if(out.getPixel(x+1, y+1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x+1][y+1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x, y+1) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x][y+1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x-1, y+1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x-1][y+1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x+1, y) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x+1][y];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
        }
    }

    // Final pass: mark the inside/outside and map to the correct output range
    for(int y = 0 ; y != m_height ; ++y) {
        for(int x = 0 ; x != m_width ; ++x) {
            byte outPixel = out.getPixel(x, y);
            char dist = getPixel(x, y) ? clamp<byte>(0, 127, outPixel) :
                                        -clamp<byte>(0, 128, outPixel);
            out.setPixel(x, y, dist >= 0 ? static_cast<byte>(dist) + 128 :
                                           128 - static_cast<byte>(-dist));
        }
    }

    return out;
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
