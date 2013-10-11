#include "image.h"


Image::Image()
{
}

void Image::grayscale()
{
    QImage *image = new QImage(this->toImage());

    if(image->isNull())
        return;

    int gray = 0;
    int nrows = image->width(), ncols = image->height();
    for( int r=0; r < nrows; r++)
        for( int c = 0; c < ncols; c++)
        {
            //get overall monochrome intesity value by using
            //the grayscale method that takes into account human
            //eye sensitivity to light.
            int blue = qBlue(image->pixel(r,c)) * 0.11 + 0.5;
            int green = qGreen(image->pixel(r,c)) * 0.59 + 0.5;
            int red = qRed(image->pixel(r,c)) * 0.3 + 0.5;
            gray = red + blue + green;
            QRgb rgbValue = qRgb(gray, gray, gray);
            image->setPixel(r,c, rgbValue);
        }
    this->convertFromImage(*image);
}

void Image::sharpen()
{
    QImage *image = new QImage(this->toImage());
    QImage *temp = new QImage(this->toImage());//Copy of the original image

    //If there is no image, exit the function.
    //This could prevent crashes when trying to reference an
    //image that doesn't exist and also saves time by eliminating
    //computations.
    if(image->isNull() || temp->isNull())
        return;

    //For every pixel in the image, apply the following mask:
    //  0  -1   0
    // -1   5  -1
    //  0  -1   0
    //"temp" is used since it's an uneditted copy
    //of the original image
    for (int r = 1; r < image->width() - 1; r++)
        for(int c = 1; c < image->height() - 1; c++)
        {
            QRgb pixel = temp->pixel(r,c); //The item being looked at
            int red = qRed(pixel) * 5;
            int blue = qBlue(pixel) * 5;
            int green = qGreen(pixel) * 5;

            pixel = temp->pixel(r, c-1); //Item to the left of [r][c]
            red -= qRed(pixel);
            blue -= qBlue(pixel);
            green -= qGreen(pixel);

            pixel = temp->pixel(r-1, c); //Item above [r][c]
            red -= qRed(pixel);
            blue -= qBlue(pixel);
            green -= qGreen(pixel);

            pixel = temp->pixel(r+1, c);  //Item below [r][c]
            red -= qRed(pixel);
            blue -= qBlue(pixel);
            green -= qGreen(pixel);

            pixel = temp->pixel(r, c+1);  //Item to the right of [r][c]
            red -= qRed(pixel);
            blue -= qBlue(pixel);
            green -= qGreen(pixel);

            //Verify rgb is within 0 to 255
            red = (red < 0) ? 0 : (red > 255) ? 255 : red;
            green = (green < 0) ? 0 : (green > 255) ? 255 : green;
            blue = (blue < 0) ? 0 : (blue > 255) ? 255 : blue;

            image->setPixel(r, c, qRgb(red, green, blue));
        }
    this->convertFromImage(*image);
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

void Image::brightness(int brightnessLevel)
{
    QImage image = this->toImage();
    if( image.isNull() )
        return;

    int nrows = image.width(), ncols = image.height(), r, c,
    lut[256];

    for(int i = 0; i < 256; i++ )
    {
        if( (i + brightnessLevel) > 255 )
            lut[i] = 255;
        else if(i + brightnessLevel < 0)
            lut[i] = 0;
        else
            lut[i] = i + brightnessLevel;
    }

    for( r = 0; r < nrows; r++ )
        for( c = 0; c < ncols; c++ )
        {
            QRgb pixel = image.pixel(r,c);
            int red = lut[qRed(pixel)];
            int green = lut[qGreen(pixel)];
            int blue = lut[qBlue(pixel)];
            image.setPixel(r, c, qRgb(red, green, blue));
        }

    this->convertFromImage(image);
}
