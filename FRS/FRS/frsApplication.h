#ifndef FRS_FRSAPPLICATION_H
#define FRS_FRSAPPLICATION_H

#include <QtWidgets/qapplication>
#include "dialogService.h"
#include "exceptions.h"

using namespace services;

namespace frs {
    class FrsApplication : public QApplication {
    public: 
        FrsApplication(int &argc, char **argv) 
            : QApplication(argc, argv) {
        }

        bool notify(QObject* receiver, QEvent* e) {
            try {
                return QApplication::notify(receiver, e);
            } catch(framework::BaseException& e) {
                DialogService::showErrorMessage(e.message());
            }
            return false;
        }
    };
}

#endif //FRS_FRSAPPLICATION_H
