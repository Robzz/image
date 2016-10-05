#include <catch.hpp>
#include "image.h"

void fill16x16Img(GreyscaleImage& img) {
    byte c = 0;
    for(int y = 0 ; y != 16 ; ++y) {
        for(int x = 0 ; x != 16 ; ++x, ++c) {
            img.setPixel(x, y, c);
        }
    }
}

TEST_CASE("Test instantiation, copy and comparison", "[basics]") {
    GreyscaleImage img(16, 16);
    fill16x16Img(img);
    auto img2(img);
    for(int y = 0 ; y != 16 ; ++y) {
        for(int x = 0 ; x != 16 ; ++x) {
            REQUIRE(img2.getPixel(x, y) == img.getPixel(x, y));
        }
    }
    REQUIRE(img == img2);
}

TEST_CASE("Test save and load", "[load-save]") {
    GreyscaleImage img(16, 16);
    fill16x16Img(img);
    img.save("test-save.bmp", ImageFormat::Bmp);
    auto img2 = GreyscaleImage::load("test-save.bmp");
    REQUIRE(img == img2);
}

TEST_CASE("Test flip functions", "[flip]") {
    GreyscaleImage img(16, 16);
    fill16x16Img(img);
    auto img2(img);
    img2.flipY();
    for(int y = 0 ; y != 16 ; ++y) {
        for(int x = 0 ; x != 16 ; ++x) {
            REQUIRE(img2.getPixel(x, 15 - y) == img.getPixel(x, y));
        }
    }
    img2.flipY();
    REQUIRE(img2 == img);
    img2.flipX();
    for(int y = 0 ; y != 16 ; ++y) {
        for(int x = 0 ; x != 16 ; ++x) {
            REQUIRE(img2.getPixel(15 - x, y) == img.getPixel(x, y));
        }
    }
    img2.flipX();
    REQUIRE(img2 == img);
}
