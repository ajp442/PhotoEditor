#include "dialog.h"

dialog::dialog(QString title)
{
    container = new QGroupBox();
    QVBoxLayout *containerLayout = new QVBoxLayout;

    titleLayout = new QGridLayout;
    bodyLayout = new QGridLayout;
    buttonLayout = new QGridLayout;

    _row = 0;

    containerLayout->addLayout(titleLayout);
    containerLayout->addLayout(bodyLayout);
    containerLayout->addLayout(buttonLayout);

    QLabel *titleLabel = new QLabel(title);
    QFont *titleFont = new QFont();
    titleFont->setPointSize(11);
    titleLabel->setFont(*titleFont);
    titleLayout->addWidget(titleLabel, 0, 0);

    QPushButton *okButton = new QPushButton(QObject::tr("OK"));
    QPushButton *cancelButton = new QPushButton(QObject::tr("Cancel"));

    buttonLayout->addWidget(okButton, 0, 0);
    buttonLayout->addWidget(cancelButton, 0, 1);
    buttonLayout->setAlignment(Qt::AlignRight);

    bodyLayout->setAlignment(Qt::AlignCenter);
    bodyLayout->setHorizontalSpacing(20);
    bodyLayout->setVerticalSpacing(10);
    bodyLayout->setMargin(10);

    container->setFixedSize(container->sizeHint());  //make the view not resizable
    container->setAlignment(Qt::AlignCenter);
    container->setLayout(containerLayout);
    container->setFocusPolicy(Qt::StrongFocus);

    container->show();
}

void dialog::addChild(QString label, double &baseValue, double min, double max)
{
    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    childSlider->setMinimumWidth(100);
    QDoubleSpinBox *childSpinBox = new QDoubleSpinBox;
    childSpinBox->setValue(baseValue);
    childSpinBox->setRange(min, max);

    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;
}

void dialog::addChild(QString label, int &baseValue, int min, int max)
{
    QLabel * childLabel = new QLabel(label);
    QSlider * childSlider = new QSlider(Qt::Horizontal);
    childSlider->setMinimumWidth(100);
    QSpinBox *childSpinBox = new QSpinBox;
    childSpinBox->setValue(baseValue);
    childSpinBox->setRange(min, max);

    bodyLayout->addWidget(childLabel, _row, 0);
    bodyLayout->addWidget(childSlider, _row, 1);
    bodyLayout->addWidget(childSpinBox, _row, 2);
    _row++;
}
