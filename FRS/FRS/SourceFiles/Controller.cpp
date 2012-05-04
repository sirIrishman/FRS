#include "Controller.h"
#include "Guard.h"

using namespace FRS;

Controller::Controller(Model* const& model) {
    Utils::Guard::argumentNotNull(model, "model");
    _model = model;
}

void Controller::loadImage(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setSource(ImageFile, filePath);
}

void Controller::loadVideo(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setSource(VideoFile, filePath);
}

void Controller::captureVideo() {
    _model->setSource(VideoWebcam);
}

void Controller::captureImage() {
    _model->setSource(ImageWebcam);
}

void Controller::setActiveWebcamIndex(int webcamIndex) {
    Utils::Guard::argumentInRange(webcamIndex, "webcamIndex", 0, _model->webcamCount());
    _model->setActiveWebcamIndex(webcamIndex);
}
