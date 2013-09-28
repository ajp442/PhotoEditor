#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QLabel>

//Andrew's comment for test push.
//Another comment by andrew.
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void OpenFile();

private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
    QLabel *label;
};

#endif // MAINWINDOW_H
