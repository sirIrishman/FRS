#ifndef FRS_MODEL_H
#define FRS_MODEL_H

#include <vector>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include "observer.h"
#include "enums.h"
#include "gettingFrameStrategyCachingFactory.h"
#include "objectDetectionStrategyCachingFactory.h"
#include "faceRecognitionStrategyCachingFactory.h"

using namespace patterns;
using namespace frs::native;

namespace frs {
    class Model : public ObservableSubject, private Observer {
    public:
        Model();
        ~Model();

        cv::Mat frame() const;
        int webcamCount() const;
        //setting frame source
        void setFrameSource(FrameSource frameSource, int webcamIndex);
        void setFrameSource(FrameSource frameSource, QString const& fileName);
        //object detection
        void setCurrentObjectDetectionAlgorithm(ObjectDetectionAlgorithm algorithm);
        std::vector<cv::Rect> detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const;
        //face recognition
        void setCurrentFaceRecognitionAlgorithm(FaceRecognitionAlgorithm algorithm);
        void setFaceRecognitionTrainingName(QString const& name);
        void trainFaceRecognition(TrainingData const& trainingData);
        bool faceRecognitionTrained() const;
        int recognizeFace(cv::Mat const& image) const;
        void saveFaceRecognitionState() const;
        void loadFaceRecognitionState(QString const& faceRecognitionTrainingName) const;
        QStringList getFaceRecognitionMethodStateFileNameList() const;
        FaceRecognitionAlgorithm currentFaceRecognitionAlgorithm() const;
        QString commonFaceRecognitionDirectoryPath() const;

    private:
        GettingFrameStrategyCachingFactory* _gettingFrameStrategyFactory;
        GettingFrameStrategyBase* _gettingFrameStrategy;
        ObjectDetectionStrategyCachingFactory* _objectDetectionStrategyFactory;
        ObjectDetectionStrategyBase* _objectDetectionStrategy;
        FaceRecognitionStrategyCachingFactory* _faceRecognitionStrategyFactory;
        FaceRecognitionStrategyBase* _faceRecognitionStrategy;

        void setFrameSource(FrameSource frameSource, QString const& fileName, int webcamIndex);
        void update();
        bool areStringListCollectionEquals(QList<QStringList> const& stringListCollection) const;
        int maxSizeStringListIndex(QList<QStringList> const& stringListCollection) const;
        bool allStringListContainsValue(QList<QStringList> const& stringListCollection, QString const& value) const;
    };
}

#endif //FRS_MODEL_H
