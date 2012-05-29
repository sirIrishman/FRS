#include <core\core.hpp>
#include <qmenu.h>
#include <qfile.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include "view.h"
#include "guard.h"
#include "dialogService.h"
#include "faceRecognitionTrainingDialog.h"

using namespace frs;
using namespace services;

View::View(Model* const& model, Controller* const& controller, QWidget* const& parent, Qt::WFlags flags)
    : QMainWindow(parent, flags) {
        utils::Guard::argumentNotNull(model, "model");
        utils::Guard::argumentNotNull(controller, "controller");
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
    _ui.actn_SetHaarCascadeObjectDetectionAlgorithm->trigger();
    _ui.actn_SetLbphFaceRecognitionMethod->trigger();
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

    //fill toolbar
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_OpenImageFile);
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_OpenVideoFile);
    _ui.tlBr_MainToolbar->addSeparator();
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_CaptureWebcamImage);
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_CaptureWebcamVideo);
    _ui.tlBr_MainToolbar->addSeparator();
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_ObjectDetection);
    _ui.tlBr_MainToolbar->addSeparator();
    _ui.tlBr_MainToolbar->addWidget(_ui.tlBttn_FaceRecognition);

    //object detection button
    QActionGroup *objectDetectionAlgorithmGroup = new QActionGroup(this);
    objectDetectionAlgorithmGroup->addAction(_ui.actn_SetHaarCascadeObjectDetectionAlgorithm);
    objectDetectionAlgorithmGroup->addAction(_ui.actn_SetLbpCascadeObjectDetectionAlgorithm);

    QMenu *objectDetectionAlgorithmMenu = new QMenu();
    objectDetectionAlgorithmMenu->addAction(_ui.actn_SetHaarCascadeObjectDetectionAlgorithm);
    objectDetectionAlgorithmMenu->addAction(_ui.actn_SetLbpCascadeObjectDetectionAlgorithm);

    _ui.tlBttn_ObjectDetection->setMenu(objectDetectionAlgorithmMenu);

    //face recognition button
    QActionGroup *faceRecognitionMethodGroup = new QActionGroup(this);
    faceRecognitionMethodGroup->addAction(_ui.actn_SetEigenfacesFaceRecognitionMethod);
    faceRecognitionMethodGroup->addAction(_ui.actn_SetFisherfacesFaceRecognitionMethod);
    faceRecognitionMethodGroup->addAction(_ui.actn_SetLbphFaceRecognitionMethod);

    QMenu *faceRecognitionMenu = new QMenu();
    faceRecognitionMenu->addAction(_ui.actn_SetEigenfacesFaceRecognitionMethod);
    faceRecognitionMenu->addAction(_ui.actn_SetFisherfacesFaceRecognitionMethod);
    faceRecognitionMenu->addAction(_ui.actn_SetLbphFaceRecognitionMethod);
    faceRecognitionMenu->addSeparator();
    faceRecognitionMenu->addAction(_ui.actn_TrainFaceRecognition);

    _ui.tlBttn_FaceRecognition->setMenu(faceRecognitionMenu);
}

void View::subscribeToEvents() {
    connect(_ui.tlBttn_OpenImageFile, SIGNAL(clicked()), this, SLOT(tlBttn_OpenImageFile_Clicked()));
    connect(_ui.tlBttn_OpenVideoFile, SIGNAL(clicked()), this, SLOT(tlBttn_OpenVideoFile_Clicked()));
    connect(_ui.tlBttn_CaptureWebcamImage, SIGNAL(clicked()), this, SLOT(tlBttn_CaptureWebcamImage_Clicked()));
    connect(_ui.tlBttn_CaptureWebcamVideo, SIGNAL(clicked()), this, SLOT(tlBttn_CaptureWebcamVideo_Clicked()));
    connect(_ui.tlBttn_ObjectDetection, SIGNAL(clicked()), this, SLOT(tlBttn_ObjectDetection_Clicked()));
    connect(_ui.tlBttn_FaceRecognition, SIGNAL(toggled(bool)), this, SLOT(tlBttn_FaceRecognition_Toggled(bool)));
    connect(_ui.actn_SetHaarCascadeObjectDetectionAlgorithm, SIGNAL(toggled(bool)), this, SLOT(actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool)));
    connect(_ui.actn_SetLbpCascadeObjectDetectionAlgorithm, SIGNAL(toggled(bool)), this, SLOT(actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool)));
    connect(_ui.actn_SetEigenfacesFaceRecognitionMethod, SIGNAL(toggled(bool)), this, SLOT(actn_SetEigenfacesFaceRecognitionMethod_Toggled(bool)));
    connect(_ui.actn_SetFisherfacesFaceRecognitionMethod, SIGNAL(toggled(bool)), this, SLOT(actn_SetFisherfacesFaceRecognitionMethod_Toggled(bool)));
    connect(_ui.actn_SetLbphFaceRecognitionMethod, SIGNAL(toggled(bool)), this, SLOT(actn_SetLbphFaceRecognitionMethod_Toggled(bool)));
    connect(_ui.actn_TrainFaceRecognition, SIGNAL(triggered(bool)), this, SLOT(actn_TrainFaceRecognition_Triggered(bool)));
}

