#include <catch.hpp>
#include "image.h"
#include <iostream>

TEST_CASE("Dead reckoning signed distance transform 3x3", "[]") {
    auto testImg = BinaryImage::load("test-deadreckoning.bmp");
    auto transformed = testImg.deadReckoning3x3(true);
    const int tolerance = 0;
    for(int y = 0 ; y != transformed.height() ; ++y) {
        for(int x = 0 ; x != transformed.width() ; ++x) {
            byte transformedPixel = transformed.getPixel(x, y);
            if(testImg.getPixel(x, y)) {
                REQUIRE(((transformedPixel > 128) || ((128 - transformedPixel) <= tolerance)));
            }
            else
                REQUIRE(((transformedPixel <= 128) || ((transformedPixel - 128) <= tolerance)));
        }
    }
    transformed.save("test-deadreckoning-result.png", ImageFormat::Png);
}
