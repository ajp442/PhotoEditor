#include "dialog.h"

dialog::dialog(QString title)
{
    container = new QDialog;//new QGroupBox();
    container->setModal(true);
    container->setWindowTitle(title);

    QVBoxLayout *containerLayout = new QVBoxLayout;

    //titleLayout = new QGridLayout;
    bodyLayout = new QGridLayout;
    buttonLayout = new QGridLayout;

    currentValues = std::vector<double>(0);
    _row = 0;

    //containerLayout->addLayout(titleLayout);
    containerLayout->addLayout(bodyLayout);
    containerLayout->addLayout(buttonLayout);

    //QLabel *titleLabel = new QLabel(title);
    //QFont *titleFont = new QFont();
    //titleFont->setPointSize(11);
    //titleLabel->setFont(*titleFont);
    //titleLayout->addWidget(titleLabel, 0, 0);

    okButton = new QPushButton(QObject::tr("OK"));
    QPushButton *cancelButton = new QPushButton(QObject::tr("Cancel"));

    QObject::connect(cancelButton, SIGNAL(clicked()), container, SLOT(close()));
    QObject::connect(okButton, SIGNAL(clicked()), container, SLOT(close()));

    buttonLayout->addWidget(okButton, 0, 0);
    buttonLayout->addWidget(cancelButton, 0, 1);
    buttonLayout->setAlignment(Qt::AlignRight);

    bodyLayout->setAlignment(Qt::AlignCenter);
    bodyLayout->setHorizontalSpacing(20);
    bodyLayout->setVerticalSpacing(10);
    bodyLayout->setMargin(10);

    containerLayout->setSizeConstraint( QLayout::SetFixedSize );

    container->setLayout(containerLayout);
    container->setFocusPolicy(Qt::StrongFocus);  //allows clicking and tabs to give the dialog focus

    container->show();


    signalMapper = new QSignalMapper;
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(emitValueChanged()));
}

void dialog::addChild(QString label, double baseValue, double min, double max)
{
    currentValues.push_back(baseValue);

    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    childSlider->setMinimumWidth(100);
    childSlider->setTickInterval(10);
    childSlider->setRange(min*100, max*100);
    childSlider->setValue(baseValue*100);  //the slider connect assumes the value was multiplied by 100 (since it's an int slider)
    QDoubleSpinBox *childSpinBox = new QDoubleSpinBox;
    childSpinBox->setValue(baseValue);
    childSpinBox->setRange(min, max);

    //When the spinBox changes, change the slider
    QObject::connect(childSpinBox, SIGNAL(valueChanged(double)), this, SLOT(reemitDoubleSpinBoxValueChanged(double)) );
    connect(this, SIGNAL(doubleSpinBoxValueChanged(int)), childSlider, SLOT(setValue(int)));

    //When the slider changes, change the spinBox
    QObject::connect(childSlider,SIGNAL(valueChanged(int)),this,SLOT(reemitSliderValueChanged(int)) );
    connect(this, SIGNAL(sliderValueChanged(double)), childSpinBox, SLOT(setValue(double)));

    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;


    connect(childSpinBox, SIGNAL(valueChanged(double)), signalMapper, SLOT(map()));

    signalMapper->setMapping(childSpinBox, childSpinBox);
    QObject::connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(returnWidget(QWidget*)));


}

void dialog::addChild(QString label, int baseValue, int min, int max)
{
    currentValues.push_back(baseValue);
    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    childSlider->setMinimumWidth(100);
    childSlider->setRange(min, max);
    childSlider->setValue(baseValue);
    QSpinBox *childSpinBox = new QSpinBox;
    childSpinBox->setValue(baseValue);
    childSpinBox->setRange(min, max);

    QObject::connect(childSpinBox, SIGNAL(valueChanged(int)),childSlider,SLOT(setValue(int)) );
    QObject::connect(childSlider,SIGNAL(valueChanged(int)),childSpinBox,SLOT(setValue(int)) );

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
