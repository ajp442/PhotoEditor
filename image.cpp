#include "image.h"


Image::Image()
{
}

void Image::grayscale()
{
    QImage image = this->toImage();
    int gray = 0;
    int nrows = image.width(), ncols = image.height();
    for( int r=0; r < nrows; r++)
        for( int c = 0; c < ncols; c++)
        {
            //get overall monochrome intesity value by using
            //the grayscale method that takes into account human
            //eye sensitivity to light.
            int blue = qBlue(image.pixel(r,c)) * 0.11 + 0.5;
            int green = qGreen(image.pixel(r,c)) * 0.59 + 0.5;
            int red = qRed(image.pixel(r,c)) * 0.3 + 0.5;
            gray = red + blue + green;
            QRgb rgbValue = qRgb(gray, gray, gray);
            image.setPixel(r,c, rgbValue);
        }
    this->convertFromImage(image);
}

void Image::sharpen()
{

}

void Image::soften()
{

}

void Image::negative()
{
    QImage image = this->toImage();
    image.invertPixels();
    this->convertFromImage(image);

}

void Image::despeckle()
{

}

void Image::posterize()
{

}

void Image::edge()
{

}

void Image::emboss()
{

}

