/**************************************************************************//**
 * @file
 *
 * @brief MdiChild class is used to create objects that act subwindows
 * for the QMdiArea.
 *
 * This class creates objects which can save and load QPixmaps onto a QLabel.
 * Objects derived from this class can also perform basic image manipulation.
 *
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

#ifndef MDICHILD_H
#define MDICHILD_H

#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QRubberBand>
#include <deque>
#include <QGraphicsPixmapItem>
#include <QRectF>
#include "image.h"

class MdiChild : public QGraphicsView
{
    Q_OBJECT

public:
    MdiChild();
    ~MdiChild();

    void newFile();
    bool loadFile(const QString &fileName = "");
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }
    void setModified(bool changed = true);
    bool isModified();
    void setZoomable(bool canZoom = true);
    bool isZoomable();
    bool isAreaSelected();


    //Image Effects
    void grayScale();
    void sharpen();
    void soften();
    void negative();
    void despeckle(int threshold);
    void posterize();
    void edge();
    void emboss();
    void gamma(double gammaValue);
    void brightness(int brightnessLevel);
    void binaryThreshold(int threshold);
    void contrast(int lower, int upper);
    void imgResize(int width, int height);

    void copy();
    void cut();
    void paste();
    void crop();

    //void balance(int brightness, int contrastLower, int contrastUpper, double gamma);


    void undo();
    void redo();
    bool undoEnabled();
    bool redoEnabled();

public slots:
    void commitImageChanges();
    void revertImageChanges();
    void resetRotation();


protected:
    void closeEvent(QCloseEvent *event);

signals:
    void zoomChanged();
    void areaSelectedChanged();
    void undoRedoUpdated();

private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    //Reimplemented functions.
    void wheelEvent(QWheelEvent * event);
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //RubberBanding
    QPoint origin;
    QPoint endPoint;
    QRubberBand* rubberBand;

    //Copy/Paste
    QClipboard* clipBoard;
    bool areaSelected;

    bool pasteRepositioning;
    bool pasteItemMoving;
    void setPasteRepositioning(bool value);
    void finalizePaste();

    QString curFile;
    bool isUntitled;
    bool modified;

    bool zoomable;

    Image image;
    QGraphicsPixmapItem *pixmap;
    QGraphicsPixmapItem *pasteItem;
    std::deque<QImage> *undoStack;
    std::deque<QImage> *redoStack;

    void setAreaSelected(bool value);
};

#endif
