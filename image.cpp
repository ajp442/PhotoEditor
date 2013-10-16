#include "image.h"


/**************************************************************************//**
 * @brief Loads in a pixmap from a given filename.  The pixmap cache is cleared
 * everytime prior to a file load.  (This forces a similar filename to load
 * from disk, rather than use a stored cached pixmap.  This is used because the
 * way the revert() is handled)
 *
 * @param[in] fileName - Name of the file.
 * @param[in] format - Format to load the pixmap as
 * @param[in] flags - Special flags for converting pixmaps
 *****************************************************************************/
bool Image::load( const QString & fileName, const char * format, Qt::ImageConversionFlags flags )
{
    //Reset the pixmapCache before loading a file
    //Whenever a file is loaded into a QPixmap, the filepath, and modified date are used as
    //a key to the pixmap.  If the file is loaded again, it just uses the cached pixmap
    //In our case, we don't want to load the saved pixmap, we want to reset the pixmap each
    //time (used to revert)
    QPixmapCache::clear();

    bool returnValue =QPixmap::load(fileName, format, flags);

    //Delete our object before setting to a new object
    if(NULL == unModifiedImage){ delete unModifiedImage; }

    //Store the original image (until a commit() occurs)
    unModifiedImage = new QImage(this->toImage());

    return returnValue;
}

/**************************************************************************//**
 * @brief Converts the unModifiedImage to a gray image using 30% red 59% green
 * and 11% blue.  The image is then saved as the current object.
 *****************************************************************************/
void Image::grayscale()
{
    QImage *image = new QImage(*unModifiedImage);

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

    //Set the current instance equal to the grayscale image
    this->convertFromImage(*image);
}

/**************************************************************************//**
 * @brief Converts the unModifiedImage to a sharpened image.  The result is
 * stored into the current object.
 *****************************************************************************/
void Image::sharpen()
{
    QImage *temp = new QImage(*unModifiedImage);//Copy of the original image
    QImage *image = new QImage(*unModifiedImage);

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

    //Set the current instance equal to the modified image
    this->convertFromImage(*image);
}



/**************************************************************************//**
 * @brief The unModifiedImage is softened and then stored as the current object
 * instance.
 *****************************************************************************/
void Image::soften()
{
    QImage *temp = new QImage(*unModifiedImage); //Copy of original image
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull() || temp->isNull())
    {
        qDebug() << "Image::soften -> Null reference";
        return;
    }

    //Author: John M. Weiss, Ph.D.
    //Class:  CSC421/521 GUI-OOP, Fall 2013.
    //Posted September 23, 2013.
    for ( int x = 1; x < image->width() - 1; x++ )
    {
        for ( int y = 1; y < image->height() - 1; y++ )
        {
            int r = 0, g = 0, b = 0;
            for ( int m = -1; m <= 1; m++ )
            {
                for ( int n = -1; n <= 1; n++ )
                {
                    QRgb p = temp->pixel( x + m, y + n );
                    r += qRed( p );
                    g += qGreen( p );
                    b += qBlue( p );
                }
            }
            image->setPixel( x, y, qRgb( r / 9, g / 9, b / 9 ) );
        }
    }

    //Sets the current instance equal to the modified image
    this->convertFromImage(*image);
}

/**************************************************************************//**
 * @brief The unModifiedImage is negated and then stored as the current object
 * instance.
 *****************************************************************************/
void Image::negative()
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::negative -> Null reference";
        return;
    }

    //Negate the image
    image->invertPixels();

    //Set the current instance to the negated image
    this->convertFromImage(*image);
}

/**************************************************************************//**
 * @brief The following function is a placeholder.  It is not implemented.
 *****************************************************************************/
void Image::despeckle(int threshold = 32)
{
    qDebug() << "Image::despeckle -> Not Implemented";
}

/**************************************************************************//**
 * @brief The following function is a placeholder.  It is not implemented.
 *****************************************************************************/
void Image::posterize()
{
    qDebug() << "Image::posterize -> Not Implemented";
}

/**************************************************************************//**
 * @brief Finds the edges of the unModifiedImage and sets the current instance
 * as the result.
 *****************************************************************************/
