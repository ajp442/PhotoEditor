/*************************************************************************//**
 * @file
 *
 * @brief Entry point of the program.
 *
 * @mainpage Program 1, Image Manipulation
 *
 * @par Overview
 *
 * @par Introduction
 * Not long ago, digital image processing was the province of a privileged few. Nowadays the ubiquitous
 * digital camera has catapulted digital image processing into the mainstream. Digital imagery coupled
 * with photo editing software provides a “digital darkroom” that allows anyone to manipulate the contrast,
 * brightness, color balance, and other properties of their photographs.
 *
 * @par Problem
 * Write a photo editor program in C++ using the Qt framework. This program will be similar in interface
 * and functionality to the Microsoft Photo Editor (formerly distributed with Windows and Microsoft
 * Office). Your program will allow the user to load, manipulate, and save digital images on disk. It should
 * feature drop-down menus, a toolbar of common image operations, a main window pane that displays
 * multiple images (each in a child window with scroll bars), and a status bar with information about the
 * currently selected image.
 *
 * @par Background
 * The Qt framework supplies a powerful toolkit for building graphical user interfaces in C++, and also
 * provides routines for reading, writing, displaying, and manipulating digital images. This assignment will
 * give you practice in writing desktop GUI applications in C++ with Qt.
 *
 * @par Specifications
 * Your photo editor GUI should feature drop-down menus similar to those in the Microsoft Photo Editor:
   @verbatim
File: New, Open, Close, Save, Save As, Revert, Properties
Edit: Cut, Copy, Paste, Paste New Image
Image: Crop, Resize, Rotate, Balance (contrast, brightness, gamma)
Effects: Sharpen, Soften, Negative, Despeckle, Posterize, Edge, Emboss
Window: Tile, Cascade (also an open image file listing)
Help: About
   @endverbatim
 * The toolbar should include icons for common File operations (New, Open, Save), Edit operations (Cut,
 * Copy, Paste), Image operations (Crop, Balance), and a list box with different zoom levels.
 * The Microsoft Photo Editor uses the Multiple Document Interface (MDI) to display each image in its
 * own child window that resides inside the main window. Use this approach in your program as well.
 * You must build a complete GUI, but not all operations need be implemented. A minimal implementation
 * will allow the user to load and save images (BMP, JPEG, PNG, etc.) using a file dialog box, manipulate
 * open images in child windows, and list fundamental image properties in a text box (filename, type of
 * image, dimensions). Image manipulations should include zooming, resizing, cropping with the mouse, at
 * least one slider bar adjustment (contrast, brightness, gamma), and at least two image processing
 * operations (sharpening, smoothing, negation, etc.). More credit will be given to feature-rich
 * implementations. Some image processing operations are already implemented in the QImage and
 * QPixmap classes of Qt.
 *
 * @par Extra Features
 * \li <b>Scroll Zooming</b> - Can zoom by scrolling and will scrool to the area where you cursor is.
 * \li <b>Fit to Window</b> - Will fit image to window and not allow zooming.
 * \li <b>Custom Zoom Factor</b> - Can edit the text in the zoom combobox for custom zoom level.
 * \li <b>Zoom Combobox</b> - Can always see what the zoom level of the selected window is.
 * \li <b>Zoom non-selected windows</b> - Can scroll zoom windows that are not selected.
 * \li <b>Zoom Hot-kyes</b> - Zoom with ctrl++ and ctrl+-, which are dynamically enabled/disabled.
 * \li <b></b>
 * \li <b></b>
 * \li <b></b>
 * \li <b></b>
 * \li <b></b>
 *
 * @authors Josh Schultz, Paul Blazi, Andrew Pierson
 *
 * @date October 06, 2013
 *
 * @par Professor
 *         Dr. Weiss
 *
 * @par Course
 *         GUI OOP; CSC 421
 *
 * @par Usage
   @verbatim
$ PhotoEdit
   @endverbatim
 *
 * @par Compiling Instructions
 *     File needed for compiling include functions.cpp, functions.h, and
 *     prog1.cpp
 *
   @verbatim
$ qmake...
   @endverbatim
 *
 *
 *
 * @par Modifications and Development Time Line:
   @verbatim
   Date          Modification
   ------------  --------------------------------------------------------------
   Sep  26, 2013  Program assigned.
   Sep  28, 2013  Initial Commits.
   Oct  06, 2013  Re-structured program and added doxygen-style comments.
   Oct  07, 2013  Sharpen, Image Class for easy manipulation.
   Oct  10, 2013  Dialog Class for code reuseabilaty.
   Oct  11, 2013  Gamma, despeckle, brightness
   Oct  12, 2013  Started working on zooming.
   Oct  13, 2013  Contrast, soften, edge detection, rotation.
   Oct  13, 2013  Redo and Undo.
   Oct  14, 2013  Finalized zooming features.
   Oct  14, 2013  Added doxygen style comments.
   Oct  15, 2013  Program Due.
   @endverbatim
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

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(PhotoEdit);

    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
