#ifndef FRS_DIALOGSERVICE_H
#define FRS_DIALOGSERVICE_H

#include <qfiledialog.h>
#include <qmessagebox.h>
#include "guard.h"
#include "exceptions.h"
#include "nameDialog.h"

namespace Services {
    class DialogServiceImplementation {
    public:
        virtual QString showOpenFileDialog(QObject* const& parent, QString const& fileFilter) const = 0;
        virtual void showErrorMessage(QObject* const& parent, QString const& errorMessage) const = 0;
        virtual QString showNameDialog(QObject* const& parent) const = 0;
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
        static QString showSetNameDialog(QObject* const& parent) {
            if(!instance().initialized()) 
                Framework::InvalidOperationException("Dialog service was not initialized").raise();
            return instance()._implementation->showNameDialog(instance()._parent);
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
            return QFileDialog::getOpenFileName((QWidget*)parent, QObject::trUtf8("Open file"), QDir::homePath(), fileFilter, 0, QFileDialog::ReadOnly);
        }
        void showErrorMessage(QObject* const& parent, QString const& errorMessage) const {
            QMessageBox::critical((QWidget*)parent, QObject::trUtf8("Error"), errorMessage);
        }
        QString showNameDialog(QObject* const& parent) const {
            return FRS::Native::NameDialog::getName((QWidget*)parent);
        }
    };
}

#endif //FRS_DIALOGSERVICE_H
