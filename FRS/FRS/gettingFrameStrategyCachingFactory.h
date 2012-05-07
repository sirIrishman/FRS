#ifndef FRS_GETTINGFRAMESTRATEGYCACHINGFACTORY_H
#define FRS_GETTINGFRAMESTRATEGYCACHINGFACTORY_H

#include "exceptions.h"
#include "enums.h"
#include "gettingFrameStrategy.h"

using namespace Framework;

namespace FRS {
    namespace Native {
        class GettingFrameStrategyCachingFactory sealed {
        public:
            GettingFrameStrategyCachingFactory(Observer* const& observer) {
                _observer = observer;
                _emptyStrategy = NULL;
                _imageFileStrategy = NULL;
                _videoFileStrategy = NULL;
                _webcamImageStrategy = NULL;
                _webcamVideoStrategy = NULL;
            }
            ~GettingFrameStrategyCachingFactory() {
                if(_emptyStrategy != NULL)
                    delete _emptyStrategy;
                if(_imageFileStrategy != NULL)
                    delete _imageFileStrategy;
                if(_videoFileStrategy != NULL)
                    delete _videoFileStrategy;
                if(_webcamImageStrategy != NULL)
                    delete _webcamImageStrategy;
                if(_webcamVideoStrategy != NULL)
                    delete _webcamVideoStrategy;
            }

            GettingFrameStrategyBase* CreateStrategy(FrameSource frameSource, QString const& fileName, int webcamIndex) {
                switch(frameSource) {
                case ImageFile: 
                    return imageFileStrategy(fileName);
                case VideoFile: 
                    return videoFileStrategy(fileName);
                case ImageWebcam:
                    return webcamImageStrategy(webcamIndex);
                case VideoWebcam:
                    return webcamVideoStrategy(webcamIndex);
                default:
                    return emptyStrategy();
                }
            }
            GettingFrameEmptyStrategy* CreateEmptyStrategy() {
                return emptyStrategy();
            }

        private:
            Observer* _observer;
            GettingFrameEmptyStrategy* _emptyStrategy;
            GettingImageFileFrameStrategy* _imageFileStrategy;
            GettingVideoFileFrameStrategy* _videoFileStrategy;
            GettingWebcamVideoFrameStrategy* _webcamVideoStrategy;
            GettingWebcamImageFrameStrategy* _webcamImageStrategy;

            GettingFrameEmptyStrategy* emptyStrategy() {
                return (_emptyStrategy != NULL) 
                    ? _emptyStrategy
                    : (_emptyStrategy = new GettingFrameEmptyStrategy(_observer));
            }
            GettingImageFileFrameStrategy* imageFileStrategy(QString const& fileName) {
                GettingImageFileFrameStrategy* frameStrategy = (_imageFileStrategy != NULL) 
                    ? _imageFileStrategy 
                    : (_imageFileStrategy = new GettingImageFileFrameStrategy(_observer));
                frameStrategy->setFileName(fileName);
                return frameStrategy;
            }
            GettingVideoFileFrameStrategy* videoFileStrategy(QString const& fileName) {
                GettingVideoFileFrameStrategy* frameStrategy = (_videoFileStrategy != NULL) 
                    ? _videoFileStrategy 
                    : (_videoFileStrategy = new GettingVideoFileFrameStrategy(_observer));
                frameStrategy->setFileName(fileName);
                return frameStrategy;
            }
            GettingWebcamVideoFrameStrategy* webcamVideoStrategy(int webcamIndex) {
                GettingWebcamVideoFrameStrategy* frameStrategy = (_webcamVideoStrategy != NULL) 
                    ? _webcamVideoStrategy 
                    : (_webcamVideoStrategy = new GettingWebcamVideoFrameStrategy(_observer));
                frameStrategy->setActiveWebcamIndex(webcamIndex);
                return frameStrategy;
            }
            GettingWebcamImageFrameStrategy* webcamImageStrategy(int webcamIndex) {
                GettingWebcamImageFrameStrategy* frameStrategy = (_webcamImageStrategy != NULL) 
                    ? _webcamImageStrategy 
                    : (_webcamImageStrategy = new GettingWebcamImageFrameStrategy(_observer));
                frameStrategy->setActiveWebcamIndex(webcamIndex);
                return frameStrategy;
            }
        };
    }
}

#endif //FRS_GETTINGFRAMESTRATEGYCACHINGFACTORY_H
