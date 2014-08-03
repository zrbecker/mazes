#ifndef IMAGE_H
#define IMAGE_H

#include <ostream>

struct Color
{
    int red;
    int green;
    int blue;
};

std::ostream &operator<<(std::ostream &out, const Color &color);

class Image
{
public:
    Image(unsigned int width, unsigned int height, Color color = Color{ 255, 255, 255 });
    Image(const Image &other);
    Image(Image &&other);
    virtual ~Image();

    Image &operator=(const Image &other);
    Image &operator=(Image &&other);

    // For "Image image;" do "image[x][y]" to get the pixel at (x, y).
    Color *operator[](unsigned int x);
    const Color *operator[](unsigned int x) const;

    unsigned int width() const;
    unsigned int height() const;

    int savePPM(const std::string &filename) const;

private:
    struct ImageImpl;
    ImageImpl *pimpl;
};

#endif
