#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>

class Image : public QPixmap
{
    //Q_OBJECT
public:
//    explicit Image(QObject *parent = 0);

    explicit Image();
    void commit();
    void revert();
    bool load( const QString & fileName, const char * format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor );

    void grayscale();
    void sharpen();
    void soften();
    void negative();
    void despeckle();
    void posterize();
    void edge();
    void emboss();
    void brightness(int brightnessLevel);


signals:

public slots:
private:
    QImage *unModifiedImage;
};

#endif// IMAGE_H
