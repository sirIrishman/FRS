#ifndef FRS_VIEW_H
#define FRS_VIEW_H

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
        void actn_LoadImage_Triggered();
        void actn_LoadVideo_Triggered();
        void actn_CaptureVideo_Triggered();
        void actn_CaptureImage_Triggered();
    };
}

#endif // FRS_VIEW_H
