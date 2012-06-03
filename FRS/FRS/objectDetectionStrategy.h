#ifndef FRS_OBJECTDETECTIONSTRATEGY_H
#define FRS_OBJECTDETECTIONSTRATEGY_H

#include <qcoreapplication.h>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include "enums.h"
#include "settingsService.h"

using namespace services;

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
            virtual double scaleFactor() const = 0;
            virtual int minNeighbors() const = 0;
            virtual int flags() const = 0;
            virtual cv::Size minSize() const = 0;

        private:
            cv::CascadeClassifier* _cascade;

            std::vector<cv::Rect> detectFaces(cv::Mat const& frame) const {
                std::vector<cv::Rect> faceRectCollection;
                _cascade->detectMultiScale(frame, faceRectCollection, scaleFactor(),  minNeighbors(), flags(), minSize());
                return faceRectCollection;
            }
        };

        class HaarCascadeObjectDetectionStrategy sealed : public CascadeObjectDetectionStrategyBase {
        public:
            HaarCascadeObjectDetectionStrategy() 
                : CascadeObjectDetectionStrategyBase(cascadeFileName()){
            }

        protected:
            double scaleFactor() const {
                return SettingsService::getSetting("objectDetection", "haarCascade", "scaleFactor").toDouble();
            }
            int minNeighbors() const {
                return SettingsService::getSetting("objectDetection", "haarCascade", "minNeighbors").toInt();
            }
            int flags() const {
                return SettingsService::getSetting("objectDetection", "haarCascade", "flags").toInt();
            }
            cv::Size minSize() const {
                int minSizeWidth = SettingsService::getSetting("objectDetection", "haarCascade", "minSizeWidth").toInt();
                int minSizeHeight = SettingsService::getSetting("objectDetection", "haarCascade", "minSizeHeight").toInt();
                return cv::Size(minSizeWidth, minSizeHeight);
            }

        private:
            QString cascadeFileName() const {
                QString fileName = SettingsService::getSetting("objectDetection", "haarCascade", "cascadeFileName").toString();
                return QString("%1/data/%2").arg(QCoreApplication::applicationDirPath(), fileName);
            }
        };

        class LbpCascadeObjectDetectionStrategy sealed : public CascadeObjectDetectionStrategyBase {
        public:
            LbpCascadeObjectDetectionStrategy() 
                : CascadeObjectDetectionStrategyBase(cascadeFileName()){
            }

        protected:
            double scaleFactor() const {
                return SettingsService::getSetting("objectDetection", "lbpCascade", "scaleFactor").toDouble();
            }
            int minNeighbors() const {
                return SettingsService::getSetting("objectDetection", "lbpCascade", "minNeighbors").toInt();
            }
            int flags() const {
                return SettingsService::getSetting("objectDetection", "lbpCascade", "flags").toInt();
            }
            cv::Size minSize() const {
                int minSizeWidth = SettingsService::getSetting("objectDetection", "lbpCascade", "minSizeWidth").toInt();
                int minSizeHeight = SettingsService::getSetting("objectDetection", "lbpCascade", "minSizeHeight").toInt();
                return cv::Size(minSizeWidth, minSizeHeight);
            }

        private:
            QString cascadeFileName() const {
                QString fileName = SettingsService::getSetting("objectDetection", "lbpCascade", "cascadeFileName").toString();
                return QString("%1/data/%2").arg(QCoreApplication::applicationDirPath(), fileName);
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
