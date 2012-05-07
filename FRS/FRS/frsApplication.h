#ifndef FRS_FRSAPPLICATION_H
#define FRS_FRSAPPLICATION_H

#include <qapplication.h>
#include "dialogService.h"
#include "exceptions.h"

using namespace Services;

namespace FRS {
    class FrsApplication : public QApplication {
    public: 
        FrsApplication(int &argc, char **argv) 
            : QApplication(argc, argv) {
        }

        bool notify(QObject* receiver, QEvent* e) {
            try {
                return QApplication::notify(receiver, e);
            } catch(Framework::BaseException& e) {
                DialogService::showErrorMessage(e.message());
            }
            return false;
        }
    };
}

#endif //FRS_FRSAPPLICATION_H
