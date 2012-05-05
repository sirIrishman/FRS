#include <QtGui/QApplication>
#include <qmessagebox.h>
#include "view.h"
#include "exceptions.h"

using namespace FRS;
using namespace Framework;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Model* model = new Model();
    Controller* controller = new Controller(model);
    View wnd(model, controller);
    wnd.show();
    return app.exec();
}
