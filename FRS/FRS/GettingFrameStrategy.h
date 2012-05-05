#ifndef FRS_GETTINGFRAMESTRATEGY_H
#define FRS_GETTINGFRAMESTRATEGY_H

#include <Windows.h>
#include <qtimer.h>
#include <highgui\highgui.hpp>
#include "observer.h"

using namespace Patterns;

namespace FRS {
    class GettingFrameStrategyBase : protected ObservableSubject, protected QObject {
    public:
        virtual ~GettingFrameStrategyBase() {
            detach(_observer);
        }

        virtual void initialize() = 0;
        virtual IplImage* frame() = 0;
        virtual void releaseResources() = 0;

    protected: 
        GettingFrameStrategyBase(Observer* const& observer) {
            _observer = observer;
            attach(_observer);
        }

    private:
        Observer* _observer;
    };

    class GettingFrameEmptyStrategy : public GettingFrameStrategyBase {
    public:
        GettingFrameEmptyStrategy(Observer* const& observer) : GettingFrameStrategyBase(observer) { }

        virtual void initialize() { }
        virtual IplImage* frame() { return NULL; }
        virtual void releaseResources() { }
    };

    class GettingCapturedFrameStrategyBase : public GettingFrameStrategyBase {
    public:
        ~GettingCapturedFrameStrategyBase() {
            releaseResources();
        }

        virtual IplImage* frame() {
            if(_capture == NULL)
                return NULL;
            return cvQueryFrame(_capture);
        }
        virtual void releaseResources() {
            if(_capture != NULL) {
                cvReleaseCapture(&_capture);
                _capture = NULL;
            }
        }

    protected:
        CvCapture* _capture;

        GettingCapturedFrameStrategyBase(Observer* const& observer) 
            : GettingFrameStrategyBase(observer) {
                _capture = NULL;
        }

        void setCapture(CvCapture* const& capture) {
            _capture = capture;
        }
    };

    class GettingCapturedVideoFrameStrategyBase : public GettingCapturedFrameStrategyBase {
        Q_OBJECT
    public:
        ~GettingCapturedVideoFrameStrategyBase() {
            releaseResources();
            delete _timer;
        }

        virtual void initialize() {
            if(_capture != NULL)
                startTimer();
        }
        virtual void releaseResources() {
            stopTimer();
            GettingCapturedFrameStrategyBase::releaseResources();
        }

    protected:
        GettingCapturedVideoFrameStrategyBase(Observer* const& observer) 
            : GettingCapturedFrameStrategyBase(observer), __fps(30), __msecInSec(1000) {
                _timer = new QTimer();
                connect(_timer, SIGNAL(timeout()), this, SLOT(tick()));
        }

    private:
        const int __fps;
        const int __msecInSec;
        QTimer* _timer;

        void startTimer() const {
            if(!_timer->isActive())
                _timer->start(interval());
        }
        void stopTimer() const {
            if(_timer->isActive())
                _timer->stop();
        }
        int interval() const {
            return __msecInSec / __fps;
        }

    private slots:
        void tick() const {
            notify();
        }
    };

    class GettingImageFileFrameStrategy : public GettingFrameStrategyBase {
    public:
        GettingImageFileFrameStrategy(Observer* observer) 
            : GettingFrameStrategyBase(observer) {
                _frame = NULL;
        }
        ~GettingImageFileFrameStrategy() {
            releaseResources();
        }

        virtual void initialize() { 
            notify();
        }
        virtual IplImage* frame() {
            if(_fileName.isNull() || _fileName.isEmpty())
                return NULL;
            _frame = cvLoadImage(_fileName.toUtf8().constData());
            return _frame;
        }
        virtual void releaseResources() {
            if(_frame != NULL) {
                cvReleaseImage(&_frame);
                _frame = NULL;
            }
        }
        void setFileName(QString const& fileName) {
            _fileName = fileName;
        }

    private:
        IplImage* _frame;
        QString _fileName;
    };

    class GettingVideoFileFrameStrategy : public GettingCapturedVideoFrameStrategyBase {
    public:
        GettingVideoFileFrameStrategy(Observer* const& observer) 
            : GettingCapturedVideoFrameStrategyBase(observer) {
        }

        virtual void initialize() { 
            if(_fileName.isNull() || _fileName.isEmpty())
                return;
            setCapture(cvCaptureFromFile(_fileName.toUtf8().constData()));
            GettingCapturedVideoFrameStrategyBase::initialize();
        }
        void setFileName(QString const& fileName) {
            _fileName = fileName;
        }

    private:
        QString _fileName;
    };

    class GettingWebcamVideoFrameStrategy : public GettingCapturedVideoFrameStrategyBase {
    public:
        GettingWebcamVideoFrameStrategy(Observer* const& observer) 
            : GettingCapturedVideoFrameStrategyBase(observer) {
                _activeWebcamIndex = -1;
        }

        virtual void initialize() { 
            setCapture(cvCaptureFromCAM(_activeWebcamIndex));
            GettingCapturedVideoFrameStrategyBase::initialize();
        }
        void setActiveWebcamIndex(int webcamIndex) {
            _activeWebcamIndex = webcamIndex;
        }

    private:
        int _activeWebcamIndex;
    };

    class GettingWebcamImageFrameStrategy : public GettingCapturedFrameStrategyBase {
    public:
        GettingWebcamImageFrameStrategy(Observer* const& observer) 
            : GettingCapturedFrameStrategyBase(observer), __webcamInitTimeInMsec(300) {
                _activeWebcamIndex = -1;
        }

        virtual void initialize() { 
            setCapture(cvCaptureFromCAM(_activeWebcamIndex));
            Sleep(__webcamInitTimeInMsec); //waiting for a webcam initializing
            notify();
        }
        void setActiveWebcamIndex(int webcamIndex) {
            _activeWebcamIndex = webcamIndex;
        }

    private:
        const int __webcamInitTimeInMsec;
        int _activeWebcamIndex;
    };
}

#endif //FRS_GETTINGFRAMESTRATEGY_H