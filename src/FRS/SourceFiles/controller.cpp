#include "controller.h"
#include "guard.h"

using namespace frs;

Controller::Controller(Model* const& model) {
    utils::Guard::argumentNotNull(model, "model");
    _model = model;
}

void Controller::loadImage(QString const& filePath) {
    utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setFrameSource(ImageFile, filePath);
}

void Controller::loadVideo(QString const& filePath) {
    utils::Guard::argumentNotNullOrEmpty(filePath, "filePath");
    _model->setFrameSource(VideoFile, filePath);
}

void Controller::captureVideo(int webcamIndex) {
    utils::Guard::argumentInRange(webcamIndex, "webcamIndex", 0, _model->webcamCount());
    _model->setFrameSource(VideoWebcam, webcamIndex);
}

void Controller::captureImage(int webcamIndex) {
    utils::Guard::argumentInRange(webcamIndex, "webcamIndex", 0, _model->webcamCount());
    _model->setFrameSource(ImageWebcam, webcamIndex);
}

void Controller::setCurrentObjectDetectionAlgorithm(ObjectDetectionAlgorithm algorithm) {
    _model->setCurrentObjectDetectionAlgorithm(algorithm);
}

void Controller::setCurrentFaceRecognitionAlgorithm(FaceRecognitionAlgorithm algorithm) {
    _model->setCurrentFaceRecognitionAlgorithm(algorithm);
}

void Controller::setFaceRecognitionTrainingName(QString const& name) {
    utils::Guard::argumentNotNullOrEmpty(name, "name");
    _model->setFaceRecognitionTrainingName(name);
}

void Controller::trainFaceRecognition(TrainingData const& trainingData) {
    utils::Guard::argumentInRange<int>(trainingData.imageCollection().size(), "trainingData.imageCollection().size()", 1, trainingData.imageCollection().max_size());
    utils::Guard::argumentInRange<int>(trainingData.classCollection().size(), "trainingData.classCollection().size()", trainingData.imageCollection().size(), trainingData.imageCollection().size());
    _model->trainFaceRecognition(trainingData);
}

void Controller::saveFaceRecognitionState() const {
    _model->saveFaceRecognitionState();
}

void Controller::loadFaceRecognitionState(QString const& faceRecognitionTrainingName) const {
    utils::Guard::argumentNotNullOrEmpty(faceRecognitionTrainingName, "faceRecognitionTrainingName");
    _model->loadFaceRecognitionState(faceRecognitionTrainingName);
}
