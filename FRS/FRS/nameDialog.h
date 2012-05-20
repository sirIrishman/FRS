#ifndef FRS_NAMEDIALOG_H
#define FRS_NAMEDIALOG_H

#include <qdialog.h>
#include "ui_nameDialog.h"

namespace FRS {
    namespace Native {
        class NameDialog : public QDialog {
            Q_OBJECT
        public:
            NameDialog(QWidget* const& parent = 0, Qt::WindowFlags f = Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
            virtual ~NameDialog() { };

            QString name() const;
            static QString getName(QWidget* const& parent = 0);

        private:
            Ui::dlg_nameDialog _ui;

        private slots:
            void lnEdt_Name_TextEdited(QString const& text);
        };
    }
}

#endif //FRS_NAMEDIALOG_H
