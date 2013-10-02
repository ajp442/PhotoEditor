#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QFileDialog>
#include <QComboBox>
#include <QScrollArea>
#include <QMdiSubWindow>
#include "image.h"
#include <QRgb>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mdiArea = new QMdiArea;
    label = new QLabel;
   // setCentralWidget(mdiArea);
    //mdiArea->
    //QComboBox *comboBox = new QComboBox;
    //comboBox->addItem("X");
    //
    //comboBox->addItem(tr("Hello"));
    //ui->mainToolBar->addWidget(comboBox);
    ui->setupUi(this);
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************************************************************//**
 * @author Josh Schultz
 *
 * @par Description:
 * This function converts a color image into a gray monochrome image.
 *
 * @param[in][out] image - variable that contains the image
 * to be manipulated.
 *
 * @returns true - functioned sucessfully.
  *******************************************************************/
bool Grayscale(QImage &image)
{
    if( image.isNull() )
        return false;
    int gray = 0;
    int nrows = image.width(), ncols = image.height();
    for( int r=0; r < nrows; r++)
        for( int c = 0; c < ncols; c++)
        {
            //get overall monochrome intesity value by using
            //the grayscale method that takes into account human
            //eye sensitivity to light.
            int blue = qBlue(image.pixel(r,c)) * 0.11 + 0.5;
            int green = qGreen(image.pixel(r,c)) * 0.59 + 0.5;
            int red = qRed(image.pixel(r,c)) * 0.3 + 0.5;
            gray = red + blue + green;
            QRgb rgbValue = qRgb(gray, gray, gray);
            image.setPixel(r,c, rgbValue);
        }
    return true;
}


/*****************************************************************//*
 * @author Josh Schultz
 *
 * @par Description: Computes the negative of an image.
 *
 * @param[in/out] Image &image- The image to be manipulated.
 *
 * @returns true- The image negative was successfully computed.
 *          false- The function ended prematurely, due to the image
 *                 being null.
 *******************************************************************/
bool Negate(QImage &image)
{
    //If there is no image selected, exit the function.
    //This could prevent crashes when trying to reference an
    //image that doesn't exist and also saves time by eliminating
    //computations.
    if(image.isNull())
        return false;

    image.invertPixels();
    return true;
}

/*****************************************************************//*
 * @author Josh Schultz
 *
 * @par Description: Converts an image into a smaller range of
 *                   variations.  (Posterizes the image)
 *
 * @param[in/out] Image &image - The image to be manipulated.
 * @param[in]     int levels - The number of intensity levels
 *
 * @returns true  - The image negative was successfully computed.
 *          false - The function ended prematurely, due to an error.
 *******************************************************************/
bool Posterize(QImage &image, int levels = 8)
{
    int lut[256];    //Look up table

    //If there is no image selected, exit the function.
    //This could prevent crashes when trying to reference an
    //image that doesn't exist and also saves time by eliminating
    //computations.
    if(image.isNull())
        return false;

    //Invalid posterization ranges exits the function
    if (levels < 1 || levels > 255)
        return false;

    //Catches a divide by zero error later in the function.
    //This will convert all pixels to black using the RGB color scheme.
    if (levels == 1)
    {
        for(int r = 0; r < int(image.width()); r++)
            for(int c = 0; c < int(image.height()); c++)
                image.setPixel(r, c, 0);

        //Exit the function successfully
        return true;
    }

    //Generate the look up table
    for(int i = 0; i < 256; i++)
        lut[i] = (int( i / (256.0 / levels)) * (256.0 / (levels-1) ) );

    //Progress through every single pixel starting at the top left
    //and reading left to right, top on down
    //Adjust each pixel's RGB to posterize based off the user input
    for(int r = 0; r < int(image.width()); r++)
        for(int c = 0; c < int(image.height()); c++)
        {
            QRgb pixel = image.pixel(r,c);
            int red = lut[qRed(pixel)];
            int green = lut[qGreen(pixel)];
            int blue = lut[qBlue(pixel)];
            image.setPixel(r,c,qRgb(red, green, blue));
        }

    //Exit the function successfully
    return true;
}