void Image::edge()
{
    QImage *temp = new QImage(*unModifiedImage); //Copy of original image
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull() || temp->isNull())
    {
        qDebug() << "Image::edge -> Null reference";
        return;
    }

    //Author: John M. Weiss, Ph.D.
    //Class:  CSC421/521 GUI-OOP, Fall 2013.
    //Posted September 25, 2013.
    for ( int x = 1; x < temp->width() - 1; x++ )
    {
        for ( int y = 1; y < temp->height() - 1; y++ )
        {
            // pseudo-Prewitt edge magnitude
            int Gx = qGray( temp->pixel( x, y + 1 ) ) - qGray( temp->pixel( x, y - 1 ) );
            int Gy = qGray( temp->pixel( x + 1, y ) ) - qGray( temp->pixel( x - 1, y ) );
            int e = 3 * sqrt( double(Gx * Gx + Gy * Gy) );
            if ( e > 255 ) e = 255;
            image->setPixel( x, y, qRgb( e, e, e ) );
        }
    }

    //Sets the current image instance to the edged image
    this->convertFromImage(*image);
}

/**************************************************************************//**
 * @brief The unModifiedImage is embossed.  The result is stored in the current
 * instance;
 *****************************************************************************/
void Image::emboss()
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::emboss -> Null reference";
        return;
    }

    // 0  0  0
    // 0  1  0
    // 0  0 -1
    //Performs the above filter on all the RGB, then averages the RGB into
    //a grayscale using 30% Red, 59% Green, 11% Blue
    for ( int x = 0; x < image->width()-1; x++ )
    {
        for ( int y = 0; y < image->height()-1; y++ )
        {
            QRgb pixel = image->pixel( x, y );
            QRgb lowerPixel = image->pixel(x+1, y+1);

            int r = abs(qRed(pixel) - qRed(lowerPixel));
            int g = abs(qGreen(pixel) - qGreen(lowerPixel));
            int b = abs(qBlue(pixel) - qBlue(lowerPixel));
            int avg = r*0.3 + g*0.59 + b*0.11;
            image->setPixel( x, y, qRgb( avg, avg, avg ) );
        }
    }

    //Set the current instance to the embossed image
    this->convertFromImage(*image);
}

/**************************************************************************//**
 * @brief Performs the gamma function to the image.
 *
 *          (R/255)^(gamma) * 255 + 0.5
 *          (G/255)^(gamma) * 255 + 0.5
 *          (B/255)^(gamma) * 255 + 0.5
 *
 *  0.5 is rounding
 *
 * @param[in] gammaValue - Power to raise to
 *****************************************************************************/
void Image::gamma(double gammaValue)
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::gamma -> Null reference";
        return;
    }

    //Gamma of the image (as stated by function header)
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

    //Sets the current instance to the gamma image
    this->convertFromImage(*image);
}


/**************************************************************************//**
 * @brief Brightens (or darkens) based on the brightnessLevel
 *
 * @param[in] brightnessLevel - Number of pixels to add to each pixel
 *****************************************************************************/
void Image::brightness(int brightnessLevel)
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::brightness -> Null reference";
        return;
    }

    int nrows = image->width(), ncols = image->height(), r, c,
    lut[256];

    //Create a Look up table for brightnesses
    for(int i = 0; i < 256; i++ )
    {
        if( (i + brightnessLevel) > 255 )
            lut[i] = 255;
        else if(i + brightnessLevel < 0)
            lut[i] = 0;
        else
            lut[i] = i + brightnessLevel;
    }

    //Set the RGB values based on the look up table
    for( r = 0; r < nrows; r++ )
        for( c = 0; c < ncols; c++ )
        {
            QRgb pixel = image->pixel(r,c);
            int red = lut[qRed(pixel)];
            int green = lut[qGreen(pixel)];
            int blue = lut[qBlue(pixel)];
            image->setPixel(r, c, qRgb(red, green, blue));
        }

    //Sets the current instance to the brightened image
    this->convertFromImage(*image);
}


