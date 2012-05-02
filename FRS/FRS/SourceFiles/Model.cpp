#include <QtGui/qimage.h>
#include "Model.h"

using namespace FRS;

Model::Model() {
    _frameSource = None;
    _filePath = QString();
}

void Model::setSource(FrameSource const& frameSource) {
    setSource(frameSource, QString());
}

void Model::setSource(FrameSource const& frameSource, QString const& filePath) {
    _frameSource = frameSource;
    _filePath = filePath;
    notify();
}

QImage Model::frame() const {
    if(_frameSource == GraphicFile)
        return QImage(_filePath, "jpg");
    else
        return QImage();
}