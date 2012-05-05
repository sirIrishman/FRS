#include "controller.h"
#include "guard.h"

using namespace FRS;

Controller::Controller(Model* const& model) {
    Utils::Guard::argumentNotNull(model, "model");
    _model = model;
}

void Controller::loadImage(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setFrameSource(ImageFile, filePath);
}

void Controller::loadVideo(QString const& filePath) {
    Utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setFrameSource(VideoFile, filePath);
}

void Controller::captureVideo(int webcamIndex) {
    Utils::Guard::argumentInRange(webcamIndex, "webcamIndex", 0, _model->webcamCount());
    _model->setFrameSource(VideoWebcam, webcamIndex);
}

void Controller::captureImage(int webcamIndex) {
    Utils::Guard::argumentInRange(webcamIndex, "webcamIndex", 0, _model->webcamCount());
    _model->setFrameSource(ImageWebcam, webcamIndex);
}
