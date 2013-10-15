/**************************************************************************//**
 * @file
 *
 * @brief MainWindow implementation.
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
#include <QStandardPaths>
#include <QUrl>
#include <QTransform>

#include "mainwindow.h"
#include "mdichild.h"
#include "dialog.h"


MainWindow::MainWindow()
{
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(handleZoomChanged()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    readSettings();

    setWindowTitle(tr("Photo Edit"));
    setUnifiedTitleAndToolBarOnMac(true);
}

/**************************************************************************//**
 * @brief Does cleanup, like closing all subwindows and saving settings
 * when the main window is closed.
 *
 * @param[in] event - The Close event
 *****************************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    }
    else {
        writeSettings();
        event->accept();
    }
}

/**************************************************************************//**
 * @brief Updates the main menus that are always (or almost always) visible
 * to the user.
 *****************************************************************************/
void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    saveAct->setEnabled(hasMdiChild);
    saveAsAct->setEnabled(hasMdiChild);
#ifndef QT_NO_CLIPBOARD
    copyAct->setEnabled(hasMdiChild);
    cutAct->setEnabled(hasMdiChild);
    pasteAct->setEnabled(hasMdiChild);
#endif
    undoAct->setEnabled(hasMdiChild);
    redoAct->setEnabled(hasMdiChild);
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    zoomInAct->setEnabled(hasMdiChild);
    zoomOutAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);

#ifndef QT_NO_CLIPBOARD
//    bool hasSelection = (activeMdiChild() &&
//                         activeMdiChild()->textCursor().hasSelection());
//    cutAct->setEnabled(hasSelection);
//    copyAct->setEnabled(hasSelection);
#endif
}

/**************************************************************************//**
 * @brief Creates/Updates the menu for manipulating the image.
 *****************************************************************************/
void MainWindow::updateImageMenu()
{
    bool hasMdiChild = (activeMdiChild() != 0);

    imageMenu->addAction(cropAct);
    cropAct->setEnabled(hasMdiChild);

    imageMenu->addAction(imgResizeAct);
    imgResizeAct->setEnabled(hasMdiChild);

    imageMenu->addAction(rotateAct);
    rotateAct->setEnabled(hasMdiChild);

    imageMenu->addAction(balanceAct);
    balanceAct->setEnabled(hasMdiChild);

}

/**************************************************************************//**
 * @brief Creates/Updates the menu that has all the effects.
 *****************************************************************************/
void MainWindow::updateEffectsMenu()
{
    bool hasMdiChild = (activeMdiChild() != 0);

    effectsMenu->addAction(grayScaleAct);
    grayScaleAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(sharpenAct);
    sharpenAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(softenAct);
    softenAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(negativeAct);
    negativeAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(despeckleAct);
    despeckleAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(posterizeAct);
    posterizeAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(edgeAct);
    edgeAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(embossAct);
    embossAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(gammaAct);
    gammaAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(brightnessAct);
    brightnessAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(binaryThresholdAct);
    binaryThresholdAct->setEnabled(hasMdiChild);

    effectsMenu->addAction(contrastAct);
    contrastAct->setEnabled(hasMdiChild);

}

/**************************************************************************//**
 * @brief Creates/Updates menu that is used for manipulating windows.
 *****************************************************************************/
void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(zoomInAct);
    windowMenu->addAction(zoomOutAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}


/**************************************************************************//**
 * @brief Creates a child window for the MDI area. This will be called by
 * MainWindow::newFile() and MainWindow::open()
 *
 * @returns child - The child window that goes in MDI area.
 *****************************************************************************/
MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    mdiArea->addSubWindow(child);

#ifndef QT_NO_CLIPBOARD
    connect(child, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
#endif
    connect(child, SIGNAL(zoomChanged()), this, SLOT(handleZoomChanged()));

    return child;
}

/**************************************************************************//**
 * @brief Constructs all the actions that will be used. Called by the
 * constructor, MainWindow::MainWindow()
 *****************************************************************************/
void MainWindow::createActions()
{
    //================New, Open, Save, Exit================
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));


    //================Copy, Cut, Paste================
