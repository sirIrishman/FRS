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