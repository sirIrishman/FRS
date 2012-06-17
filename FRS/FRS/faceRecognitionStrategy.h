#ifndef FRS_FACERECOGNITIONSTRATEGY_H
#define FRS_FACERECOGNITIONSTRATEGY_H

#include <qdir.h>
#include <qstringlist.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\contrib\contrib.hpp>
#include "trainingData.h"

using namespace cv;

namespace frs {
    namespace native {
        class FaceRecognitionStrategyBase {
        public:
            virtual void releaseResources() = 0;
            virtual void train(TrainingData const& trainingData) = 0;
            virtual int recognize(Mat const& image) = 0;
            virtual bool trained() const = 0;
            virtual void save() const = 0;
            virtual void load(QString const& faceRecognitionTrainingName) = 0;
            virtual QStringList stateFileNameList() const = 0;
            virtual FaceRecognitionAlgorithm algorithm() const = 0;
            QString name() const {
                return _name;
            }
            void setName(QString const& name) {
                _name = name;
            }
            QString commonFaceRecognitionDirectoryPath() const {
                return QString("%1/face_recognition/").arg(QCoreApplication::applicationDirPath());;
            }

        protected:
            QString _name;
        };

        class OpenCvFaceRecognizerStrategyBase : public FaceRecognitionStrategyBase {
        public:
            OpenCvFaceRecognizerStrategyBase(Ptr<FaceRecognizer> faceRecognizer) {
                _faceRecognizer = faceRecognizer;
                _trained = false;
                _width = 0;
                _height = 0;
            }
            virtual ~OpenCvFaceRecognizerStrategyBase() {
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
            void save() const {
                QDir().mkpath(saveDirectoryPath());
                QString fileName = QString("%1%2.yaml").arg(saveDirectoryPath(), _name);
                _faceRecognizer->save(fileName.toStdString());
                saveWidthAndHeight();
            }
            void load(QString const& faceRecognitionTrainingName) {
                setName(faceRecognitionTrainingName);
                QString fileName = QString("%1%2.yaml").arg(saveDirectoryPath(), faceRecognitionTrainingName);
                _faceRecognizer->load(fileName.toStdString());
                _trained = true;
                loadWidthAndHeight();
            }
            QStringList stateFileNameList() const { 
                QDir statePath(saveDirectoryPath());
                QStringList fileFilters;
                fileFilters.push_back("*.yaml");
                QStringList stateFileNames = statePath.entryList(fileFilters,QDir::Files, QDir::Name);
                for(int i = 0; i < stateFileNames.size(); i++){
                    QFileInfo stateFileInfo(stateFileNames[i]);
                    stateFileNames[i] = stateFileInfo.baseName();
                }
                return stateFileNames;
            }

        protected:
            int _width;
            int _height;

            virtual QString directoryName() const = 0;
            virtual cv::Mat preprocessImage(cv::Mat const& image) const {
                cv::Mat grayImage = makeGreyImage(image);
                return grayImage;
            }
            QString saveDirectoryPath() const {
                return QString("%1%2/").arg(commonFaceRecognitionDirectoryPath(), directoryName());
            }

        private:
            Ptr<FaceRecognizer> _faceRecognizer;
            bool _trained;

            cv::Mat makeGreyImage(cv::Mat const& image) const {
                cv::Mat grayImage;
                cvtColor(image, grayImage, CV_BGR2GRAY);
                cv::equalizeHist(grayImage, grayImage);
                return grayImage;
            }
            void loadWidthAndHeight() {
                QString fullFileName = QString("%1%2.csv").arg(saveDirectoryPath(), _name);
                QFile file(fullFileName);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    FileOperationException(QString("Can not open '%1' file").arg(fullFileName)).raise();

                QTextStream inputTextStream(&file);
                _width = inputTextStream.readLine().toInt();
                _height = inputTextStream.readLine().toInt();
                file.close();
            }
            void saveWidthAndHeight() const {
                QString fullFileName = QString("%1%2.csv").arg(saveDirectoryPath(), _name);
                QFile file(fullFileName);
                if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                    FileOperationException(QString("Can not create\open '%1' file").arg(fullFileName)).raise();

                QTextStream outputTextStream(&file);
                outputTextStream << _width << "\n";
                outputTextStream << _height << "\n";
                file.close();
            }
        };

        class EigenFaceRecognitionStrategy sealed : public OpenCvFaceRecognizerStrategyBase {
        public:
            EigenFaceRecognitionStrategy()
                : OpenCvFaceRecognizerStrategyBase(createEigenFaceRecognizer()) {
            }

        protected:
            QString directoryName() const {
                return "eigenfaces";
            }
            FaceRecognitionAlgorithm algorithm() const {
                return Eigenfaces;
            }
            cv::Mat preprocessImage(cv::Mat const& image) const {
                cv::Mat preprocessedImage = OpenCvFaceRecognizerStrategyBase::preprocessImage(image);
                if(_width != 0 && _height != 0)
                    cv::resize(preprocessedImage, preprocessedImage, cv::Size(_width, _height));
                return preprocessedImage;
            }
        };

        class FisherFaceRecognitionStrategy sealed : public OpenCvFaceRecognizerStrategyBase {
        public:
            FisherFaceRecognitionStrategy()
                : OpenCvFaceRecognizerStrategyBase(createFisherFaceRecognizer()) {
            }

        protected:
            QString directoryName() const {
                return "fisherfaces";
            }
            FaceRecognitionAlgorithm algorithm() const {
                return Fisherfaces;
            }
            cv::Mat preprocessImage(cv::Mat const& image) const {
                cv::Mat preprocessedImage = OpenCvFaceRecognizerStrategyBase::preprocessImage(image);
                if(_width != 0 && _height != 0)
                    cv::resize(preprocessedImage, preprocessedImage, cv::Size(_width, _height));
                return preprocessedImage;
            }
        };

        class LbphFaceRecognitionStrategy sealed : public OpenCvFaceRecognizerStrategyBase {
        public:
            LbphFaceRecognitionStrategy()
                : OpenCvFaceRecognizerStrategyBase(createLBPHFaceRecognizer()) {
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
            void releaseResources() { }
            void train(TrainingData const& trainingData) { }
            int recognize(Mat const& image) { return -1; }
            bool trained() const { return true; }
            void save() const { }
            void load(QString const& faceRecognitionTrainingName) { }
            QStringList stateFileNameList() const { return QStringList(); }
            FaceRecognitionAlgorithm algorithm() const { return (FaceRecognitionAlgorithm)-1; }
        };
    }
}

#endif //FRS_FACERECOGNITIONSTRATEGY_H
