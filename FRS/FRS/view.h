#ifndef FRS_VIEW_H
#define FRS_VIEW_H

#include <QtGui/QMainWindow>
#include <qpainter.h>
#include <qstring.h>
#include "ui_View.h"
#include "controller.h"

using namespace Patterns;

namespace FRS {
    class View : public QMainWindow, public Observer {
        Q_OBJECT
    public:
        View(Model* const& model, Controller* const& controller, QWidget* const& parent = 0, Qt::WFlags flags = 0);
        ~View();

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

        void initialize(Model* const& model, Controller* const& controller);
        QString fileName(FileType fileType) const;
        QString videoFileFilter() const;
        QString imageFileFilter() const;
        void drawRects(QImage* const& image, std::vector<cv::Rect> rectCollection) const;

    private slots:
        void tlBttn_OpenImageFile_Clicked();
        void tlBttn_OpenVideoFile_Clicked();
        void tlBttn_CaptureWebcamImage_Clicked();
        void tlBttn_CaptureWebcamVideo_Clicked();
        void actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool checked);
        void actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool checked);
    };
}

#endif // FRS_VIEW_H
