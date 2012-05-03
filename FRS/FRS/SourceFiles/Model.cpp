#include <QtGui/qimage.h>
#include <Windows.h>
#include "Model.h"

using namespace FRS;

Model::Model() : __fps(30), __msecInSec(1000), __webcamInitTimeInMsec(250) {
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
    if(_frameSource == VideoFile || _frameSource == VideoWebcam)
        unsetVideoSource();

    _frameSource = frameSource;
    _filePath = filePath;

    if(_frameSource == ImageFile)
        notify();
    else if(_frameSource == VideoFile)
        setVideoSource(cvCaptureFromFile(_filePath.toUtf8().constData()));
    else if(_frameSource == VideoWebcam)
        setVideoSource(cvCaptureFromCAM(-1));
    else if(_frameSource == ImageWebcam) {
        setVideoSource(cvCaptureFromCAM(-1));
        Sleep(__webcamInitTimeInMsec); //waiting for a webcam initializing
        notify();
        unsetVideoSource();
    }
}

IplImage* Model::frame() {
    if(_frameSource == ImageFile)
        return cvLoadImage(_filePath.toUtf8().constData());
    else {//?
        _frame = cvQueryFrame(_capture);
        return _frame;
    }
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
    //cvReleaseImage(&_frame);
}