#ifndef FRS_EXCEPTIONS_H
#define FRS_EXCEPTIONS_H

#include <qtconcurrentexception.h>
#include <qstring.h>

namespace Framework {
    class BaseException : public QtConcurrent::Exception {
    public:
        void raise() const {
            throw *this;
        }
        virtual Exception* clone() const {
            return new BaseException(*this);
        }
        virtual QString message() const {
            return _message;
        }

    protected:
        BaseException() {
            initialize(QString());
        }
        BaseException(QString const& message) {
            initialize(message);
        }

    private:
        QString _message;

        void initialize(QString const& message) {
            _message = QString(message);
        }
    };

    class ArgumentException : public BaseException {
    public:
        ArgumentException() : BaseException("Provided argument is not valid") {}
        ArgumentException(QString const& message) : BaseException(message) {}
    };

    class InvalidOperationException : public BaseException {
    public:
        InvalidOperationException() : BaseException("Method call is invalid for the object's current state") {}
        InvalidOperationException(QString const& message) : BaseException(message) {}
    };
}

#endif //FRS_EXCEPTIONS_H
