#ifndef FRS_ABOUTDIALOG_H
#define FRS_ABOUTDIALOG_H

#include <qdialog.h>
#include <ui_aboutDialog.h>
#include <opencv2\core\core.hpp>

namespace frs {
    namespace native {
        class AboutDialog : public QDialog {
            Q_OBJECT
        public:
            AboutDialog(QWidget* const& parent);

        private:
            Ui::dlg_AboutDialog _ui;

            QString findVersionInOpenCvBuildInfo() const;
        };
    }
}

#endif //FRS_ABOUTDIALOG_H
