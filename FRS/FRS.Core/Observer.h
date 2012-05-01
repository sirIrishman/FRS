#include <qglobal.h>
#include <qlist.h>

namespace Patterns {
    class ObservableSubject;

    class Observer {
    public:
        virtual ~Observer();
        virtual void update(ObservableSubject* observableSubject) = 0;

    protected:
        Observer();
    };

    class ObservableSubject {
    public:
        virtual ~ObservableSubject() {
            if(_observerCollection != NULL) {
                delete _observerCollection;
                _observerCollection = NULL;
            }
        }

        void attach(Observer* observer) {
            _observerCollection->append(observer);
        }
        void detach(Observer* observer) {
            _observerCollection->removeOne(observer);
        }

        void notify() {
            for ( QList<Observer*>::const_iterator iterator = _observerCollection->constBegin(); iterator != _observerCollection->constEnd(); ++iterator) {
                Observer* observer = *iterator;
                observer->update(this);
            }
        }

    protected:
        ObservableSubject() {
            _observerCollection = new QList<Observer*>();
        }

    private:
        QList<Observer*>* _observerCollection;
    };
}