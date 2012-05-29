#ifndef FRS_OBJECTDETECTIONSTRATEGY_H
#define FRS_OBJECTDETECTIONSTRATEGY_H

#include <qcoreapplication.h>
#include <core\core.hpp>
#include <imgproc\imgproc.hpp>
#include <objdetect\objdetect.hpp>
#include "enums.h"

namespace frs {
    namespace native {
        class ObjectDetectionStrategyBase {
        public:
            virtual void releaseResources() = 0;
            virtual bool canDetect(DetectableObjectType objectType) const = 0;
            std::vector<cv::Rect> detect(cv::Mat const& frame, DetectableObjectType objectType) const {
                if(!canDetect(objectType))
                    return std::vector<cv::Rect>();
                cv::Mat frameCopy = preprocessFrame(frame);
                std::vector<cv::Rect> objectRectCollection = detectObjects(frameCopy, objectType);
                frameCopy.release();
                return objectRectCollection;
            }

        protected:
            virtual std::vector<cv::Rect> detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const = 0;

        private:
            cv::Mat preprocessFrame(cv::Mat const& frame) const {
                cv::Mat grayFrame;
                cvtColor(frame, grayFrame, CV_BGR2GRAY);
                cv::equalizeHist(grayFrame, grayFrame);
                return grayFrame;
            }
        };

        class CascadeObjectDetectionStrategyBase : public ObjectDetectionStrategyBase {
        public:
            CascadeObjectDetectionStrategyBase(QString cascadeFileName) {
                _cascade = new cv::CascadeClassifier(cascadeFileName.toStdString());
            }
            virtual ~CascadeObjectDetectionStrategyBase() {
                releaseResources();
            }

            virtual void releaseResources() {
                if(_cascade != NULL) {
                    delete _cascade;
                    _cascade = NULL;
                }
            }
            virtual bool canDetect(DetectableObjectType objectType) const {
                return (objectType == Face) ? true : false;
            }

        protected:
            virtual std::vector<cv::Rect> detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const {
                return detectFaces(frame);
            }

        private:
            cv::CascadeClassifier* _cascade;

            std::vector<cv::Rect> detectFaces(cv::Mat const& frame) const {
                std::vector<cv::Rect> faceRectCollection;
                _cascade->detectMultiScale(frame, faceRectCollection, 1.15, 3, CV_HAAR_SCALE_IMAGE, cv::Size(40, 40));
                return faceRectCollection;
            }
        };

        class HaarCascadeObjectDetectionStrategy sealed : public CascadeObjectDetectionStrategyBase {
        public:
            HaarCascadeObjectDetectionStrategy() 
                : CascadeObjectDetectionStrategyBase(cascadeFileName()){
            }

        private:
            QString cascadeFileName() const {
                return QString("%1/data/haarcascade_frontalface_alt.xml").arg(QCoreApplication::applicationDirPath());
            }
        };

        class LbpCascadeObjectDetectionStrategy sealed : public CascadeObjectDetectionStrategyBase {
        public:
            LbpCascadeObjectDetectionStrategy() 
                : CascadeObjectDetectionStrategyBase(cascadeFileName()){
            }

        private:
            QString cascadeFileName() const {
                return QString("%1/data/lbpcascade_frontalface.xml").arg(QCoreApplication::applicationDirPath());
            }
        };

        class EmptyObjectDetectionStrategy sealed : public ObjectDetectionStrategyBase {
        public:
            virtual void releaseResources() { }
            virtual bool canDetect(DetectableObjectType objectType) const { return false; }

        protected:
            virtual std::vector<cv::Rect> detectObjects(cv::Mat const& frame, DetectableObjectType objectType) const { 
                return std::vector<cv::Rect>(); 
            }
        };
    }
}

#endif //FRS_OBJECTDETECTIONSTRATEGY_H
