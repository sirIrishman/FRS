#include <QtGui/qimage.h>
#include <Windows.h>
#include "Model.h"

using namespace FRS;

Model::Model() : __fps(30), __msecInSec(1000), __webcamInitTimeInMsec(250) {
    _frameSource = None;
    _capture = NULL;
    _frame = NULL;
    _activeWebcamIndex = -1;
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

Model::~Model() {
    unsetVideoSource();
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
    switch(_frameSource) {
    case ImageFile:
        _frame = cvLoadImage(_filePath.toUtf8().constData());
        break;
    case VideoFile:
    case VideoWebcam:
    case ImageWebcam:
        _frame = cvQueryFrame(_capture);
        break;
    default:
        _frame = NULL;
        break;
    }
    return _frame;
}

int Model::interval() const {
    return __msecInSec / __fps;
}

void Model::startTimer() {
    if(!_timer->isActive())
        _timer->start(interval());
}

void Model::stopTimer() {
    if(_timer->isActive())
        _timer->stop();
}

void Model::tick() const {
    notify();
}

void Model::setVideoSource(CvCapture* capture) {
    _capture = capture;
    startTimer();
}

void Model::unsetVideoSource() {
    stopTimer();
    if(_frameSource == ImageFile && _frame != NULL) {
        cvReleaseImage(&_frame);
        _frame = NULL;
    }
    if(_capture != NULL) {
        cvReleaseCapture(&_capture);
        _capture = NULL;
    }
}

int Model::webcamCount() const {
    return 5;
}

int Model::activeWebcamIndex() const {
    return _activeWebcamIndex;
}

void Model::setActiveWebcamIndex(int webcamIndex) {
    _activeWebcamIndex = webcamIndex;
}
