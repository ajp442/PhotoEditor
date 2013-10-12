#include "image.h"


Image::Image()
{
}

bool Image::load( const QString & fileName, const char * format, Qt::ImageConversionFlags flags )
{
    bool returnValue =QPixmap::load(fileName, format, flags);
    unModifiedImage = new QImage(this->toImage());
    return returnValue;
}

//WORKS
void Image::grayscale()
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug("Null reference found");
        return;
    }

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

//WORKS
void Image::sharpen()
{
    QImage *image = new QImage(*unModifiedImage);
    QImage *temp = new QImage(*unModifiedImage);//Copy of the original image

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

//WORKS
void Image::negative()
{
    QImage *image = new QImage(*unModifiedImage);
    image->invertPixels();
    this->convertFromImage(*image);
}

//Does not work
void Image::despeckle(int threshold = 32)
{
    QImage *image = new QImage(*unModifiedImage);
    QImage *temp = new QImage(*unModifiedImage);          //Copy of the original image

    //If there is no image selected, exit the function.
    //This could prevent crashes when trying to reference an
    //image that doesn't exist and also saves time by eliminating
    //computations.
    if(image->isNull())
        return;

    //For every pixel in the image, change the value of the pixel
    //if it's out of range of the threshold
    for (int r = 1; r < image->width() - 1; r++)
        for(int c = 1; c < image->height() - 1; c++)
        {
            //remove the middle pixel from the average
            int blueSum = -qBlue(temp->pixel(r, c));
            int greenSum = -qGreen(temp->pixel(r, c));
            int redSum = -qRed(temp->pixel(r, c));

            //Add all 9 pixels to the sum
            //The middle pixel was already subtracted, so it will add
            //zero to the sum overall
            for(int i = -1; i <= 1; i++)
                for(int j = -1; j <= 1; j++)
                {
                    QRgb pixel = temp->pixel(r+i, c+j);
                    blueSum = -qBlue(pixel);
                    greenSum = -qGreen(pixel);
                    redSum = -qRed(pixel);
                }

            //Calculate the average (8 total pixels)
            double blueAvg = blueSum/8.0;
            double greenAvg = greenSum/8.0;
            double redAvg = redSum/8.0;

            double avg = blueAvg * 0.11 + redAvg * 0.30 + greenAvg * 0.59;

            //If the difference between the center pixel and the
            //average is greater than than the threshold, replace
            //the middle pixel with the average of the other 8 pixels
            if(abs(qGray(temp->pixel(r, c)) - avg) > threshold)
                image->setPixel(r, c, qRgb(redAvg, greenAvg, blueAvg));
        }
    this->convertFromImage(*image);
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

//WORKS
void Image::gamma(double gammaValue)
{
    QImage *image = new QImage(*unModifiedImage);

    if( image->isNull() )
        return;

    for ( int x = 0; x < image->width(); x++ )
    {
        for ( int y = 0; y < image->height(); y++ )
        {
            QRgb p = image->pixel( x, y );
            int r = pow( qRed( p ) / 255.0, gammaValue ) * 255 + 0.5;
            int g = pow( qGreen( p ) / 255.0, gammaValue ) * 255 + 0.5;
            int b = pow( qBlue( p ) / 255.0, gammaValue ) * 255 + 0.5;
            image->setPixel( x, y, qRgb( r, g, b ) );
        }
    }

    this->convertFromImage(*image);
}


//WORKS
void Image::brightness(int brightnessLevel)
{
    QImage *image = new QImage(*unModifiedImage);//this->toImage();
    if( image->isNull() )
        return;

    int nrows = image->width(), ncols = image->height(), r, c,
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
            QRgb pixel = image->pixel(r,c);
            int red = lut[qRed(pixel)];
            int green = lut[qGreen(pixel)];
            int blue = lut[qBlue(pixel)];
            image->setPixel(r, c, qRgb(red, green, blue));
        }

    this->convertFromImage(*image);
}


//WORKS
void Image::binaryThreshold(int threshold)
{
    QImage *image = new QImage(*unModifiedImage);

    if(threshold < 0 || threshold > 255)
    {
        qDebug() << "Image::binaryThreshold -> Invalid parameter, expecting a value [0,255] instead got " << threshold;
    }

    if( image->isNull() )
    {
        qDebug() << "Image::binaryThreshold -> Null reference";
        return;
    }

    int lut[256] = {0};

    for( int i = threshold; i < 256; i++ )
        lut[i] = 255;

    //Initialize image to grayscale to facilitate
    // binary threshold operation
    //Menu_Palette_Grayscale( image );

    for( int r=0; r < image->width(); r++)
        for( int c = 0; c < image->height(); c++)
        {
            QRgb pixel = image->pixel(r, c);
            int pixelValue = lut[(int)(qRed(pixel) * 0.3 +
                                       qGreen(pixel) * 0.59 +
                                       qBlue(pixel) * 0.11)];
            image->setPixel(r, c, qRgb( pixelValue, pixelValue, pixelValue));
        }

    this->convertFromImage(*image);
}

void Image::contrast(int lower, int upper)
{

}

void Image::commit()
{
    unModifiedImage = new QImage(this->toImage());
}

void Image::revert()
{
    this->convertFromImage(*unModifiedImage);
}