void View::update() {
    cv::Mat frame = _model->frame();
    if(frame.empty())
        return;
    updateDisplayedFrame(frame);
    if(_lastFrame.empty() == false)
        _lastFrame.release();
    _lastFrame = frame.clone();
    frame.release();
}

void View::updateDisplayedFrame(cv::Mat const& frame) {
    if(frame.empty())
        return;

    QImage* frameImage = new QImage(frame.data, frame.cols, frame.rows, frame.step[0], QImage::Format_RGB888);
    QPixmap frameImagePixMap = QPixmap::fromImage(frameImage->rgbSwapped());
    delete frameImage;

    if(objectDetectionEnabled()) {
        std::vector<cv::Rect> rectCollection = _model->detectObjects(frame, Face);
        drawRects(&frameImagePixMap, rectCollection);
        if(faceRecognitionEnabled() && !rectCollection.empty()) {
            std::vector<QString> textCollection;
            for(int i = 0; i < rectCollection.size(); i++) {
                cv::Mat faceImage = frame(rectCollection[i]);
                int classNumber = _model->recognizeFace(faceImage);
                faceImage.release();
                textCollection.push_back(QString("[%1]").arg(_classNumberClassNameMap[classNumber]));
            }
            drawText(&frameImagePixMap, rectCollection, textCollection);
        }
    } else if(faceRecognitionEnabled()) {
        int classNumber = _model->recognizeFace(frame);
        drawText(&frameImagePixMap, cv::Point(0, 0), QString("[%1]").arg(_classNumberClassNameMap[classNumber]));
    }
    _ui.lbl_Frame->setPixmap(frameImagePixMap);
}

void View::drawRects(QPixmap* const& image, std::vector<cv::Rect> rectCollection) const {
    _painter->begin(image);
    _painter->setPen(Qt::green);
    for(int i = 0; i < rectCollection.size(); i++)
        _painter->drawRect(rectCollection[i].x, rectCollection[i].y, rectCollection[i].width, rectCollection[i].height);
    _painter->end();
}

void View::drawText(QPixmap* const& image, std::vector<cv::Rect> rectCollection, std::vector<QString> textCollection) const {
    for(int i = 0; i < rectCollection.size(); i++)
        drawText(image, rectCollection[i].tl(), textCollection[i]);
}

void View::drawText(QPixmap* const& image, cv::Point const& leftTop, QString const& text) const {
    _painter->begin(image);
    _painter->setPen(Qt::yellow);
    const int offset = 5;
    _painter->drawText(leftTop.x + offset, leftTop.y + _painter->fontMetrics().height(), text);
    _painter->end();
}

bool View::faceRecognitionEnabled() const {
    return _ui.tlBttn_FaceRecognition->isChecked();
}

bool View::objectDetectionEnabled() const {
    return _ui.tlBttn_ObjectDetection->isChecked();
}

void View::showStatusMessage(QString const& message) const {
    _ui.stsBr_MainStatusBar->showMessage(message);
}

void View::clearStatusMessage() const {
    _ui.stsBr_MainStatusBar->clearMessage();
}

void View::tlBttn_OpenImageFile_Clicked() {
    QString imageFileName = fileName(Image);
    if(imageFileName.isNull() || imageFileName.isEmpty())
        return;
    _controller->loadImage(imageFileName);
    showStatusMessage(imageFileName);
}

void View::tlBttn_OpenVideoFile_Clicked() {
    QString videoFileName = fileName(Video);
    if(videoFileName.isNull() || videoFileName.isEmpty())
        return;
    _controller->loadVideo(videoFileName);
    showStatusMessage(videoFileName);
}

void View::tlBttn_CaptureWebcamImage_Clicked() {
    _controller->captureImage(0);
    clearStatusMessage();
}

void View::tlBttn_CaptureWebcamVideo_Clicked() {
    _controller->captureVideo(0);
    clearStatusMessage();
}

void View::tlBttn_ObjectDetection_Clicked() {
    updateDisplayedFrame(_lastFrame);
}

void View::tlBttn_FaceRecognition_Toggled(bool checked) {
    if(checked) {
        if(_model->faceRecognitionTrained() == false)
            trainFaceRecognition();
    }
    updateDisplayedFrame(_lastFrame);
}

