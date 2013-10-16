#include "dialog.h"

/**************************************************************************//**
 * @brief Constructor for Dialog class.  Creates two gridLayouts, one for the
 * buttons, the other for the main body.  The main body is built as addChild()
 * is called.
 *
 * @param[in] title - The name of the modal popup window
 *****************************************************************************/
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
    QObject::connect(okButton, SIGNAL(clicked()), container, SLOT(accept()));
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


/**************************************************************************//**
 * @brief Creates a row in the dialog containing a label, slider, and spinBox
 * based on double values.
 *
 * @param[in] label - Name of the label on the row
 * @param[in] baseValue - Initial value to start the slider/spinBox at
 * @param[in] min - Minimum value of the slider/spinBox
 * @param[in] max - Maximum value of the slider/spinBox
 *****************************************************************************/
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

    //Add the label, slider, and spinBox the the dialog's layout
    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;

    //Add the initial value to the private vector member
    currentValues.push_back(baseValue);
}


/**************************************************************************//**
 * @brief Creates a row in the dialog containing a left label, a slider, and a
 * spinBox based on integers.
 *
 * @param[in] label - Name of the label on the row
 * @param[in] baseValue - Initial value to start the slider/spinBox at
 * @param[in] min - Minimum value of the slider/spinBox
 * @param[in] max - Maximum value of the slider/spinBox
 *****************************************************************************/
void dialog::addChild(QString label, int baseValue, int min, int max)
{
    //Initialize the three widgets for the row of the dialog
    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    QSpinBox *childSpinBox = new QSpinBox;

    //Set the range of the two widgets
    childSlider->setRange(min, max);
    childSpinBox->setRange(min, max);

    //Cause the slider and spinBox to update each other
    QObject::connect(childSpinBox, SIGNAL(valueChanged(int)),childSlider,SLOT(setValue(int)) );
    QObject::connect(childSlider,SIGNAL(valueChanged(int)),childSpinBox,SLOT(setValue(int)) );

    //must be placed after spinBox -> slider slot/signal
    childSpinBox->setValue(baseValue);

    //When the spinBox changes, update the currentValues private vector member
    connect(childSpinBox, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
    signalMapper->setMapping(childSpinBox, childSpinBox);
    QObject::connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(returnWidget(QWidget*)));

    //Add the label, slider, and spinBox to the dialog's layout
    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;

    //Add the initial value to the private vector member
    currentValues.push_back(baseValue);
}


/**************************************************************************//**
 * @brief A slot that contains the spinBox that was changed.  This method updates
 * the currentValues vector based on the widget changed.
 *
 * @param[in] object - The spinBox of the row changed
 *****************************************************************************/
void dialog::returnWidget(QWidget *object)
{
    QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(object);
    QSpinBox *integerSpinBox = qobject_cast<QSpinBox*>(object);

    //If the object was converted successfully to a QDoubleSpinBox
    if(doubleSpinBox)
    {
        for(int currentRow = 0; currentRow < _row; currentRow++)
        {
            //Update the currentValues based on the row it was located at
            if(object == bodyLayout->itemAtPosition(currentRow, 2)->widget())
            {
                currentValues[currentRow] = doubleSpinBox->value();
            }
        }
    }
    else if(integerSpinBox)
    {
        for(int currentRow = 0; currentRow < _row; currentRow++)
        {
            //Update the currentValues based on the row it was located at
            if(object == bodyLayout->itemAtPosition(currentRow, 2)->widget())
            {
                currentValues[currentRow] = integerSpinBox->value();
            }
        }
    }

    //Dynamically updates the image as the dialog changes
    emit valueChanged(currentValues);
}



/**************************************************************************//**
 * @brief A simple slot that simply emits the vector of doubles signal.
 *****************************************************************************/
void dialog::emitValueChanged()
{
    emit valueChanged(currentValues);
}


/**************************************************************************//**
 * @brief A simple slot that emits the value changes on an integer slider.  The
 * slider's value is 100 times the 'actual' value.  This slot converts the value
 * down to the double for QDoubleSpinBox.
 *****************************************************************************/
void dialog::reemitSliderValueChanged(int value)
{
    emit sliderValueChanged(value/100.0);
}


/**************************************************************************//**
 * @brief A simple slot that emits the values changed on a QDoubleSpinBox.  The
 * spin box's value is multiplied by 100 and passed to the slider.  (Since the
 * slider is an integer slider.
 *****************************************************************************/
void dialog::reemitDoubleSpinBoxValueChanged(double value)
{
    emit doubleSpinBoxValueChanged(value*100.0);
}
