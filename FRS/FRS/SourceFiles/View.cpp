#include <qfiledialog.h>
#include <qstring.h>
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
    IplImage* frame = _model->frame();
    if(frame == NULL)
        return;
    QImage* image = new QImage((uchar*)frame->imageData, frame->width, frame->height, frame->widthStep, QImage::Format_RGB888);
    _ui.lbl_Frame->setPixmap(QPixmap::fromImage(image->rgbSwapped()));
    delete image;
}

void View::actn_LoadImage_Triggered() {
    QString imageFileName = fileName(Image);
    if(imageFileName.isNull())
        return;
    _controller->loadImage(imageFileName);
}

void View::actn_LoadVideo_Triggered() {
    QString videoFileName = fileName(Video);
    if(videoFileName.isNull())
        return;
    _controller->loadVideo(videoFileName);
}

void View::actn_CaptureVideo_Triggered() {
    _controller->captureVideo(0);
}

void View::actn_CaptureImage_Triggered() {
    _controller->captureImage(0);
}

QString View::fileName(FileType fileType) {
    QString fileFilter = (fileType == Video) 
        ? tr("All supported video files (*.avi);; AVI files (*.avi)") 
        : tr("All supported image files (*.bmp *.dib *.jpeg *.jpg *.jpe *.png *.pbm *.pgm *.ppm *.sr *.ras *.tiff *.tif);;Windows bitmaps (*.bmp *.dib);;JPEG files (*.jpeg *.jpg *.jpe);;Portable Network Graphics (*.png);;Portable image format (*.pbm *.pgm *.ppm);;Sun rasters (*.sr *.ras);;TIFF files (*.tiff *.tif)");
    return QFileDialog::getOpenFileName( 
        this, 
        tr("Open file"), 
        QDir::currentPath(), 
        fileFilter,
        0,
        QFileDialog::ReadOnly);
}
