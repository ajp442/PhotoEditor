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
    void valueChanged(std::vector<double> params);
    void sliderValueChanged(double);
    void doubleSpinBoxValueChanged(int);
public slots:
    void emitValueChanged();
    void returnWidget(QWidget *object);
    void reemitSliderValueChanged(int);
    void reemitDoubleSpinBoxValueChanged(double);
private:
    //QGroupBox *container;
    QDialog *container;
    //QGridLayout *titleLayout;
    QGridLayout *bodyLayout;
    QGridLayout *buttonLayout;
    QPushButton *okButton;
    int _row;
    QSignalMapper *signalMapper;
    std::vector<double> currentValues;
};

//This needs to be added to mainWindow eventually
/*
connect(softenDialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(customVectorSlot(std::vector<double>)));

void MainWindow::customVectorSlot(std::vector<double> vec)
{
    //stringstream ss;
    for(int i = 0; i < vec.size(); i++)
    {
        qDebug() << vec[i];
    }
}
*/

#endif // DIALOG_H
