#ifndef FRS_CONTROLLER_H
#define FRS_CONTROLLER_H

#include <QtCore/qstring.h>
#include "model.h"

namespace frs {
    class Controller {
    public:
        Controller(Model* const& model);

        void loadImage(QString const& filePath);
        void loadVideo(QString const& filePath);
        void captureVideo(int webcamIndex);
        void captureImage(int webcamIndex);
        void setCurrentObjectDetectionAlgorithm(ObjectDetectionAlgorithm algorithm);
        void setCurrentFaceRecognitionAlgorithm(FaceRecognitionAlgorithm algorithm);
        void setFaceRecognitionTrainingName(QString const& name);
        void trainFaceRecognition(TrainingData const& trainingData);
        void saveFaceRecognitionState() const;
        void loadFaceRecognitionState(QString const& faceRecognitionTrainingName) const;

    private:
        Model* _model;
    };
}

#endif //FRS_CONTROLLER_H
