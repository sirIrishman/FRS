#ifndef FRS_MODEL_H
#define FRS_MODEL_H

#include <qstring.h>
#include <core\core.hpp>
#include "observer.h"
#include "enums.h"
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
        cv::Mat frame() const;
        int webcamCount() const;

    private:
        GettingFrameStrategyCachingFactory* _frameStrategyFactory;
        GettingFrameStrategyBase* _activeGettingFrameStrategy;

        void setFrameSource(FrameSource frameSource, QString const& fileName, int webcamIndex);
        void update();
    };
}

#endif //FRS_MODEL_H
