/**************************************************************************//**
 * @file
 *
 * @brief MainWindow class is used for the first gui layer that the user sees.
 *
 * This class creates an object that directs all user input to its
 * appropriate functions.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //New, Open, Save, Exit
    void newFile();
    void open();
    void save();
    void saveAs();

    //Copy, Cut, Paste
#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif

    //Undo, redo
    void undo();
    void redo();

    //Image
    void crop();
    void imgResize();
    void rotate(const std::vector<double> &dialogValues);
    void balance(const std::vector<double> &dialogValues);

    //Dialogs
    void brightnessDialog();
    void despeckleDialog();
    void gammaDialog();
    void binaryThresholdDialog();
    void balanceDialog();
    void rotateDialog();

    //Effects
    void grayScale();
    void sharpen();
    void soften();
    void negative();
    void despeckle(const std::vector<double> &dialogValues);
    void posterize();
    void edge();
    void emboss();
    void gamma(const std::vector<double> &dialogValues);
    void brightness(const std::vector<double> &dialogValues);
    void binaryThreshold(const std::vector<double> &dialogValues);

    //About
    void about();

    //Windows & Menus
    void updateMenus();
    void updateImageMenu();
    void updateEffectsMenu();
    void updateWindowMenu();
    MdiChild *createMdiChild();
    void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);
    void zoomIn();
    void zoomOut();
    void zoomTo(double zoomLevel);

private:
    //========Functions========
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    MdiChild *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName);

    //========MDI========
    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;

    //========Menus========
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *imageMenu;
    QMenu *effectsMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    //========ToolBars========
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    //========Actions========
    //New, Open, Save, Exit
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    //Copy, Cut, Paste
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
#endif

    //Undo, Redo
    QAction *undoAct;
    QAction *redoAct;

    //Windowing
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *separatorAct;

    //Image
    QAction *cropAct;
    QAction *imgResizeAct;
    QAction *rotateAct;
    QAction *balanceAct;

    //Effects
    QAction *grayScaleAct;
    QAction *sharpenAct;
    QAction *softenAct;
    QAction *negativeAct;
    QAction *despeckleAct;
    QAction *posterizeAct;
    QAction *edgeAct;
    QAction *embossAct;
    QAction *gammaAct;
    QAction *brightnessAct;
    QAction *binaryThresholdAct;

    //About
    QAction *aboutAct;


};

#endif
