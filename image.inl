#ifndef IMAGE_H
#include "image.h"
#endif

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value, Image>::type
Image::from_rgb(std::vector<T> vec, int width, int height, bool flip) {
    assert(vec.size() == (static_cast<unsigned int>(width * height) * 3u));
    Image img(FreeImage_ConvertFromRawBits(vec.data(), width, height, width * 3, 24u, 0x00FF0000,
                                           0x0000FF00, 0x000000FF, flip));
    return img;
}
