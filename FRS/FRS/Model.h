#include <qimage.h>
#include <qstring.h>
#include <qtimer.h>
#include <highgui\highgui.hpp>
#include <core\core.hpp>
#include "Observer.h"
#include "Enums.h"

using namespace Patterns;

namespace FRS {
    class Model : public QObject, public ObservableSubject {
        Q_OBJECT
    public:
        Model();
        ~Model();

        void setSource(FrameSource const& frameSource);
        void setSource(FrameSource const& frameSource, QString const& filePath);
        IplImage* frame() const;

    private:
        const int __fps;
        const int __msecInSec;
        FrameSource _frameSource;
        QString _filePath;
        QTimer* _timer;
        //IplImage* _frame;
        CvCapture* _capture;

        int interval() const;
        void startTimer();
        void stopTimer();

        void setVideoSource(CvCapture* capture);
        void unsetVideoSource();

    private slots:
        void tick();
    };
}