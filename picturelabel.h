#ifndef PICTURELABEL_H
#define PICTURELABEL_H

#include <QLabel>
#include <QRect>
#include <QMouseEvent>

class PictureLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PictureLabel(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:

public slots:

private:
    bool TrackingRectangle;
    bool DrawRectangle;

    QRect Rectangle;
};

#endif // PICTURELABEL_H
