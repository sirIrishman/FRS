#ifndef FRS_GETTINGFRAMESTRATEGY_H
#define FRS_GETTINGFRAMESTRATEGY_H

#include <Windows.h>
#include <qtimer.h>
#include <core\core.hpp>
#include <highgui\highgui.hpp>
#include "observer.h"

using namespace Patterns;

namespace FRS {
    namespace Native {
        class GettingFrameStrategyBase : protected ObservableSubject, protected QObject {
        public:
            virtual ~GettingFrameStrategyBase() {
                detach(_observer);
            }

            virtual void initialize() = 0;
            virtual cv::Mat frame() = 0;
            virtual void releaseResources() = 0;

        protected: 
            GettingFrameStrategyBase(Observer* const& observer) {
                _observer = observer;
                attach(_observer);
            }

        private:
            Observer* _observer;
        };

        class GettingCapturedFrameStrategyBase : public GettingFrameStrategyBase {
        public:
            ~GettingCapturedFrameStrategyBase() {
                releaseResources();
            }

            virtual cv::Mat frame() {
                cv::Mat frame = cv::Mat();
                if(_capture.isOpened())
                    _capture.read(frame);
                return frame;
            }
            virtual void releaseResources() {
                if(_capture.isOpened())
                    _capture.release();
            }

        protected:
            cv::VideoCapture _capture;

            GettingCapturedFrameStrategyBase(Observer* const& observer) 
                : GettingFrameStrategyBase(observer) {
            }

            void openCapture(int webcamIndex) {
                if(_capture.isOpened() == false)
                    _capture.open(webcamIndex);
            }
            void openCapture(std::string fileName) {
                if(_capture.isOpened() == false)
                    _capture.open(fileName);
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
                if(_capture.isOpened())
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

        class GettingFrameEmptyStrategy : public GettingFrameStrategyBase {
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
                return cv::imread(_fileName.toStdString());
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
                if(_fileName.isNull() || _fileName.isEmpty())
                    return;
                openCapture(_fileName.toStdString());
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
                openCapture(_activeWebcamIndex);
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
                openCapture(_activeWebcamIndex);
                Sleep(__webcamInitTimeInMsec); //waiting for a webcam initializing
                notify();
                releaseResources();
            }
            void setActiveWebcamIndex(int webcamIndex) {
                _activeWebcamIndex = webcamIndex;
            }

        private:
            const int __webcamInitTimeInMsec;
            int _activeWebcamIndex;
        };
    }
}

#endif //FRS_GETTINGFRAMESTRATEGY_H