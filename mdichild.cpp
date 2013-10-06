/**************************************************************************//**
 * @file
 *
 * @brief MdiChild implementation.
 *****************************************************************************/

/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "mdichild.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    modified = false;

}

MdiChild::~MdiChild()
{
}

/**************************************************************************//**
 * @brief Sets up an "blank" MdiChild.
 *
 * @todo Right now all it does is gives it a default name. It will error if
 * you try to save it since there is no pixmap.
 *****************************************************************************/
void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("img%1.png").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
    setModified();
}

/**************************************************************************//**
 * @brief Loads an image into the QPixmap and adds it to the QLabel.
 *
 * @param[in] fileName - The name of the file to load.
 *
 * @returns false - Empty filename or faild to load image.
 * @returns true - Successfully loaded image.
 *****************************************************************************/
bool MdiChild::loadFile(const QString &fileName)
{

    imageLabel = new QLabel;
    if (!fileName.isEmpty()) {
        image.load(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1").arg(fileName));
            return false;
        }

        imageLabel = new QLabel;
        imageLabel->setPixmap(image);
        setBackgroundRole(QPalette::Dark);
        setWidget(imageLabel);

        scaleFactor = 1.0;
        setCurrentFile(fileName);
        return true;
    }
    return false;
}

/**************************************************************************//**
 * @brief Saves the current image.
 *
 * @returns false - Unsucessful save.
 * @returns true - Successful save.
 *****************************************************************************/
bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    }
    else {
        return saveFile(curFile);
    }
    return false;
}


/**************************************************************************//**
 * @brief Prompts the user for a file location and name. Then saves.
 *
 * @returns false - Unsucessful save.
 * @returns true - Successful save.
 *****************************************************************************/
bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty()) {
        return false;
    }

    curFile = fileName;
    this->setCurrentFile(fileName);
    return saveFile(fileName);
}


/**************************************************************************//**
 * @brief Saves the current pixmap.
 *
 * @param[in] fileName - The name of the file to save.
 *
 * @returns false - Unsucessful save.
 * @returns true - Successful save.
 *****************************************************************************/
bool MdiChild::saveFile(const QString &fileName)
{
    if (!image.save(fileName)){
        QMessageBox::warning(this, tr("Photo Edit"), tr("Cannot write file %1").arg(fileName));
        return false;
    }
    setModified(false);
    return true;
}

/**************************************************************************//**
 * @brief Returns the stripped name of the file, used for display purposes.
 * @returns The stripped name of the file.
 *****************************************************************************/
QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}


/**************************************************************************//**
 * @brief Handles when a MdiChild window is closed. If nesicery it will
 * prompt the user if they want to save, discard, or cancle.
 *****************************************************************************/
void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

/**************************************************************************//**
 * @brief Prompts the user if they want to save, discard, or cancle.
 *
 * @returns true - Already saved, or saved successfuly.
 * @returns false - Cancle was selected.
 *****************************************************************************/
bool MdiChild::maybeSave()
{
    if (isModified()) {
    QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("MDI"),
                     tr("'%1' has been modified.\n"
                        "Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard
             | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }

    return true;
}


/**************************************************************************//**
 * @brief Sets the current file, and titles the window appropriately.
 *
 * @param[in] fileName - Name/path of the file.
 *****************************************************************************/
void MdiChild::setCurrentFile(const QString &fileName)
{
    //curFile = QFileInfo(fileName).canonicalFilePath();
    curFile = fileName;
    isUntitled = false;
    setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

/**************************************************************************//**
 * @brief Returns the stripped name of the file, used for display purposes.
 * @returns The stripped name of the file.
 *****************************************************************************/
QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


bool MdiChild::grayScale()
{
    qDebug() << "grayscale from child";
    QImage theImage = image.toImage();
    theImage.invertPixels();
    image.convertFromImage(theImage);
    imageLabel->setPixmap(image);
    setModified();
}

/**************************************************************************//**
 * @brief Sets the modified flag that indicates if the image has been modified.
 * @param[in] changed - A flag indicating if the document has been changed.
 *****************************************************************************/
void MdiChild::setModified(bool changed)
{
    modified = changed;
}

/**************************************************************************//**
 * @brief Returns whether or not the image has been modified.
 *
 * @returns true - The image has been modified.
 * @returns false - The image has not been modified.
 *****************************************************************************/
bool MdiChild::isModified()
{
    return modified;
}
