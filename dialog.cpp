#include "dialog.h"

dialog::dialog(QString title)
{
    //Initialize private members
    container = new QDialog;
    bodyLayout = new QGridLayout;
    buttonLayout = new QGridLayout;
    currentValues = std::vector<double>(0);
    signalMapper = new QSignalMapper;
    _row = 0;

    //Create the OK and Cancel buttons
    QPushButton *okButton = new QPushButton(QObject::tr("OK"));
    QPushButton *cancelButton = new QPushButton(QObject::tr("Cancel"));

    //When Cancel or [X] is pressed, close the dialog and emit cancelled()
    QObject::connect(cancelButton, SIGNAL(clicked()), container, SLOT(close()));
    connect(cancelButton, SIGNAL(clicked()), this, SIGNAL(cancelled()));
    connect(container, SIGNAL(rejected()), this, SIGNAL(cancelled()));

    //When OK is pressed, close the dialog and emit accepted()
    QObject::connect(okButton, SIGNAL(clicked()), container, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SIGNAL(accepted()));

    //Add the buttons with a right alignment
    buttonLayout->addWidget(okButton, 0, 0);
    buttonLayout->addWidget(cancelButton, 0, 1);
    buttonLayout->setAlignment(Qt::AlignRight);

    //Set the spacing and margins for the bodyLayout
    bodyLayout->setAlignment(Qt::AlignCenter);
    bodyLayout->setHorizontalSpacing(20);
    bodyLayout->setVerticalSpacing(10);
    bodyLayout->setMargin(10);

    //Add both of the layouts into a vertical box layout
    //Make the dialog a fixed size (can't click a corner and drag it out)
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->addLayout(bodyLayout);
    containerLayout->addLayout(buttonLayout);
    containerLayout->setSizeConstraint( QLayout::SetFixedSize );

    //Add the layout to the modal dialog
    container->setLayout(containerLayout);
    container->setFocusPolicy(Qt::StrongFocus);  //allows clicking and tabs to give the dialog focus
    container->setModal(true);
    container->setWindowTitle(title);
    container->show();
}

void dialog::addChild(QString label, double baseValue, double min, double max)
{
    //Create a label, horizontal slider, and spinBox
    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    QDoubleSpinBox *childSpinBox = new QDoubleSpinBox;

    //The slider uses integers, to make it work with a doubleSpinBox, the values
    //are multiplied by 100 to give the illusion it is a doubleSlider
    childSlider->setRange(min*100, max*100);

    //Prevent the user from typing in values below/above the min/max
    childSpinBox->setRange(min, max);

    //When the spinBox changes, change the slider
    QObject::connect(childSpinBox, SIGNAL(valueChanged(double)), this, SLOT(reemitDoubleSpinBoxValueChanged(double)) );
    connect(this, SIGNAL(doubleSpinBoxValueChanged(int)), childSlider, SLOT(setValue(int)));

    //When the slider changes, change the spinBox
    QObject::connect(childSlider,SIGNAL(valueChanged(int)),this,SLOT(reemitSliderValueChanged(int)) );
    connect(this, SIGNAL(sliderValueChanged(double)), childSpinBox, SLOT(setValue(double)));

    //Must be placed after spinBox -> slider slot/signal
    childSpinBox->setValue(baseValue);

    //When the slider/spinBox changes, update the vector of parameters
    //The spinBox widget is passed in because the slot needs to somehow determine
    //which parameter updated
    connect(childSpinBox, SIGNAL(valueChanged(double)), signalMapper, SLOT(map()));
    signalMapper->setMapping(childSpinBox, childSpinBox);
    QObject::connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(returnWidget(QWidget*)));

    currentValues.push_back(baseValue);
    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;
}

void dialog::addChild(QString label, int baseValue, int min, int max)
{
    currentValues.push_back(baseValue);
    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    QSpinBox *childSpinBox = new QSpinBox;

    childSlider->setRange(min, max);
    childSpinBox->setRange(min, max);

    QObject::connect(childSpinBox, SIGNAL(valueChanged(int)),childSlider,SLOT(setValue(int)) );
    QObject::connect(childSlider,SIGNAL(valueChanged(int)),childSpinBox,SLOT(setValue(int)) );

    //must be placed after spinBox -> slider slot/signal
    childSpinBox->setValue(baseValue);

    connect(childSpinBox, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
    signalMapper->setMapping(childSpinBox, childSpinBox);
    QObject::connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(returnWidget(QWidget*)));


    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;
}

void dialog::returnWidget(QWidget *object)
{
    QDoubleSpinBox *spinster;
    QSpinBox *spinster2;
    spinster = qobject_cast<QDoubleSpinBox*>(object);
    spinster2 = qobject_cast<QSpinBox*>(object);
    if(spinster)
    {
        for(int currentRow = 0; currentRow < _row; currentRow++)
        {
            if(object == bodyLayout->itemAtPosition(currentRow, 2)->widget())
            {
                currentValues[currentRow] = spinster->value();
            }
        }
    }
    else if(spinster2)
    {
        for(int currentRow = 0; currentRow < _row; currentRow++)
        {
            if(object == bodyLayout->itemAtPosition(currentRow, 2)->widget())
            {
                currentValues[currentRow] = spinster2->value();
            }
        }
    }

    //Uncomment this if we want to try dynamically updating the image
    emit valueChanged(currentValues);
}




void dialog::emitValueChanged()
{
    emit valueChanged(currentValues);
}

void dialog::reemitSliderValueChanged(int value)
{
    emit sliderValueChanged(value/100.0);
}

void dialog::reemitDoubleSpinBoxValueChanged(double value)
{
    emit doubleSpinBoxValueChanged(value*100.0);
}
