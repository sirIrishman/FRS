#ifndef FRS_VIEW_H
#define FRS_VIEW_H

#include <QtGui/QMainWindow>
#include <qhash.h>
#include <qpainter.h>
#include <qchar.h>
#include <qstring.h>
#include "ui_View.h"
#include "controller.h"
#include "trainingData.h"

using namespace patterns;

namespace frs {
    class View : public QMainWindow, public Observer {
        Q_OBJECT
    public:
        View(Model* const& model, Controller* const& controller, QWidget* const& parent = 0, Qt::WFlags flags = 0);
        virtual ~View();

    protected:
        virtual void initializeInnerState(Model* const& model, Controller* const& controller);
        virtual void initializeUI();
        virtual void subscribeToEvents();
        void update();

    private:
        Ui::mnWnd_View _ui;
        Model* _model;
        Controller* _controller;
        QPainter* _painter;
        cv::Mat _lastFrame;
        QHash<int, QString> _classNumberClassNameMap;

        void initialize(Model* const& model, Controller* const& controller);
        QString fileName(FileType fileType) const;
        QString videoFileFilter() const;
        QString imageFileFilter() const;
        void updateDisplayedFrame(cv::Mat const& frame);
        void drawRects(QPixmap* const& image, std::vector<cv::Rect> rectCollection) const;
        void drawText(QPixmap* const& image, std::vector<cv::Rect> rectCollection, std::vector<QString> textCollection) const;
        void drawText(QPixmap* const& image, cv::Point const& topLeft, QString const& text) const;
        void trainFaceRecognition();
        TrainingData readImageClassMapFile(QString const& fileName, QChar separator = ',');
        QString combinePathIfRelative(QString const& checkableFileName, QString const& absoluteFileName) const;
        bool faceRecognitionEnabled() const;
        bool objectDetectionEnabled() const;
        void showStatusMessage(QString const& message) const;
        void clearStatusMessage() const;
        void validateTrainingData(TrainingData trainingData) const;

    private slots:
        void tlBttn_OpenImageFile_Clicked();
        void tlBttn_OpenVideoFile_Clicked();
        void tlBttn_CaptureWebcamImage_Clicked();
        void tlBttn_CaptureWebcamVideo_Clicked();
        void tlBttn_ObjectDetection_Clicked();
        void tlBttn_FaceRecognition_Toggled(bool checked);
        void actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool checked);
        void actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool checked);
        void actn_SetEigenfacesFaceRecognitionMethod_Toggled(bool checked);
        void actn_SetFisherfacesFaceRecognitionMethod_Toggled(bool checked);
        void actn_SetLbphFaceRecognitionMethod_Toggled(bool checked);
        void actn_TrainFaceRecognition_Triggered(bool checked);
    };
}

#endif // FRS_VIEW_H
