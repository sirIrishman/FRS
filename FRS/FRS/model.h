#ifndef FRS_MODEL_H
#define FRS_MODEL_H

#include <qstring.h>
#include "observer.h"
#include "enums.h"
#include "objectDetectionStrategyCachingFactory.h"
#include "gettingFrameStrategyCachingFactory.h"

using namespace Patterns;
using namespace FRS::Native;

namespace FRS {
    class Model : public ObservableSubject, private Observer {
    public:
        Model();
        ~Model();

        void setFrameSource(FrameSource frameSource, int webcamIndex);
        void setFrameSource(FrameSource frameSource, QString const& fileName);
        void setObjectDetectionAlgorithm(ObjectDetectionAlgorithm algorithm);
        cv::Mat frame() const;
        int webcamCount() const;
        std::vector<cv::Rect> detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const;

    private:
        GettingFrameStrategyCachingFactory* _gettingFrameStrategyFactory;
        GettingFrameStrategyBase* _gettingFrameStrategy;
        ObjectDetectionStrategyCachingFactory* _objectDetectionStrategyFactory;
        ObjectDetectionStrategyBase* _objectDetectionStrategy;

        void setFrameSource(FrameSource frameSource, QString const& fileName, int webcamIndex);
        void update();
    };
}

#endif //FRS_MODEL_H