#ifndef QT_NO_CLIPBOARD
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
#endif

    //================Undo, Redo=======================
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last image effect on the current window"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last command undone"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));


    //================Windowing Actions================
    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    zoomInAct = new QAction(tr("Zoom In"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom In"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom Out"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom Out"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));


    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));




    //================Image Actions================
    cropAct = new QAction(tr("Crop"), this);
    cropAct->setStatusTip(tr("not color"));
    connect(cropAct, SIGNAL(triggered()), this, SLOT(crop()));

    imgResizeAct = new QAction(tr("Resize"), this);
    imgResizeAct->setStatusTip(tr(""));
    connect(imgResizeAct, SIGNAL(triggered()), this, SLOT(imgResize()));

    rotateAct = new QAction(tr("Rotate"), this);
    rotateAct->setStatusTip(tr(""));
    connect(rotateAct, SIGNAL(triggered()), this, SLOT(rotateDialog()));

    balanceAct = new QAction(tr("Balance (Not Implemented)"), this);
    balanceAct->setStatusTip(tr(""));
    connect(balanceAct, SIGNAL(triggered()), this, SLOT(balanceDialog()));



    //================Effect Actions================
    grayScaleAct = new QAction(tr("GrayScale"), this);
    grayScaleAct->setStatusTip(tr("convert image to grayscale"));
    connect(grayScaleAct, SIGNAL(triggered()), this, SLOT(grayScale()));

    sharpenAct = new QAction(tr("Sharpen"), this);
    sharpenAct->setStatusTip(tr(""));
    connect(sharpenAct, SIGNAL(triggered()), this, SLOT(sharpen()));

    softenAct = new QAction(tr("Soften"), this);
    softenAct->setStatusTip(tr(""));
    connect(softenAct, SIGNAL(triggered()), this, SLOT(soften()));

    negativeAct = new QAction(tr("Negative"), this);
    negativeAct->setStatusTip(tr(""));
    connect(negativeAct, SIGNAL(triggered()), this, SLOT(negative()));

    despeckleAct = new QAction(tr("Despeckle"), this);
    despeckleAct->setStatusTip(tr(""));
    connect(despeckleAct, SIGNAL(triggered()), this, SLOT(despeckleDialog()));

    posterizeAct = new QAction(tr("Posterize"), this);
    posterizeAct->setStatusTip(tr(""));
    connect(posterizeAct, SIGNAL(triggered()), this, SLOT(posterize()));

    edgeAct = new QAction(tr("Edge"), this);
    edgeAct->setStatusTip(tr(""));
    connect(edgeAct, SIGNAL(triggered()), this, SLOT(edge()));

    embossAct = new QAction(tr("Emboss"), this);
    embossAct->setStatusTip(tr(""));
    connect(embossAct, SIGNAL(triggered()), this, SLOT(emboss()));

    gammaAct = new QAction(tr("Gamma"), this);
    gammaAct->setStatusTip(tr(""));
    connect(gammaAct, SIGNAL(triggered()), this, SLOT(gammaDialog()));

    brightnessAct = new QAction(tr("Brightness"), this);
    brightnessAct->setStatusTip(tr(""));
    connect(brightnessAct, SIGNAL(triggered()), this, SLOT(brightnessDialog()));

    binaryThresholdAct = new QAction(tr("Binary Threshold"), this);
    binaryThresholdAct->setStatusTip(tr(""));
    connect(binaryThresholdAct, SIGNAL(triggered()), this, SLOT(binaryThresholdDialog()));

    contrastAct = new QAction(tr("Contrast"), this);
    contrastAct->setStatusTip(tr(""));
    connect(contrastAct, SIGNAL(triggered()), this, SLOT(contrastDialog()));
}


/**************************************************************************//**
 * @brief Constructs and initializes the menus that traditionally go at the
 * top of the application. Called by the constructor, MainWindow::MainWindow()
 *****************************************************************************/
