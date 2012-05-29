#include "faceRecognitionTrainingDialog.h"

#include <qfile.h>
#include "dialogService.h"

using namespace frs::native;

FaceRecognitionTrainingDialog::FaceRecognitionTrainingDialog(QWidget* const& parent, Qt::WindowFlags f) 
    : QDialog(parent, f) {
        _ui.setupUi(this);
        layout()->setSizeConstraint(QLayout::SetFixedSize);
        connect(_ui.lnEdt_Name, SIGNAL(textChanged(QString)), this, SLOT(lnEdt_TextChanged(QString)));
        connect(_ui.lnEdt_ClassImageMapFileName, SIGNAL(textChanged(QString)), this, SLOT(lnEdt_TextChanged(QString)));
        connect(_ui.pshBttn_OpenClassImageMapFileDialog, SIGNAL(pressed()), this, SLOT(pshBttn_OpenClassImageMapFileDialog_Pressed()));
}

QString FaceRecognitionTrainingDialog::name() const {
    return _ui.lnEdt_Name->text();
}

QString FaceRecognitionTrainingDialog::classImageMapFileName() const {
    return _ui.lnEdt_ClassImageMapFileName->text();
}

void FaceRecognitionTrainingDialog::pshBttn_OpenClassImageMapFileDialog_Pressed() {
    QString fileName = services::DialogService::showOpenFileDialog("CSV files (*.csv)");
    if(fileName.isNull() || fileName.isEmpty())
        return;
    _ui.lnEdt_ClassImageMapFileName->setText(fileName);
}

void FaceRecognitionTrainingDialog::lnEdt_TextChanged(QString const& text) {
    bool nameEmpty = _ui.lnEdt_Name->text().isNull() || _ui.lnEdt_Name->text().isEmpty();
    bool classImageMapFileNameEmpty = _ui.lnEdt_ClassImageMapFileName->text().isNull() || _ui.lnEdt_ClassImageMapFileName->text().isEmpty();
    bool classImageMapFileExist = !classImageMapFileNameEmpty && QFile::exists(_ui.lnEdt_ClassImageMapFileName->text());
    _ui.bttn_Ok->setEnabled(!nameEmpty && classImageMapFileExist);
}

TrainingInfo FaceRecognitionTrainingDialog::getTrainingInfo(QWidget* const& parent) {
    FaceRecognitionTrainingDialog* trainingDialog = new FaceRecognitionTrainingDialog(parent);
    TrainingInfo trainingInfo = (trainingDialog->exec() == QDialog::Accepted) 
        ? TrainingInfo(trainingDialog->name(), trainingDialog->classImageMapFileName()) 
        : TrainingInfo::empty();
    delete trainingDialog;
    return trainingInfo;
}
