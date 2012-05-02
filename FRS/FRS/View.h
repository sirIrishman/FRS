#ifndef VIEW_H
#define VIEW_H

#include <QtGui/QMainWindow>
#include "ui_View.h"
#include "Controller.h"

using namespace Patterns;

namespace FRS {
    class View : public QMainWindow, public Observer {
        Q_OBJECT
    public:
        View(Model* model, Controller* controller, QWidget *parent = 0, Qt::WFlags flags = 0);
        ~View();

    protected:
        virtual void initialize(Model* model, Controller* controller);
        virtual void subscribeToEvents();
        void update();

    private:
        Ui::MainWindowClass _ui;
        Model* _model;
        Controller* _controller;

    private slots:
        void pshBtn_LoadImage_Clicked();
    };
}

#endif // VIEW_H