void MainWindow::createMenus()
{
    //========File Menu========
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    fileMenu->addAction(exitAct);

    //========Edit Menu========
    editMenu = menuBar()->addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(cropAct);
#endif

    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    //========Image Menu========
    imageMenu = menuBar()->addMenu(tr("Image"));
    updateImageMenu();
    connect(imageMenu, SIGNAL(aboutToShow()), this, SLOT(updateImageMenu()));

    //========Effects Menu========
    effectsMenu = menuBar()->addMenu(tr("Effects"));
    updateEffectsMenu();
    connect(effectsMenu, SIGNAL(aboutToShow()), this, SLOT(updateEffectsMenu()));

    //========Window Menu========
    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    menuBar()->addSeparator();

    //========Help Menu========
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

/**************************************************************************//**
 * @brief Makes all the toolbars. Called by MainWindow::MainWindow()
 *****************************************************************************/
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    zoombox = new QComboBox;
    QStringList zoomLevels;
    zoomLevels << "10%" << "25%" << "50%" << "75%" << "100%" << "125%" << "150%" << "200%" << "400%" << "800%" << "1600%" << "Fit to Window";
    zoombox->addItems(zoomLevels);
    zoombox->setEditable(true);
    zoombox->setCurrentIndex(4);
    zoombox->setFocusPolicy(Qt::ClickFocus);
    fileToolBar->addWidget(zoombox);
    connect(zoombox, SIGNAL(editTextChanged(QString)), this, SLOT(zoomTo(QString)));

#ifndef QT_NO_CLIPBOARD
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
#endif
}

/**************************************************************************//**
 * @brief Initializes the Status Bar at the bottom of the application window.
 * Called by MainWindow::MainWindow()
 *****************************************************************************/
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

/**************************************************************************//**
 * @brief Reads the settings from when the program was last ran. For example,
 * it will start in the same position as it was when it was last closed.
 * Called by MainWindow::MainWindow()
 *****************************************************************************/
