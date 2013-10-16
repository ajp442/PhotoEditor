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
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QPainter>

#include "mdichild.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    this->setAlignment(Qt::AlignCenter);
    isUntitled = true;
    modified = false;
    zoomable = true;
    areaSelected = false;

    pasteItemMoving = false;
    pasteRepositioning = false;

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    clipBoard = QApplication::clipboard();

    setDragMode(NoDrag);

    undoStack = new std::deque<QImage>(0);
    redoStack = new std::deque<QImage>(0);
}

MdiChild::~MdiChild()
{
    delete rubberBand;
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
 * @returns false - Empty filename or failed to load image.
 * @returns true - Successfully loaded image.
 *****************************************************************************/
bool MdiChild::loadFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        image.load(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1").arg(fileName));
            return false;
        }

        this->commitImageChanges();

        QGraphicsScene *scene = new QGraphicsScene;
        scene->setBackgroundBrush(QBrush(QColor(0,0,0,48)));
        pixmap = scene->addPixmap(image);
        this->setScene(scene);

        setCurrentFile(fileName);
        return true;
    }
    else
    {
        qDebug() << "in load";
        bool retVal;
        retVal = image.load(currentFile());
        commitImageChanges();
        pixmap->setPixmap(image);
        qDebug() << "retval:" << retVal;
        return retVal;
    }
    return false;
}

/**************************************************************************//**
 * @brief Saves the current image.
 *
 * @returns false - Unsuccessful save.
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
 * @returns false - Unsuccessful save.
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
 * @returns false - Unsuccessful save.
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
 * @brief Handles when a MdiChild window is closed. If necessary it will
 * prompt the user if they want to save, discard, or cancel.
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
 * @brief Prompts the user if they want to save, discard, or cancel.
 *
 * @returns true - Already saved, or saved successfully.
 * @returns false - Cancel was selected.
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

/**************************************************************************//**
 * @brief Sets the zoomable flag that indicates if the image can be zoomed.
 * @param[in] canZoom - A flag indicating if zooming should be set to
 * enabled. Default is true.
 *****************************************************************************/
void MdiChild::setZoomable(bool canZoom)
{
    zoomable = canZoom;
}

/**************************************************************************//**
 * @brief Returns whether or not zooming is enabled.
 *
 * @returns true - The image can be zoomed.
 * @returns false - The image has not be zoomed.
 *****************************************************************************/
bool MdiChild::isZoomable()
{
    return zoomable;
}

bool MdiChild::isAreaSelected()
{
    return areaSelected;
}

void MdiChild::setAreaSelected(bool value)
{
    areaSelected = value;

    if(!areaSelected)
        rubberBand->hide();

    emit areaSelectedChanged();
}

void MdiChild::setPasteRepositioning(bool value)
{
    pasteRepositioning = value;

    if(!pasteRepositioning)
        finalizePaste();
}

//-----------------------------------------------------------------------------
//                   Image Effects
//-----------------------------------------------------------------------------
void MdiChild::grayScale()
{
    image.grayscale();
    pixmap->setPixmap(image);
    setModified();
}