void View::actn_SetHaarCascadeObjectDetectionAlgorithm_Toggled(bool checked) {
    if(checked) {
        _controller->setCurrentObjectDetectionAlgorithm(HaarCascade);
        if(objectDetectionEnabled())
            updateDisplayedFrame(_lastFrame);
    }
}

void View::actn_SetLbpCascadeObjectDetectionAlgorithm_Toggled(bool checked) {
    if(checked) {
        _controller->setCurrentObjectDetectionAlgorithm(LbpCascade);
        if(objectDetectionEnabled())
            updateDisplayedFrame(_lastFrame);
    }
}

void View::actn_SetEigenfacesFaceRecognitionMethod_Toggled(bool checked) {
    if(checked) {
        _controller->setCurrentFaceRecognitionAlgorithm(Eigenfaces);
        if(faceRecognitionEnabled())
            updateDisplayedFrame(_lastFrame);
    }
}

void View::actn_SetFisherfacesFaceRecognitionMethod_Toggled(bool checked) {
    if(checked) {
        _controller->setCurrentFaceRecognitionAlgorithm(Fisherfaces);
        if(faceRecognitionEnabled())
            updateDisplayedFrame(_lastFrame);
    }
}

void View::actn_SetLbphFaceRecognitionMethod_Toggled(bool checked) {
    if(checked) {
        _controller->setCurrentFaceRecognitionAlgorithm(Lbph);
        if(faceRecognitionEnabled())
            updateDisplayedFrame(_lastFrame);
    }
}

void View::actn_TrainFaceRecognition_Triggered(bool checked) {
    trainFaceRecognition();
}

void View::trainFaceRecognition() {
    //TrainingInfo trainingInfo = FaceRecognitionTrainingDialog::getTrainingInfo((QWidget*)this);
    TrainingInfo trainingInfo("name", "d:\\Test\\test.csv");
    if(trainingInfo == TrainingInfo::empty()) {
        _ui.tlBttn_FaceRecognition->setChecked(false);
        return;
    }
    showStatusMessage("Reading face recognition trainging file...");
    _controller->setFaceRecognitionTrainingName(trainingInfo.name());
    TrainingData trainingData = readImageClassMapFile(trainingInfo.classImageMapFileName());
    validateTrainingData(trainingData);

    showStatusMessage("Training face recognition...");
    _controller->trainFaceRecognition(trainingData);
    trainingData.release();

    showStatusMessage("Saving face recognition state...");
    _controller->saveFaceRecognitionState();
    clearStatusMessage();
}

TrainingData View::readImageClassMapFile(QString const& fileName, QChar separator) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        FileOperationException(QString("Can not open '%1' file").arg(fileName)).raise();

    _classNumberClassNameMap.clear();
    int lastImageClassNumber = -1;
    TrainingData trainingData;
    QTextStream inputTextStream(&file);
    while (!inputTextStream.atEnd()) {
        QString line = inputTextStream.readLine();
        QStringList imagePathClassName = line.split(separator, QString::SkipEmptyParts);
        QString imageName = combinePathIfRelative(imagePathClassName.first(), fileName);

        if(QFile::exists(imageName) == false) {
            trainingData.release();
            FileOperationException(QString("File '%1' is not found").arg(imageName)).raise();
        }
        QString imageClass = imagePathClassName.last();
        int imageClassNumber;
        if(_classNumberClassNameMap.values().contains(imageClass)) {
            imageClassNumber = _classNumberClassNameMap.key(imageClass);
        } else {
            imageClassNumber = ++lastImageClassNumber;
            _classNumberClassNameMap[imageClassNumber] = imageClass;
        }
        trainingData.add(cv::imread(imageName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE), imageClassNumber);
    }
    file.close();

    return trainingData;
}

QString View::combinePathIfRelative(QString const& checkableFileName, QString const& absolutePathFileName) const {
    if(QDir::isAbsolutePath(checkableFileName))
        return checkableFileName;
    QDir absolutePathFileDir(absolutePathFileName.left(absolutePathFileName.lastIndexOf('\\')));
    QDir relativaPathFileDir(checkableFileName);
    return absolutePathFileDir.absoluteFilePath(relativaPathFileDir.path());
}

void View::validateTrainingData(TrainingData trainingData) const {
    if(trainingData.empty())
        ArgumentException("Training file is empty").raise();
    const int width = trainingData.imageCollection()[0].cols;
    const int height = trainingData.imageCollection()[0].rows;
    for(int i = 0; i < trainingData.size(); i++) {
        if(trainingData.imageCollection()[i].cols != width || trainingData.imageCollection()[i].rows != height) {
            trainingData.release();
            ArgumentException("All training images should have an equal size").raise();
        }
    }
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
