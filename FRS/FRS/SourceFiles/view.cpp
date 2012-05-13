#include <core\core.hpp>
#include <qmenu.h>
#include "view.h"
#include "guard.h"
#include "dialogService.h"

using namespace FRS;
using namespace Services;

View::View(Model* const& model, Controller* const& controller, QWidget* const& parent, Qt::WFlags flags) 
    : QMainWindow(parent, flags) {
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

void View::initialize(Model* const& model, Controller* const& controller) {
    initializeInnerState(model, controller);
    initializeUI();
    subscribeToEvents();
}

void View::initializeInnerState(Model* const& model, Controller* const& controller) {
    DialogService::associateWith(this);

    _controller = controller;

    _model = model;
    _model->attach(this);

    _painter = new QPainter();
}

void View::initializeUI() {
    _ui.setupUi(this);

    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_OpenImageFile);
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_OpenVideoFile);
    _ui.tlBr_MainToolbar->addSeparator();
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_CaptureWebcamImage);
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_CaptureWebcamVideo);
    _ui.tlBr_MainToolbar->addSeparator();
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_ObjectDetection);

    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(_ui.actn_SetHaarCascadeObjectDetectionAlgorithm);
    actionGroup->addAction(_ui.actn_SetLbpCascadeObjectDetectionAlgorithm);

    QMenu *menu = new QMenu();
    menu->addAction(_ui.actn_SetHaarCascadeObjectDetectionAlgorithm);
    menu->addAction(_ui.actn_SetLbpCascadeObjectDetectionAlgorithm);

    _ui.tlBttn_ObjectDetection->setMenu(menu);
}

void View::subscribeToEvents() {
    connect(_ui.tlBttn_OpenImageFile, SIGNAL(clicked()), this, SLOT(tlBttn_OpenImageFile_Clicked()));
    connect(_ui.tlBttn_OpenVideoFile, SIGNAL(clicked()), this, SLOT(tlBttn_OpenVideoFile_Clicked()));
    connect(_ui.tlBttn_CaptureWebcamImage, SIGNAL(clicked()), this, SLOT(tlBttn_CaptureWebcamImage_Clicked()));
    connect(_ui.tlBttn_CaptureWebcamVideo, SIGNAL(clicked()), this, SLOT(tlBttn_CaptureWebcamVideo_Clicked()));
    connect(_ui.actn_SetHaarCascadeObjectDetectionAlgorithm, SIGNAL(toggled(bool)), this, SLOT(actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool)));
    connect(_ui.actn_SetLbpCascadeObjectDetectionAlgorithm, SIGNAL(toggled(bool)), this, SLOT(actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool)));
}

void View::update() {
    cv::Mat frame = _model->frame();
    if(frame.empty())
        return;
    QImage* image = new QImage(frame.data, frame.cols, frame.rows, frame.step[0], QImage::Format_RGB888);
    if(_ui.tlBttn_ObjectDetection->isChecked())
        drawRects(image, _model->detectObjects(frame, Face));
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

void View::tlBttn_OpenImageFile_Clicked() {
    QString imageFileName = fileName(Image);
    if(imageFileName.isNull())
        return;
    _controller->loadImage(imageFileName);
    _ui.stsBr_MainStatusBar->showMessage(imageFileName);
}

void View::tlBttn_OpenVideoFile_Clicked() {
    QString videoFileName = fileName(Video);
    if(videoFileName.isNull())
        return;
    _controller->loadVideo(videoFileName);
    _ui.stsBr_MainStatusBar->showMessage(videoFileName);
}

void View::tlBttn_CaptureWebcamImage_Clicked() {
    _controller->captureImage(0);
    _ui.stsBr_MainStatusBar->clearMessage();
}

void View::tlBttn_CaptureWebcamVideo_Clicked() {
    _controller->captureVideo(0);
    _ui.stsBr_MainStatusBar->clearMessage();
}

void View::actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool checked) {
    if(checked)
        _model->setObjectDetectionAlgorithm(HaarCascade);
}

void View::actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool checked) {
    if(checked)
        _model->setObjectDetectionAlgorithm(LbpCascade);
}

QString View::fileName(FileType fileType) const {
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