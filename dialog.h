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
    explicit dialog(QString title);  //Title -> name of the window

    //Add integer or double rows
    void addChild(QString label, int baseValue, int min, int max);
    void addChild(QString label, double baseValue, double min, double max);

signals:
    //Everytime a widget changes value, emit all of the dialog values
    void valueChanged(const std::vector<double> &values);

    void cancelled();  //Cancel or [X]
    void accepted();  //OK

    //Update the integer slider linked with the double spinBox
    void sliderValueChanged(double);
    void doubleSpinBoxValueChanged(int);

private slots:
    void emitValueChanged();  //Handles passing out the correct params for valueChanged
    void returnWidget(QWidget *object);  //object -> spinBox that changes

    //Conversions from int slider to/from double spinBox
    void reemitSliderValueChanged(int);
    void reemitDoubleSpinBoxValueChanged(double);

private:
    //The dialog that displays (we used a private member rather than inheriting
    //from QDialog, becuase we didn't want the user to all that functionality
    QDialog *container;

    //Holds all of the widgets in grids
    QGridLayout *bodyLayout;  //Each row of label, slider, and spinBox
    QGridLayout *buttonLayout; //The OK and Cancel buttons

    int _row; //Current row the dialog is about to insert into
    QSignalMapper *signalMapper; //Handles mapping widgets and int/double conversions

    //List of values displayed in the dialog
    //Doubles were used since the only values stored in the dialog are
    //Integers or doubles
    std::vector<double> currentValues;
};

#endif // DIALOG_H
