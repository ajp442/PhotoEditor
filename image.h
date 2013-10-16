#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QDebug>
#include <QPixmapCache>
#include <QObject>
#include <math.h>

class Image :public QObject, public QPixmap
{
    Q_OBJECT
public:
    Image();
    //Load in the image
    bool load( const QString & fileName, const char * format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor );

    //Image effects
    void grayscale();
    void sharpen();
    void soften();
    void negative();
    void despeckle(int threshold);
    void posterize();
    void edge();
    void emboss();
    void gamma(double gammaValue);
    void brightness(int brightnessLevel);
    void binaryThreshold(int threshold);
    void contrast(int lower, int upper);
    void imgResize(int width, int height);
    void balance(int brightness, int contrastLower, int contrastUpper, double gamma);

public slots:
    void commit();  //Commits the image change
    void revert();  //Reverts the current image back to unModifiedImage

private:
    //Stores the original image until commit() is called
    //Used to dynamically update the images
    QImage *unModifiedImage;
};

#endif// IMAGE_H
