#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_View.h"

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~View();

private:
    Ui::MainWindowClass ui;
};

#endif // MAINWINDOW_H
