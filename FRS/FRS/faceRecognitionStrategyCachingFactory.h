#ifndef FRS_FACERECOGNITIONSTRATEGYCACHINGFACTORY_H
#define FRS_FACERECOGNITIONSTRATEGYCACHINGFACTORY_H

#include "faceRecognitionStrategy.h"
#include "enums.h"

namespace frs {
    namespace native {
        class FaceRecognitionStrategyCachingFactory sealed {
        public:
            FaceRecognitionStrategyCachingFactory() {
                _eigenFaceStrategy = NULL;
                _fisherFaceStrategy = NULL;
                _lbphStrategy = NULL;
                _emptyStrategy = NULL;
            }
            ~FaceRecognitionStrategyCachingFactory() {
                if(_eigenFaceStrategy != NULL) {
                    delete _eigenFaceStrategy;
                    _eigenFaceStrategy = NULL;
                }
                if(_fisherFaceStrategy != NULL) {
                    delete _fisherFaceStrategy;
                    _fisherFaceStrategy = NULL;
                }
                if(_lbphStrategy != NULL) {
                    delete _lbphStrategy;
                    _lbphStrategy = NULL;
                }
                if(_emptyStrategy != NULL) {
                    delete _emptyStrategy;
                    _emptyStrategy = NULL;
                }
            }

            FaceRecognitionStrategyBase* createEmptyStrategy() {
                return emptyStrategy();
            }
            FaceRecognitionStrategyBase* createStrategy(FaceRecognitionAlgorithm faceRecognitionAlgorithm) {
                switch(faceRecognitionAlgorithm) {
                case Eigenfaces:
                    return eigenFaceStrategy();
                case Fisherfaces:
                    return fisherFaceStrategy();
                case Lbph:
                    return lbphStrategy();
                default:
                    return emptyStrategy();
                }
            }

        private:
            EigenFaceRecognitionStrategy* _eigenFaceStrategy;
            FisherFaceRecognitionStrategy* _fisherFaceStrategy;
            LbphFaceRecognitionStrategy* _lbphStrategy;
            EmptyFaceRecognitionStrategy* _emptyStrategy;

            EigenFaceRecognitionStrategy* eigenFaceStrategy() {
                return _eigenFaceStrategy != NULL 
                    ? _eigenFaceStrategy
                    : (_eigenFaceStrategy = new EigenFaceRecognitionStrategy());
            }
            FisherFaceRecognitionStrategy* fisherFaceStrategy() {
                return _fisherFaceStrategy != NULL 
                    ? _fisherFaceStrategy
                    : (_fisherFaceStrategy = new FisherFaceRecognitionStrategy());
            }
            LbphFaceRecognitionStrategy* lbphStrategy() {
                return _lbphStrategy != NULL 
                    ? _lbphStrategy
                    : (_lbphStrategy = new LbphFaceRecognitionStrategy());
            }
            EmptyFaceRecognitionStrategy* emptyStrategy() {
                return _emptyStrategy != NULL 
                    ? _emptyStrategy
                    : (_emptyStrategy = new EmptyFaceRecognitionStrategy());
            }
        };
    }
}

#endif //FRS_FACERECOGNITIONSTRATEGYCACHINGFACTORY_H
