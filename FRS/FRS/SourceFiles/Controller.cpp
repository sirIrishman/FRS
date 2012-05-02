#include "Controller.h"
#include "Guard.h"

using namespace FRS;

Controller::Controller(Model* const& model) {
    Utils::Guard::argumentNotNull(model, "model");
    _model = model;
}

Controller::~Controller() {
    //delete _model;
}

void Controller::loadImage(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setSource(GraphicFile, filePath);
}