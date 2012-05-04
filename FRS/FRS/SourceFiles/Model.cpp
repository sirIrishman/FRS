#include "Model.h"

using namespace FRS;

Model::Model() {
    _frameStrategyFactory = new GettingFrameStrategyCachingFactory(this);
    _activeGettingFrameStrategy = _frameStrategyFactory->CreateEmptyStrategy();
}

Model::~Model() {
    delete _frameStrategyFactory;
}

IplImage* Model::frame() {
    return _activeGettingFrameStrategy->frame();
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
    _activeGettingFrameStrategy->releaseResources();
    _activeGettingFrameStrategy = _frameStrategyFactory->CreateStrategy(frameSource, fileName, webcamIndex);
    _activeGettingFrameStrategy->initialize();
}

void Model::update() {
    notify();
}
