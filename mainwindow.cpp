#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QFileDialog>
#include <QComboBox>
#include <QScrollArea>
#include <QMdiSubWindow>

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

void MainWindow::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                    tr("Files (*.*)"));
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(fileName));
    label->show();
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(label);
    QMdiSubWindow *mdiSubWindow = new QMdiSubWindow;
    mdiSubWindow->setWidget(scrollArea);
    mdiArea->addSubWindow(mdiSubWindow);
    //mdiArea->subWindowActivated(mdiSubWindow);
   setCentralWidget(mdiArea);
}
