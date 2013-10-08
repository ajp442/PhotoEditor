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

#include "mainwindow.h"
#include "mdichild.h"


MainWindow::MainWindow()
{
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
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
    pasteAct->setEnabled(hasMdiChild);
#endif
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
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
    connect(nextAct, SIGNAL(triggered()),
            mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));




    //================Image Actions================
    cropAct = new QAction(tr("Crop"), this);
    cropAct->setStatusTip(tr("not color"));
    connect(cropAct, SIGNAL(triggered()), this, SLOT(crop()));

    imgResizeAct = new QAction(tr("resize"), this);
    imgResizeAct->setStatusTip(tr(""));
    connect(imgResizeAct, SIGNAL(triggered()), this, SLOT(imgResize()));

    rotateAct = new QAction(tr("rotate"), this);
    rotateAct->setStatusTip(tr(""));
    connect(rotateAct, SIGNAL(triggered()), this, SLOT(rotate()));

    balanceAct = new QAction(tr("balance"), this);
    balanceAct->setStatusTip(tr(""));
    connect(balanceAct, SIGNAL(triggered()), this, SLOT(balance()));



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
    connect(despeckleAct, SIGNAL(triggered()), this, SLOT(despeckle()));

    posterizeAct = new QAction(tr("Posterize"), this);
    posterizeAct->setStatusTip(tr(""));
    connect(posterizeAct, SIGNAL(triggered()), this, SLOT(posterize()));

    edgeAct = new QAction(tr("Edge"), this);
    edgeAct->setStatusTip(tr(""));
    connect(edgeAct, SIGNAL(triggered()), this, SLOT(edge()));

    embossAct = new QAction(tr("Emboss"), this);
    embossAct->setStatusTip(tr(""));
    connect(embossAct, SIGNAL(triggered()), this, SLOT(emboss()));

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
#endif

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
 * to verrify that a window from the same file exists. Does not create an
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
 * @brief Changes how menus are layed out.
 *****************************************************************************/
void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

/**************************************************************************//**
 * @brief Chanes the active QMdiSubWindow. Called by MainWindow::open(), and
 * connected to some signal.
 *****************************************************************************/
void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

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
    QString fileName = QFileDialog::getOpenFileName(this, caption, defaultDir, "images (*.png *.xpm *.jpg);;all (*.*)");
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
//    if (activeMdiChild())
//        activeMdiChild()->copy();
}

void MainWindow::paste()
{
//    if (activeMdiChild())
//        activeMdiChild()->paste();
}
#endif


//------------------------------------------------------------------------------
//                  Image
//------------------------------------------------------------------------------
void MainWindow::crop()
{

}

void MainWindow::imgResize()
{

}

void MainWindow::rotate()
{

}

void MainWindow::balance()
{

}


//------------------------------------------------------------------------------
//                  Effects
//------------------------------------------------------------------------------
void MainWindow::grayScale()
{
    if (activeMdiChild())
    {
        activeMdiChild()->grayScale();
        statusBar()->showMessage(tr("Image Grayed"), 2000);
    }
}

void MainWindow::sharpen()
{
    if (activeMdiChild())
    {
        activeMdiChild()->sharpen();
        statusBar()->showMessage(tr("Image Sharpened"), 2000);
    }
}

void MainWindow::soften()
{

}

void MainWindow::negative()
{
    if (activeMdiChild()) {
        activeMdiChild()->negative();
        statusBar()->showMessage(tr("Image Negated"), 2000);
    }
}

void MainWindow::despeckle()
{

}

void MainWindow::posterize()
{
    if(activeMdiChild())
    {
        QGroupBox *box = new QGroupBox(tr(""));

        QLabel * label = new QLabel("Levels:");
        QSlider * slider = new QSlider(Qt::Horizontal);
        slider->setMinimumWidth(100);
        QSpinBox *spinBox = new QSpinBox;
        spinBox->setRange(0,255);
        QSlider * slider2 = new QSlider(Qt::Horizontal);
        slider->setMinimumWidth(100);
        QLabel * label2 = new QLabel(tr("Gamma:"));
        QDoubleSpinBox *doubleSpingBox = new QDoubleSpinBox;
        doubleSpingBox->setRange(0,1);

        QPushButton *okButton = new QPushButton(tr("OK"));
        okButton->setMaximumWidth(90);
        QPushButton *cancelButton = new QPushButton(tr("Cancel"));
        cancelButton->setMaximumWidth(90);
        QGridLayout *layout3 = new QGridLayout;

        QGridLayout *layout = new QGridLayout;

        QLabel *titleLabel = new QLabel(tr("Posterize"));
        QFont *titleFont = new QFont();
        titleFont->setPointSize(11);
        titleLabel->setFont(*titleFont);
        layout3->addWidget(titleLabel, 0, 0);
        layout->addWidget(slider, 1, 1);
        layout->addWidget(spinBox, 1, 2);
        layout->addWidget(label, 1, 0);
        layout->addWidget(slider2, 2, 1);
        layout->addWidget(doubleSpingBox, 2, 2);
        layout->addWidget(label2, 2, 0);
        layout->setAlignment(Qt::AlignCenter);
        layout->setHorizontalSpacing(20);
        layout->setVerticalSpacing(10);
        layout->setMargin(10);

        QGridLayout *layout2 = new QGridLayout;

        QVBoxLayout *hBox = new QVBoxLayout;
        layout2->addWidget(okButton, 0, 1);
        layout2->addWidget(cancelButton, 0, 2);
        layout2->setAlignment(Qt::AlignRight);  //align buttons to the right
        hBox->addLayout(layout3);
        hBox->addLayout(layout);
        hBox->addLayout(layout2);

        box->setFixedSize(box->sizeHint());  //make the view not resizable
        box->setAlignment(Qt::AlignCenter);
        box->setLayout(hBox);
        box->setFocusPolicy(Qt::StrongFocus);

        box->show();


    }
}

void MainWindow::edge()
{

}

void MainWindow::emboss()
{

}

//------------------------------------------------------------------------------
//                  About
//------------------------------------------------------------------------------
void MainWindow::about()
{
   QMessageBox::about(this, tr("About MDI"),
            tr("The <b>MDI</b> example demonstrates how to write multiple "
               "document interface applications using Qt."));
}
