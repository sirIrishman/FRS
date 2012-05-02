#include "Controller.h"
#include "Guard.h"

using namespace FRS;

Controller::Controller(Model* const& model) {
    Utils::Guard::argumentNotNull(model, "model");
    _model = model;
}

void Controller::loadImageFromFile(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setSource(ImageFile, filePath);
}

void Controller::loadVideoFromFile(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setSource(VideoFile, filePath);
}

void Controller::loadVideoFromWebcam(int webcamIndex) {
    //Utils::Guard::argumentInRange(webcamIndex, "webcamIndex", 0, 10);
    _model->setSource(Webcam);
}