#include <QtCore>

namespace Framework {
    class BaseException : public QtConcurrent::Exception {
    public:
        virtual ~BaseException() {
            if(_message != NULL) {
                delete _message;
                _message = NULL;
            }
        }

        void raise() const {
            throw this;
        }
        virtual Exception *clone() const {
            return new BaseException(*_message);
        }
        virtual QString message() const {
            return *_message;
        }

    protected:
        BaseException() {
            initialize("");
        }
        BaseException(QString const& message) {
            initialize(message);
        }

    private:
        QString* _message;

        void initialize(QString const& message) {
            _message = new QString(message);
        }
    };

    class ArgumentException : public BaseException {
    public:
        ArgumentException() : BaseException() {}
        ArgumentException(QString const& message) : BaseException(message) {}
    };
}