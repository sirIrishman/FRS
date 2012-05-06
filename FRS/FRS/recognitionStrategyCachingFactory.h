#ifndef FRS_RECOGNITIONSTRATEGYCACHINGFACTORY_H
#define FRS_RECOGNITIONSTRATEGYCACHINGFACTORY_H

#include "recognitionStrategy.h"

namespace FRS {
    namespace Native {
        class RecognitionStrategyCachingFactory sealed {
        public:
            RecognitionStrategyCachingFactory() {
                _emptyStrategy = NULL;
                _haarCascadeStrategy = NULL;
                _lbpCascadeStrategy = NULL;
            }
            ~RecognitionStrategyCachingFactory() {
                if(_emptyStrategy != NULL)
                    delete _emptyStrategy;
                if(_haarCascadeStrategy != NULL)
                    delete _haarCascadeStrategy;
                if(_lbpCascadeStrategy != NULL)
                    delete _lbpCascadeStrategy;
            }

            RecognitionStrategyBase* createEmptytStrategy() {
                return emptyStrategy();
            }
            RecognitionStrategyBase* createStrategy(RecognitionAlgorithm algorithm) {
                if(algorithm == HaarCascade)
                    return haarCascadeStrategy();
                else 
                    return lbpCascadeStrategy();
            }

        private:
            EmptyRecognitionStrategy* _emptyStrategy;
            HaarCascadeRecognitionStrategy* _haarCascadeStrategy;
            LbpCascadeRecognitionStrategy* _lbpCascadeStrategy;

            EmptyRecognitionStrategy* emptyStrategy() {
                return (_emptyStrategy != NULL) 
                    ? _emptyStrategy
                    : (_emptyStrategy = new EmptyRecognitionStrategy());
            }
            HaarCascadeRecognitionStrategy* haarCascadeStrategy() {
                return (_haarCascadeStrategy != NULL) 
                    ? _haarCascadeStrategy
                    : (_haarCascadeStrategy = new HaarCascadeRecognitionStrategy());
            }
            LbpCascadeRecognitionStrategy* lbpCascadeStrategy() {
                return (_lbpCascadeStrategy != NULL) 
                    ? _lbpCascadeStrategy
                    : (_lbpCascadeStrategy = new LbpCascadeRecognitionStrategy());
            }
        };
    }
}

#endif //FRS_RECOGNITIONSTRATEGYCACHINGFACTORY_H
