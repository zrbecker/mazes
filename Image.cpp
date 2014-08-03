#include "Image.hpp"

#include <algorithm>
#include <fstream>
#include <ostream>
#include <vector>



std::ostream &operator<<(std::ostream &out, const Color &color)
{
    return out << color.red << " " << color.green << " " << color.blue;
}



struct Image::ImageImpl
{
    unsigned int width;
    unsigned int height;
    std::vector<Color> pixels;
};



Image::Image(unsigned int width, unsigned int height, Color color)
    : pimpl(nullptr)
{
    pimpl = new ImageImpl;
    pimpl->width = width;
    pimpl->height = height;
    pimpl->pixels.resize(width * height);
    std::fill(pimpl->pixels.begin(), pimpl->pixels.end(), color);
}



Image::Image(const Image &other)
    : pimpl(nullptr)
{
    pimpl = new ImageImpl;
    pimpl->width = other.pimpl->width;
    pimpl->height = other.pimpl->width;
    pimpl->pixels.resize(pimpl->width * pimpl->height);
    std::copy(other.pimpl->pixels.begin(), other.pimpl->pixels.end(), pimpl->pixels.begin());
}



Image::Image(Image &&other)
    : pimpl(nullptr)
{
    pimpl = other.pimpl;
    other.pimpl = nullptr;
}



Image::~Image()
{
    delete pimpl;
    pimpl = nullptr;
}



Image &Image::operator=(const Image &other)
{
    if (this == &other)
    {
        pimpl->width = other.pimpl->width;
        pimpl->height = other.pimpl->height;
        pimpl->pixels.resize(pimpl->width * pimpl->height);
        std::copy(other.pimpl->pixels.begin(), other.pimpl->pixels.end(), pimpl->pixels.begin());
    }
    return *this;
}



Image &Image::operator=(Image &&other)
{
    if (this == &other)
    {
        delete pimpl;
        pimpl = other.pimpl;
        other.pimpl = nullptr;
    }
    return *this;
}



Color *Image::operator[](unsigned int x)
{
    return &pimpl->pixels[x * pimpl->height];
}



const Color *Image::operator[](unsigned int x) const
{
    return const_cast<Image *>(this)->operator[](x);
}



unsigned int Image::width() const
{
    return pimpl->width;
}



unsigned int Image::height() const
{
    return pimpl->height;
}



int Image::savePPM(const std::string &filename) const
{
    std::ofstream output(filename);

    if (!output.is_open())
        return 1;

    output << "P3" << std::endl;
    output << pimpl->width << " " << pimpl->height << std::endl;
    output << "255" << std::endl;
    for (unsigned int x = 0; x < pimpl->width; ++x)
    {
        for (unsigned int y = 0; y < pimpl->height; ++y)
            output << (*this)[x][y] << " ";
        output << std::endl;
    }

    int result = 0;
    if (output.bad() || output.fail())
        result = 1;
    output.close();

    return result;
}
