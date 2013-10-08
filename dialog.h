#ifndef DIALOG_H
#define DIALOG_H

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QPushButton>

class dialog
{
public:
    explicit dialog(QString title);
    void addChild(QString label, int &baseValue, int min, int max);
    void addChild(QString label, double &baseValue, double min, double max);
signals:
    
public slots:
    
private:
    QGroupBox *container;
    QGridLayout *titleLayout;
    QGridLayout *bodyLayout;
    QGridLayout *buttonLayout;
    int _row;
};

#endif // DIALOG_H
