/**
  * \file include/image.h
  * \brief Contains the definition of the Image class
  * \author R.Chavignat
  */
#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <FreeImage.h>
#include <cmath>
#include <iostream>

using byte = unsigned char;
using RGBTriple= RGBTRIPLE;
using RGBQuad = RGBQUAD;

struct Rect {
    int x;
    int y;
    int width;
    int height;

    bool operator==(Rect const& other);

    bool isIn(Rect const& other);
};

struct ImageCoords {
    int x;
    int y;

    bool isIn(Rect const& r);

    bool operator==(ImageCoords const& other);
};

std::ostream& operator<<(std::ostream& s, ImageCoords const& c);
std::ostream& operator<<(std::ostream& s, Rect const& r);

template <class T>
T clamp(T a, T b, T f) {
    return std::max(std::min(f, b), a);
}

/**
  * \enum ImageType
  * \brief Enumerate image types
  */
enum class ImageType : typename std::underlying_type<FREE_IMAGE_TYPE>::type {
    Bitmap  = FIT_BITMAP,
    Uint16  = FIT_UINT16,
    Int16   = FIT_INT16,
    Uint32  = FIT_UINT32,
    Int32   = FIT_INT32,
    Float   = FIT_FLOAT,
    Double  = FIT_DOUBLE,
    Complex = FIT_COMPLEX,
    Rgb16   = FIT_RGB16,
    Rgba16  = FIT_RGBA16,
    Rgbf    = FIT_RGBF,
    Rgbaf   = FIT_RGBAF,
    Unknown = FIT_UNKNOWN
};

/**
  * \enum ImageFormat
  * \brief Represent the supported image file formats.
  */
enum class ImageFormat : typename std::underlying_type<FREE_IMAGE_FORMAT>::type {
    /** Bitmap format */
    Bmp = FIF_BMP,
    /** Bitmap format, with run length encoding. Only available for 4-8 bpp
     * bitmaps. */
    BmpRle,
    /** PNG format */
    Png = FIF_PNG
};

template <class T>
class Image {
    public:
        virtual ~Image();

        /* Copy constructor */
        Image(Image const& other);
        /* Move constructor */
        Image(Image&& other);
        /* Assignment operator */
        Image& operator=(Image const& other);
        /* Move-assignment operator */
        Image& operator=(Image&& other);

        /**
         * @brief Compare two images.
         *
         * @param other Image to compare with.
         *
         * @return true if the images are identical, false otherwise
         */
        bool operator==(Image const& other);

        /**
          * \brief Return the width of the image.
          */
        int width() const;

        /**
          * \brief Return the height of the image.
          */
        int height() const;

        virtual T getPixel(int x, int y) const = 0;
        virtual void setPixel(int x, int y, T pixel) = 0;

        virtual const T* getBits() const;
        virtual const T* getScanline(int scanline) const;

        Rect getAABB(T backgroundColor) const;

        void flipX();
        void flipY();

        void blit(ImageCoords c, Rect r, Image<T> const& other);
        void crop(Rect r);

        /**
          * \brief Save an image to the disk.
          * \param filename File name
          * \param f Image format
          */
        void save(std::string const& filename, ImageFormat f) const;

    protected:
        /* Default constructor */
        Image(int width, int height, ImageType t, int bpp, unsigned int rMask,
                unsigned int gMask, unsigned int bMask);

        FIBITMAP* m_image;
        int m_width;
        int m_height;
        explicit Image(FIBITMAP* fi);
};

/**
  * \class GreyscaleImage
  * \brief Represents an 8-bit greyscale image.
  */
class GreyscaleImage : public Image<byte> {
    public:
        /**
          * \brief Construct an empty image of specified dimensions and bit depth.
          * \param width Image width
          * \param height Image height
          */
        GreyscaleImage(int width, int height);

        /* Destructor */
        virtual ~GreyscaleImage();

        /**
          * \brief Copy constructor.
          */
        GreyscaleImage(GreyscaleImage const& other);

        /**
          * \brief Assignment operator.
          */
        GreyscaleImage& operator=(GreyscaleImage const& other);

