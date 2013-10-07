#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>

class Image : public QPixmap
{
    //Q_OBJECT
public:
//    explicit Image(QObject *parent = 0);

    explicit Image();

    void grayscale();
    void sharpen();
    void soften();
    void negative();
    void despeckle();
    void posterize();
    void edge();
    void emboss();


signals:

public slots:

};

#endif// IMAGE_H
