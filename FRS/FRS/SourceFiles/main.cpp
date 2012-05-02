#include <QtGui/QApplication>
#include <qmessagebox.h>
#include "View.h"
#include "Exceptions.h"

using namespace FRS;
using namespace Framework;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    try { 
        Model* model = new Model();
        Controller* controller = new Controller(model);
        View wnd(model, controller);
        wnd.show();
        return app.exec();
    } catch(BaseException* e) {
        QMessageBox::critical(NULL, "Run-Time Exception", e->message());
    }
}