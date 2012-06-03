#include <qglobal.h>
#include "aboutDialog.h"

using namespace frs::native;

AboutDialog::AboutDialog(QWidget* const& parent) 
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::MSWindowsFixedSizeDialogHint) {
        _ui.setupUi(this);

        QString qtText = QString("Qt %1").arg(QT_VERSION_STR);
        qtText = _ui.lbl_BasedOnSectionQtText->text().replace("{Qt_Text}", qtText);
        _ui.lbl_BasedOnSectionQtText->setText(qtText);

        QString openCVText = QString("OpenCV %1").arg(findVersionInOpenCvBuildInfo());
        openCVText = _ui.lbl_BasedOnSectionOpenCvText->text().replace("{OpenCV_Text}", openCVText);
        _ui.lbl_BasedOnSectionOpenCvText->setText(openCVText);
}

QString AboutDialog::findVersionInOpenCvBuildInfo() const {
    QString buildInfo = QString::fromStdString(cv::getBuildInformation());
    const QString searchingString("OpenCV ");
    int start = buildInfo.indexOf(searchingString) + searchingString.length();
    int finish = buildInfo.indexOf(" ", start);
    return buildInfo.mid(start, finish - start);
}
