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

QStringList Model::getFaceRecognitionMethodStateFileNameList() const {
    QList<QStringList> stateFileNamesList;
    stateFileNamesList.append(_faceRecognitionStrategyFactory->createStrategy(Eigenfaces)->stateFileNameList());
    stateFileNamesList.append(_faceRecognitionStrategyFactory->createStrategy(Fisherfaces)->stateFileNameList());
    stateFileNamesList.append(_faceRecognitionStrategyFactory->createStrategy(Lbph)->stateFileNameList());
    if(stateFileNamesList.size() == 1 || areStringListCollectionEquals(stateFileNamesList))
        return stateFileNamesList.first();
    int maxSizeIndex = maxSizeStringListIndex(stateFileNamesList);
    QStringList maxSizeStringList = stateFileNamesList.takeAt(maxSizeIndex);
    QStringList stateFileNames;
    for(int i = 0; i < maxSizeStringList.size(); i++){
        if(allStringListContainsValue(stateFileNamesList, maxSizeStringList[i]))
            stateFileNames.append(maxSizeStringList[i]);
    }
    return stateFileNames;
}

bool Model::areStringListCollectionEquals(QList<QStringList> const& stringListCollection) const {
    for(int i = 0; i < stringListCollection.size() - 1; i++)
        if(stringListCollection[i] != stringListCollection[i + 1])
            return false;
    return true;
}

int Model::maxSizeStringListIndex(QList<QStringList> const& stringListCollection) const {
    int maxSize = 0;
    int maxSizeIndex = 0;
    for(int i = 0; i < stringListCollection.size(); i++)
        if(stringListCollection[i].size() > maxSize){
            maxSize = stringListCollection[i].size();
            maxSizeIndex = i;
        }
    return maxSizeIndex;
}

bool Model::allStringListContainsValue(QList<QStringList> const& stringListCollection, QString const& value) const {
    for(int i = 0; i < stringListCollection.size(); i++)
        if(stringListCollection[i].contains(value) == false)
            return false;
    return true;
}

FaceRecognitionAlgorithm Model::currentFaceRecognitionAlgorithm() const {
    return _faceRecognitionStrategy->algorithm();
}

QString Model::commonFaceRecognitionDirectoryPath() const {
    return _faceRecognitionStrategy->commonFaceRecognitionDirectoryPath();
}