        /**
          * \brief Move constructor.
          */
        GreyscaleImage(GreyscaleImage&& other);

        /**
          * \brief Move-assignment operator.
          */
        GreyscaleImage& operator=(GreyscaleImage&& other);

        /**
          * \brief Return the color of the specified pixel.
          */
        byte getPixel(int x, int y) const override;

        /**
          * \brief Set the color of the specified pixel.
          */
        void setPixel(int x, int y, byte pixel) override;

        /**
          * \brief Construct an image from a file.
          * In theory, any format supported by the FreeImage library should work.
          */
        static GreyscaleImage load(std::string const& filename);

        static GreyscaleImage fromRawData(std::vector<byte> vec, int width, int height, bool flip = false);

    private:
        explicit GreyscaleImage(FIBITMAP* fi);
        void buildPalette();
};

/**
  * \class RGBImage
  * \brief Represents a 24-bit RGB image.
  */
class RGBImage : public Image<RGBTriple> {
    public:
        /**
          * \brief Construct an empty image of specified dimensions and bit depth.
          * \param width Image width
          * \param height Image height
          */
        RGBImage(int width, int height);

        /* Destructor */
        virtual ~RGBImage();

        /**
          * \brief Copy constructor.
          */
        RGBImage(RGBImage const& other);

        /**
          * \brief Assignment operator.
          */
        RGBImage& operator=(RGBImage const& other);

        /**
          * \brief Move constructor.
          */
        RGBImage(RGBImage&& other);

        /**
          * \brief Move-assignment operator.
          */
        RGBImage& operator=(RGBImage&& other);

        /**
          * \brief Return the color of the specified pixel.
          */
        RGBTriple getPixel(int x, int y) const override;

        /**
          * \brief Set the color of the specified pixel.
          */
        void setPixel(int x, int y, RGBTriple pixel) override;

        /**
          * \brief Construct an image from a file.
          * In theory, any format supported by the FreeImage library should work.
          */
        static RGBImage load(std::string const& filename);

        static RGBImage fromRawData(std::vector<RGBTriple> vec, int width, int height, bool flip = false);

    private:
        explicit RGBImage(FIBITMAP* fi);
};

/**
  * \class BinaryImage
  * \brief Represents a binary image.
  */
class BinaryImage : public Image<bool> {
    public:
        /**
          * \brief Construct an empty binary image of specified dimensions.
          * \param width Image width
          * \param height Image height
          */
        BinaryImage(int width, int height);

        /* Destructor */
        virtual ~BinaryImage();

        /**
          * \brief Copy constructor.
          */
        BinaryImage(BinaryImage const& other);

        /**
          * \brief Assignment operator.
          */
        BinaryImage& operator=(BinaryImage const& other);

        /**
          * \brief Move constructor.
          */
        BinaryImage(BinaryImage&& other);

        /**
          * \brief Move-assignment operator.
          */
        BinaryImage& operator=(BinaryImage&& other);

        /**
          * \brief Return the color of the specified pixel.
          */
        bool getPixel(int x, int y) const override;

        /**
          * \brief Set the color of the specified pixel.
          */
        void setPixel(int x, int y, bool pixel) override;

        bool isImmediateInterior(int x, int y) const;
        bool isImmediateExterior(int x, int y) const;

        /**
         * \brief Return the signed distance transform of the image, using
         * the "Dead Reckoning" algorithm.
         *
         * Precision is limited by the underlying pixel type. The output range
         * is [-128, 127] mapped to the integer range [0, 255] in 8 bit
         * precision. Values out of the range are clamped.
         *
         * \param symmetry If set to true, the transform will be symmetrical
         * under complement.
         * \return Signed distance transform greyscale image
         */
        GreyscaleImage deadReckoning3x3(bool symmetry = false) const;

        /**
          * \brief Construct an image from a file.
          * In theory, any format supported by the FreeImage library should work.
          */
        static BinaryImage load(std::string const& filename);

        static BinaryImage fromRawData(std::vector<bool> vec, int width, int height, bool flip = false);

    private:
        explicit BinaryImage(FIBITMAP* fi);
        void buildPalette();
};
#include "image.inl"

#endif
