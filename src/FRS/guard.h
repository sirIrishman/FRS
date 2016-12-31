#ifndef FRS_GUARD_H
#define FRS_GUARD_H

#include <QtCore/qstring.h>
#include "exceptions.h"

using namespace framework;

namespace utils {
    class Guard sealed {
    public:
        template<typename T>
        static void argumentNotNull(typename T* const& argumentValue, QString const& argumentName) {
            if(argumentValue == NULL)
                throwException(QString("Argument '%1' can not be null").arg(argumentName));
        }

        template<typename T>
        static void argumentInRange(typename T const& argumentValue, QString const& argumentName, typename T minValue, typename T maxValue) {
            if(argumentValue < minValue || argumentValue > maxValue)
                throwException(QString("Argument '%1' is out of range [%2, %3]").arg(argumentName, QString::number(minValue), QString::number(maxValue)));
        }

        static void argumentNotNullOrEmpty(QString const& argumentValue, QString const& argumentName) {
            if(argumentValue.isNull() || argumentValue.isEmpty())
                throwException(QString("Argument '%1' can not be null or empty").arg(argumentName));
        }

    private:
        Guard() {}

        static void throwException(QString const& message) {
            ArgumentException(message).raise();
        }
    };
}

#endif //FRS_GUARD_H
