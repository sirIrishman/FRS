#include <imgproc\imgproc_c.h>
#include "View.h"
#include "Guard.h"

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
    connect(_ui.actn_captureVideo, SIGNAL(triggered()), this, SLOT(actn_CaptureVideo_Triggered()));
}

void View::update() {
    IplImage* sourceFrame = _model->frame();
    if(sourceFrame == NULL)
        return;
    IplImage* destFrame = cvCreateImage(cvGetSize(sourceFrame), sourceFrame->depth, sourceFrame->nChannels);
    cvCvtColor(sourceFrame, destFrame, CV_BGR2RGB);
    QImage* image = new QImage((uchar*)destFrame->imageData, destFrame->width, destFrame->height, destFrame->widthStep, QImage::Format_RGB888);
    _ui.lbl_Frame->setPixmap(QPixmap::fromImage(*image));
    delete image;
    cvReleaseImage(&destFrame);
    //cvReleaseImage(&sourceFrame);
}

void View::actn_LoadImage_Triggered() {
    _controller->loadImage("d:\\cat_smiles.jpg");
}

void View::actn_LoadVideo_Triggered() {
    _controller->loadVideo("d:\\m4_penthouse.avi");
}

void View::actn_CaptureVideo_Triggered() {
    _controller->captureVideo(-1);
}