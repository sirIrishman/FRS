#ifndef FRS_DIALOGSERVICE_H
#define FRS_DIALOGSERVICE_H

#include <qfiledialog.h>
#include <qmessagebox.h>
#include "guard.h"
#include "exceptions.h"

namespace Services {
    class DialogServiceImplementation {
    public:
        virtual QString showOpenFileDialog(QObject* const& parent, QString const& fileFilter) const = 0;
        virtual void showErrorMessage(QObject* const& parent, QString const& errorMessage) const = 0;
    };

    class DialogService sealed {
    public:
        ~DialogService() {
            unset();
        }

        static void initialize(DialogServiceImplementation* const& implementation) {
            Utils::Guard::argumentNotNull(implementation, "implementation");
            instance()._implementation = implementation;
        }
        static void associateWith(QObject* const& parent) {
            instance()._parent = parent;
        }
        static void unset() {
            if(instance()._implementation != NULL) {
                delete instance()._implementation;
                instance()._implementation = NULL;
            }
            instance()._parent = NULL;
        }
        static QString showOpenFileDialog(QString const& fileFilter) {
            if(!instance().initialized()) 
                Framework::InvalidOperationException("Dialog service was not initialized").raise();
            return instance()._implementation->showOpenFileDialog(instance()._parent, fileFilter);
        }
        static void showErrorMessage(QString const& errorMessage) {
            if(!instance().initialized()) 
                Framework::InvalidOperationException("Dialog service was not initialized").raise();
            instance()._implementation->showErrorMessage(instance()._parent, errorMessage);
        }

    private:
        DialogServiceImplementation* _implementation;
        QObject* _parent;

        DialogService() {
            _implementation = NULL;
            _parent = NULL;
        }

        static DialogService& instance() {
            static DialogService inst;
            return inst;
        }

        bool initialized() {
            return _implementation != NULL;
        }
    };

    class QtDialogServiceImplementation sealed : public DialogServiceImplementation {
    public:
        QString showOpenFileDialog(QObject* const& parent, QString const& fileFilter) const {
            return QFileDialog::getOpenFileName((QWidget*)parent, QObject::tr("Open file"), QDir::currentPath(), fileFilter, 0, QFileDialog::ReadOnly);
        }
        void showErrorMessage(QObject* const& parent, QString const& errorMessage) const {
            QMessageBox::critical((QWidget*)parent, QObject::tr("Error"), errorMessage);
        }
    };
}

#endif //FRS_DIALOGSERVICE_H
