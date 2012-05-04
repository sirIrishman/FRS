#ifndef FRS_MODEL_H
#define FRS_MODEL_H

#include <qstring.h>
#include <highgui\highgui.hpp>
#include "Observer.h"
#include "Enums.h"
#include "GettingFrameStrategyCachingFactory.h"

using namespace Patterns;

namespace FRS {
    class Model : public ObservableSubject, private Observer {
    public:
        Model();
        ~Model();

        void setFrameSource(FrameSource frameSource, int webcamIndex);
        void setFrameSource(FrameSource frameSource, QString const& fileName);
        IplImage* frame();
        int webcamCount() const;

    private:
        GettingFrameStrategyCachingFactory* _frameStrategyFactory;
        GettingFrameStrategyBase* _activeGettingFrameStrategy;

        void setFrameSource(FrameSource frameSource, QString const& fileName, int webcamIndex);
        void update();
    };
}

#endif //FRS_MODEL_H
