#ifndef DIALOG_H
#define DIALOG_H

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSignalMapper>
#include <sstream>
#include <vector>
#include <QDebug>
#include <QDialog>

class dialog : public QObject
{
    Q_OBJECT
public:
    explicit dialog(QString title);
    void addChild(QString label, int baseValue, int min, int max);
    void addChild(QString label, double baseValue, double min, double max);
signals:
    void valueChanged(const std::vector<double> &values);
    void cancelled();
    void accepted();
    void sliderValueChanged(double);
    void doubleSpinBoxValueChanged(int);
private slots:
    void emitValueChanged();
    void returnWidget(QWidget *object);
    void reemitSliderValueChanged(int);
    void reemitDoubleSpinBoxValueChanged(double);
private:
    QDialog *container;
    QGridLayout *bodyLayout;
    QGridLayout *buttonLayout;
    int _row;
    QSignalMapper *signalMapper;
    std::vector<double> currentValues;
};

#endif // DIALOG_H
