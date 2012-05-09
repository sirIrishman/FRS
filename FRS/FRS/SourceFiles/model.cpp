#include "model.h"

using namespace FRS;
using namespace FRS::Native;

Model::Model() {
    _gettingFrameStrategyFactory = new GettingFrameStrategyCachingFactory(this);
    _gettingFrameStrategy = _gettingFrameStrategyFactory->CreateEmptyStrategy();
    _objectDetectionStrategyFactory = new ObjectDetectionStrategyCachingFactory();
    _objectDetectionStrategy = _objectDetectionStrategyFactory->createEmptytStrategy();
}

Model::~Model() {
    delete _gettingFrameStrategyFactory;
}

cv::Mat Model::frame() const {
    return _gettingFrameStrategy->frame();
}

int Model::webcamCount() const {
    return 5;
}

void Model::setFrameSource(FrameSource frameSource, int webcamIndex) {
    setFrameSource(frameSource, QString(), webcamIndex);
}

void Model::setFrameSource(FrameSource frameSource, QString const& fileName) {
    setFrameSource(frameSource, fileName, -1);
}

void Model::setFrameSource(FrameSource frameSource, QString const& fileName, int webcamIndex) {
    _gettingFrameStrategy->releaseResources();
    _gettingFrameStrategy = _gettingFrameStrategyFactory->CreateStrategy(frameSource, fileName, webcamIndex);
    _gettingFrameStrategy->initialize();
}

void Model::update() {
    notify();
}

void Model::setObjectDetectionAlgorithm(ObjectDetectionAlgorithm algorithm) {
    _objectDetectionStrategy->releaseResources();
    _objectDetectionStrategy = _objectDetectionStrategyFactory->createStrategy(algorithm);
    _objectDetectionStrategy->initialize();
}

std::vector<cv::Rect> Model::detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const {
    return _objectDetectionStrategy->detect(frame, objectType);
}
