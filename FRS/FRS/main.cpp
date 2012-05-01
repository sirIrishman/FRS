#include "View.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Controller* controller = new Controller();
    Model* model = new Model();
    View wnd(controller, model);
    wnd.show();
    return app.exec();
}