void MdiChild::sharpen()
{
    image.sharpen();
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::soften()
{
    image.soften();
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::negative()
{
    image.negative();
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::despeckle(int threshold)
{
    image.despeckle(threshold);
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::posterize()
{

}

void MdiChild::edge()
{
    image.edge();
    pixmap->setPixmap(image);
    setModified();
}




void MdiChild::emboss()
{
    image.emboss();
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::gamma(double gammaValue)
{
    image.gamma(gammaValue);
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::brightness(int brightnessLevel)
{
    image.brightness(brightnessLevel);
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::binaryThreshold(int threshold)
{
    image.binaryThreshold(threshold);
    pixmap->setPixmap(image);
    setModified();
}

void MdiChild::contrast(int lower, int upper)
{
    image.contrast(lower, upper);
    pixmap->setPixmap(image);
    setModified();
}
/*
void MdiChild::balance(int brightness, int contrastLower, int contrastUpper, double gamma)
{
    //image.brightness(brightness);
    //image.contrast(contrastLower, contrastUpper);
    //image.gamma(gamma);
    image.balance(brightness, contrastLower, contrastUpper, gamma);
    imageLabel->setPixmap(image);
    setModified();
}
*/

void MdiChild::imgResize(int width, int height)
{
    image.imgResize(width, height);
    //image.convertFromImage(image.scaled(width, height).toImage());
    pixmap->setPixmap(image);
    scene()->setSceneRect(pixmap->boundingRect());
    setModified();
}

void MdiChild::commitImageChanges()
{
    image.commit();
    redoStack->clear();
    undoStack->push_front(image.toImage());
    //prevent the stack from storing 'too much'
    if(undoStack->size() > 12)
    {
        qDebug() << "MdiChild -> undoStack getting too large, popping off oldest value";
        undoStack->pop_back();
    }

    //scene()->setSceneRect(image.rect());
}

void MdiChild::revertImageChanges()
{
    image.revert();
}

void MdiChild::resetRotation()
{
    this->resetTransform();
}

void MdiChild::undo()
{
    qDebug() << "Entered MdiChild::undo()";
    qDebug() << undoStack->size();
    if(undoStack->size() > 1)
    {
        qDebug() << "Popping off undoStack";
        undoStack->pop_front();
        redoStack->push_front(image.toImage());
        image.convertFromImage(undoStack->front());
        image.commit();
        pixmap->setPixmap(image);
        scene()->setSceneRect(pixmap->boundingRect());
    }
}

void MdiChild::redo()
{
        qDebug() << "Entered MdiChild::redo()";
    if(!redoStack->empty())
    {
        qDebug() << "Popping off redoStack";
        undoStack->push_front(image.toImage());
        image.convertFromImage(redoStack->front());
        image.commit();
        redoStack->pop_front();
        pixmap->setPixmap(image);
        scene()->setSceneRect(pixmap->boundingRect());
    }
}

//-----------------------------------------------------------------------------
//                   Copy / Paste
//-----------------------------------------------------------------------------
void MdiChild::copy()
{
    if(areaSelected){
        QImage copyImage = image.copy(QRect(origin - this->mapFromScene(0,0), endPoint - this->mapFromScene(0,0))).toImage();
        clipBoard->setImage(copyImage);
    }

    setAreaSelected(false);
}

void MdiChild::paste()
{
    Image * clipImage = new Image();
    clipImage->convertFromImage(clipBoard->image());

    if(!clipImage->isNull())
    {
        pasteItem = scene()->addPixmap(*clipImage);
        pasteItem->setFlag(QGraphicsItem::ItemIsMovable);
        setModified();
        setPasteRepositioning(true);
    }
}

void MdiChild::finalizePaste()
{
    QPoint pasteOrigin = pasteItem->mapToScene(0,0).toPoint();
    QSize pasteSize = pasteItem->boundingRect().size().toSize();

    QRect pasteRect = QRect(pasteOrigin, pasteSize);

    QPainter painter(&image);

    painter.drawTiledPixmap(pasteRect, pasteItem->pixmap(), QPoint(0,0));

    scene()->removeItem(pasteItem);

    pixmap->setPixmap(image);

    pasteItemMoving = false;
    pasteRepositioning = false;
}

void MdiChild::crop()
{
    if(areaSelected)
    {
        image.convertFromImage(image.copy(QRect(origin, endPoint)).toImage());
        pixmap->setPixmap(image);
        scene()->setSceneRect(pixmap->boundingRect());
        setModified();
    }

    setAreaSelected(false);
}

bool MdiChild::undoEnabled()
{
    if(undoStack->size() > 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MdiChild::redoEnabled()
{
    return !redoStack->empty();
}


//-----------------------------------------------------------------------------
//                   Re-implemented Functions
//-----------------------------------------------------------------------------

/**************************************************************************//**
 * @brief Re-implementing wheel event so we can zoom instead of scroll.
 *****************************************************************************/
void MdiChild::wheelEvent(QWheelEvent* event) {

    if(isZoomable())
    {
        //Zoom where the cursor is.
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        // Scale the view / do the zoom
        double scaleFactor = 1.15;
        if(event->delta() > 0) {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        } else {
            // Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }

        emit zoomChanged();
        // Don't call superclass handler here
        // as wheel is normally used for moving scrollbars
    }
    else
    {
        event->setAccepted(false);
        QGraphicsView::wheelEvent(event);
    }

}

/**************************************************************************//**
 * @brief Starts rubberbanding action, then completes the base mousePressEvent.
 *****************************************************************************/
void MdiChild::mousePressEvent(QMouseEvent *event)
{
    if(!pasteRepositioning)
    {
        rubberBand->hide();
        setAreaSelected(false);

        origin = event->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
    }

    QGraphicsView::mousePressEvent(event);
}

/**************************************************************************//**
 * @brief Resizes the rubberband from the start of the press to the current
 * mous position, then completes the base mouseMoveEvent.
 *****************************************************************************/
void MdiChild::mouseMoveEvent(QMouseEvent *event)
{
    if(!pasteRepositioning)
    {
        endPoint = event->pos();
        rubberBand->setGeometry(QRect(origin, endPoint));

        setAreaSelected(true);
    }

    else if(pasteRepositioning && !pasteItemMoving)
    {
        QPoint pasteOrigin = this->mapFromScene(pasteItem->mapToScene(0,0));
        QRect pasteRect = QRect(pasteOrigin, pasteItem->boundingRect().size().toSize());

        if(pasteRect.contains(event->pos()))
        {
            pasteItemMoving = true;
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

/**************************************************************************//**
 * @brief Finishes tracking the rubberband, then completes the base
 * mouseReleaseEvent.
 *****************************************************************************/
void MdiChild::mouseReleaseEvent(QMouseEvent *event)
{
    if(pasteRepositioning && !pasteItemMoving)
        finalizePaste();

    pasteItemMoving = false;

    QGraphicsView::mouseReleaseEvent(event);
}

/**************************************************************************//**
 * @brief Re-implementing so we can fit the image in our view if zoomable
 * is set to false.
 *****************************************************************************/
void MdiChild::resizeEvent(QResizeEvent *event)
{
    if(!isZoomable())
    {
        fitInView(scene()->itemsBoundingRect());
    }
    QGraphicsView::resizeEvent(event);
}

