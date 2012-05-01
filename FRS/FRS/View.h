#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_View.h"
#include "Model.h"
#include "Controller.h"

using namespace FRS;
using namespace Patterns;

class View : public QMainWindow, public Observer {
    Q_OBJECT
public:
    View(Controller* controller, Model* model, QWidget *parent = 0, Qt::WFlags flags = 0);
    ~View();

protected:
    virtual void initialize(Controller* controller, Model* model) {
        _controller = controller;
        _model = model;
        _model->attach(this);
        subscibeToEvents();
    }
    virtual void subscibeToEvents();
    void update();

private:
    Ui::MainWindowClass _ui;
    Controller* _controller;
    Model* _model;
};

#endif // MAINWINDOW_H