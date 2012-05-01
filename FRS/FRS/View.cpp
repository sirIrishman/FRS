#include "View.h"
#include "Guard.h"

using namespace FRS;

View::View(Controller* controller, Model* model, QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags) {
        Utils::Guard::argumentNotNull(controller, "controller");
        Utils::Guard::argumentNotNull(model, "model");
        initialize(controller, model);
        _ui.setupUi(this);
}

View::~View() {
    _model->detach(this);
    delete _controller;
    delete _model;
}

void View::subscibeToEvents() {

}

void View::update() {
}