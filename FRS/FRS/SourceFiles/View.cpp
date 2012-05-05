#include <qfiledialog.h>
#include <qstring.h>
#include <core\core.hpp>
#include "view.h"
#include "guard.h"

using namespace FRS;

View::View(Model* model, Controller* controller, QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
    Utils::Guard::argumentNotNull(model, "model");
    Utils::Guard::argumentNotNull(controller, "controller");
    initialize(model, controller);
    _ui.setupUi(this);
    subscribeToEvents();
}

View::~View() {
    _model->detach(this);
    delete _controller;
    delete _model;
}

void View::initialize(Model* model, Controller* controller) {
    _controller = controller;
    _model = model;
    _model->attach(this);
}

void View::subscribeToEvents() {
    connect(_ui.actn_LoadImage, SIGNAL(triggered()), this, SLOT(actn_LoadImage_Triggered()));
    connect(_ui.actn_LoadVideo, SIGNAL(triggered()), this, SLOT(actn_LoadVideo_Triggered()));
    connect(_ui.actn_CaptureVideo, SIGNAL(triggered()), this, SLOT(actn_CaptureVideo_Triggered()));
    connect(_ui.actn_CaptureImage, SIGNAL(triggered()), this, SLOT(actn_CaptureImage_Triggered()));
}

void View::update() {
    cv::Mat frame = _model->frame();
    if(frame.empty())
        return;
    QImage* image = new QImage(frame.data, frame.cols, frame.rows, frame.step[0], QImage::Format_RGB888);
    _ui.lbl_Frame->setPixmap(QPixmap::fromImage(image->rgbSwapped()));
    frame.release();
    delete image;
}

void View::actn_LoadImage_Triggered() {
    QString imageFileName = fileName(Image);
    if(imageFileName.isNull())
        return;
    _controller->loadImage(imageFileName);
    _ui.stsBr_MainStatusBar->showMessage(imageFileName);
}

void View::actn_LoadVideo_Triggered() {
    QString videoFileName = fileName(Video);
    if(videoFileName.isNull())
        return;
    _controller->loadVideo(videoFileName);
    _ui.stsBr_MainStatusBar->showMessage(videoFileName);
}

void View::actn_CaptureVideo_Triggered() {
    _controller->captureVideo(0);
    _ui.stsBr_MainStatusBar->clearMessage();
}

void View::actn_CaptureImage_Triggered() {
    _controller->captureImage(0);
    _ui.stsBr_MainStatusBar->clearMessage();
}

QString View::fileName(FileType fileType) {
    QString fileFilter = (fileType == Video) ? videoFileFilter() : imageFileFilter();
    return QFileDialog::getOpenFileName( 
        this, 
        tr("Open file"), 
        QDir::currentPath(), 
        fileFilter,
        0,
        QFileDialog::ReadOnly);
}

QString View::videoFileFilter() const {
    QString fileFilter;
    fileFilter += tr("All supported video files (*.avi *.wmv *.mp4 *.mpg *.mpeg *.flv *.3gp *.mkv);;");
    fileFilter += tr("AVI files (*.avi);;");
    fileFilter += tr("Windows Media Video files (*.wmv);;");
    fileFilter += tr("MPEG-4 files (*.mp4);;");
    fileFilter += tr("MPEG files (*.mpg *.mpeg);;");
    fileFilter += tr("Flash Video files (*.flv);;");
    fileFilter += tr("3GPP files (*.3gp);;");
    fileFilter += tr("Matroska Multimedia Container files (*.mkv)");
    return fileFilter;
}

QString View::imageFileFilter() const {
    QString fileFilter;
    fileFilter += tr("All supported image files (*.bmp *.dib *.jpeg *.jpg *.jpe *.png *.pbm *.pgm *.ppm *.sr *.ras *.tiff *.tif);;");
    fileFilter += tr("Windows Bitmap files (*.bmp *.dib);;");
    fileFilter += tr("JPEG files (*.jpeg *.jpg *.jpe);;");
    fileFilter += tr("Sun Raster files (*.sr *.ras);;");
    fileFilter += tr("Netpbm files (*.pbm *.pgm *.ppm);;");
    fileFilter += tr("PNG files (*.png);;");
    fileFilter += tr("TIFF files (*.tiff *.tif)");
    return fileFilter;
}
