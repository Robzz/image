cmake_minimum_required(VERSION 2.8)
project(Image CXX)

find_library(FREEIMAGE_LIBRARY freeimage)
if(${FREEIMAGE_LIBRARY} STREQUAL FREEIMAGE_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "libfreeimage not found.")
endif()

set(image_SOURCES image.h image.inl image.cpp)
set(image_LIBRARIES ${FREEIMAGE_LIBRARY})
add_library(image ${image_SOURCES})
target_link_libraries(image ${image_LIBRARIES})

set_property(TARGET image PROPERTY CXX_STANDARD 14)
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(extra_cxxflags "-Wdocumentation")
endif()
target_compile_options(image PUBLIC -Wall -Wextra ${extra_cxxflags})
