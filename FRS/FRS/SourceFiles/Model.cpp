#include "model.h"

using namespace FRS;
using namespace FRS::Native;

Model::Model() {
    _gettingFrameStrategyFactory = new GettingFrameStrategyCachingFactory(this);
    _gettingFrameStrategy = _gettingFrameStrategyFactory->CreateEmptyStrategy();
    _reconitionStrategyFactory = new RecognitionStrategyCachingFactory();
    _recognitionStrategy = _reconitionStrategyFactory->createEmptytStrategy();
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

void Model::setRecognitionAlgorithm(RecognitionAlgorithm algorithm) {
    _recognitionStrategy->releaseResources();
    _recognitionStrategy = _reconitionStrategyFactory->createStrategy(algorithm);
    _recognitionStrategy->initialize();
}

std::vector<cv::Rect> Model::recognizeObjects(cv::Mat const& frame, RecognizableObjectType objectType) const {
    return _recognitionStrategy->recognize(frame, objectType);
}
