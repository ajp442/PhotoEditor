#include "image.h"
#include <math.h>

Image::Image()
{
}
/*
QImage* Image::average(const QImage *image1, const QImage *image2, const QImage *image3)
{
    if(image1 == NULL || image2 == NULL || image3 == NULL || image1->isNull() || image2->isNull() || image3->isNull())
    {
        qDebug() << "Image::average -> Null reference";
        return NULL;
    }

    if(image1->width() != image2->width() != image3->width() ||
       image1->height() != image2->height() != image3->height())
    {
        qDebug() << "Image::average -> Image sizes don't match";
        //return NULL;
    }

    QImage *image = new QImage(*image1);
    for(int x = 0; x < image1->width(); x++)
        for(int y = 0; y < image1->height(); y++)
        {
            QRgb pixel1 = image1->pixel(x, y);
            QRgb pixel2 = image2->pixel(x, y);
            QRgb pixel3 = image3->pixel(x, y);
            int redAvg = (qRed(pixel1) + qRed(pixel2) + qRed(pixel3)) / 3.0;
            int greenAvg = (qGreen(pixel1) + qGreen(pixel2) + qGreen(pixel3)) / 3.0;
            int blueAvg = (qBlue(pixel1) + qBlue(pixel2) + qBlue(pixel3)) / 3.0;
            image->setPixel(x, y, qRgb(redAvg, greenAvg, blueAvg));
        }
    return image;
}
*/
bool Image::load( const QString & fileName, const char * format, Qt::ImageConversionFlags flags )
{
    bool returnValue =QPixmap::load(fileName, format, flags);
    unModifiedImage = new QImage(this->toImage());
    return returnValue;
}

//WORKS
void Image::grayscale(QImage *image)
{
    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
    }

    if(image->isNull())
    {
        qDebug() << "Image::grayscale -> Null reference";
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
void Image::sharpen(QImage *image)
{
    QImage *temp;//Copy of the original image

    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
        temp = new QImage(*unModifiedImage);
    }
    else
    {
        temp = new QImage(*image);
    }

    if(image->isNull() || temp->isNull())
    {
        qDebug() << "Image::sharpen -> Null reference";
        return;
    }

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

void Image::soften(QImage *image)
{
    QImage *temp; //Copy of original image

    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
        temp = new QImage(*unModifiedImage);
    }
    else
    {
        temp = new QImage(*image);
    }

    if(image->isNull() || temp->isNull())
    {
        qDebug() << "Image::soften -> Null reference";
        return;
    }
}

//WORKS
void Image::negative(QImage *image)
{
    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
    }

    if(image->isNull())
    {
        qDebug() << "Image::negative -> Null reference";
        return;
    }

    image->invertPixels();
    this->convertFromImage(*image);
}

//Does not work
void Image::despeckle(int threshold = 32, QImage *image)
{
    QImage *temp; //Copy of original image

    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
        temp = new QImage(*unModifiedImage);
    }
    else
    {
        temp = new QImage(*image);
    }

    if(image->isNull() || temp->isNull())
    {
        qDebug() << "Image::despeckle -> Null reference";
        return;
    }

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

void Image::posterize(QImage *image)
{

}

void Image::edge(QImage *image)
{
    QImage *temp; //Copy of original image

    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
        temp = new QImage(*unModifiedImage);
    }
    else
    {
        temp = new QImage(*image);
    }

    if(image->isNull() || temp->isNull())
    {
        qDebug() << "Image::edge -> Null reference";
        return;
    }

    for ( int x = 1; x < temp->width() - 1; x++ )
    {
        for ( int y = 1; y < temp->height() - 1; y++ )
        {
            // pseudo-Prewitt edge magnitude
            int Gx = qGray( temp->pixel( x, y + 1 ) ) - qGray( temp->pixel( x, y - 1 ) );
            int Gy = qGray( temp->pixel( x + 1, y ) ) - qGray( temp->pixel( x - 1, y ) );
            int e = 3 * sqrt( Gx * Gx + Gy * Gy );
            if ( e > 255 ) e = 255;
            image->setPixel( x, y, qRgb( e, e, e ) );
        }
    }
}

void Image::emboss(QImage *image)
{

}

//WORKS
void Image::gamma(double gammaValue, QImage *image)
{

    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
    }

    if(image->isNull())
    {
        qDebug() << "Image::gamma -> Null reference";
        return;
    }

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
void Image::brightness(int brightnessLevel, QImage *image)
{
    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
    }

    if(image->isNull())
    {
        qDebug() << "Image::brightness -> Null reference";
        return;
    }

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
void Image::binaryThreshold(int threshold, QImage *image)
{
    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
    }

    if(image->isNull())
    {
        qDebug() << "Image::binaryThreshold -> Null reference";
        return;
    }

    if(threshold < 0 || threshold > 255)
    {
        qDebug() << "Image::binaryThreshold -> Invalid parameter, expecting a value [0,255] instead got " << threshold;
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

void Image::contrast(int lower, int upper, QImage *image)
{
    if(image == NULL)
    {
        image = new QImage(*unModifiedImage);
    }

    if(image->isNull())
    {
        qDebug() << "Image::binaryThreshold -> Null reference";
        return;
    }
}
/*
void Image::balance(int brightness, int contrastLower, int contrastUpper, double gamma)
{
    static int oldBrightness = -999;
    static int oldContrastLower = -999;
    static int oldContrastUpper = -999;
    static double oldGamma = -999;
    static QImage *brightImage = new QImage(*unModifiedImage);
    static QImage *contrastImage = new QImage(*unModifiedImage);
    static QImage *gammaImage = new QImage(*unModifiedImage);

    if(brightness != oldBrightness)
    {
        oldBrightness = brightness;
        this->brightness(brightness, brightImage);
    }

    if(contrastLower != oldContrastLower || contrastUpper != oldContrastUpper)
    {
        oldContrastLower = contrastLower;
        oldContrastUpper = contrastUpper;
        //this->contrast(contrastLower, contrastUpper, contrastImage);
    }

    if(oldGamma != gamma)
    {
        oldGamma = gamma;
        this->gamma(gamma, gammaImage);
    }

    QImage *image = Image::average(brightImage, contrastImage, gammaImage);
    this->convertFromImage(*image);
}
*/
void Image::commit()
{
    unModifiedImage = new QImage(this->toImage());
}

void Image::revert()
{
    this->convertFromImage(*unModifiedImage);
}

