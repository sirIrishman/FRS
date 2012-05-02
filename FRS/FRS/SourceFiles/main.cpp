#include "View.h"
#include <QtGui/QApplication>

using namespace FRS;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Model* model = new Model();
    Controller* controller = new Controller(model);
    View wnd(model, controller);
    wnd.show();
    return app.exec();
}