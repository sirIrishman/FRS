#include <core\core.hpp>
#include "view.h"
#include "guard.h"
#include "dialogService.h"

using namespace FRS;
using namespace Services;

View::View(Model* model, Controller* controller, QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
    Utils::Guard::argumentNotNull(model, "model");
    Utils::Guard::argumentNotNull(controller, "controller");
    initialize(model, controller);
}

View::~View() {
    delete _painter;

    delete _controller;

    _model->detach(this);
    delete _model;
}

void View::initialize(Model* model, Controller* controller) {
    DialogService::associateWith(this);

    _controller = controller;

    _model = model;
    _model->attach(this);
    _model->setRecognitionAlgorithm(LbpCascade);

    _painter = new QPainter();

    _ui.setupUi(this);

    subscribeToEvents();
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
    if(_ui.actn_RecognizeObjects->isChecked())
        drawRects(image, _model->recognizeObjects(frame, Face));
    _ui.lbl_Frame->setPixmap(QPixmap::fromImage(image->rgbSwapped()));
    frame.release();
    delete image;
}

void View::drawRects(QImage* const& image, std::vector<cv::Rect> rectCollection) const {
    _painter->begin(image);
    _painter->setPen(Qt::green);
    for(int i = 0; i < rectCollection.size(); i++)
        _painter->drawRect(rectCollection[i].x, rectCollection[i].y, rectCollection[i].width, rectCollection[i].height);
    _painter->end();
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
    return DialogService::showOpenFileDialog(fileFilter);
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
