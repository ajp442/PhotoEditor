#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QDebug>

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

public slots:
    void commit();
    void revert();

private:
    QImage *unModifiedImage;
};

#endif// IMAGE_H
