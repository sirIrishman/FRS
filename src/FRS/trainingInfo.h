#ifndef FRS_TRAININGINFO_H
#define FRS_TRAININGINFO_H

#include <QtCore/qstring.h>
#include "guard.h"

namespace frs {
    namespace native {
        struct TrainingInfo sealed {
        public:
            TrainingInfo(QString const& name, QString const& classImageMapFileName) {
                utils::Guard::argumentNotNullOrEmpty(name, "name");
                utils::Guard::argumentNotNullOrEmpty(classImageMapFileName, "classImageMapFileName");
                _name = name;
                _classImageMapFileName = classImageMapFileName;
            }

            static TrainingInfo empty() {
                return TrainingInfo();
            }

            friend bool operator == (TrainingInfo const& first, TrainingInfo const& second) {
                return first.name() == second.name()
                    && first.classImageMapFileName() == second.classImageMapFileName();
            }
            friend bool operator != (TrainingInfo const& first, TrainingInfo const& second) {
                return first.name() != second.name()
                    || first.classImageMapFileName() != second.classImageMapFileName();
            }

            QString name() const {
                return _name;
            }
            QString classImageMapFileName() const {
                return _classImageMapFileName;
            }

        private:
            QString _name;
            QString _classImageMapFileName;

            TrainingInfo() {
            }
        };
    }
}

#endif //FRS_TRAININGINFO_H