#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QDebug>

class Image :public QObject, public QPixmap
{
    Q_OBJECT
public:
    Image();

    //static QImage *average(const QImage *image1, const QImage *image2, const QImage *image3);

    //Load in the image
    bool load( const QString & fileName, const char * format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor );

    //Image effects
    void grayscale(QImage *image = NULL);
    void sharpen(QImage *image = NULL);
    void soften(QImage *image = NULL);
    void negative(QImage *image = NULL);
    void despeckle(int threshold, QImage *image = NULL);
    void posterize(QImage *image = NULL);
    void edge(QImage *image = NULL);
    void emboss(QImage *image = NULL);
    void gamma(double gammaValue, QImage *image = NULL);
    void brightness(int brightnessLevel, QImage *image = NULL);
    void binaryThreshold(int threshold, QImage *image = NULL);
    void contrast(int lower, int upper, QImage *image = NULL);
    //void balance(int brightness, int contrastLower, int contrastUpper, double gamma);
signals:
    void finished();
public slots:
    void commit();
    void revert();

private:
    QImage *unModifiedImage;
};

#endif// IMAGE_H
