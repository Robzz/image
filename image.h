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

using byte = unsigned char;
using RGBTriple= RGBTRIPLE;
using RGBQuad = RGBQUAD;

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
enum class ImageFormat {
    /** Bitmap format */
    Bmp,
    /** Bitmap format, with run length encoding */
    BmpRle,
    /** PNG format */
    Png
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
        virtual byte getPixel(int x, int y) const;

        /**
          * \brief Set the color of the specified pixel.
          */
        virtual void setPixel(int x, int y, byte pixel);

        /**
          * \brief Construct an image from a file.
          * In theory, any format supported by the FreeImage library should work.
          */
        static GreyscaleImage load(std::string const& filename);

        static GreyscaleImage fromRawData(std::vector<byte> vec, int width, int height, bool flip = false);

    private:
        explicit GreyscaleImage(FIBITMAP* fi);
};

/**
  * \class RGBImage
  * \brief Represents an 8-bit greyscale image.
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
        virtual RGBTriple getPixel(int x, int y) const;

        /**
          * \brief Set the color of the specified pixel.
          */
        virtual void setPixel(int x, int y, RGBTriple pixel);

        /**
          * \brief Construct an image from a file.
          * In theory, any format supported by the FreeImage library should work.
          */
        static RGBImage load(std::string const& filename);

        static RGBImage fromRawData(std::vector<RGBTriple> vec, int width, int height, bool flip = false);

    private:
        explicit RGBImage(FIBITMAP* fi);
};

#include "image.inl"

#endif
