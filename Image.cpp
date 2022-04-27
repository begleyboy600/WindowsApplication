//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"


bool Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users

    try {
        if (ifs.fail())
        {
            throw("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int x = 0; x < w * h; ++x)
        {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[x].r = pix[0];
            this->pixels[x].g = pix[1];
            this->pixels[x].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }

    return true;
}
bool Image::loadRaw(string filename)
{
    // load raw file
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    try{
        if (ifs.fail())
        {
            throw("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int x = 0; x < w * h; ++x)
        {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[x].r = pix[0];
            this->pixels[x].g = pix[1];
            this->pixels[x].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }
    return true;
}
bool Image::savePPM(string filename)
{
    // save as PPM
    std::ofstream ofs;
    ofs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ofs.fail())
        {
            throw("Can't open output file");
        }
        ofs << "P6\n" << w << " " << h << "\n255\n";
        for (int x = 0; x < w * h; ++x)
        {
            ofs << static_cast<unsigned char>(pixels[x].r) << static_cast<unsigned char>(pixels[x].g) << static_cast<unsigned char>(pixels[x].b);
        }
        ofs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }
    return true;
}
void Image::filterRed()
{
    // filter red channel
    for (int x = 0; x < w * h; ++x)
    {
        pixels[x].r = pixels[x].r;
        pixels[x].g = 0;
        pixels[x].b = 0;
    }
}
void Image::filterGreen()
{
    // filter green channel
    for (int x = 0; x < w * h; ++x)
    {
        pixels[x].r = 0;
        pixels[x].g = pixels[x].g;
        pixels[x].b = 0;
    }
}
void Image::filterBlue()
{
    // filter blue channel
    for (int x = 0; x < w * h; ++x)
    {
        pixels[x].r = 0;
        pixels[x].g = 0;
        pixels[x].b = pixels[x].b;
    }
}
void Image::greyScale()
{
    // convert to grey scale
    for (int x = 0; x < w * h; ++x)
    {
        pixels[x].r = (pixels[x].r + pixels[x].g + pixels[x].b) / 3;
        pixels[x].g = (pixels[x].r + pixels[x].g + pixels[x].b) / 3;
        pixels[x].b = (pixels[x].r + pixels[x].g + pixels[x].b) / 3;
    }
}
void Image::flipHorizontal()
{
    for (int x = 0; x < h; ++x)
    {
        for(int y = 0; y < w/2; ++y)
        {

            int temp = pixels[x * w + y].r;
            pixels[x * w + y].r = pixels[x * w + w - y - 1].r;
            pixels[x * w + w - y - 1].r = temp;

            temp = pixels[x * w + y].g;
            pixels[x * w + y].g = pixels[x * w + w - y - 1].g;
            pixels[x * w + w - y - 1].g = temp;

            temp = pixels[x * w + y].b;
            pixels[x * w + y].b = pixels[x * w + w - y - 1].b;
            pixels[x * w + w - y - 1].b = temp;
        }
    }
}
void Image::flipVertically()
{
    for(int x = 0; x < w; ++x)
    {
        for(int y = 0; y < h/2; ++y)
        {

            int temp = pixels[y * w + x].r;
            pixels[y * w + x].r = pixels[(h - y - 1) * w + x].r;
            pixels[(h - y - 1) * w + x].r = temp;

            temp = pixels[y * w + x].g;
            pixels[y * w + x].g = pixels[(h - y - 1) * w + x].g;
            pixels[(h - y - 1) * w + x].g = temp;

            temp = pixels[y * w + x].b;
            pixels[y * w + x].b = pixels[(h - y - 1) * w + x].b;
            pixels[(h - y - 1) * w + x].b = temp;
        }
    }
}
void Image::AdditionalFunction1()
{
    // add gaussian blur
    for(int x = 0; x < h; ++x)
    {
        for(int y = 0; y < w; ++y)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            for(int a = -1; a <= 1; ++a)
            {
                for(int b = -1; b <= 1; ++b)
                {
                    if(x + a >= 0 && x + a < h && y + b >= 0 && y + b < w)
                    {
                        red += pixels[(x + a) * w + (y + b)].r;
                        green += pixels[(x + a) * w + (y + b)].g;
                        blue += pixels[(x + a) * w + (y + b)].b;
                    }
                }
            }
            pixels[x * w + y].r = red / 10;
            pixels[x * w + y].g = green / 10;
            pixels[x * w + y].b = blue / 10;
        }
    }

}
void Image::AdditionalFunction2()
{
    Image temp_img(h, w);
    unsigned int destination;
    for(int x = 0; x < h; ++x)
    {
        for(int y = 0; y < w; ++y)
        {
            destination = (y * h) + (h - x - 1);
            temp_img.pixels[destination] = pixels[(x * w) + y];
        }
    }
    *this = temp_img;
}
Image& Image::operator=(const Image &imageRef)
{
    if(this != &imageRef) {
        w = imageRef.w;
        h = imageRef.h;
        pixels = new Rgb[w * h];
        for(int x = 0; x < w * h; ++x)
        {
            pixels[x] = imageRef.pixels[x];
        }
    }
    return *this;
}
void Image::AdditionalFunction3()
{
    // add salt and pepper noise
    for (int x = 0; x < w * h; ++x)
    {
        if (rand() % 100 < 10)
        {
            pixels[x].r = 0;
            pixels[x].g = 0;
            pixels[x].b = 0;
        }
        else if (rand() % 100 < 10)
        {
            pixels[x].r = 255;
            pixels[x].g = 255;
            pixels[x].b = 255;
        }
    }
}
void Image::AdditionalFunction4()
{
    // edge detection using roberts cross algorithm
    // explanation: https://en.wikipedia.org/wiki/Roberts_cross_operator
    for(int x = 0; x < h; ++x)
    {
        for(int y = 0; y < w; ++y)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            if(x + 1 < h && y + 1 < w)
            {
                red = abs(pixels[(x + 1) * w + (y + 1)].r - pixels[(x + 1) * w + (y)].r) +
                        abs(pixels[(x + 1) * w + (y + 1)].r - pixels[(x) * w + (y + 1)].r);

                green = abs(pixels[(x + 1) * w + (y + 1)].g - pixels[(x + 1) * w + (y)].g) +
                        abs(pixels[(x + 1) * w + (y + 1)].g - pixels[(x) * w + (y + 1)].g);

                blue = abs(pixels[(x + 1) * w + (y + 1)].b - pixels[(x + 1) * w + (y)].b) +
                        abs(pixels[(x + 1) * w + (y + 1)].b - pixels[(x) * w + (y + 1)].b);
            }
            pixels[x * w + y].r = red;
            pixels[x * w + y].g = green;
            pixels[x * w + y].b = blue;
        }
    }
//    //
//    Image temp_img(h, w);
//    for(int x = 0; x < h; ++x)
//    {
//        for(int y = 0; y < w; ++y)
//        {
//            int red = 0;
//            int green = 0;
//            int blue = 0;
//            for(int a = -1; a <= 1; ++a)
//            {
//                for(int b = -1; b <= 1; ++b)
//                {
//                    if(x + a >= 0 && x + a < h && y + b >= 0 && y + b < w)
//                    {
//                        red += pixels[(x + a) * w + (y + b)].r * sobel_operator[a + 1][b + 1];
//                        green += pixels[(x + a) * w + (y + b)].g * sobel_operator[a + 1][b + 1];
//                        blue += pixels[(x + a) * w + (y + b)].b * sobel_operator[a + 1][b + 1];
//                    }
//                }
//            }
//            temp_img.pixels[x * w + y].r = red;
//            temp_img.pixels[x * w + y].g = green;
//            temp_img.pixels[x * w + y].b = blue;
//        }
//    }

}

// add gamma correction using a float
void Image::gammaCorrection()
{
    float gamma = 1/2.2;
    for(int x = 0; x < w * h; ++x)
    {
        pixels[x].r = pow(pixels[x].r / 255.0f, gamma) * 255;
        pixels[x].g = pow(pixels[x].g / 255.0f, gamma) * 255;
        pixels[x].b = pow(pixels[x].b / 255.0f, gamma) * 255;
    }
}


/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}