void MainWindow::readSettings()
{
    QSettings settings("QtProject", "MDI Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

/**************************************************************************//**
 * @brief Writes current settings. For example it will save the size and
 * position of the window so it can be re-opend in the same location later on.
 * Called by MainWindow::closeEvent() when the QCloseEvent occurs.
 *****************************************************************************/
void MainWindow::writeSettings()
{
    QSettings settings("QtProject", "MDI Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

/**************************************************************************//**
 * @brief Returns the active MdiChild or 0 if none exist. Often used to check
 * if there are any active MdiChild objects.
 *
 * @returns MdiChild - On Success of finding an active MdiChild.
 * @returns 0 - Failed to find an active MdiChild.
 *****************************************************************************/
MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

/**************************************************************************//**
 * @brief Returns the active MdiChild window. Called by MainWindow::open()
 * to verify that a window from the same file exists. Does not create an
 * MdiChild or QMdiSubWindow.
 *
 * @param[in] fileName - The criteria used to search for an MdiSubwindow.
 *
 * @returns MdiChild - On Success of finding an active subwindow.
 * @returns 0 - Failed to find an active subwindow.
 *****************************************************************************/
QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

/**************************************************************************//**
 * @brief Changes how menus are laid out.
 *****************************************************************************/
void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

/**************************************************************************//**
 * @brief Changes the active QMdiSubWindow. Called by MainWindow::open(), and
 * connected to some signal.
 *****************************************************************************/
void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
    handleZoomChanged();

}

//------------------------------------------------------------------------------
//              File
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//              New, Open, Save, Exit
//------------------------------------------------------------------------------
void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::open()
{
    QString caption = "Photo Edit - Select Image";
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString fileName = QFileDialog::getOpenFileName(this, caption, defaultDir, "images (*.png *.bmp *.jpg);;all (*.*)");
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        }
        else {
            child->close();
        }
    }
}

void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save()) {
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void MainWindow::saveAs()
{
    if (activeMdiChild() && activeMdiChild()->saveAs()) {
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

//------------------------------------------------------------------------------
//              Edit
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//              Copy, Cut, Paste
//------------------------------------------------------------------------------
#ifndef QT_NO_CLIPBOARD
void MainWindow::cut()
{
//    if (activeMdiChild())
//        activeMdiChild()->cut();
}

void MainWindow::copy()
{
    if (activeMdiChild())
    {
        activeMdiChild()->copy();
        activeMdiChild()->commitImageChanges();
    }
}

void MainWindow::paste()
{
    if (activeMdiChild())
    {
        activeMdiChild()->paste();
        activeMdiChild()->commitImageChanges();
    }
}

#endif


//------------------------------------------------------------------------------
//                  Undo, Redo
//------------------------------------------------------------------------------
void MainWindow::undo()
{
    if(activeMdiChild())
    {
        activeMdiChild()->undo();
        statusBar()->showMessage(tr("Undo committed"), 2000);
    }
}

void MainWindow::redo()
{
    if(activeMdiChild())
    {
        activeMdiChild()->redo();
        statusBar()->showMessage(tr("Redid last undo"), 2000);
    }
}

//------------------------------------------------------------------------------
//                  Image
//------------------------------------------------------------------------------
void MainWindow::crop()
{
    if(activeMdiChild())
    {
        activeMdiChild()->crop();
        activeMdiChild()->commitImageChanges();
    }
}

void MainWindow::imgResize()
{

}

void MainWindow::rotate(const std::vector<double> &dialogValues)
{
    if(activeMdiChild())
    {
        activeMdiChild()->resetRotation();
        activeMdiChild()->rotate(dialogValues[0]);
        statusBar()->showMessage(tr("Image Rotated"), 2000);
    }
}

void MainWindow::balance(const std::vector<double> &dialogValues)
{
    if (activeMdiChild())
    {
        //activeMdiChild()->brightness(dialogValues[0]);
        //activeMdiChild()->contrast(dialogValues[1], dialogValues[2]);
        //activeMdiChild()->gamma(dialogValues[3]);
        //activeMdiChild()->balance(dialogValues[0],
        //                          dialogValues[1],
        //                          dialogValues[2],
        //                          dialogValues[3]);
        //activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Image Balanced"), 2000);
    }
}

//------------------------------------------------------------------------------
//                  Dialogs
//------------------------------------------------------------------------------
void MainWindow::brightnessDialog()
{
    if(activeMdiChild())
    {
        int baseValue = 0, min = -255, max = 255;

        dialog *myBrightnessDialog = new dialog(tr("Brightness"));
        myBrightnessDialog->addChild(tr("Level:"), baseValue, min, max);

        connect(myBrightnessDialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(brightness(std::vector<double>)));
        connect(myBrightnessDialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(revertImageChanges()));
        connect(myBrightnessDialog, SIGNAL(accepted()), activeMdiChild(), SLOT(commitImageChanges()));
    }
}

void MainWindow::despeckleDialog()
{
    if(activeMdiChild())
    {
        int baseValue = 32, min = 0, max = 255;

        dialog *despeckle_dialog = new dialog(tr("Despeckle"));
        despeckle_dialog->addChild(tr("Threshold:"), baseValue, min, max);

        connect(despeckle_dialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(despeckle(std::vector<double>)));
        connect(despeckle_dialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(revertImageChanges()));
        connect(despeckle_dialog, SIGNAL(accepted()), activeMdiChild(), SLOT(commitImageChanges()));
    }
}

void MainWindow::gammaDialog()
{
    if(activeMdiChild())
    {
        double baseValue = 1, min = 0, max = 5;

        dialog *gamma_dialog = new dialog(tr("Gamma"));
        gamma_dialog->addChild(tr("Gamma Value:"), baseValue, min, max);

        connect(gamma_dialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(gamma(std::vector<double>)));
        connect(gamma_dialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(revertImageChanges()));
        connect(gamma_dialog, SIGNAL(accepted()), activeMdiChild(), SLOT(commitImageChanges()));
    }
}


void MainWindow::binaryThresholdDialog()
{
    if(activeMdiChild())
    {
        int baseValue = 100, min = 0, max = 255;

        dialog *binaryThreshold_dialog = new dialog(tr("Binary Threshold"));
        binaryThreshold_dialog->addChild(tr("Threshold:"), baseValue, min, max);

        connect(binaryThreshold_dialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(binaryThreshold(std::vector<double>)));
        connect(binaryThreshold_dialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(revertImageChanges()));
        connect(binaryThreshold_dialog, SIGNAL(accepted()), activeMdiChild(), SLOT(commitImageChanges()));
    }
}

void MainWindow::balanceDialog()
{
    if(activeMdiChild())
    {
        int brightness = 0, brightnessMin = -255, brightnessMax = 255;
        int contrastLower = 0, contrastMin = 0, contrastMax = 255;
        int contrastUpper = 255;
        double gamma = 1, gammaMin = 0, gammaMax = 5;

        dialog *binaryThreshold_dialog = new dialog(tr("Balance (Not Implemented)"));
        binaryThreshold_dialog->addChild(tr("Brightness:"), brightness, brightnessMin, brightnessMax);
        binaryThreshold_dialog->addChild(tr("Contrast Lower:"), contrastLower, contrastMin, contrastMax);
        binaryThreshold_dialog->addChild(tr("Contrast Upper:"), contrastUpper, contrastMin, contrastMax);
        binaryThreshold_dialog->addChild(tr("Gamma:"), gamma, gammaMin, gammaMax);

        connect(binaryThreshold_dialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(balance(std::vector<double>)));
        connect(binaryThreshold_dialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(revertImageChanges()));
        connect(binaryThreshold_dialog, SIGNAL(accepted()), activeMdiChild(), SLOT(commitImageChanges()));
    }
}

void MainWindow::rotateDialog()
{
    if(activeMdiChild())
    {
        int angle = 0, angleMin = 0, angleMax = 360;

        dialog *rotate_dialog = new dialog(tr("Rotation"));
        rotate_dialog->addChild(tr("Angle:"), angle, angleMin, angleMax);

        //no reason to connect OK, since it automatically changes the view without the need for .commit
        connect(rotate_dialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(rotate(std::vector<double>)));
        connect(rotate_dialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(resetRotation()));
    }
}

void MainWindow::contrastDialog()
{
    if(activeMdiChild())
    {
        int lower = 0, upper = 255, min = 0, max = 255;

        dialog *contrast_dialog = new dialog(tr("Contrast"));
        contrast_dialog->addChild("Lower:", lower, min, max);
        contrast_dialog->addChild("Upper:", upper, min, max);

        connect(contrast_dialog, SIGNAL(valueChanged(std::vector<double>)), this, SLOT(contrast(std::vector<double>)));
        connect(contrast_dialog, SIGNAL(cancelled()), activeMdiChild(), SLOT(revertImageChanges()));
        connect(contrast_dialog, SIGNAL(accepted()), activeMdiChild(), SLOT(commitImageChanges()));
    }
}

//------------------------------------------------------------------------------
//                  Effects
//------------------------------------------------------------------------------
void MainWindow::grayScale()
{
    if (activeMdiChild())
    {
        activeMdiChild()->grayScale();
        activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Image Grayed"), 2000);
    }
}

void MainWindow::sharpen()
{
    if (activeMdiChild())
    {
        activeMdiChild()->sharpen();
        activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Image Sharpened"), 2000);
    }
}

void MainWindow::soften()
{
    if(activeMdiChild())
    {
        activeMdiChild()->soften();
        activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Image Softened"), 2000);
    }
}

void MainWindow::negative()
{
    if (activeMdiChild())
    {
        activeMdiChild()->negative();
        activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Image Negated"), 2000);
    }
}

void MainWindow::despeckle(const std::vector<double> &dialogValues)
{
    //assumes dialogValues is valid and only has 1 value
    if (activeMdiChild())
    {
        activeMdiChild()->despeckle(dialogValues[0]);
        statusBar()->showMessage(tr("Image Despeckled"), 2000);
    }
}

void MainWindow::posterize()
{

}

void MainWindow::edge()
{
    if (activeMdiChild())
    {
        activeMdiChild()->edge();
        activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Edges for image"), 2000);
    }
}

void MainWindow::emboss()
{
    if (activeMdiChild())
    {
        activeMdiChild()->emboss();
        activeMdiChild()->commitImageChanges();
        statusBar()->showMessage(tr("Image embossed"), 2000);
    }
}

void MainWindow::gamma(const std::vector<double> &dialogValues)
{
    //assumes dialogValues is valid and only has 1 value
    if (activeMdiChild())
    {
        activeMdiChild()->gamma(dialogValues[0]);
        statusBar()->showMessage(tr("Image Gamma Changed"), 2000);
    }
}

void MainWindow::brightness(const std::vector<double> &dialogValues)
{
    //assumes dialogValues is valid and only has 1 value
    if (activeMdiChild())
    {
        activeMdiChild()->brightness(dialogValues[0]);
        statusBar()->showMessage(tr("Image Brightened"), 2000);
    }
}

void MainWindow::binaryThreshold(const std::vector<double> &dialogValues)
{
    //assumes dialogValues is valid and only has 1 value
    if (activeMdiChild())
    {
        activeMdiChild()->binaryThreshold(dialogValues[0]);
        statusBar()->showMessage(tr("Binary Threshold commited"), 2000);
    }
}

void MainWindow::contrast(const std::vector<double> &dialogValues)
{
    if(activeMdiChild())
    {
        activeMdiChild()->contrast(dialogValues[0], dialogValues[1]);
        statusBar()->showMessage(tr("Contrast altered"), 2000);
    }
}

//------------------------------------------------------------------------------
//                  Window
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                   Zooming
//-----------------------------------------------------------------------------
/**************************************************************************//**
 * @brief This is connected to the zoomInAct.
 *****************************************************************************/
void MainWindow::zoomIn()
{
    if (activeMdiChild())
    {
        if (activeMdiChild()->isZoomable())
        {
            activeMdiChild()->scale(1.15, 1.15);
            handleZoomChanged();
        }
    }
}

/**************************************************************************//**
 * @brief This is connected to the zoomOutAct.
 *****************************************************************************/
void MainWindow::zoomOut()
{
    if (activeMdiChild())
    {
        if (activeMdiChild()->isZoomable())
        {
            activeMdiChild()->scale(1/1.15, 1/1.15);
            handleZoomChanged();
        }
    }
}

/**************************************************************************//**
 * @brief This slot is used for setting the text in the zoom combobox.
 *****************************************************************************/
void MainWindow::handleZoomChanged()
{
    if (activeMdiChild())
    {
        QString text = "Fit to Window";
        if(!activeMdiChild()->isZoomable())
        {
            zoombox->setEditText(text);
        }

        else
        {
            QTransform transform;
            transform = activeMdiChild()->transform();
            qreal hScale = transform.m11();
            text = QString::number(hScale * 100, 'f', 1) + "%";
            zoombox->setEditText(text);
        }
        emit zoombox->editTextChanged(text);
    }
}

/**************************************************************************//**
 * @brief Zooms the activeMdiChild to a size.
 * @param[in] text - QString indicating the percent to scale the image by.
 *****************************************************************************/
void MainWindow::zoomTo(QString text)
{
    if (activeMdiChild())
    {
        bool ok;
        double factor;

        if (text == "Fit to Window")
        {
            zoomInAct->setEnabled(false);
            zoomOutAct->setEnabled(false);
            activeMdiChild()->setZoomable(false);
            activeMdiChild()->fitInView(activeMdiChild()->scene()->itemsBoundingRect());
        }
        else
        {
            zoomInAct->setEnabled(true);
            zoomOutAct->setEnabled(true);
            activeMdiChild()->resetMatrix();
            activeMdiChild()->setZoomable(true);
            text.remove("%");
            factor = text.toDouble(&ok);
            if(ok)
            {
                factor = factor/100;
                activeMdiChild()->scale(factor, factor);
            }
        }
    }
}


//------------------------------------------------------------------------------
//                  About
//------------------------------------------------------------------------------
void MainWindow::about()
{
   QMessageBox::about(this, tr("About MDI"),
            tr("This program demonstrates image manipulation using Qt"));
}
