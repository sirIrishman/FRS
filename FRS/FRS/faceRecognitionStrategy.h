#ifndef FRS_FACERECOGNITIONSTRATEGY_H
#define FRS_FACERECOGNITIONSTRATEGY_H

#include <qdir.h>
#include <core\core.hpp>
#include <imgproc\imgproc.hpp>
#include <contrib\contrib.hpp>
#include "trainingData.h"

using namespace cv;

namespace frs {
    namespace native {
        class FaceRecognitionStrategyBase {
        public:
            FaceRecognitionStrategyBase(Ptr<FaceRecognizer> faceRecognizer) {
                _faceRecognizer = faceRecognizer;
                _trained = false;
                _width = 170;
                _height = 170;
            }
            virtual ~FaceRecognitionStrategyBase() {
                releaseResources();
            }

            void releaseResources() {
                if(_faceRecognizer.empty() == false) {
                    _faceRecognizer.release();
                }
            }
            void train(TrainingData const& trainingData) {
                if(_faceRecognizer.empty())
                    return;
                _width = trainingData.imageCollection()[0].cols;
                _height = trainingData.imageCollection()[0].rows;
                _faceRecognizer->train(trainingData.imageCollection(), trainingData.classCollection());
                _trained = true;
            }
            int recognize(Mat const& image) {
                if(_faceRecognizer.empty())
                    return -1;
                cv::Mat preprocessedImage = preprocessImage(image);
                int predictedClass = _faceRecognizer->predict(preprocessedImage);
                preprocessedImage.release();
                return predictedClass;
            }
            bool trained() const {
                return _trained;
            }
            QString name() const {
                return _name;
            }
            void setName(QString const& name) {
                _name = name;
            }
            QString save() const {
                QDir().mkpath(saveDirectoryPath());
                QString fileName = QString("%1%2.yaml").arg(saveDirectoryPath(), _name);
                _faceRecognizer->save(fileName.toStdString());
                return fileName;
            }
            void load(QString const& faceRecognitionTrainingName) {
                setName(faceRecognitionTrainingName);
                QString fileName = QString("%1%2.yaml").arg(saveDirectoryPath(), faceRecognitionTrainingName);
                _faceRecognizer->load(fileName.toStdString());
            }
            virtual FaceRecognitionAlgorithm algorithm() const = 0;

        protected:
            int _width;
            int _height;

            virtual QString directoryName() const = 0;
            virtual cv::Mat preprocessImage(cv::Mat const& image) const {
                cv::Mat grayImage = makeGreyImage(image);
                return grayImage;
            }
            QString saveDirectoryPath() const {
                return QString("%1/face_recognition/%2/").arg(QCoreApplication::applicationDirPath(), directoryName());
            }

        private:
            Ptr<FaceRecognizer> _faceRecognizer;
            QString _name;
            bool _trained;

            cv::Mat makeGreyImage(cv::Mat const& image) const {
                cv::Mat grayImage;
                cvtColor(image, grayImage, CV_BGR2GRAY);
                cv::equalizeHist(grayImage, grayImage);
                return grayImage;
            }
        };

        class EigenFaceRecognitionStrategy sealed : public FaceRecognitionStrategyBase {
        public:
            EigenFaceRecognitionStrategy()
                : FaceRecognitionStrategyBase(createEigenFaceRecognizer()) {
            }

        protected:
            QString directoryName() const {
                return "eigenfaces";
            }
            FaceRecognitionAlgorithm algorithm() const {
                return Eigenfaces;
            }
            cv::Mat preprocessImage(cv::Mat const& image) const {
                cv::Mat preprocessedImage = FaceRecognitionStrategyBase::preprocessImage(image);
                cv::resize(preprocessedImage, preprocessedImage, cv::Size(_width, _height));
                return preprocessedImage;
            }
        };

        class FisherFaceRecognitionStrategy sealed : public FaceRecognitionStrategyBase {
        public:
            FisherFaceRecognitionStrategy()
                : FaceRecognitionStrategyBase(createFisherFaceRecognizer()) {
            }

        protected:
            QString directoryName() const {
                return "fisherfaces";
            }
            FaceRecognitionAlgorithm algorithm() const {
                return Fisherfaces;
            }
            cv::Mat preprocessImage(cv::Mat const& image) const {
                cv::Mat preprocessedImage = FaceRecognitionStrategyBase::preprocessImage(image);
                cv::resize(preprocessedImage, preprocessedImage, cv::Size(_width, _height));
                return preprocessedImage;
            }
        };

        class LbphFaceRecognitionStrategy sealed : public FaceRecognitionStrategyBase {
        public:
            LbphFaceRecognitionStrategy()
                : FaceRecognitionStrategyBase(createLBPHFaceRecognizer()) {
            }

        protected:
            QString directoryName() const {
                return "lbph";
            }
            FaceRecognitionAlgorithm algorithm() const  {
                return Lbph;
            }
        };

        class EmptyFaceRecognitionStrategy sealed : public FaceRecognitionStrategyBase {
        public:
            EmptyFaceRecognitionStrategy()
                : FaceRecognitionStrategyBase(Ptr<FaceRecognizer>()) {
            }
        protected:
            QString directoryName() const {
                return "empty";
            }
            FaceRecognitionAlgorithm algorithm() const {
                return (FaceRecognitionAlgorithm)-1;
            }
        };
    }
}

#endif //FRS_FACERECOGNITIONSTRATEGY_H
