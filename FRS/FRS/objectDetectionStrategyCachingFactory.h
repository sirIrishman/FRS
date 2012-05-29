#ifndef FRS_OBJECTDETECTIONSTRATEGYCACHINGFACTORY_H
#define FRS_OBJECTDETECTIONSTRATEGYCACHINGFACTORY_H

#include "objectDetectionStrategy.h"

namespace frs {
    namespace native {
        class ObjectDetectionStrategyCachingFactory sealed {
        public:
            ObjectDetectionStrategyCachingFactory() {
                _emptyStrategy = NULL;
                _haarCascadeStrategy = NULL;
                _lbpCascadeStrategy = NULL;
            }
            ~ObjectDetectionStrategyCachingFactory() {
                if(_emptyStrategy != NULL)
                    delete _emptyStrategy;
                if(_haarCascadeStrategy != NULL)
                    delete _haarCascadeStrategy;
                if(_lbpCascadeStrategy != NULL)
                    delete _lbpCascadeStrategy;
            }

            ObjectDetectionStrategyBase* createEmptytStrategy() {
                return emptyStrategy();
            }
            ObjectDetectionStrategyBase* createStrategy(ObjectDetectionAlgorithm algorithm) {
                if(algorithm == HaarCascade)
                    return haarCascadeStrategy();
                else 
                    return lbpCascadeStrategy();
            }

        private:
            EmptyObjectDetectionStrategy* _emptyStrategy;
            HaarCascadeObjectDetectionStrategy* _haarCascadeStrategy;
            LbpCascadeObjectDetectionStrategy* _lbpCascadeStrategy;

            EmptyObjectDetectionStrategy* emptyStrategy() {
                return (_emptyStrategy != NULL) 
                    ? _emptyStrategy
                    : (_emptyStrategy = new EmptyObjectDetectionStrategy());
            }
            HaarCascadeObjectDetectionStrategy* haarCascadeStrategy() {
                return (_haarCascadeStrategy != NULL) 
                    ? _haarCascadeStrategy
                    : (_haarCascadeStrategy = new HaarCascadeObjectDetectionStrategy());
            }
            LbpCascadeObjectDetectionStrategy* lbpCascadeStrategy() {
                return (_lbpCascadeStrategy != NULL) 
                    ? _lbpCascadeStrategy
                    : (_lbpCascadeStrategy = new LbpCascadeObjectDetectionStrategy());
            }
        };
    }
}

#endif //FRS_OBJECTDETECTIONSTRATEGYCACHINGFACTORY_H
