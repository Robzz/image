/**
  * \file include/image.h
  * \brief Contains the definition of the Image class
  * \author R.Chavignat
  */
#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <type_traits>
#include <FreeImage.h>

using byte = unsigned char;

/**
  * \class Image
  * \brief Represents an image in memory.
  */
class Image {
    public:
        /**
          * \enum Type
          * \brief Enumerate image types
          */
        enum class Type : typename std::underlying_type<FREE_IMAGE_TYPE>::type {
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
          * \brief Construct an empty image of specified dimensions and bit depth.
          * \param width Image width
          * \param height Image height
          * \param t Type of the Image
          * \param bpp Image Depth, in bits per pixel
          */
        Image(unsigned int width, unsigned int height, Type t = Type::Bitmap, int bpp = 24);

        /**
          * \brief Construct an Image object that is not bound to any image.
          */
        Image();

        /**
          * \brief Construct an image from a file.
          * In theory, any format supported by the FreeImage library should work.
          */
        explicit Image(std::string const& filename);
        /* Destructor */
        virtual ~Image();

        /**
          * \enum Format
          * \brief Represent the supported image file formats.
          */
        enum class Format {
            /** Bitmap format */
            Bmp,
            /** Bitmap format, with run length encoding */
            BmpRle,
            /** PNG format */
            Png };

        /**
          * \brief Construct a copy of an Image.
          */
        Image(Image const& other);
        /**
          * \brief Image assignment operator.
          */
        Image& operator=(Image const& other);

        /**
          * \brief Return false if the image is empty, i.e. it contains only metadata
          * and no pixel data.
          */
        bool hasPixels() const;

        /**
          * \brief Return the width of the image.
          */
        unsigned int width() const;

        /**
          * \brief Return the height of the image.
          */
        unsigned int height() const;

        unsigned int bpp() const;

        Type type() const;

        /**
          * \brief Return the color of the specified pixel.
          */
        RGBQUAD getPixel(unsigned int x, unsigned int y) const;

        /**
          * \brief Set the color of the specified pixel.
          */
        void setPixel(unsigned int x, unsigned int y, RGBQUAD color);

        const byte* getBits() const;
        const byte* getScanLine(int scanLine) const;

        /**
          * \brief Create an image from an array RGB values.
          * \tparam T Type of the pixel components
          * \param vec RGB data array
          * \param width Image width
          * \param height Image height
          * \param flip Flip the image vertically
          */
        template <class T>
        static typename std::enable_if<std::is_arithmetic<T>::value, Image>::type
            from_rgb(std::vector<T> vec, int width, int height, bool flip = false);
        static Image from_greyscale(std::vector<unsigned short> vec, int width, int height, bool flip = false);

        /**
          * \brief Save an image to the disk.
          * \param filename File name
          * \param f Image format
          */
        bool save(std::string const& filename, Format f) const;

    private:
        explicit Image(FIBITMAP* other);
        FIBITMAP* m_image;
};

#include "image.inl"

#endif
