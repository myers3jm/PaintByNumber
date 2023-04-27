# PaintByNumber
A paint by number generator

This project seeks to provide and maintain a desktop program for the creation of paint-by-number products from an image or multiple images.

## Goals
1. Provide a cross-platform executable (Windows, Linux, MacOS) which follows this brief specification:
- Inputs: an image path, a palette size (number of colors).
- Outputs: an image, black and white, consisting of lines corresponding to color regions in the original image, with numbers in each bounded region to indicate the color to use.
2. Provide support for GPU-acceleration of supported operations.
3. Provide a rudimentary gui for the application, with a preview of the generated image (black and white + colored in).

## Implementation Decisions
This project will be implemented in C++23 and will make use of the OpenCV library (with experimental features).

The Cmake build system will be used for this project.

Object-oriented practices will be employed to the fullest extent.
