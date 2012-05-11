#ifndef FRS_GETTINGFRAMESTRATEGY_H
#define FRS_GETTINGFRAMESTRATEGY_H

#include <Windows.h>
#include <qtimer.h>
#include <core\core.hpp>
#include <highgui\highgui.hpp>
#include "observer.h"
#include "exceptions.h"

using namespace Patterns;
using namespace Framework;

namespace FRS {
    namespace Native {
        class GettingFrameStrategyBase : protected ObservableSubject, protected QObject {
        public:
            GettingFrameStrategyBase(Observer* const& observer) {
                _observer = observer;
                attach(_observer);
            }
            virtual ~GettingFrameStrategyBase() {
                detach(_observer);
            }

            virtual void initialize() = 0;
            virtual cv::Mat frame() = 0;
            virtual void releaseResources() = 0;

        private:
            Observer* _observer;
        };

        class GettingCapturedFrameStrategyBase : public GettingFrameStrategyBase {
        public:
            ~GettingCapturedFrameStrategyBase() {
                releaseResources();
            }

            virtual cv::Mat frame() {
                cv::Mat frame;
                if(_capture.isOpened()) {
                    _capture.read(frame);
                    if(frame.empty())
                        onEmptyFrameRead();
                }
                return frame;
            }
            virtual void releaseResources() {
                if(_capture.isOpened())
                    _capture.release();
            }
            void setActiveWebcamIndex(int webcamIndex) {
                _activeWebcamIndex = webcamIndex;
            }
            void setFileName(QString const& fileName) {
                _fileName = fileName;
            }

        protected:
            cv::VideoCapture _capture;

            GettingCapturedFrameStrategyBase(Observer* const& observer) 
                : GettingFrameStrategyBase(observer) {
                    _activeWebcamIndex = -1;
            }

            virtual void onEmptyFrameRead() const = 0;
            void openWebcamCapture() {
                if(_capture.isOpened() == false)
                    _capture.open(_activeWebcamIndex);
                if(_capture.isOpened() == false)
                    ArgumentException(QString("Active webcam device with %1 index was not found").arg(QString::number(_activeWebcamIndex))).raise();
            }
            void openVideoFileCapture() {
                if(_fileName.isNull() || _fileName.isEmpty())
                    return;
                if(_capture.isOpened() == false)
                    _capture.open(_fileName.toStdString());
                if(_capture.isOpened() == false)
                    ArgumentException(QString("Can not open '%1' video file").arg(_fileName)).raise();
            }

        private:
            int _activeWebcamIndex;
            QString _fileName;
        };

        class GettingCapturedVideoFrameStrategyBase : public GettingCapturedFrameStrategyBase {
            Q_OBJECT
        public:
            ~GettingCapturedVideoFrameStrategyBase() {
                releaseResources();
                delete _timer;
            }

            virtual void initialize() {
                if(_capture.isOpened())
                    startTimer();
            }
            virtual void releaseResources() {
                stopTimer();
                GettingCapturedFrameStrategyBase::releaseResources();
            }

        protected:
            GettingCapturedVideoFrameStrategyBase(Observer* const& observer) 
                : GettingCapturedFrameStrategyBase(observer), __fps(24), __msecInSec(1000) {
                    _timer = new QTimer();
                    connect(_timer, SIGNAL(timeout()), this, SLOT(tick()));
            }

            void onEmptyFrameRead() const {
                stopTimer();
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

        class GettingFrameEmptyStrategy sealed : public GettingFrameStrategyBase {
        public:
            GettingFrameEmptyStrategy(Observer* const& observer) : GettingFrameStrategyBase(observer) { }

            virtual void initialize() { }
            virtual cv::Mat frame() { return cv::Mat(); }
            virtual void releaseResources() { }
        };

        class GettingImageFileFrameStrategy : public GettingFrameStrategyBase {
        public:
            GettingImageFileFrameStrategy(Observer* observer) 
                : GettingFrameStrategyBase(observer) {
            }
            ~GettingImageFileFrameStrategy() {
                releaseResources();
            }

            virtual void initialize() { 
                notify();
            }
            virtual cv::Mat frame() {
                if(_fileName.isNull() || _fileName.isEmpty())
                    return cv::Mat();
                cv::Mat frame = cv::imread(_fileName.toStdString());
                if(frame.empty())
                    ArgumentException(QString("Can not open '%1' image file").arg(_fileName)).raise();
                return frame;
            }
            virtual void releaseResources() {
            }
            void setFileName(QString const& fileName) {
                _fileName = fileName;
            }

        private:
            QString _fileName;
        };

        class GettingVideoFileFrameStrategy : public GettingCapturedVideoFrameStrategyBase {
        public:
            GettingVideoFileFrameStrategy(Observer* const& observer) 
                : GettingCapturedVideoFrameStrategyBase(observer) {
            }

            virtual void initialize() { 
                openVideoFileCapture();
                GettingCapturedVideoFrameStrategyBase::initialize();
            }
        };

        class GettingWebcamVideoFrameStrategy : public GettingCapturedVideoFrameStrategyBase {
        public:
            GettingWebcamVideoFrameStrategy(Observer* const& observer) 
                : GettingCapturedVideoFrameStrategyBase(observer) {
            }

            virtual void initialize() { 
                openWebcamCapture();
                GettingCapturedVideoFrameStrategyBase::initialize();
            }
        };

        class GettingWebcamImageFrameStrategy : public GettingCapturedFrameStrategyBase {
        public:
            GettingWebcamImageFrameStrategy(Observer* const& observer) 
                : GettingCapturedFrameStrategyBase(observer), __webcamInitializingTimeInMsec(300) {
            }

            virtual void initialize() { 
                openWebcamCapture();
                Sleep(__webcamInitializingTimeInMsec); //waiting for a webcam initializing
                notify();
                releaseResources();
            }

        protected:
            void onEmptyFrameRead() const {
            }

        private:
            const int __webcamInitializingTimeInMsec;
        };
    }
}

#endif //FRS_GETTINGFRAMESTRATEGY_H
