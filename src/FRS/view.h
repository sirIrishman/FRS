#ifndef FRS_VIEW_H
#define FRS_VIEW_H

#include <QtCore/qhash.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtWidgets/QMainWindow>
#include <QtGui/qpainter.h>
#include "ui_View.h"
#include "controller.h"
#include "trainingData.h"

using namespace patterns;

namespace frs {
    class View : public QMainWindow, public Observer {
        Q_OBJECT
    public:
        View(Model* const& model, Controller* const& controller, QWidget* const& parent = 0, Qt::WindowFlags flags = 0);
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
        QMenu* _loadFaceRecognitionMethodState;

        void initialize(Model* const& model, Controller* const& controller);
        QString fileName(FileType fileType) const;
        QString videoFileFilter() const;
        QString imageFileFilter() const;
        void updateDisplayedFrame(cv::Mat const& frame);
        void drawRects(QPixmap* const& image, std::vector<cv::Rect> rectCollection) const;
        void drawText(QPixmap* const& image, std::vector<cv::Rect> rectCollection, std::vector<QString> textCollection) const;
        void drawText(QPixmap* const& image, cv::Point const& topLeft, QString const& text) const;
        void trainFaceRecognition();
        TrainingData readImageClassMapFile(QString const& fileName);
        QString combinePathIfRelative(QString const& checkableFileName, QString const& absoluteFileName) const;
        bool faceRecognitionEnabled() const;
        bool objectDetectionEnabled() const;
        void showStatusMessage(QString const& message) const;
        void clearStatusMessage() const;
        void validateTrainingData(TrainingData trainingData) const;
        QAction* createLoadFaceRecognitionMethodStateAction(QString const& fileName) const;
        void addLoadFaceRecognitionMethodStateMenuItem(QString const& fileName);
        void loadFaceRecognitionMethodState(QString const& fileName);
        void saveFaceRecognitionMethodState(QString const& fileName) const;
        void loadClassNumberClassNameMap(QString const& fileName);
        void saveClassNumberClassNameMap(QString const& fileName) const;

    private slots:
        void tlBttn_OpenImageFile_Clicked();
        void tlBttn_OpenVideoFile_Clicked();
        void tlBttn_CaptureWebcamImage_Clicked();
        void tlBttn_CaptureWebcamVideo_Clicked();
        void tlBttn_ObjectDetection_Clicked();
        void tlBttn_FaceRecognition_Toggled(bool checked);
        void tlBttn_About_Clicked();
        void actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool checked);
        void actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool checked);
        void actn_SetEigenfacesFaceRecognitionMethod_Toggled(bool checked);
        void actn_SetFisherfacesFaceRecognitionMethod_Toggled(bool checked);
        void actn_SetLbphFaceRecognitionMethod_Toggled(bool checked);
        void actn_TrainFaceRecognition_Triggered();
        void actn_LoadFaceRecognitionMethodState_Triggered();
    };
}

#endif // FRS_VIEW_H
