#ifndef FRS_GETTINGFRAMESTRATEGYCACHINGFACTORY_H
#define FRS_GETTINGFRAMESTRATEGYCACHINGFACTORY_H

#include "Exceptions.h"
#include "Enums.h"
#include "GettingFrameStrategy.h"

using namespace Framework;

namespace FRS {
    class GettingFrameStrategyCachingFactory {
    public:
        GettingFrameStrategyCachingFactory(Observer* const& observer) {
            _observer = observer;
            _frameEmptyStrategy = NULL;
            _imageFileFrameStrategy = NULL;
            _videoFileFrameStrategy = NULL;
            _webcamImageFrameStrategy = NULL;
            _webcamVideoFrameStrategy = NULL;
        }
        ~GettingFrameStrategyCachingFactory() {
            if(_frameEmptyStrategy != NULL)
                delete _frameEmptyStrategy;
            if(_imageFileFrameStrategy != NULL)
                delete _imageFileFrameStrategy;
            if(_videoFileFrameStrategy != NULL)
                delete _videoFileFrameStrategy;
            if(_webcamImageFrameStrategy != NULL)
                delete _webcamImageFrameStrategy;
            if(_webcamVideoFrameStrategy != NULL)
                delete _webcamVideoFrameStrategy;
        }

        GettingFrameStrategyBase* CreateStrategy(FrameSource frameSource, QString const& fileName, int webcamIndex) {
            switch(frameSource) {
            case ImageFile: 
                return imageFileFrameStrategy(fileName);
            case VideoFile: 
                return videoFileFrameStrategy(fileName);
            case ImageWebcam:
                return webcamImageFrameStrategy(webcamIndex);
            case VideoWebcam:
                return webcamVideoFrameStrategy(webcamIndex);
            default:
                return frameEmptyStrategy();
            }
        }
        GettingFrameEmptyStrategy* CreateEmptyStrategy() {
            return frameEmptyStrategy();
        }

    private:
        Observer* _observer;
        GettingFrameEmptyStrategy* _frameEmptyStrategy;
        GettingImageFileFrameStrategy* _imageFileFrameStrategy;
        GettingVideoFileFrameStrategy* _videoFileFrameStrategy;
        GettingWebcamVideoFrameStrategy* _webcamVideoFrameStrategy;
        GettingWebcamImageFrameStrategy* _webcamImageFrameStrategy;

        GettingFrameEmptyStrategy* frameEmptyStrategy() {
            return (_frameEmptyStrategy != NULL) 
                ? _frameEmptyStrategy
                : (_frameEmptyStrategy = new GettingFrameEmptyStrategy(_observer));
        }
        GettingImageFileFrameStrategy* imageFileFrameStrategy(QString const& fileName) {
            GettingImageFileFrameStrategy* frameStrategy = (_imageFileFrameStrategy != NULL) 
                ? _imageFileFrameStrategy 
                : (_imageFileFrameStrategy = new GettingImageFileFrameStrategy(_observer));
            frameStrategy->setFileName(fileName);
            return frameStrategy;
        }
        GettingVideoFileFrameStrategy* videoFileFrameStrategy(QString const& fileName) {
            GettingVideoFileFrameStrategy* frameStrategy = (_videoFileFrameStrategy != NULL) 
                ? _videoFileFrameStrategy 
                : (_videoFileFrameStrategy = new GettingVideoFileFrameStrategy(_observer));
            frameStrategy->setFileName(fileName);
            return frameStrategy;
        }
        GettingWebcamVideoFrameStrategy* webcamVideoFrameStrategy(int webcamIndex) {
            GettingWebcamVideoFrameStrategy* frameStrategy = (_webcamVideoFrameStrategy != NULL) 
                ? _webcamVideoFrameStrategy 
                : (_webcamVideoFrameStrategy = new GettingWebcamVideoFrameStrategy(_observer));
            frameStrategy->setActiveWebcamIndex(webcamIndex);
            return frameStrategy;
        }
        GettingWebcamImageFrameStrategy* webcamImageFrameStrategy(int webcamIndex) {
            GettingWebcamImageFrameStrategy* frameStrategy = (_webcamImageFrameStrategy != NULL) 
                ? _webcamImageFrameStrategy 
                : (_webcamImageFrameStrategy = new GettingWebcamImageFrameStrategy(_observer));
            frameStrategy->setActiveWebcamIndex(webcamIndex);
            return frameStrategy;
        }
    };
}

#endif //FRS_GETTINGFRAMESTRATEGYCACHINGFACTORY_H
