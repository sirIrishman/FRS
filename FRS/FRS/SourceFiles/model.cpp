#include "model.h"

using namespace frs;
using namespace frs::native;

Model::Model() {
    _gettingFrameStrategyFactory = new GettingFrameStrategyCachingFactory(this);
    _gettingFrameStrategy = _gettingFrameStrategyFactory->CreateEmptyStrategy();
    _objectDetectionStrategyFactory = new ObjectDetectionStrategyCachingFactory();
    _objectDetectionStrategy = _objectDetectionStrategyFactory->createEmptytStrategy();
    _faceRecognitionStrategyFactory = new FaceRecognitionStrategyCachingFactory();
    _faceRecognitionStrategy = _faceRecognitionStrategyFactory->createEmptyStrategy();
}

Model::~Model() {
    delete _gettingFrameStrategyFactory;
    delete _objectDetectionStrategyFactory;
    delete _faceRecognitionStrategyFactory;
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

void Model::setCurrentObjectDetectionAlgorithm(ObjectDetectionAlgorithm algorithm) {
    _objectDetectionStrategy = _objectDetectionStrategyFactory->createStrategy(algorithm);
}

std::vector<cv::Rect> Model::detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const {
    return _objectDetectionStrategy->detect(frame, objectType);
}

void Model::setCurrentFaceRecognitionAlgorithm(FaceRecognitionAlgorithm algorithm) {
    _faceRecognitionStrategy = _faceRecognitionStrategyFactory->createStrategy(algorithm);
}

void Model::setFaceRecognitionTrainingName(QString const& name) {
    _faceRecognitionStrategyFactory->createStrategy(Eigenfaces)->setName(name);
    _faceRecognitionStrategyFactory->createStrategy(Fisherfaces)->setName(name);
    _faceRecognitionStrategyFactory->createStrategy(Lbph)->setName(name);
}

void Model::trainFaceRecognition(TrainingData const& trainingData) {
    _faceRecognitionStrategyFactory->createStrategy(Eigenfaces)->train(trainingData);
    _faceRecognitionStrategyFactory->createStrategy(Fisherfaces)->train(trainingData);
    _faceRecognitionStrategyFactory->createStrategy(Lbph)->train(trainingData);
}

bool Model::faceRecognitionTrained() const {
    return _faceRecognitionStrategy->trained();
}

int Model::recognizeFace(cv::Mat const& image) const {
    return _faceRecognitionStrategy->recognize(image);
}

void Model::saveFaceRecognitionState() const {
    _faceRecognitionStrategyFactory->createStrategy(Eigenfaces)->save();
    _faceRecognitionStrategyFactory->createStrategy(Fisherfaces)->save();
    _faceRecognitionStrategyFactory->createStrategy(Lbph)->save();
}

void Model::loadFaceRecognitionState(QString const& faceRecognitionTrainingName) const {
    _faceRecognitionStrategyFactory->createStrategy(Eigenfaces)->load(faceRecognitionTrainingName);
    _faceRecognitionStrategyFactory->createStrategy(Fisherfaces)->load(faceRecognitionTrainingName);
    _faceRecognitionStrategyFactory->createStrategy(Lbph)->load(faceRecognitionTrainingName);
}

FaceRecognitionAlgorithm Model::currentFaceRecognitionAlgorithm() const {
    return _faceRecognitionStrategy->algorithm();
}
