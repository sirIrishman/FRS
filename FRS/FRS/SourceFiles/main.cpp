#include "frsApplication.h"
#include "view.h"
#include "exceptions.h"
#include "dialogService.h"

using namespace FRS;
using namespace Services;

int main(int argc, char *argv[]) {
    try{
        FrsApplication app(argc, argv);
        DialogService::initialize(new QtDialogServiceImplementation());
        Model* model = new Model();
        Controller* controller = new Controller(model);
        View wnd(model, controller);
        wnd.show();
        return app.exec();
    } catch(Framework::BaseException& e) {
        DialogService::showErrorMessage(e.message());
    }
}
