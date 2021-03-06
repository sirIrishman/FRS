#ifndef FRS_FACERECOGNITIONTRAININGDIALOG_H
#define FRS_FACERECOGNITIONTRAININGDIALOG_H

#include <QtWidgets/qdialog.h>
#include "ui_faceRecognitionTrainingDialog.h"
#include "trainingInfo.h"

namespace frs {
    namespace native {
        class FaceRecognitionTrainingDialog : public QDialog {
            Q_OBJECT
        public:
            FaceRecognitionTrainingDialog(QWidget* const& parent = 0);
            virtual ~FaceRecognitionTrainingDialog() { };

            QString name() const;
            QString classImageMapFileName() const;
            static TrainingInfo getTrainingInfo(QWidget* const& parent = 0);

        private:
            Ui::dlg_faceRecognitionTrainingDialog _ui;

        private slots:
            void lnEdt_TextChanged(QString const& text);
            void pshBttn_OpenClassImageMapFileDialog_Pressed();
        };
    }
}

#endif //FRS_FACERECOGNITIONTRAININGDIALOG_H
