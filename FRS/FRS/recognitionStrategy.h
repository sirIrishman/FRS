#ifndef FRS_RECOGNITIONSTRATEGY_H
#define FRS_RECOGNITIONSTRATEGY_H

#include <qcoreapplication.h>
#include <core\core.hpp>
#include <imgproc\imgproc.hpp>
#include <objdetect\objdetect.hpp>
#include "enums.h"

namespace FRS {
    namespace Native {
        class RecognitionStrategyBase {
        public:
            virtual void initialize() = 0;
            virtual void releaseResources() = 0;
            virtual bool canRecognize(RecognizableObjectType objectType) const = 0;
            std::vector<cv::Rect> recognize(cv::Mat const& frame, RecognizableObjectType objectType) const {
                if(!canRecognize(objectType))
                    return std::vector<cv::Rect>();
                cv::Mat frameCopy = preprocessFrame(frame);
                std::vector<cv::Rect> objectRectCollection = recognizeObjects(frameCopy, objectType);
                frameCopy.release();
                return objectRectCollection;
            }

        protected:
            virtual std::vector<cv::Rect> recognizeObjects(cv::Mat const& frame, RecognizableObjectType objectType) const = 0;

        private:
            cv::Mat preprocessFrame(cv::Mat const& frame) const {
                cv::Mat grayFrame;
                cvtColor(frame, grayFrame, CV_BGR2GRAY);
                cv::equalizeHist(grayFrame, grayFrame);
                return grayFrame;
            }
        };

        class CascadeRecognitionStrategyBase : public RecognitionStrategyBase {
        public:
            virtual ~CascadeRecognitionStrategyBase() {
                releaseResources();
            }

            virtual void initialize() {
                _cascade = new cv::CascadeClassifier(cascadeFileName());
            }
            virtual void releaseResources() {
                delete _cascade;
            }
            virtual bool canRecognize(RecognizableObjectType objectType) const {
                return (objectType == Face) ? true : false;
            }

        protected:
            virtual std::vector<cv::Rect> recognizeObjects(cv::Mat const& frame, RecognizableObjectType objectType) const {
                return recognizeFaces(frame);
            }
            virtual std::string cascadeFileName() const = 0;

        private:
            cv::CascadeClassifier* _cascade;

            std::vector<cv::Rect> recognizeFaces(cv::Mat const& frame) const {
                std::vector<cv::Rect> faceRectCollection;
                _cascade->detectMultiScale(frame, faceRectCollection, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
                return faceRectCollection;
            }
        };

        class HaarCascadeRecognitionStrategy sealed : public CascadeRecognitionStrategyBase {
        protected:
            virtual std::string cascadeFileName() const {
                QString cascadeFileName;
                cascadeFileName += QCoreApplication::applicationDirPath();
                cascadeFileName += "/data/haarcascade_frontalface_alt.xml";
                return cascadeFileName.toStdString();
            }
        };

        class LbpCascadeRecognitionStrategy sealed : public CascadeRecognitionStrategyBase {
        protected:
            virtual std::string cascadeFileName() const {
                QString cascadeFileName;
                cascadeFileName += QCoreApplication::applicationDirPath();
                cascadeFileName += "/data/lbpcascade_frontalface.xml";
                return cascadeFileName.toStdString();
            }
        };

        class EmptyRecognitionStrategy sealed : public RecognitionStrategyBase {
        public:
            virtual void initialize() { }
            virtual void releaseResources() { }
            virtual bool canRecognize(RecognizableObjectType objectType) const { return false; }

        protected:
            virtual std::vector<cv::Rect> recognizeObjects(cv::Mat const& frame, RecognizableObjectType objectType) const { 
                return std::vector<cv::Rect>(); 
            }
        };
    }
}

#endif //FRS_RECOGNITIONSTRATEGY_H