/**************************************************************************//**
 * @brief Sets pixels black or white.  If the pixel is less than the threshold,
 * the pixel is set black, otherwise it's set white.
 *
 * @param[in] threshold - The intensity level to check against
 *****************************************************************************/
void Image::binaryThreshold(int threshold)
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::binaryThreshold -> Null reference";
        return;
    }

    //Avoid corrupted input
    if(threshold < 0 || threshold > 255)
    {
        qDebug() << "Image::binaryThreshold -> Invalid parameter, expecting a value [0,255] instead got " << threshold;
    }

    int lut[256] = {0};

    //Create a simple lookup table
    for( int i = threshold; i < 256; i++ )
        lut[i] = 255;

    //Compair 30% Red 59% Green, 11% Blue value against the threshold
    //Set the value based on the lookup table
    for( int r=0; r < image->width(); r++)
        for( int c = 0; c < image->height(); c++)
        {
            QRgb pixel = image->pixel(r, c);
            int pixelValue = lut[(int)(qRed(pixel) * 0.3 +
                                       qGreen(pixel) * 0.59 +
                                       qBlue(pixel) * 0.11)];
            image->setPixel(r, c, qRgb( pixelValue, pixelValue, pixelValue));
        }

    //Set the current instance to the binary threshold image
    this->convertFromImage(*image);
}

/**************************************************************************//**
 * @brief Contrasts the image.  This function works semi-well.  It individually
 * changes the RGB values, which it should change the intensity, but it gets
 * the point across.  The contrast is changed by specifying a lower and upper
 * bound.  The image then maps the pixels to values within that range.
 *
 * @param[in] lower - Lowerbound of the intensity
 * @param[in] upper - Upperbound of the intensity
 *****************************************************************************/
void Image::contrast(int lower, int upper)
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::contrast -> Null reference";
        return;
    }

    if(lower >= upper)
    {
        qDebug() << "Image::contrast -> lower >= upper, aborting to prevent divide by 0";
        return;
    }

    for(int r = 0; r < int(image->width()); r++)
           for(int c = 0; c < int(image->height()); c++)
           {
               QRgb pixel = image->pixel(r, c);
               int red = qRed(pixel), green = qGreen(pixel), blue = qBlue(pixel);

               //Perform the contrast operation/formula
               red = (red - lower) * 256.0 / (upper - lower) + 0.5;
               green = (green - lower) * 256.0 / (upper - lower) + 0.5;
               blue = (blue - lower) * 256.0 / (upper - lower) + 0.5;

               //Handle values < 0 or > 255
               red = (red < lower) ? 0 : (red > upper) ? 255 : red;
               green = (green < lower) ? 0 : (green > upper) ? 255 : green;
               blue = (blue < lower) ? 0 : (blue > upper) ? 255 : blue;

               image->setPixel(r, c, qRgb(red, green, blue));
           }

    //Set the current instance to the contrasted image
    this->convertFromImage(*image);
}


/**************************************************************************//**
 * @brief The following function is a placeholder.  It is not implemented.
 *****************************************************************************/
void Image::balance(int brightness, int contrastLower, int contrastUpper, double gamma)
{
    qDebug() << "Image::balance -> Not Implemented";
}


/**************************************************************************//**
 * @brief Resizes the image to the desired width and height (in pixels)
 *
 * @param[in] width - Number of pixels wide
 * @param[in] height - Number of pixels tall
 *****************************************************************************/
void Image::imgResize(int width, int height)
{
    QImage *image = new QImage(*unModifiedImage);

    if(image->isNull())
    {
        qDebug() << "Image::imgResize -> Null reference";
        return;
    }

    //Set the current instance to the resized image
    image = new QImage(image->scaled(width, height));
    this->convertFromImage(*image);
}


/**************************************************************************//**
 * @brief Simple slot to save the current image as a backlog. (When OK is
 * pressed.)
 *****************************************************************************/
void Image::commit()
{
    delete unModifiedImage;
    unModifiedImage = new QImage(this->toImage());
}


/**************************************************************************//**
 * @brief Used to undo a recent change (when Cancel is pressed)
 *****************************************************************************/
void Image::revert()
{
    this->convertFromImage(*unModifiedImage);
}

