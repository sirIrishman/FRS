#include <QtWidgets/qwidget.h>
#include <QtWidgets/qdialog.h>
#include "aboutDialog.h"

using namespace frs::native;

AboutDialog::AboutDialog(QWidget* const& parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint) {
        _ui.setupUi(this);

        QString qtText = QString("Qt %1").arg(QT_VERSION_STR);
        qtText = _ui.lbl_BasedOnSectionQtText->text().replace("{Qt_Text}", qtText);
        _ui.lbl_BasedOnSectionQtText->setText(qtText);

        QString openCVText = QString("OpenCV %1").arg(CV_VERSION);
        openCVText = _ui.lbl_BasedOnSectionOpenCvText->text().replace("{OpenCV_Text}", openCVText);
        _ui.lbl_BasedOnSectionOpenCvText->setText(openCVText);
}
