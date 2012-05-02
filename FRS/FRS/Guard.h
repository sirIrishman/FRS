#include <QtCore>
#include "Exceptions.h"

using namespace Framework;

namespace Utils {
    class Guard {
    public:
        template<typename T>
        static void argumentNotNull(typename T* const& argumentValue, QString const& argumentName) {
            if(argumentValue == NULL)
                throwException(QString("Argument '%1' can not be null").arg(argumentName));
        }

        template<typename T>
        static void argumentInRange(typename T const& argumentValue, QString const& argumentName, typename T minValue, typename T maxValue) {
            if(argumentValue < minValue || argumentValue > maxValue)
                throwException(QString("Argument '%1' is out of range [%2, %3]").arg(argumentName, minValue, maxValue));
        }

        static void argumentNotNullOrEmpty(QString const& argumentValue, QString const& argumentName) {
            if(argumentValue.isNull() || argumentValue.isEmpty())
                throwException(QString("Argument '%1' can not be null or empty").arg(argumentName));
        }

    private:
        Guard() {}

        static void throwException(QString const& message) {
            ArgumentException* exception = new ArgumentException(message);
            exception->raise();
        }
    };
}