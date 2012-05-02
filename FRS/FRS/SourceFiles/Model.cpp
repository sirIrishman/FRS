#include <QtGui/qimage.h>
#include "Model.h"

using namespace FRS;

Model::Model() : __fps(30), __msecInSec(1000) {
    _frameSource = None;
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(tick()));
    /*_frame = NULL;*/
}

Model::~Model() {
    delete _timer;
}

void Model::setSource(FrameSource const& frameSource) {
    setSource(frameSource, QString());
}

void Model::setSource(FrameSource const& frameSource, QString const& filePath) {
    if(_frameSource == VideoFile || _frameSource == Webcam)
        unsetVideoSource();

    _frameSource = frameSource;
    _filePath = filePath;

    if(_frameSource == ImageFile)
        notify();
    else if(_frameSource == VideoFile)
        setVideoSource(cvCaptureFromFile(_filePath.toLatin1().constData()));
    else if(_frameSource == Webcam)
        setVideoSource(cvCaptureFromCAM(-1));
}

IplImage* Model::frame() const {
    if(_frameSource == ImageFile)
        return cvLoadImage(_filePath.toLatin1().constData());
    else //if(_frameSource == VideoFile || _frameSource == Webcam)
        return cvQueryFrame(_capture);
}

int Model::interval() const {
    return __msecInSec / __fps;
}

void Model::startTimer() {
    _timer->start(interval());
}

void Model::stopTimer() {
    _timer->stop();
}

void Model::tick() {
    notify();
}

void Model::setVideoSource(CvCapture* capture) {
    _capture = capture;
    startTimer();
}

void Model::unsetVideoSource() {
    stopTimer();
    cvReleaseCapture(&_capture);
}