/****************************************************************//**
 * @author Josh Schultz
 *
 * @par Description:
 * This is the function adjusts the image intensity by a value
 *[-255, 255]
 *
 * @param[in][out] image - variable that contains the image
 * to be manipulated.
 *
 * @returns true - Functioned sucessfully.
 * @returns false - Image was null
  *******************************************************************/
bool AdjustBrightness( QImage &image, int brightnessLevel = 25)
{
    if( image.isNull() )
        return false;

    int nrows = image.width(), ncols = image.height(), r, c,
    lut[256];

    for(int i = 0; i < 256; i++ )
    {
        if( (i + brightnessLevel) > 255 )
            lut[i] = 255;
        else if(i + brightnessLevel < 0)
            lut[i] = 0;
        else
            lut[i] = i + brightnessLevel;
    }

    for( r = 0; r < nrows; r++ )
        for( c = 0; c < ncols; c++ )
        {
            QRgb pixel = image.pixel(r,c);
            int red = lut[qRed(pixel)];
            int green = lut[qGreen(pixel)];
            int blue = lut[qBlue(pixel)];
            image.setPixel(r,c,qRgb(red, green, blue));
        }
    return true;
}


/*****************************************************************//*
 * @author Josh Schultz
 *
 * @par Description:  Adjusts the contrast of an image based on a
 *                    lower and upper bound.
 *
 * @param[in] Image &image - The image to be manipulated.
 * @param[in] int lower - The lowest value the contrast can be
 * @param[in] int upper - The highest value the contrast can be
 *
 * @returns true  - The image contrast was successfully computed.
 *          false - The function end prematurely, due to an error.
 *******************************************************************/
bool Contrast(QImage &image, int lower = 0, int upper = 255)
{
    //If there is no image selected, exit the function.
    //This could prevent crashes when trying to reference an
    //image that doesn't exist and also saves time by eliminating
    //computations.
    if(image.isNull())
        return false;

    if(upper < lower)
        return false;

    //Progress through every single pixel starting at the top left
    //and reading left to right, top on down
    //Change each pixel's brightness based off the contrast transformation
    //function
    for(int r = 0; r < int(image.width()); r++)
        for(int c = 0; c < int(image.height()); c++)
        {
           QRgb pixel = image.pixel(r,c);
           int red = (qRed(pixel) - lower) * 256.0 / (upper - lower) + 0.5;
           int green = (qGreen(pixel) - lower) * 256.0 / (upper - lower) + 0.5;
           int blue = (qBlue(pixel) - lower) * 256.0 / (upper - lower) + 0.5;

           //Verify rgb is within 0 to 255
           red = (red < 0) ? 0 : (red > 255) ? 255 : red;
           green = (green < 0) ? 0 : (green > 255) ? 255 : green;
           blue = (blue < 0) ? 0 : (blue > 255) ? 255 : blue;

           image.setPixel(r,c, qRgb(red, green, blue));
        }

    //Exit the function successfully
    return true;
}


//I'm using this function to test various image processing functions.
QPixmap ManipulateImage(QPixmap &pixmap)
{
    QImage qimage = pixmap.toImage();
    //Grayscale2(qimage);
    //Negate(qimage);
    //Posterize(qimage);
    //AdjustBrightness(qimage);
    Contrast(qimage, 40, 100);
    return QPixmap::fromImage(qimage);
}

void MainWindow::OpenFile()

{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                    tr("Files (*.*)"));
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(fileName));
    //label->setPixmap(ManipulateImage(QPixmap(fileName)));
    label->show();
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(label);
    QMdiSubWindow *mdiSubWindow = new QMdiSubWindow;
    mdiSubWindow->setWidget(scrollArea);
    mdiArea->addSubWindow(mdiSubWindow);
    //mdiArea->subWindowActivated(mdiSubWindow);
   setCentralWidget(mdiArea);

}


